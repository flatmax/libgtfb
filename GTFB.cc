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
#include <math.h>

#include "GTFB.H"
#include "perceptual.H"

GTFB::
GTFB(int lowF, int sampleF, int count){
  //  std::cout <<"GTFB: Init "<<this<<std::endl;

  if (!(CFEdges=new double[count+1]))
    std::cout<<"GTFB: malloc 1 error"<<std::endl;

  if (!(edgeFreq=new double[count]))
    std::cout<<"GTFB: edgeFreq malloc error"<<std::endl;

  // Init various var's ...
  lowFreq=lowF; highFreq=(int)rint((double)sampleF/2.0);
  T=1.0/(double)sampleF;

  for (int i=1; i<=count; i++){
    //    std::cout <<findCF(i, count)<<'\t';
    GT *tempPtr = new GT(findCF(i, count), T, i);
    if (tempPtr)
      add(tempPtr);
    else {
      std::cout << "GTFB: Couldn't create GT filter "<<i<<std::endl;
      exit(-1);
    }
  }

  // Set up the CFEdges ....
  grab(1);
  for (int i=0; i<=count; i++){
    if (i==0) CFEdges[i]=0.0;
    else if (i==count) CFEdges[i]=highFreq;
    else{
      CFEdges[i]=(prev()->cf+prev()->cf)/2.0;
      //      std::cout<<prev()->cf<<'\t';
      //std::cout<<prev()->cf<<'\t';
      next();
    }
    //    std::cout<<CFEdges[i]<<'\n';
  }
  //  std::cout<<std::endl;

  // Set up the edgeFreqs ....
  grab(1);prev();
  for (int i=0; i<count; i++){
    edgeFreq[i]=ERB2freq((freq2ERB(current()->cf)+freq2ERB(prev()->cf))/2.0);
  }
  edgeFreq[count-1]=highFreq;

  /*  std::cout <<"CentreFreqs, EdgeFreqs ..."<<std::endl;
  grab(1);
  for (int i=0; i<count; i++){
    std::cout<<prev()->cf<<'\t'<<edgeFreq[i]<<std::endl;
    }*/
}

GTFB::
~GTFB(){
  //  std::cout<<"GTFB: deconstructor"<<std::endl;
  if (CFEdges) delete [] CFEdges;
  if (edgeFreq) delete [] edgeFreq;
  GT *tempPtr;  
  while ((tempPtr=remove()))
    delete tempPtr;
  std::cout<<"GTFB: deconstructor ... done"<<std::endl;
}

#define EarQ 9.26449 //Glasberg and Moore Parameters
#define minBW 24.7
#define order 1.0
double GTFB::
findCF(int which, int count){
  return (-(EarQ*minBW) + exp((double)which*
			      (-log((double)highFreq + EarQ*minBW) +
			       log((double)lowFreq + EarQ*minBW))
			      /count)*(highFreq + EarQ*minBW));
}




