#include <iostream>
#include <cmath>
#include <cstdlib>
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

float sequential_integrate (int argc, char* argv[]);

  
int main (int argc, char* argv[]) {

  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    
  float integrate = sequential_integrate(argc, argv);
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;
  
  std::cout<<integrate<<std::endl;
  
  std::cerr<<elapsed_seconds.count()<<std::endl;
  
  return 0;
}


float sequential_integrate (int argc, char* argv[]){

  int a = atoi(argv[2]);
  int b = atoi(argv[3]);
  double n = atoi(argv[4]);
  float result = 0;
  
  if (atoi(argv[1]) < 1 || atoi(argv[1]) > 4) {
    std::cerr<<"Invalid parameters passed: functionId should be within 1 and 4"<<std::endl;
    return -1;
  }

  if (b < a) {
    std::cerr<<"Invalid parameters passed: b should be greater than a"<<std::endl;
    return -1;
  }
  double intermediateResult = (b - a)/(float)n;
    for(int i=0; i<n ; i++){
    double temp = a + ((i+0.5) * intermediateResult);
    
    switch(atoi(argv[1])){
      case 1: result =result + (f1(temp, atoi(argv[5]))) * intermediateResult;
      break;
      case 2: result = result + (f2(temp, atoi(argv[5]))) * intermediateResult;
            break;
      case 3: result = result + (f3(temp, atoi(argv[5]))) * intermediateResult;
            break;
      case 4: result = result + (f4(temp, atoi(argv[5]))) * intermediateResult;
            break;
      default:std::cerr<<"Invalid parameters pased"<<std::endl;
        return -1; 
      break;
    }
  }
  return result;
}
