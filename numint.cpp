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

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

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
  
  if (argc < 9) {
    std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  int a = atoi(argv[2]);
  int b = atoi(argv[3]);
  long n = atoi(argv[4]);
  
  int nbthreads = atoi(argv[6]);
  std::string scheduling = argv[7];
  int granularity = atoi(argv[8]);

  float result = 0;

  double intermediateResult = (b - a)/(float)n;

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  if(scheduling == "static")
        omp_set_schedule(omp_sched_static,granularity);
      else if(scheduling == "dynamic")
        omp_set_schedule(omp_sched_dynamic,granularity);
      else if(scheduling == "guided")
        omp_set_schedule(omp_sched_guided,granularity);
      else std::cerr << "Invalid scheduling options entered" << std::endl;

  #pragma omp parallel num_threads(nbthreads)
    {    
    #pragma omp for reduction(+:result) schedule(runtime)
      for(long j=0; j<n ; j++){
      double temp = a + ((j+0.5) * intermediateResult);
    
      switch(atoi(argv[1])){
        case 1: result =result + (f1(temp, atoi(argv[5]))) * intermediateResult;
        break;
        case 2: result = result + (f2(temp, atoi(argv[5]))) * intermediateResult;
          break;
        case 3: result = result + (f3(temp, atoi(argv[5]))) * intermediateResult;
            break;
        case 4: result = result + (f4(temp, atoi(argv[5]))) * intermediateResult;
            break;
        default:std::cerr<<"Invalid parameters passed"<<std::endl;        
        break;
        }
      }
    }

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  std::cout << result << std::endl;
  return 0;
}
