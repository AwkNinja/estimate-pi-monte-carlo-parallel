// set the threads using `set OMP_NUM_THREADS=<no. of threads>`
// run the program using `gcc -o test -fopenmp estimate_pi.c` then `./test`
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main(int argc, char *argv[])
{
    long long int TOSSES = 10000000000; // Number of samples to use in the Monte Carlo simulation
    long long int toss;
    long long int num_in_circle = 0;
    int serial_time = 487.843000;

    omp_set_num_threads(16); // set number of threads
    double start_time = omp_get_wtime();

    // Seed the random number generator
    srand(time(NULL));

#pragma omp parallel
    {
        long long int num_circle = 0;

#pragma omp for
        for (toss = 0; toss < TOSSES; toss++)
        {
            // Generate a random point between -1 to 1
            double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
            double y = (double)rand() / RAND_MAX * 2.0 - 1.0;

            // Check if the point lies within the unit circle
            if (x * x + y * y <= 1.0)
            {
                num_circle++;
            }
        }

#pragma omp critical
        num_in_circle += num_circle;
    }

    // Estimate the value of pi
    double pi_estimate = 4.0 * (double)num_in_circle / (TOSSES);
    printf("Estimated value of num_in_circle: %lld \n", num_in_circle);
    printf("Estimated value of pi: %1.20f \n", pi_estimate);

    double elapsed_time = omp_get_wtime() - start_time;
    printf("Elapsed time: %f seconds\n", elapsed_time);

    // calculating speed-up
    double speedup = serial_time / elapsed_time;
    printf("Speed-up: %f\n", speedup);

    return 0;
}
