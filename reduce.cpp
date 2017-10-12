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

  void generateReduceData (int* arr, size_t n);
  
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
  
  if (argc < 5) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }

  int * arr = new int [atoi(argv[1])];

  generateReduceData (arr, atoi(argv[1]));
  
  //write code here
  // for(int j=0 ; j<20 ; j++)
  //   std::cout << arr[j] << std::endl;
  long result=0;
  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  std::string scheduling = argv[3];
  int granularity = atoi(argv[4]);
  //printf("GRanularity = %d\n", granularity);
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  // omp_set_dynamic(0); 
  // omp_set_num_threads(nbthreads);
  
  #pragma omp parallel num_threads(nbthreads)
    {

      if(scheduling == "static")
        omp_set_schedule(omp_sched_static,granularity);
      else if(scheduling == "dynamic")
        omp_set_schedule(omp_sched_dynamic,granularity);
      else if(scheduling == "guided")
        omp_set_schedule(omp_sched_guided,granularity);
      else std::cerr << "Invalid scheduling options entered" << std::endl;

    #pragma omp for reduction(+:result) schedule(runtime)
      for(int j=0 ; j<n ; j++)
      result+= arr[j];
    }

/*  if(scheduling == "static"){
    #pragma omp parallel num_threads(nbthreads)
    {
    #pragma omp for schedule(static,granularity) reduction(+:result)
      for(int j=0 ; j<n ; j++)
      result+= arr[j];
    }
  }
  else if(scheduling == "dynamic"){
    #pragma omp parallel num_threads(nbthreads)
    {
    #pragma omp for schedule(dynamic,granularity) reduction(+:result)
      for(int j=0 ; j<n ; j++)
      result+= arr[j];
    }
  }
  else if(scheduling == "guided"){
    #pragma omp parallel num_threads(nbthreads)
    {
    #pragma omp for schedule(guided) reduction(+:result)
      for(int j=0 ; j<n ; j++)
      result+= arr[j];
    }
  }
  else std::cerr << "Invalid scheduling options entered" << std::endl;*/
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  std::cout << result << std::endl;

  //printf("Sum of the array = %ld\n",result);
  delete[] arr;

  return 0;
  }


/*void generateReduceData (int* arr, size_t n){
  int* array = arr;
  long arraySize = n;
  srand(time(NULL));
  //int[arraySize] array= {0};
  for (int i=0 ; i<arraySize ; i++){
    //array[i] = 1;
    array[i] = rand() % arraySize;
    
  }

}*/