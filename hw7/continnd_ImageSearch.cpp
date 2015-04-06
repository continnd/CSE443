#include <cmath>
#include <iostream>
#include <algorithm>
#include "PNG.h"

std::vector<int>& findAvgBg(const unsigned char* img,
        const std::vector<unsigned char>& msk, const int mskWidth,
        const int imgWidth) {
    int avgR = 0, avgG = 0, avgB = 0, avgA = 0, numBlk = 0;
    std::vector<int> avg(3);
    for (int i = 0; i < msk.size(); i += 4) {
        if (msk[i] == 0) {
            int imgI = (i / mskWidth * imgWidth + i % mskWidth) * 4;
            numBlk++;
            avg[0] += img[imgI];
            avg[1] += img[imgI + 1];
            avg[2] += img[imgI + 2];
        }
    }
    avg[0] /= numBlk;
    avg[1] /= numBlk;
    avg[2] /= numBlk;
    return avg;
}

float calculateMatchPerc(const unsigned char* img,
        const std::vector<unsigned char>& msk, std::vector<int>& avg,
        const int mskWidth, const int imgWidth, const int tolerance) {
    int numCorrect = 0;
    std::vector<int> tolDiff(3);
    for (int i = 0; i < msk.size(); i += 4) {
        int imgI = (i / mskWidth * imgWidth + i % mskWidth) * 4;
        tolDiff[0] = abs(img[imgI] - avg[0]);
        tolDiff[1] = abs(img[imgI + 1] - avg[1]);
        tolDiff[2] = abs(img[imgI + 2] - avg[2]);
        if (msk[i] == 0 && tolDiff[0] <= tolerance && tolDiff[1] <= tolerance &&
                tolDiff[2] <= tolerance) {
            numCorrect++;
        } else if (msk[i] != 0 && (tolDiff[0] > tolerance ||
                    tolDiff[1] > tolerance ||
                    tolDiff[2] > tolerance)) {
            numCorrect++;
        }
    }
    return (numCorrect + 0.0f)/msk.size();
}

void drawRed(PNG img, const int width, const int height) {
    for (int i = 0; i < width; i++) {
        img.setRed(0, i);
    }
    for (int i = 0; i < width; i++) {
        img.setRed(height, i);
    }
    for (int i = 0; i < height; i++) {
        img.setRed(i, 0);
    }
    for (int i = 0; i < height; i++) {
        img.setRed(i, width);
    }
}

int main(int argc, char const* argv[])
{
    if (argc < 4 || argc > 6) {
        std::cout << "Invalid number of arguments." << std::endl;
    }

    const PNG img(argv[2]);
    const PNG msk(argv[3]);
    const std::vector<char> mskPixels = msk.getBuffer();
    PNG result(img);
    char* outputFilename = argv[4];
    const bool isMask = true;
    int percentMatch = 75
        int tolerance = 32;

    if (argc >= 5) {
        percentMatch = argv[5];
    }
    if (arc == 6) {
        tolerance = arv[6];
    }
    char* stopPixel = 

    for(char* imgPixels = img.getBuffer(); imgPixels != ) {
        std::vector<int> avg = findAvgBg(imgPixels, mskPixels,
                msk.width, img.width);
        float perc = calculateMatchPerc(imgPixels, mskPixels, avg,
                msk.width, img.width, tolerance);
        if (perc >= percentMatch) {
            drawRed(result, msk.width, msk.height);
        }
    }
    return 0;
}
