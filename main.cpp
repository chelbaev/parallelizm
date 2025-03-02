#include <iostream>
#include <cmath>
#include <ctime>
#include <omp.h>

#define _USE_MATH_DEFINES
const double a = -4.0;
const double b = 4.0;
const int nsteps = 40000000;

double cpuSecond()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ((double)ts.tv_sec + (double)ts.tv_nsec * 1.e-9);
}

double func(double x)
{
    return exp(-x * x);
}

double integrate_omp(double (*func)(double), int nthreads)
{
    double h = (b - a) / nsteps;
    double sum = 0.0;

    #pragma omp parallel num_threads(nthreads)
    {
        int threadid = omp_get_thread_num();
        int items_per_thread = nsteps / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (nsteps - 1) : (lb + items_per_thread - 1);
        double current_sum = 0.0;
        for (int i = lb; i <= ub; i++){
            current_sum += func(a + h * (i + 0.5));
        }
        #pragma omp atomic
            sum += current_sum;
    }
    sum *= h;

    return sum;
}

double run_integrade(int nthreads){
    double t = cpuSecond();
    double res = integrate_omp(func, nthreads);
    printf("Result: %.12f; error %.12f\n", res, fabs(res - sqrt(M_PI)));
    t = cpuSecond() - t;
    return t;
}   

int main(){
    int nthreads;
    std::cin >> nthreads;
    printf("Integration f(x) on [%.12f, %.12f], nsteps = %d\n", a, b, nsteps);
    double tparallel = run_integrade(nthreads);

    printf("Execution time (parallel): %.6f\n", tparallel);
    return 0;
}
