#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string>

// Copyright continnd@miamioh.edu 2015

const int NUMBER_TAG  = 1;
const int FACTORS_TAG = 2;

const int MANAGER_RANK = 0;
const int WORKER_RANK  = 1;

/**
   This is a simple function that can be used to determine the number
   of factors for a given integer i.
*/
int getFactorCount(const long long n) {
    long long i = 0;
    int factorCount = 0;
    
    for (i = 1; (i <= n); i++) {
        if (!(n % i)) {
            factorCount++;
        }
    }
    return factorCount;
}

void doWorkerTasks() {
    long long number;
    do {
        /* read number from the manager (rank == 0) */
        MPI_Recv(&number, 1, MPI_LONG_LONG_INT, MANAGER_RANK, NUMBER_TAG,
                 MPI_COMM_WORLD, NULL);
        if (number != -1) {
            int factors[2] = {number, getFactorCount(number)};
            /* send number of factors back to the manager */
            MPI_Send(factors, 2, MPI_INT, MANAGER_RANK, FACTORS_TAG,
                     MPI_COMM_WORLD);
        }
    } while (number != -1);
}

void doManagerTasks(const int size, std::string filename) {
    MPI_Status status;
    std::ifstream data(filename);
    int working = 0;
    if (!data.good()) {
        std::cout << "Unable to open numbers.txt for input. "
            << "Manager aborting.\n";
    } else {
        long long number;
        int factors[2];
        // Send number to be converted to the worker
        for (int i = 1; i < size && data >> number; i++) {
            working++;
            MPI_Send(&number, 1, MPI_LONG_LONG_INT, i, NUMBER_TAG,
                    MPI_COMM_WORLD);
        }
        while (data >> number) {
            MPI_Recv(&factors, 2, MPI_INT, MPI_ANY_SOURCE, FACTORS_TAG,
                    MPI_COMM_WORLD, &status);
            std::cout << "Number " << factors[0] << " has " << factors[1]
                << " factors.\n";
            MPI_Send(&number, 1, MPI_LONG_LONG_INT, status.MPI_SOURCE,
                    NUMBER_TAG, MPI_COMM_WORLD);
        }
        number = -1;
        for (int i = 0; i < working; i++) {
            MPI_Recv(&factors, 2, MPI_INT, MPI_ANY_SOURCE, FACTORS_TAG,
                    MPI_COMM_WORLD, &status);
            std::cout << "Number " << factors[0] << " has " << factors[1]
                << " factors.\n";
            MPI_Send(&number, 1, MPI_LONG_LONG_INT, status.MPI_SOURCE,
                    NUMBER_TAG, MPI_COMM_WORLD);
        }
        for (int i = working - 1; i < size; i++) {
            MPI_Send(&number, 1, MPI_LONG_LONG_INT, i, NUMBER_TAG,
                    MPI_COMM_WORLD);
        }
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    if (argc != 2) {
        std::cout << "There must be one parameter (filename)." << std::endl;
        return -1;
    }
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        std::cerr << "This program must be run with at least 2 processes!\n";
    } else {
        int rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank == 0) {
            doManagerTasks(size, argv[1]);
        } else {
            doWorkerTasks();
        }
    }

    MPI_Finalize(); 
    return 0;
}

// End of source code.
