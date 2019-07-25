#include "yuv/info.hh"

int getWidth(string& header) {
    return stoi(header.substr(header.find(" W") + 2, header.find(" H") - header.find(" W") - 2));
}

int getHeight(string& header) {
    return stoi(header.substr(header.find(" H") + 2, header.find(" F") - header.find(" H") - 2));
}

int getType(string& header) {
    int positionOfColorSpace = (int)header.find(" C");
    if (header.find(" C") == string::npos) {
        return 3;
    } else {
        switch (stoi(header.substr((unsigned long)positionOfColorSpace+2, header.size()-1))) {
            case 444:
                return 1;
            case 422:
                return 2;
            case 420:
                return 3;
            default:
                // TODO : handle this case
                return 0;
        }
    }
}

int getQuantizationLevel(string& header) {
    int positionOfQuantizationLevel = (int)header.find(" Q");
    if (header.find(" Q") == string::npos) {
        return 0;
    } else {
        return stoi(header.substr((unsigned long)positionOfQuantizationLevel+2, header.size()-1));
    }
}

int getNumberOfFrames(string& header) {
    return stoi(header.substr(header.find(" N") + 2, header.size() - header.find(" N") - 2));
}
