#include "wavcb.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536;
constexpr int DISTORTION_THRESHOLD = 1;
constexpr int ITERATION_UPPER_BOUND = 100;

random_device rd;
mt19937 g(rd());

/**
 * @brief Construct a new Codebook:: Codebook object
 * 
 * @param bookSize the length of the codebook (number of codewords)
 * @param overlap factor step with which the samples will be read, in order to create the codebook. This value must be an integer 1 or bigger.
 * 
 * @throw std::invalid_argument if the overlap factor is given as a number equal or smaller than 0.
 */
Codebook::Codebook(size_t bookSize, int overlap) {
    Codebook::codebookSize = bookSize;
    if (overlap <= 0) {
        throw invalid_argument("The overlap factor must 1 or bigger."); 
    }
    Codebook::overlapFactor = overlap;                                                              
    Codebook::iterationSamples = vector<vector<tuple<bitset<16>,int> > >(Codebook::codebookSize);
}

/**
 * @brief creates a codebook vector. 
 * 
 * @param filename the name of the audio file under which the codebook is meant to be created.
 * @return int 0 if it runs successfully; otherwise, an error code.
 */
int Codebook::create_codebook(std::string filename) {
    SndfileHandle file { filename };                                                    // open file as WAV audio file
                                                                                        // TODO : file validation   
    Codebook::generate_centroids(file);                                                 // generate the centroids 
    int iteration = 0;                                                                  // mark this iteration as the first one
    while (!Codebook::hasCodebookConverged() || iteration < ITERATION_UPPER_BOUND) {    // while the codebook has not converged and the iteration has not targeted the given upper bound
        Codebook::produce_codebook(file);                                               //     produce the codebook with the training samples
        iteration++;                                                                    //     mark this iteration as done
    }                                                                                   //
    return 0;                                                                           // exit successfully.
}

/**
 * @brief generates the main centroids to start the LBG Algorithm. This is a private method.
 * 
 * @param file the audio file under which the codebook is meant to be created.
 * @return int 0 if it runs successfully; otherwise, an error code.
 */
int Codebook::generate_centroids(SndfileHandle file) {
    size_t numberOfFrames;                                                                                                  // prepare to receive number of read frames
    vector<short> chunk(FRAMES_BUFFER_SIZE * file.channels());                                                              // prepare to receive a chunk of audio    
    while ((numberOfFrames = file.readf(chunk.data(), FRAMES_BUFFER_SIZE))) {                                               // while WAV has something to read
        for (auto sample : chunk) {                                                                                         //     for each sample inside a chunk
            Codebook::samples.push_back(bitset<16>(sample));                                                                //         read the sample as a 16 bits and put it on samples vector
        }                                                                                                                   // 
        chunk.resize(numberOfFrames * file.channels());                                                                     //     prepare space for another chunk
    }                                                                                                                       //
    //shuffle(Codebook::samples.begin(), Codebook::samples.end(), g);                                                       // shuffle samples vector
    for (int i = 0; i < Codebook::samples.size()*16-16; i+=Codebook::overlapFactor) {                                       // for each bit on samples with a step set by overlapping factor
        Codebook::centroids.push_back( bitset<16>(Codebook::samples.at(i/16) << (i%16)) |                                   //     read the entire 16-bits (overlapped or not) and put it on centroids list
                                       bitset<16>(Codebook::samples.at((i/16)+1) >> (16-(i%16)) ));                         //     
    }                                                                                                                       //
    Codebook::centroids.erase(unique(Codebook::centroids.begin(), Codebook::centroids.end()), Codebook::centroids.end());   // remove duplicates from centroids
    if (Codebook::centroids.size() < Codebook::codebookSize) {                                                              // if the current centroid is smaller than what we want, then
        return 1;                                                                                                           //     we should cancel the operation and report the error.
    }                                                                                                                       //
    Codebook::centroids.erase(Codebook::centroids.begin()+Codebook::codebookSize, Codebook::centroids.end());               // as samples are already shuffled, we can remove leftovers (more than codebookSize) 
                                                                                                                            //
    return 0;                                                                                                               // exit successfully.
}

/**
 * @brief produce the codebook running the LBG Algorithm. This is a private method.
 * 
 * @param file the audio file under which the codebook is meant to be created.
 * @return int 0 if it runs successfully; otherwise, an error code.
 */
int Codebook::produce_codebook(SndfileHandle file) {
    long numberOfSamples = Codebook::iterate_centroids(file);                                       // as samples are our training set, we should place them on the space and group them into the 512 centroids
    double distortion = Codebook::evaluate_distortion(numberOfSamples);                             // after grouping them, there is a consequent error to it, which is evaluated then
    if (Codebook::distortion - distortion < DISTORTION_THRESHOLD && Codebook::distortion >= 0) {    // if distortion is smaller than what we want, then
        Codebook::codebook = Codebook::centroids;                                                   //     this should be our codebook, as is
        Codebook::codebookHasConverged = true;                                                      //     one should mark this as converged
        return 0;                                                                                   //     quitting successfully.
    }                                                                                               //
    Codebook::distortion = distortion;                                                              // update the current distortion
    Codebook::evaluate_centroids();                                                                 // let's evaluate the centroids for the next round
    return 0;                                                                                       // quit successfully.
}

/**
 * @brief iterate a training sample set over a set of centroids already evaluated. This is a private method.
 * 
 * @param file the audio file under which the codebook is meant to be created.
 * @return int 0 if it runs successfully; otherwise, an error code.
 */
int Codebook::iterate_centroids(SndfileHandle file) {
    long numberOfSamples;                                                                                               // prepare to receive a number of samples
    size_t numberOfFrames;                                                                                              // prepare to receive a number of frames
    vector<short> chunk(FRAMES_BUFFER_SIZE * file.channels());                                                          // prepare to receive a chunk of audio samples
    while ((numberOfFrames = file.readf(chunk.data(), FRAMES_BUFFER_SIZE))) {                                           // while there is something new to read on the audio file, 
        for (auto sample : chunk) {                                                                                     //     for each sample present on the chunk,
            int distance = INT_MAX;                                                                                     //         we mark our distance as infinity
            int temporaryDistance = INT_MAX;                                                                            //         we mark a temporary distance as infinity
            int minimumDistanceCentroid;                                                                                //         we prepare to receive a minimum distance centroid
            numberOfSamples++;                                                                                          //         we increase our number of samples
            for (int i = 0; i != Codebook::centroids.size(); i++) {                                                     //         for each centroid,
                if (temporaryDistance = euclidean_distance16(sample, Codebook::centroids.at(i)) < distance) {           //             if the distance from the sample to this centroid is smaller than the latter, then
                    distance = temporaryDistance;                                                                       //                 this will be our chosen distance value
                    minimumDistanceCentroid = i;                                                                        //                 this index will be our centroid identification
                }                                                                                                       //
            }                                                                                                           //
            Codebook::iterationSamples[minimumDistanceCentroid].push_back(make_tuple(bitset<16>(sample), distance));    //         we insert the sample into the marked centroid, as a pair sample, distance
        }                                                                                                               //
        chunk.resize(numberOfFrames * file.channels());                                                                 //     we clean the collect chunk
    }
    return numberOfSamples;                                                                                             // exit successfully, returning the number of samples. 
}

short Codebook::closest_vector(bitset<16> sample) {
    int distance = INT_MAX;
    int temporaryDistance = INT_MAX;
    int minimumDistanceCentroid;
    for (int i = 0; i != Codebook::codebook.size(); i++) {
        if (temporaryDistance = euclidean_distance16(sample, Codebook::codebook.at(i)) < distance) {
            distance = temporaryDistance;
            minimumDistanceCentroid = i;
        }
    }
    return minimumDistanceCentroid;
}

/**
 * @brief evaluates the distortion (error of distances) between iterations. This is a private method.
 * 
 * @param numberOfSamples number of samples read on one iteration.
 * @return double the evaluated distortion.
 */
double Codebook::evaluate_distortion(int numberOfSamples) { 
    long errorSum;                                              // we prepare to receive a sum of errors
    for (auto index : Codebook::iterationSamples) {             // for each index on the samples we've collected on the this iteration,
        for (auto sample : index) {                             //     for each sample present in this index,   
            errorSum += get<1>(sample);                         //         we increase our sum of errors by the value in the given pair sample,distance (the distance)
        }                                                       // 
    }                                                           //
    return errorSum / numberOfSamples;                          // exit successfully, returning the distortion.
}

/**
 * @brief evaluate the new set of centroids after one iteration. This is a private method.
 * 
 * @return int 0 if it runs successfully; otherwise, an error code.
 */
int Codebook::evaluate_centroids() {
    for (int centroid = 0; centroid != Codebook::iterationSamples.size(); centroid++) {         // for each centroid in the set of centroids evaluated on this iteration,
        int bitSum[16] = { };                                                                   //     we prepare an integer array to sum the distances on each dimension of the centroids
        for (auto sample : Codebook::iterationSamples.at(centroid)) {                           //     for each sample marked in the given centroid,
            for (int bit = 0; bit != 16; bit++) {                                               //         for each dimension (in this case, a bit),
                bitSum[bit] += get<0>(sample)[bit];                                             //             increase the given dimension on the index, by the read value
            }                                                                                   //
        }                                                                                       //
        string finalCentroid = "";                                                              //     prepare to receive a binary quantity represented as a string
        for (auto bit : bitSum) {                                                               //     for each bit represented on this integer array,
            bit = ((double)(bit)/(double)(Codebook::iterationSamples.at(centroid).size()) >= 0.5) &&              //         the bit will be 0 if average is under 0.5 or 1 if 0.5 or bigger
                  ((double)(bit)/(double)(Codebook::iterationSamples.at(centroid).size()) <= 1.0) ? 1 : 0;          //
            finalCentroid += to_string(bit);                                                    //         this bit must be concatenated with the string
        }                                                                                       //
        Codebook::centroids[centroid] = bitset<16>(finalCentroid);                              //     having the new centroid evaluated, then we must add it to our new list of centroids.
    }                                                                                           //
    return 0;                                                                                   // exit successfully.
}

/**
 * @brief saves the codebook onto a *.cb file.
 * 
 * @param filename the name of the file to be saved.
 * @return int 0 if it runs successfully; otherwise, an error code.
 */
int Codebook::save(std::string filename) {
    if (!Codebook::codebookHasConverged) {                                                          // if codebook has not been already created, then
        return 1;                                                                                   //     exit with an error TODO : place a convenient error
    }                                                                                               //
    fstream file(filename + ".cb", ios::out|ios::binary);                                           // open a file with a name ended in *.cb and in binary mode
    if (file.is_open()) {                                                                           // if the file is open, then
        for (auto codeword : Codebook::codebook) {                                                  //     for each codeword in the codebook,
            file.write(codeword.to_string().data(), sizeof(codeword.to_string().data()));           //         write the codeword onto the file // TODO: not working
        }                                                                                           //
    }                                                                                               //
    file.close();                                                                                   // close the file
    return 0;                                                                                       // exit successfully.
}

/**
 * @brief loads the codebook from a *.cb file.
 * 
 * @param filename the name of the file to be loaded.
 * @return int 0 if it runs successfully; otherwise, an error code.
 */
int Codebook::load(std::string filename) {
    if (!Codebook::codebook.empty()) {                                      // if a codebook is already created, then
        Codebook::codebook.clear();                                         //     clear the codebook
    }                                                                       //
    fstream file(filename + ".cb", ios::in|ios::binary);                    // open a file with a name ended in *.cb and in binary mode
    char* codeword;                                                         // prepare to receive a codeword
    if (file.is_open()) {                                                   // if the file is open, then
        for (int i = 0; i != Codebook::codebookSize; i++) {                 //     for each expected codeword ,
            file.read(codeword, 2);                                         //         read a codeword of 16-bits
            Codebook::codebook.push_back(bitset<16>(string(codeword)));     //         and push it on our codebook
        }                                                                   // 
    }                                                                       //
    file.close();                                                           // close the file
    if (Codebook::codebook.empty()) {                                       // if the codebook remains empty, then
        return 1;                                                           //     return a proper error (TODO)
    }                                                                       //
    return 0;                                                               // exit successfully.
}

/**
 * @brief getter to confirm if the codebook has already been officially created.
 * 
 * @return true if the codebook has been already created.
 * @return false if the codebook has not been created or still hasn't converged.
 */
bool Codebook::hasCodebookConverged() {
    return Codebook::codebookHasConverged;  
}

/**
 * @brief getter to obtain the codebook, after being created.
 * 
 * @return vector<bitset<16> > the codebook, as a vector of 16-bits bitset structures (codewords).
 */
vector<bitset<16> > Codebook::getCodebook() {
    return Codebook::codebook; 
}

/**
 * @brief getter to obtain the size of the codebook (the number of codewords).
 * 
 * @return int the number of codewords of the current codebook.
 */
int Codebook::size() {
    return Codebook::codebookSize;
}
