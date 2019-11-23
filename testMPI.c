#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
// Integral limits [a;b]
const double a = 0;
const double b = pi;
// Number of quantization steps in [a,b]
const int N = (500*1000*1000);
// Quantization step
const double step = (b - a) / N;
// Integral evaluation function
static inline double f(double x)
{
    return cos(x)*sin(x)*x*x + 2;
}
double evalIntegral(int start, int end)
{
    double result = 0;
    for (int i = start; i < end; i++)
    {
        double x = a + i * step;
        result += f(x) * step;
    }
    return result;
}
int main (int argc, char* argv[])
{
MPI Init(&argc, &argv);
int rank;
int sizeOfWorld;
MPI Status mpiStatus;
MPI Comm rank(MPI COMM WORLD, &rank);
MPI Comm size(MPI COMM WORLD, &sizeOfWorld);
static const int MasterRank = 0;
if (rank == MasterRank)
{
// Master
printf("=> MPI\n");
fflush(stdout);
}
printf("Start working\n");
fflush(stdout);
int numOfWorkers = sizeOfWorld;
int start = N / numOfWorkers * rank;
int end = start + N / numOfWorkers;
double result = evalIntegral(start, end);
printf("Job is done! [%i; %i] Result = %lf\n", start, end, result);
fflush(stdout);
if (rank != MasterRank)
{
// Workers send results to master
MPI Send(&result, 1, MPI DOUBLE, MasterRank, 0, MPI COMM WORLD);
}
// Master collect results
if (rank == MasterRank)
{
double resultFromWorker = 0;
for (int id = 1; id < sizeOfWorld; id++)
{
MPI Recv(&resultFromWorker, 1, MPI DOUBLE, id,
MPI ANY TAG, MPI COMM WORLD, &mpiStatus);
result += resultFromWorker;
}
printf("Result with MPI: %lf\n", result);
}
MPI Finalize();
return 0;
}