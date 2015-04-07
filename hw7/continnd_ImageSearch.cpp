#include <cmath>
#include <iostream>
#include <algorithm>
#include "PNG.h"

void findAvgBg(std::vector<int> avg, const unsigned char* img,
        const std::vector<unsigned char>& msk, const int mskWidth,
        const int imgWidth) {
    unsigned int numBlk = 0;
    for (size_t i = 0; i < msk.size(); i += 4) {
        if (msk[i] == 0) {
            size_t imgI = (i / mskWidth * imgWidth + i % mskWidth) * 4;
            numBlk++;
            avg[0] += img[imgI];
            avg[1] += img[imgI + 1];
            avg[2] += img[imgI + 2];
        }
    }
    avg[0] /= numBlk;
    avg[1] /= numBlk;
    avg[2] /= numBlk;
}

float calculateMatchPerc(const unsigned char* img,
        const std::vector<unsigned char>& msk, std::vector<int>& avg,
        const int mskWidth, const int imgWidth, const unsigned int tolerance) {
    int numCorrect = 0;
    std::vector<unsigned int> tolDiff(3);
    for (size_t i = 0; i < msk.size(); i += 4) {
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

void addSkips(std::vector<unsigned char>::iterator img,
        std::vector<std::vector<unsigned char>::iterator>& skips, int mskWidth,
        int mskHeight, int imgWidth) {
    for (int i = imgWidth; i < mskHeight * imgWidth; i += imgWidth) {
        skips.push_back(img + i);
    }
}

int main(int argc, char const* argv[])
{
    if (argc < 4 || argc > 7) {
        std::cout << "Invali7 number of arguments." << std::endl;
        return 1;
    }

    PNG img, msk;
    std::string temp(argv[2]);
    img.load(std::string(temp));
    temp= std::string(argv[3]);
    msk.load(std::string(temp));
    std::vector<unsigned char> mskPixels = msk.getBuffer();
    PNG result(img);
    const char* outputFilename = argv[4];
    unsigned int percentMatch = 75, tolerance = 32;

    if (argc >= 6) {
        percentMatch = std::stoi(argv[6]);
    }

    std::cout << "made it" << std::endl;

    if (argc == 7) {
        tolerance = std::stoi(argv[7]);
    }

    std::vector<unsigned char>::iterator stopPixel = img.getBuffer().end() - ((msk.getHeight()-1) * img.getWidth() - msk.getWidth()) * 4;
    std::vector<std::vector<unsigned char>::iterator> skipPixels;
    int i = 0;

    for (std::vector<unsigned char>::iterator imgPixels = img.getBuffer().begin();
            imgPixels < stopPixel; imgPixels++) {
        for (size_t k = 0; k < skipPixels.size(); k++) {
            if (imgPixels == skipPixels[k]) {
                imgPixels += msk.getWidth();
                i += msk.getWidth();
            }
        }
        if (i % img.getWidth() > img.getWidth() - msk.getWidth()) {
            i += i / img.getWidth() * (img.getWidth() + 1);
            imgPixels = img.getBuffer().begin() + i;
        }
        std::vector<int> avg(3);
        findAvgBg(avg, &(*imgPixels), mskPixels,
                msk.getWidth(), img.getWidth());
        float perc = calculateMatchPerc(&(*imgPixels), mskPixels, avg,
                msk.getWidth(), img.getWidth(), tolerance);
        if (perc >= percentMatch) {
            drawRed(result, msk.getWidth(), msk.getHeight());
            addSkips(imgPixels, skipPixels, msk.getWidth(), msk.getHeight(), img.getWidth());
        }
        i++;
    }
    result.write(outputFilename);
    return 0;
}
