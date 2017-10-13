#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
  
  
#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {

  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }
  
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int * arr = new int [atoi(argv[1])];
  int * pr = new int [atoi(argv[1])];
  generatePrefixSumData (arr, atoi(argv[1]));
  
  //write code here
  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);

   // for(int k=0 ; k<n ; k++)
   //     arr[k]=1;

   std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  float *suma;
    #pragma omp parallel num_threads(nbthreads)
    {
        const int ithread = omp_get_thread_num();
        //const int nthreads = omp_get_num_threads();
        #pragma omp single
        {
            suma = new float[nbthreads+1];
            suma[0] = 0;
        }
        float sum = 0;
        #pragma omp for schedule(static)
        for (int i=0; i<n; i++) {
            sum += arr[i];
            pr[i] = sum;
        }
        suma[ithread+1] = sum;
        #pragma omp barrier
        float offset = 0;
        for(int i=0; i<(ithread+1); i++) {
            offset += suma[i];
        }
        #pragma omp for schedule(static)
        for (int i=0; i<n; i++) {
            pr[i] += offset;
        }
    }
    delete[] suma;

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cerr<<elapsed_seconds.count()<<std::endl;

  //for(int i=0; i<n; i++) printf("%d %f\n", pr[i], 0.5*i*(i+1));
  
  checkPrefixSumResult(pr, atoi(argv[1]));
  
  delete[] arr;

  return 0;
}
