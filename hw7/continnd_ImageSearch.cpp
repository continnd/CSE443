#include <cmath>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <string>
#include "PNG.h"

// Copyright Nick Contini 2015

using pixCor = std::pair<int, int>;
using u_char  = unsigned char;

inline bool isBlack(const u_char* pixel) {
    return pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0;
}

inline bool inThreshold(const int tolDiff[],
        const int tol) {
    return tolDiff[0] < tol && tolDiff[1] < tol && tolDiff[2] < tol;
}

int countBlack(const u_char msk[], const u_int mskW, const u_int mskH) {
    int blk = 0;
    for (size_t i = 0; i < mskH * mskW * 4; i += 4) {
        if (isBlack(&msk[i])) {
            blk++;
        }
    }
    return blk;
}

void findAvgBg(int avg[], const u_char* img,
        const u_char msk[], const u_int mskW, const u_int mskH,
        const int imgW, const float blk) {
    for (size_t i = 0; i < mskW * mskH; i++) {
        if (isBlack(&msk[i * 4])) {
            size_t imgI = (i / mskW * imgW + i % mskW) * 4;
            avg[0] += img[imgI];
            avg[1] += img[imgI + 1];
            avg[2] += img[imgI + 2];
        }
    }
    avg[0] /= blk;
    avg[1] /= blk;
    avg[2] /= blk;
}

float calcMatch(const u_char* img, const u_char msk[],
        int avg[], const int mskW, const u_int mskH, const u_int imgW,
        const int tol) {
    float numCorrect = 0;
    int tolDiff[3];
    for (size_t i = 0; i < mskH * mskW; i++) {
        int imgI = (i / mskW * imgW + i % mskW) * 4;
        tolDiff[0] = abs(img[imgI] - avg[0]);
        tolDiff[1] = abs(img[imgI + 1] - avg[1]);
        tolDiff[2] = abs(img[imgI + 2] - avg[2]);
        if (isBlack(&msk[i * 4]) && inThreshold(tolDiff, tol)) {
            numCorrect++;
        } else if (!isBlack(&msk[i * 4]) && !inThreshold(tolDiff, tol)) {
            numCorrect++;
        } else {
            numCorrect--;
        }
    }
    return numCorrect / (mskH * mskW) * 100;
}

void drawBox(PNG& png, int row, int col, int width, int height) {
    // Draw horizontal lines
    for (int i = 0; (i < width); i++) {
        png.setRed(row, col + i);
        png.setRed(row + height, col + i);
    }
    // Draw vertical lines
    for (int i = 0; (i < height); i++) {
        png.setRed(row + i, col);
        png.setRed(row + i, col + width);
    }
}

bool checkBounds(int row, int& col, int mskW, int mskH,
        std::vector<pixCor>& skipPixels, int imgW) {
    for (pixCor p : skipPixels) {
        int x = (row <= p.first) ? row + mskH - 1 : row;
        int y = (col <= p.second) ? col + mskW - 1 : col;
        if (x >= p.first && x < p.first + mskH &&
                y >= p.second && y < p.second + mskW) {
            col = p.second + mskW - 1;
            return true;
        }
    }
    return false;
}

void printMatch(int row, int col, int mskW, int mskH) {
    std::cout << "sub-image matched at: " << row << ", " <<
        col << ", " << row + mskH << ", " << col + mskW
        << std::endl;
}

void process(u_char* imgSt, int mskW, int mskH,
        std::vector<pixCor>& skipPixels, int imgW, int imgH,
        u_char mskB[], int blk, const int tol,
        const int lmtPrc, PNG& result, int matches) {
#pragma omp parallel for schedule(static) default(shared)
    for (int row = 0; row <= imgH - mskH; row++) {
        for (int col = 0; col <= imgW - mskW; col++) {
            if (!checkBounds(row, col, mskW, mskH, skipPixels, imgW)) {
                int i = row * imgW + col, avg[3];
                findAvgBg(avg, imgSt + i * 4, mskB, mskW, mskH, imgW, blk);
                float perc = calcMatch(imgSt + i * 4, mskB, avg, mskW, mskH,
                        imgW, tol);
                if (perc >= lmtPrc) {
#pragma omp critical(a)
                    {
                        drawBox(result, row, col, mskW, mskH), matches++;
                        skipPixels.push_back(pixCor(row, col));
                        printMatch(row, col, mskW, mskH);
                    }
                    col += mskW - 1;
                }
            }
        }
    }
    std::cout << "Number of matches: " << matches << std::endl;
}

int main(int argc, char const* argv[]) {
    if (argc < 5 || argc > 7) {
        std::cout << "Invalid number of arguments." << std::endl;
        return 1;
    }

    PNG img, msk;
    img.load(std::string(argv[1]));
    msk.load(std::string(argv[2]));

    int blk = countBlack(msk.getBuffer().data(), msk.getWidth(),
            msk.getHeight());

    PNG result(img);

    const int lmtPrc = (argc >= 6) ? std::stoi(argv[5]) : 75,
          tol = (argc == 7) ? std::stoi(argv[6]) : 32; 
    std::vector<pixCor> skipPixels;

    u_char* imgSt = &(img.getBuffer()[0]);
    int matches = 0;

    process(imgSt, msk.getWidth(), msk.getHeight(), skipPixels,
            img.getWidth(), img.getHeight(), msk.getBuffer().data(), blk, tol,
            lmtPrc, result, matches);

    result.write(argv[3]);
    return 0;
}
