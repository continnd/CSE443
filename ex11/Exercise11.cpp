// Copyright (C) Miami University 2015

#include <mpi.h>
#include <cmath>
#include <iostream>

// Forward declarations to keep compiler happy
void doWorker(const int myRank, const int numProcs);
double getArea(const int totalRects, const int myRank, const int numProcs);
void doManager(const int myRank, const int numProcs);

// In the exercise, you are required to complete the doWorker method
// below so that it operates correctly with the doManager method. A
// skeleton code has been supplied to streamline implementation in
// this exercise.
void doWorker(const int myRank, const int numProcs) {
    int totalRects = 0;
    do {
        // Obtain value of totalRects from manager
        MPI_Bcast(&totalRects, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // If totalRect's value is not zero, then:
        if (totalRects != 0) {
            // 1.  compute local area value by calling the getArea()
            //     method (already implemented).

            double myArea = getArea(totalRects, myRank, numProcs);
            // 2.  Send local area value back as part of reduction operation.
            double totalArea;
            MPI_Reduce(&myArea, &totalArea, 1, MPI_DOUBLE, MPI_SUM, 0,
                    MPI_COMM_WORLD);
        }
    } while (totalRects != 0);
}

//--------------------------------------------------------------
//     DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
//--------------------------------------------------------------

const double EXPECTED_AREA = 666.666666;

void doManager(const int myRank, const int numProcs) {
    int totalRects = 0;
    do {
        std::cout << "Enter the number of intervals (0 to quit): ";
        std::cin >> totalRects;
        // Send value of totalRects to all processes
        MPI_Bcast(&totalRects, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (totalRects == 0) {
            break;
        }
        double totalArea = 0;
        double myArea    = getArea(totalRects, myRank, numProcs);
        // add all the results from various processes.
        MPI_Reduce(&myArea, &totalArea, 1, MPI_DOUBLE, MPI_SUM, 0,
                MPI_COMM_WORLD);
        if (myRank == 0) {
            std::cout << "Area is approximately " << totalArea
                << ", Error is " << std::abs(totalArea - EXPECTED_AREA)
                << std::endl;
        }
    } while (totalRects != 0);
}

double getArea(const int totalRects, const int myRank, const int numProcs) {
    const int numRects     = totalRects / numProcs;
    const double rectWidth = 20.0 / totalRects;
    const double startX    = -10.0 + (myRank * numRects * rectWidth);
    double sum             = 0;
    int i;
    for (i = 0; (i < numRects); i++) {
        double x = startX + (i * rectWidth);
        sum += (x * x);
    }
    const double total = sum * rectWidth;
    std::cout << "sum on rank " << myRank << " is " <<  total << std::endl;
    return total;
}

int main(int argc, char *argv[]) {
    int myRank, numProcs;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0) {
        doManager(myRank, numProcs);
    } else {
        doWorker(myRank, numProcs);
    }
    MPI_Finalize();
    return 0;
}
// End of source code

