#include <mpi.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include "HTMLDownloader.h"

#define MAX_LENGTH 15

// Copyright 2015 continnd@miamioh.edu

// Used to find the minimum of three numbers.
int minCorners(const int num1, const int num2, const int num3) {
    int winner = std::min(num1, num2);
    return std::min(winner, num3);
}

// Calculates the Levenshtein distance between two strings
int levenshteinDist(const std::string& str1, const std::string& str2) {
    int m[str1.size()][str2.size()];

    // Initialize memoization table
    m[0][0] = (str1[0] != str2[0]);

    // This boolean is used to keep track of whether or not a match has been
    // made during initialization. This is because you cannot match a letter
    // in one string with more than one letter in the other string.
    bool matchInc = str1[0] == str2[0];

    for (size_t i = 1; i < str1.size(); i++) {
        m[i][0] = m[i-1][0] + (str1[i] != str2[0] || matchInc);
        if (!matchInc && str1[i] == str2[0]) {
            matchInc = true;
        }
    }
    matchInc = str1[0] == str2[0];
    for (size_t i = 1; i < str2.size(); i++) {
        m[0][i] = m[0][i - 1] + (str1[0] != str2[i] || matchInc);
        if (!matchInc && str1[0] == str2[i]) {
            matchInc = true;
        }
    }

    // fill out the rest of the table
    for (size_t i = 1; i < str1.size(); i++) {
        for (size_t j = 1; j < str2.size(); j++) {
            // the table is used by either grabbing the top left adjacent
            // square when the letters match or by choosing the minimum of the
            // three available table values and adding one.
            m[i][j] = str1[i] == str2[j] ? m[i - 1][j- 1] :
                minCorners(m[i - 1][j- 1], m[i][j - 1], m[i - 1][j]) + 1;
        }
    }
    return m[str1.size() - 1][str2.size() - 1];
}

// A method used to print the results.
void printOutput(int* output, std::string url,
        std::vector<std::string> checkWords) {
    std::cout << "Statistics for " << url << ":" << std::endl;
    std::cout << "Using words: " << std::flush;
    for (std::string word : checkWords) {
        std::cout << word << " " << std::flush;
    }
    std::cout << std::endl;
    std::cout << "Words with distance=0: " << output[0] << std::endl;
    std::cout << "Words with distance=1: " << output[1] << std::endl;
    std::cout << "Words with distance=2: " << output[2] << std::endl;
    std::cout << "Words with distance=3: " << output[3] << std::endl;
    std::cout << "Words with distance=4: " << output[4] << std::endl;
}

// Used by the manager to load words into the sendBuff from a stream based on
// the number of processes.
void extractWords(char* sendBuff, std::istream& is, int numProcs) {
    for (int i = 0; i < numProcs; i++) {
        std::string temp;
        // The no operation string when the stream runs out
        if (!(is >> temp)) {
            temp = "";
        }
        // Outputs the string into the buffer but makes sure not to go out of
        // bounds
        for (int j = 0; j < MAX_LENGTH; j++) {
            sendBuff[i * MAX_LENGTH + j] = j < temp.size() ? temp[j] : '\0';
        }
    }
}

// A loop designed to make the probe through all the works to check against
// and call the levenshteinDistance method.
void checkLev(std::vector<std::string> checkWords, int* count,
        std::string& work) {
    for (std::string word : checkWords) {
        int levDist = levenshteinDist(word, work);
        if (levDist < 5) {
            count[levDist]++;
        }
    }
}

// Instructions for the manager.
// is : a stream of words from the url
// numProcs : number of processes running
// checkWords : the words to be check the stream against
// url : the url to be checked
void manager(std::istream& is, const int numProcs,
        std::vector<std::string>& checkWords, std::string& url) {
    char sendBuff[numProcs * MAX_LENGTH], recvBuff[MAX_LENGTH];
    int count[5] = {0, 0, 0, 0, 0};

    // Don't stop until the end of the string.
    while (!is.eof()) {
        extractWords(sendBuff, is, numProcs);
        MPI_Scatter(sendBuff, MAX_LENGTH, MPI_CHAR, recvBuff, MAX_LENGTH,
                MPI_CHAR, 0, MPI_COMM_WORLD);
        std::string work = "";

        // Creates a string from the receive buffer, but makes sure to keep
        // everything lowercase
        for (int i = 0; i < MAX_LENGTH && recvBuff[i] != '\0'; i++) {
            work += (recvBuff[i] >= 'A' && recvBuff[i] <= 'Z') ? recvBuff[i] +
                32 : recvBuff[i];
        }
        checkLev(checkWords, count, work);
    }
    // Sends out reduction command
    for (int i = 0; i < MAX_LENGTH * numProcs; i++) {
        sendBuff[i] = (i % 15 == 0) ? ' ' : '\0';
    }
    MPI_Scatter(sendBuff, MAX_LENGTH, MPI_CHAR, recvBuff, MAX_LENGTH,
            MPI_CHAR, 0, MPI_COMM_WORLD);
    int totalLev[5] = {0, 0, 0, 0, 0};
    MPI_Reduce(&count, &totalLev, 5, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    printOutput(totalLev, url, checkWords);
}

void worker(std::vector<std::string>& checkWords) {
    char recvBuff[MAX_LENGTH], *sendBuff;
    int count[5] = {0, 0, 0, 0, 0};
    while (true) {
        MPI_Scatter(sendBuff, MAX_LENGTH, MPI_CHAR, recvBuff, MAX_LENGTH,
                MPI_CHAR, 0, MPI_COMM_WORLD);
        std::string work = "";

        // Creates a string from the receive buffer, but makes sure to keep
        // everything lowercase
        for (int i = 0; i < MAX_LENGTH && recvBuff[i] != '\0'; i++) {
            work += (recvBuff[i] <= 'Z' && recvBuff[i] >= 'A') ? recvBuff[i] +
                32 : recvBuff[i];
        }

        // Goes to reduction
        if (work[0] == ' ') {
            break;
        }

        // Don't do anything during no operation command
        if (work != "") {
            checkLev(checkWords, count, work);
        }
    }
    int totalLev[5] = {0, 0, 0, 0, 0};
    MPI_Reduce(&count, &totalLev, 5, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
}

// Sorts the arguments into either urls or words to be checked against
void parseArgs(int argc, char* argv[], std::vector<std::string>& urls,
        std::vector<std::string>& checkWords) {
    for (int i = 1; i < argc; i++) {
        std::string temp(argv[i]);
        if (temp.find("http") == 0) {
            urls.push_back(temp);
        } else {
            checkWords.push_back(temp);
        }
    }
}

int main(int argc, char* argv[]) {
    int myRank, numProcs;
    std::vector<std::string> checkWords, urls;
    parseArgs(argc, argv, urls, checkWords);

    // Ends program when there are not enough arguments of each type
    if (!checkWords.size() || !urls.size()) {
        std::cout << "Error: Not enough URLs or seach words." << std::endl;
        return -1;
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // Print output for each url
    for (std::string url : urls) {
        if (myRank == 0) {
            HTMLDownloader src(url);
            std::istream& words = src.getText();
            manager(words, numProcs, checkWords, url);
        } else {
            worker(checkWords);
        }
    }
    MPI_Finalize();
    return 0;
}
