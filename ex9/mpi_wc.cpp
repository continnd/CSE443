#include <mpi.h>
#include <iostream>


/* A simple MPI-based program in which each process prints could of
   words from subset of the files specified as command line parameters
   passed to the program.

   Each process utilizes a subset of files specified as command-line
   arguments and prints the count of words on the screen. If n command
   line arguemnts are provided and your program is run using k
   processes, then each one of the processes must process n/k of the
   command line arguments. That is, process with rank 0 will process
   the command line arguments 0 to n/k, and process with rank 1 will
   process the command line arguments n/k to 2n/k and so on.

   If the number of command line arguments is not evenly divisible
   then the last process should process the remaining command line
   parameters.
*/
int main(int argc, char *argv[]) {
    /* Determine the configuration in which the program is being run. */
    int numProcesses, myRank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    
    /* Using the above information print the subset of parameters that
       this process is responsible for. */
    int startArg = myRank   * (argc / numProcesses) + 1;
    int endArg   = startArg + (argc / numProcesses);
    
    /* Let the last process have any remaining parameters if the number
       of arguments is not evenly divisble. */
    if (myRank == (numProcesses - 1)) {
        endArg = argc;
    }
    
    if ((startArg < argc) && (startArg < endArg)) {
        /* Print the command line arguments as per requirement. */
        for(int i = startArg; (i < endArg); i++) {
            std::cout << "Rank[" << myRank     << "]: argv[" << i
                      << "] = " << argv[i]     << std::endl;
        }
    } else {
        std::cout << "Process with rank " << myRank
                  << " has no parameters to print.\n";
    }
    
    /* Finalize MPI */
    MPI_Finalize();
    
    return 0;
}
