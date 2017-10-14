#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

  
#ifdef __cplusplus
}
#endif

void merge(int *a, long l, long m, long r);
long min2(long a1, long a2);

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

  generateMergeSortData (arr, atoi(argv[1]));
  
  //write code here

  long n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  std::string scheduling = argv[3];
  int granularity = atoi(argv[4]);

  omp_set_num_threads(nbthreads);

  if(scheduling == "static")
        omp_set_schedule(omp_sched_static,granularity);
      else if(scheduling == "dynamic")
        omp_set_schedule(omp_sched_dynamic,granularity);
      else if(scheduling == "guided")
        omp_set_schedule(omp_sched_guided,granularity);
      else std::cerr << "Invalid scheduling options entered" << std::endl;

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  for (long len=1; len<=n-1; len = 2*len)
  {
    #pragma omp parallel for schedule(runtime)
    for (long ls=0; ls<n-1; ls += 2*len)
    {
      long mid = min2(ls+len-1, n-1);
      long right = min2(ls+(2*len)-1, n-1);
      merge(arr, ls, mid, right);
    }
  }
     
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  checkMergeSortResult (arr, atoi(argv[1]));
  
  delete[] arr;

  return 0;
}

long min2(long a1, long a2)
{
  return a1 < a2 ? a1 : a2;
}

void merge(int *a, long l, long m, long r)
{
  long n1 = m - l + 1, n2 = r - m;

  // Creating left and right arrays
  int L[n1], R[n2];
  for (int i = 0; i < n1; i++)
  {
    L[i] = a[l + i]; // from l to l+n1-1
  }
  for (int j = 0; j < n2; j++)
  {
    R[j] = a[m + 1+ j]; // from m+1 to m+n2
  }

  
  long i = 0, j = 0, k = l;

  // Merging 2 arrays of length n1 & n2
  while (i < n1 && j < n2)
  {
    if (L[i] <= R[j])
    {
      a[k++] = L[i++];
    }
    else
    {
      a[k++] = R[j++];
    }
  }

  if(j == n2) // in case R finishes first, leftover elements of L will get inserted as it is
  {
    while (i < n1)
    {
      a[k++] = L[i++];
    }
  }
  else if(i == n1) // in case L finishes first, leftover elements of R will get inserted as it is
  {
    while (j < n2)
    {
      a[k++] = R[j++];
    }
  }
  return;
}
