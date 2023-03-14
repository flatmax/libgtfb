#include "GTSensitivity.H"
#include <fstream.h>

int main(void){
  GTSensitivity gtsens;
  double CF=4096;
  double fs=16384;
  ofstream out("sens.dat");

  for (int i=0;i<fs/2;i++){
    //    cout<<i<<endl;
    out<<gtsens.find(i,CF,fs)<<'\n';
  }
  out.close();
}

