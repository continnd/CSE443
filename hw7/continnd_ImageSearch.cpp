#include <cmath>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <string>
#include "PNG.h"

typedef std::pair<int, int> pixCor;
typedef unsigned int u_int;
typedef unsigned char u_char;

// Copyright Nick Contini 2015

inline bool isBlack(const u_char* pixel) {
    return pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0;
}

inline bool inThreshold(const std::vector<u_int> tolDiff,
        const u_int tol) {
    return tolDiff[0] <= tol && tolDiff[1] <= tol &&
                tolDiff[2] <= tol;
}

u_int countBlack(const std::vector<u_char>& msk) {
    u_int blk = 0;
    for (size_t i = 0; i < msk.size(); i += 4) {
        if (isBlack(&msk[i])) {
            blk++;
        }
    }
    return blk;
}

void findAvgBg(std::vector<u_int>& avg, const u_char* img,
        const std::vector<u_char>& msk, const int mskW,
        const int imgW, const float blk) {
    for (size_t i = 0; i < msk.size() / 4; i++) {
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

float calcMatch(const u_char* img,
        const std::vector<u_char>& msk, std::vector<u_int>& avg,
        const int mskW, const int imgW, const u_int tol) {
    int numCorrect = 0;
    std::vector<u_int> tolDiff(3);
    for (size_t i = 0; i < msk.size() / 4; i++) {
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
    return (numCorrect * 4.0f) / msk.size() * 100;
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

bool checkBounds(int& row, int& col, int mskW, int mskH,
        std::vector<pixCor>& skipPixels,
        u_int& i, int imgW) {
    for (pixCor p : skipPixels) {
        int x = (row <= p.first) ? row + mskH : row;
        int y = (col <= p.second) ? col + mskW : col;
        if (x >= p.first && x <= p.first + mskH &&
                y >= p.second && y <= p.second + mskW) {
            i += p.first - row + mskW;
            row = i / imgW;
            col = i % imgW;
            return true;
        }
    }
    return false;
}

bool checkEdge(int& col, int& row, int imgW, int mskW, u_int& pixel) {
    if (col > imgW- mskW) {
        pixel += imgW - col;
        row = pixel / imgW;
        col = pixel % imgW;
        return true;
    }
    return false;
}

void printMatch(int row, int col, int mskW, int mskH) {
    std::cout << "sub-image matched at: " << row << ", " <<
        col << ", " << row + mskH << ", " << col + mskW
        << std::endl;
}

void process(const u_int stopPixel, std::vector<u_char>::iterator& imgSt,
        int mskW, int mskH, std::vector<pixCor>& skipPixels, int imgW,
        std::vector<u_char> mskB, u_int blk, const u_int tol,
        const u_int lmtPrc, PNG& result, u_int matches) {
#pragma omp parallel for default(shared) schedule(static)
    for (u_int i = 0; i < stopPixel; i++) {
        int row = i / imgW, col = i % imgW;
#pragma omp critical(a)
        while(checkBounds(row, col, mskW, mskH, skipPixels, i, imgW)
                || checkEdge(row, col, imgW, mskW, i)) { };
        std::vector<u_int> avg(3);
        findAvgBg(avg, &(*imgSt) + i * 4, mskB, mskW, imgW, blk);
        float perc = calcMatch(&(*imgSt) + i * 4, mskB, avg, mskW, imgW, tol);
        if (perc >= lmtPrc) {
#pragma omp critical(a)
            {
                drawBox(result, row, col, mskW, mskH), matches++;
                skipPixels.push_back(pixCor(row, col));
                printMatch(row, col, mskW, mskH);
            }
            i += mskW - 1;
        }
    }
    std::cout << "Number of matches: " << matches << std::endl;
}

int main(int argc, char const* argv[]) {
    if (argc < 4 || argc > 7) {
        std::cout << "Invalid number of arguments." << std::endl;
        return 1;
    }

    PNG img, msk;
    img.load(std::string(argv[1]));
    msk.load(std::string(argv[2]));

    u_int blk = countBlack(msk.getBuffer());

    std::vector<u_char> mskB = msk.getBuffer();
    PNG result(img);

    const u_int lmtPrc = argc >= 6 ? std::stoi(argv[5]) : 75,
          tol = argc == 7 ? std::stoi(argv[6]) : 32, stopPixel =
              (img.getBuffer().size() - ((msk.getHeight() - 1) *
                                         img.getWidth() - msk.getWidth()) * 4) / 4;

    std::vector<pixCor> skipPixels;

    std::vector<u_char>::iterator imgSt = img.getBuffer().begin();
    u_int matches = 0;

    process(stopPixel, imgSt, msk.getWidth(), msk.getHeight(), skipPixels,
            img.getWidth(), mskB, blk, tol, lmtPrc, result, matches);

    result.write(argv[3]);
    return 0;
}
