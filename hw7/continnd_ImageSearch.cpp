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

// determines if a pixel is black
// input u_char* pixel: pointer to red value of pixel
inline bool isBlack(const u_char* pixel) {
    return pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0;
}

// determines if a pixel is within background bounds
// input int tolDiff[]: the difference between the subimage pixel and the
// average background color
// input int tol: tolerance used to determine the max deviation from the
// average each value can be for the background pixel
inline bool inThreshold(const int tolDiff[],
        const int tol) {
    return tolDiff[0] < tol && tolDiff[1] < tol && tolDiff[2] < tol;
}

// Counts the number of black pixels in the image
// input u_int msk[]: array of pixels, starting from the top left pixel, starting
// with the red value, moving to green, then blue, then alpha, then continuing
// the next pixel in the row.
// input u_int mskW: Width of the image
// input u_int mskH: Height of the image
// output int the number of black pixels in the image
int countBlack(const u_char msk[], const u_int mskW, const u_int mskH) {
    int blk = 0;
    for (size_t i = 0; i < mskH * mskW * 4; i += 4) {
        if (isBlack(&msk[i])) {
            blk++;
        }
    }
    return blk;
}

// Finds the average value for colors within the background (defined by black
// pixels in the mask)
// output avg[]: array of average values (RGB)
// input u_char* img; pointer to the red value of the top left corner of the
// sub-image
// input u_int msk[]: array of pixels, starting from the top left pixel, starting
// with the red value, moving to green, then blue, then alpha, then continuing
// the next pixel in the row.
// input u_int mskW: Width of the mask
// input u_int mskH: Height of the mask
// input u_int imgW: Width of the image
// input float blk: number of black pixels in the mask (or the number of
// background pixels in the mask)
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

// Calculate the percent of pixels that are within the threshold for a
// background pixel. From there it will check to make sure it is meant to be a
// background or foreground pixel, increasing or decreasing the match
// percentage accordingly.
// input u_char* img; pointer to the red value of the top left corner of the
// sub-image
// input u_int msk[]: array of pixels, starting from the top left pixel, starting
// with the red value, moving to green, then blue, then alpha, then continuing
// the next pixel in the row.
// input int avg[]: average values for background pixel (RGB)
// input u_int mskW: Width of the mask
// input u_int mskH: Height of the mask
// input u_int imgW: Width of the image
// input int tol: tolerance used to determine the max deviation from the
// average each value can be for the background pixel
// output float: percentage the mask matches the subimage
float calcMatch(const u_char* img, const u_char msk[],
        int avg[], const int mskW, const u_int mskH, const u_int imgW,
        const int tol) {
    float numCorrect = 0;
    int tolDiff[3];
    // loop through sub image
    for (size_t i = 0; i < mskH * mskW; i++) {
        int imgI = (i / mskW * imgW + i % mskW) * 4;

        // difference between average background color and subimage pixel
        tolDiff[0] = abs(img[imgI] - avg[0]);
        tolDiff[1] = abs(img[imgI + 1] - avg[1]);
        tolDiff[2] = abs(img[imgI + 2] - avg[2]);

        // if pixel correctly corresponds to foreground or background, increase
        // the number of matched pixels. If not, subtract from number correct.
        // This ensures if you have a lot of pixels that do not match, the
        // match percentage will be pushed lower
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

// Function to draw a red box around matches
// input PNG png: output image of the whole program
// input row: row of pixel of top left corner of match
// input col: col of pixel of top left corner of match
// input width: width of match
// input height: height of match
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

// Checks to make sure the subimage being checked is valid. For example, the
// subimage cannot be outside the bounds of the image nor overlap with any
// previously determined matches. Will skip over a sub image if it is invalid
// (which is why the col is passed by reference)
// input row: row of pixel of top left corner of subimage
// input col: col of pixel of top left corner of subimage
// input u_int mskW: Width of the mask
// input u_int mskH: Height of the mask
// input std::vector<pixCor> skipPixels: coordinates of the top left corner of
// previously found matches. These matches must be skipped over.
// input u_int imgW: Width of the image
// output bool: true if the location of the subimage is valid
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

// Prints the coordinates of a match
// input row: row of pixel of top left corner of match
// input col: col of pixel of top left corner of match
// input u_int mskW: Width of the mask
// input u_int mskH: Height of the mask
// input u_int imgW: Width of the image
void printMatch(u_int  row, u_int  col, u_int  mskW, u_int  mskH) {
    std::cout << "sub-image matched at: " << row << ", " <<
        col << ", " << row + mskH << ", " << col + mskW
        << std::endl;
}

// Master function of the whole program. Calls all the other functions of the
// program.
// input u_char* imgSt: pointer to the top left corner of the first subimage
// to check
// input u_int mskW: Width of the mask
// input u_int mskH: Height of the mask
// input u_int imgW: Width of the image
// input u_int imgH: Height of the image
// input u_char maskB[]: buffer containing the pixels of the mask
// input u_int blk: number of black pixels in the mask
// input int tol: tolerance used to determine the max deviation from the
// average each value can be for the background pixel
// input u_int lmtPrc: The minimum match percentage for a subimage to count as a match
void process(u_char* imgSt, u_int mskW, u_int mskH,
        std::vector<pixCor>& skipPixels, u_int imgW, u_int imgH,
        u_char mskB[], u_int blk, const int tol,
        const u_int lmtPrc, PNG& result, u_int matches) {
#pragma omp parallel for schedule(static) default(shared)

    // loop through input image
    for (int row = 0; row <= imgH - mskH; row++) {
        for (int col = 0; col <= imgW - mskW; col++) {

            // if the subimage is not valid, skip to the next valid pixel. If
            // not, see if the subimage is a match
            if (!checkBounds(row, col, mskW, mskH, skipPixels, imgW)) {
                int i = row * imgW + col, avg[3];
                findAvgBg(avg, imgSt + i * 4, mskB, mskW, mskH, imgW, blk);
                float perc = calcMatch(imgSt + i * 4, mskB, avg, mskW, mskH,
                        imgW, tol);

                // output results
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

// input argv[1]: image
// input argv[2]: mask
// input argv[3]: output img
// input argv[4]: unused
// input argv[5]: minmal percentage to count as a match; defaults to 75% - optional input
// input argv[6]: tolerance used to determine if pixel is within background
// pixel bounds; defaults to 32 - optional input
int main(int argc, char const* argv[]) {
    // parse inputs to the program
    if (argc < 5 || argc > 7) {
        std::cout << "Invalid number of arguments." << std::endl;
        return 1;
    }

    PNG img, msk;
    img.load(std::string(argv[1]));
    msk.load(std::string(argv[2]));

    int blk = countBlack(msk.getBuffer().data(), msk.getWidth(),
            msk.getHeight());

    // copy input image to create base for output image
    PNG result(img);

    const int lmtPrc = (argc >= 6) ? std::stoi(argv[5]) : 75,
          tol = (argc == 7) ? std::stoi(argv[6]) : 32; 
    std::vector<pixCor> skipPixels;

    u_char* imgSt = &(img.getBuffer()[0]);
    int matches = 0;

    process(imgSt, msk.getWidth(), msk.getHeight(), skipPixels,
            img.getWidth(), img.getHeight(), msk.getBuffer().data(), blk, tol,
            lmtPrc, result, matches);

    // write the output image to png file
    result.write(argv[3]);
    return 0;
}
