#include "stages/stage2.hh"

uchar* Stage2::frame = NULL;
int Stage2::height = -1;
int Stage2::width = -1;
string Stage2::filename = "video.y4mz";
int Stage2::frames = -1;
int Stage2::YUVType = -1;
int Stage2::quantization = 0;

void Stage2::updateFrame(uchar* givenFrame, int rows, int columns, int type, int level) {
    frame = givenFrame;
    height = rows;
    width = columns;
    YUVType = type;
    quantization = level;
}

/**
 * Frame-by-frame
 * @return
 */
int Stage2::encode(bstream& file) {
    if (noFrameHasBeenGiven()) {
        cerr << "No Frame has been inserted to do decoding... Please execute Stage2::updateFrame(uchar[], int, int, int) before this method." << endl;
        return 1;
    }

    int m = 4; // TODO : adaptive golomb value
    Golomb golomb = Golomb(m);
    uchar x, x_a, x_b, x_c;

    // encode Y plane
    for (int row = 0; row != height; row++) {
        for (int column = 0; column != width; column++) {
            x = getYSample(frame, Size(width, height), row, column, YUVType);
            x_a = getYSampleA(frame, Size(width, height), row, column, YUVType);
            x_b = getYSampleB(frame, Size(width, height), row, column, YUVType);
            x_c = getYSampleC(frame, Size(width, height), row, column, YUVType);

            int mu_x = predict(x_a, x_b, x_c);
            int e_x = computeResidual(x, mu_x) >> quantization;

            /* TODO : what about e_x = s_x(x-mu_x) - beta_x? */
            golomb.encode(e_x, file);
            if (quantization) {
                putYSample(frame, Size(width, height), row, column, YUVType, mu_x + (e_x << quantization));
            }
        }
    }

    // encode U plane
    if (YUVType == 1) { /* if YUV 4:4:4 */
        for (int row = 0; row != height; row++) {
            for (int column = 0; column != width; column++) {
                x = getUSample(frame, Size(width, height), row, column, YUVType);
                x_a = getUSampleA(frame, Size(width, height), row, column, YUVType);
                x_b = getUSampleB(frame, Size(width, height), row, column, YUVType);
                x_c = getUSampleC(frame, Size(width, height), row, column, YUVType);

                int mu_x = predict(x_a, x_b, x_c);
                int e_x = computeResidual(x, mu_x) >> quantization;

                /* TODO : what about e_x = s_x(x-mu_x) - beta_x? */
                golomb.encode(e_x, file);
                if (quantization) {
                    putUSample(frame, Size(width, height), row, column, YUVType, mu_x + (e_x << quantization));
                }
            }
        }
    } else if (YUVType == 2) { /* if YUV 4:2:2 or YUV 4:2:0 */
        int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
        for (int row = 0; row != height; row++) {
            for (int column = 0; column != boundWidth; column++) {
                x = getUSample(frame, Size(width, height), row, column, YUVType);
                x_a = getUSampleA(frame, Size(width, height), row, column, YUVType);
                x_b = getUSampleB(frame, Size(width, height), row, column, YUVType);
                x_c = getUSampleC(frame, Size(width, height), row, column, YUVType);

                int mu_x = predict(x_a, x_b, x_c);
                int e_x = computeResidual(x, mu_x) >> quantization;

                /* TODO : what about e_x = s_x(x-mu_x) - beta_x? */
                golomb.encode(e_x, file);
                if (quantization) {
                    putUSample(frame, Size(width, height), row, column, YUVType, mu_x + (e_x << quantization));
                }
            }
        }
    } else {
        int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
        int boundHeight = (height % 2 == 0) ? height/2 : height/2 + 1;
        for (int row = 0; row != boundHeight; row++) {
            for (int column = 0; column != boundWidth; column++) {
                x = getUSample(frame, Size(width, height), row, column, YUVType);
                x_a = getUSampleA(frame, Size(width, height), row, column, YUVType);
                x_b = getUSampleB(frame, Size(width, height), row, column, YUVType);
                x_c = getUSampleC(frame, Size(width, height), row, column, YUVType);

                int mu_x = predict(x_a, x_b, x_c);
                int e_x = computeResidual(x, mu_x) >> quantization;

                /* TODO : what about e_x = s_x(x-mu_x) - beta_x? */
                golomb.encode(e_x, file);
                if (quantization) {
                    putUSample(frame, Size(width, height), row, column, YUVType, mu_x + (e_x << quantization));
                }
            }
        }
    }

    // encode V plane
    if (YUVType == 1) {
        for (int row = 0; row != height; row++) {
            for (int column = 0; column != width; column++) {
                x = getVSample(frame, Size(width, height), row, column, YUVType);
                x_a = getVSampleA(frame, Size(width, height), row, column, YUVType);
                x_b = getVSampleB(frame, Size(width, height), row, column, YUVType);
                x_c = getVSampleC(frame, Size(width, height), row, column, YUVType);

                int mu_x = predict(x_a, x_b, x_c);
                int e_x = computeResidual(x, mu_x) >> quantization;

                /* TODO : what about e_x = s_x(x-mu_x) - beta_x? */
                golomb.encode(e_x, file);
                if (quantization) {
                    putVSample(frame, Size(width, height), row, column, YUVType, mu_x + (e_x << quantization));
                }
            }
        }
    } else if (YUVType == 2) {
        int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
        for (int row = 0; row != height; row++) {
            for (int column = 0; column != boundWidth; column++) {
                x = getVSample(frame, Size(width, height), row, column, YUVType);
                x_a = getVSampleA(frame, Size(width, height), row, column, YUVType);
                x_b = getVSampleB(frame, Size(width, height), row, column, YUVType);
                x_c = getVSampleC(frame, Size(width, height), row, column, YUVType);

                int mu_x = predict(x_a, x_b, x_c);
                int e_x = computeResidual(x, mu_x) >> quantization;

                /* TODO : what about e_x = s_x(x-mu_x) - beta_x? */
                golomb.encode(e_x, file);
                if (quantization) {
                    putVSample(frame, Size(width, height), row, column, YUVType, mu_x + (e_x << quantization));
                }
            }
        }
    } else {
        int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
        int boundHeight = (height % 2 == 0) ? height/2 : height/2 + 1;
        for (int row = 0; row != boundHeight; row++) {
            for (int column = 0; column != boundWidth; column++) {
                x = getVSample(frame, Size(width, height), row, column, YUVType);
                x_a = getVSampleA(frame, Size(width, height), row, column, YUVType);
                x_b = getVSampleB(frame, Size(width, height), row, column, YUVType);
                x_c = getVSampleC(frame, Size(width, height), row, column, YUVType);

                int mu_x = predict(x_a, x_b, x_c);
                int e_x = computeResidual(x, mu_x) >> quantization;

                /* TODO : what about e_x = s_x(x-mu_x) - beta_x? */
                golomb.encode(e_x, file);
                if (quantization) {
                    putVSample(frame, Size(width, height), row, column, YUVType, mu_x + (e_x << quantization));
                }
            }
        }
    }
    //file.close();
    return 0;
}

/**
 * All frames in one time.
 * @return
 */
vector<vector<uchar>> Stage2::decode() {
    clock_t begin = clock();
    int m = 4; // TODO : adaptive golomb value
    Golomb golomb = Golomb(m);
    bstream file {filename.data(), ios::binary|ios::in};
    bstream decoded {(filename+".y4m").data(), ios::binary|ios::out};
    string header;
    vector<uchar> decodedFrame;
    vector<vector<uchar>> video;

    getline(file, header);
    width = getWidth(header);
    height = getHeight(header);
    frames = getNumberOfFrames(header);
    YUVType = getType(header);
    quantization = getQuantizationLevel(header);

    cout << "The quantization level is " << quantization << endl;

    decoded.write(header.data(), header.size());

    for (int frameIndex = 0; frameIndex < frames; frameIndex++) {
        int e_x = 0;
        int x = 0;
        int mu_x = 0;
        uchar x_a, x_b, x_c;

        if (frameIndex == 0) {
            decoded.write("\n", 1);
        }

        decoded.write("FRAME\n", 6);

        // decode Y plane
        for (int row = 0; row != height; row++) {
            for (int column = 0; column != width; column++) {
                e_x = golomb.decode(file);
                x_a = getDecodedYSampleA(&decodedFrame, Size(width, height), row, column, YUVType);
                x_b = getDecodedYSampleB(&decodedFrame, Size(width, height), row, column, YUVType);
                x_c = getDecodedYSampleC(&decodedFrame, Size(width, height), row, column, YUVType);
                mu_x = predict(x_a, x_b, x_c);
                x = (e_x << quantization) + mu_x;
                decodedFrame.push_back((uchar)x);
                decoded.write((const char*)&x, 1);
            }
        }

        // decode U plane
        mu_x = 0;
        if (YUVType == 1) { /* if YUV 4:4:4 */
            for (int row = 0; row != height; row++) {
                for (int column = 0; column != width; column++) {
                    e_x = golomb.decode(file);
                    x_a = getDecodedUSampleA(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_b = getDecodedUSampleB(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_c = getDecodedUSampleC(&decodedFrame, Size(width, height), row, column, YUVType);
                    mu_x = predict(x_a, x_b, x_c);
                    x = (e_x << quantization) + mu_x;
                    decodedFrame.push_back((uchar) x);
                    decoded.write((const char *) &x, 1);
                }
            }
        } else if (YUVType == 2) { /* if YUV 4:2:2 */
            int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
            for (int row = 0; row != height; row++) {
                for (int column = 0; column != boundWidth; column++) {
                    e_x = golomb.decode(file);
                    x_a = getDecodedUSampleA(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_b = getDecodedUSampleB(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_c = getDecodedUSampleC(&decodedFrame, Size(width, height), row, column, YUVType);
                    mu_x = predict(x_a, x_b, x_c);
                    x = (e_x << quantization) + mu_x;
                    decodedFrame.push_back((uchar) x);
                    decoded.write((const char *) &x, 1);
                }
            }
        } else { /* if YUV 4:2:0 */
            int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
            int boundHeight = (height % 2 == 0) ? height/2 : height/2 + 1;
            for (int row = 0; row != boundHeight; row++) {
                for (int column = 0; column != boundWidth; column++) {
                    e_x = golomb.decode(file);
                    x_a = getDecodedUSampleA(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_b = getDecodedUSampleB(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_c = getDecodedUSampleC(&decodedFrame, Size(width, height), row, column, YUVType);
                    mu_x = predict(x_a, x_b, x_c);
                    x = (e_x << quantization) + mu_x;
                    decodedFrame.push_back((uchar) x);
                    decoded.write((const char *) &x, 1);
                }
            }
        }

        // decode V plane
        mu_x = 0;
        if (YUVType == 1) {
            for (int row = 0; row != height; row++) {
                for (int column = 0; column != width; column++) {
                    e_x = golomb.decode(file);
                    x_a = getDecodedVSampleA(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_b = getDecodedVSampleB(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_c = getDecodedVSampleC(&decodedFrame, Size(width, height), row, column, YUVType);
                    mu_x = predict(x_a, x_b, x_c);
                    x = (e_x << quantization) + mu_x;
                    decodedFrame.push_back((uchar) x);
                    decoded.write((const char *) &x, 1);
                }
            }
        } else if (YUVType == 2) {  /* if YUV 4:2:2 */
            int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
            for (int row = 0; row != height; row++) {
                for (int column = 0; column != boundWidth; column++) {
                    e_x = golomb.decode(file);
                    x_a = getDecodedVSampleA(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_b = getDecodedVSampleB(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_c = getDecodedVSampleC(&decodedFrame, Size(width, height), row, column, YUVType);
                    mu_x = predict(x_a, x_b, x_c);
                    x = (e_x << quantization) + mu_x;
                    decodedFrame.push_back((uchar) x);
                    decoded.write((const char *) &x, 1);
                }
            }
        } else { /* if YUV 4:2:0 */
            int boundWidth = (width % 2 == 0) ? width/2 : width/2 + 1;
            int boundHeight = (height % 2 == 0) ? height/2 : height/2 + 1;
            for (int row = 0; row != boundHeight; row++) {
                for (int column = 0; column != boundWidth; column++) {
                    e_x = golomb.decode(file);
                    x_a = getDecodedVSampleA(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_b = getDecodedVSampleB(&decodedFrame, Size(width, height), row, column, YUVType);
                    x_c = getDecodedVSampleC(&decodedFrame, Size(width, height), row, column, YUVType);
                    mu_x = predict(x_a, x_b, x_c);
                    x = (e_x << quantization) + mu_x;
                    decodedFrame.push_back((uchar) x);
                    decoded.write((const char *) &x, 1);
                }
            }
        }

        printProgress((frameIndex+1)/(double)frames, "Decoding");

        video.push_back(decodedFrame);
        decodedFrame.clear();
    }
    clock_t end = clock();
    double elapsedSeconds = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Decoding done in " << elapsedSeconds << " seconds." << endl;
    file.close();
    return video;
}

int Stage2::writeHeader(bstream& file, string originalHeader, int numberOfFrames, int quantizationLevel) {
    frames = numberOfFrames;

    string header = originalHeader + " X N" + to_string(numberOfFrames) + " Q" + to_string(quantizationLevel) + "\n";
    file.write(header.data(), header.length());
    cout << "the title is " << filename << " and the header " << header.length() << endl;
}

void Stage2::endEncode() {
    int m = 4; // TODO : adaptive golomb value
    Golomb golomb = Golomb(m);
    bstream file { filename.data(), ios::binary|ios::out };
    golomb.endEncode(file);
    file.close();
}

bool Stage2::noFrameHasBeenGiven() {
    return (height == -1 || width == -1);
}

void Stage2::setFilename(string name) {
    filename = name;
}
