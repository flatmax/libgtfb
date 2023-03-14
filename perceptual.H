/***************************************************************************
 *   Copyright (C) 1999 by Matt R. Flax (flatmax@)                         *
 *   flatmax@flatmaxstudios.com                                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef PERCEPTUAL_H_
#define PERCEPTUAL_H_

#include <iostream>
#include <math.h>
#include <stdlib.h>

//#define C1 24.673
#define C2 4.368
//C3=(2302.6/(c1 * c2));
#define C3 21.366

double 
freq2ERB(double freq){
  return (C3*log10((C2 * freq/1000.0) + 1.0));
}
double
ERB2freq(double erb){
  return 1000.0 * (pow(10.0,(erb/C3)) - 1.0) / C2;
}

class PerceptualIndex {
  int *indexArray;
  int size, fs;

  void indexMap(){
    double highERB=freq2ERB((double)fs/2.0);
    double factor=highERB/(double)size;
    double scale=size/(fs/2.0);
    int index=0;
    for (double i=0.0;i<highERB;i+=factor){ //Find the ERBs
      indexArray[index++]=(int)rint(scale*ERB2freq(i));
    }

    /*//check
    index=0;
    for (double i=0.0;i<highERB;i+=factor){
      std::cout<<index<<'\t'<<indexArray[index]<<std::endl;
      index++;
      }*/
  }
public:
  /// Perceptual Index requires the size of the desired array and the samplefreq.
  PerceptualIndex(int sz, int fs1){
    size=sz; fs=fs1;
    indexArray=NULL;
    if (!(indexArray=new int[size])){
      std::cerr<<"PerceptualIndex::PerceptualIndex malloc error"<<std::endl;
      exit(-1);
    }
    indexMap();
  }
  ~PerceptualIndex(){
    if (indexArray) delete [] indexArray;
  }

  int operator[](int i){return indexArray[i];}
};
#endif //PERCEPTUAL_H_



