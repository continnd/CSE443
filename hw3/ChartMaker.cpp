// Copyright (C) 2014 continnd@miamiOH.edu
#include <string>
#include "continndChart.h"
#include "ChartMaker.h"

int temp, temp1;
std::string response;
std::ofstream os;
char state = '0';

void printMenu() {
    std::cout << "All commands are single characters." << std::endl;
    std::cout << "Valid commands are:" << std::endl;
    std::cout << "l: Load data from file as current set of points"
        << std::endl;
    std::cout << "+: add data from another to to current points"
        << std::endl;
    std::cout
        << "-: subtract data from another file from current poitns"
        << std::endl;
    std::cout << "*: scale current points by given pair of values"
        << std::endl;
    std::cout << "/: scale current points by given pair of values"
        << std::endl;
    std::cout << "?: print histogram current point distribution"
        << std::endl;
    std::cout << "<: shift points to left by a given value."
        << std::endl;
    std::cout << ">: shift points to right by a given value."
        << std::endl;
    std::cout << "s: Save the current set of points to a given file"
        << std::endl;
    std::cout << "p: Print current set of points on screen."
        << std::endl;
    std::cout << "h: Print this message\nq: Quit" << std::endl;
}

void retrieveScale() {
    std::cout << "Enter values to scale x & y coordinates: "
        << std::flush;
    std::cin >> temp;
    std::cin >> temp1;
}

void getFilePath() {
    std::cout << "Enter path to file: " << std::flush;
    std::cin >> response;
}

void getShift() {
    std::cout << "Enter positions to shift: " << std::flush;
    std::cin >> temp;
}

void menu1(char state, Chart& current) {
        switch (state) {
            case 'l':
                getFilePath();
                current = Chart(response);
                break;
            case '+':
                getFilePath();
                current = current + Chart(response);
                break;
            case '-':
                getFilePath();
                current = current - Chart(response);
                break;
            case '*':
                retrieveScale();
                current = current * Point(temp, temp1);
                break;
            default:
                break;
        }
}

void menu2(char state, Chart& current) {
        switch (state) {
            case '/':
                retrieveScale();
                current = current / Point(temp, temp1);
                break;
            case '?':
                std::cout << "Enter max size for histogram: " << std::flush;
                std::cin >> temp;
                current.analyze(std::cout, temp);
                break;
            case '<' :
                getShift();
                current = current << temp;
                break;
            default:
                break;
        }
}

void menu3(char state, Chart& current) {
        switch (state) {
            case '>' :
                getShift();
                current = current >> temp;
                break;
            case 's' :
                getFilePath();
                os.open(response);
                os << current << std::flush;
                os.close();
                break;
            case 'p':
                std::cout << current << std::endl;
                break;
            case 'h':
                printMenu();
                break;
            default:
                break;
        }
}

int ChartMaker::run() {
    while (state != 'q') {
        menu1(state, current);
        menu2(state, current);
        menu3(state, current);
        std::cout << "Enter command (h for help): " << std::flush;
        std::cin >> state;
    }
    return 0;
}
