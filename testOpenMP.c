#include <omp.h>
#include <stdio.h>
#include <cmath>
#include <string>
// Integral limits [a;b]
const double a = 0;
const double b = Pi;
// Number of quantization steps in [a,b]
const int N = (500*1000*1000);
// Quantization step const double step = (b - a) / N;
// Integral evaluation function
static inline double f(double x)
{
return cos(x)*sin(x)*x*x + 2;
}
double evalIntegral()
{
double result = 0;
# pragma omp parallel for reduction(+:result) schedule(dynamic,(N/1000))
for(int i = 0; i < N; i++)
{
double x = a + i * step;
result += f(x) * step;
}
return result;
}
int main(int argc, char **argv)
{
printf("=> OpenMP\n");
printf("Number of quantization steps %iM\n", N / 1000000);
fflush(stdout);
# pragma omp parallel
{
# pragma omp critical
{
printf("Start working thread #%i of %i\n",
omp get thread num(), omp get num threads());
}
}
double result = evalIntegral();
printf("Result with OpenMP: %lf\n", result);
return 0;
}