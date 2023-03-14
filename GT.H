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
#ifndef GT_H_
#define GT_H_

/**
  OO implementation of Slaney's :
                  An Efficient Implementation of the
                      Patterson-Holdsworth 
                        Auditory Filter Bank
                                      Malcolm Slaney
                          Apple Computer Technical Report #35
                   Perception Group-Advanced Technology Group
                                <A9> 1993, Apple Computer, Inc.

  Author: flatmax
 */

#include <iostream>
#include <complex>
typedef std::complex< double > double_complex; 
using namespace std;

#define Pi M_PI
#define pi M_PI
#define Power(num1, num2) pow(num1, num2)
#define Cos cos
#define Sin sin
#define Sqrt sqrt

/// Implementation of a 2'nd order IIR filter
class _2ndOrderIIR {
  double in, out, z1, z2, gain;
  int bankNo;
public:
  double B;
  /// Feed forward and back coefficients
  double A0, A1, A2, B0, B1, B2;

  /// Must be called before 'filter', to re-init the filter
  void filterInit(){
    z1=z2=out=0.0;
  }

  /// Direct from II filter
  inline double filter(double input){
    // DIRECT FROM II
    out = A0*input          + z1;
    z1 =  A1*input - B1*out + z2;
    z2 =  A2*input - B2*out;
    return out;
  }
  /// Direct from II filter with feed forward gain
  inline double filter(double input, double g){
    gain=g;
    // DIRECT FROM II
    out = A0*input/gain          + z1;
    z1 =  A1*input/gain - B1*out + z2;
    z2 =  A2*input/gain - B2*out;
    return out;
  }

  /// Print filter coefficients
  friend std::ostream& operator <<(std::ostream& o, const _2ndOrderIIR& s) {
    o << "B, A0, A1, A2:"<<std::endl;
    o << s.B<<' '<<s.A0<<' '<< s.A1<<' '<< s.A2<<std::endl;
    o << "B0, B1, B2 :"<<std::endl;
    o << s.B0<<' '<< s.B1<<' '<< s.B2<<std::endl;
    return o;
  }
};

/// Implements a single GammaTone filter
class GT {
  /// The four cascaded 2'nd order filters
  _2ndOrderIIR zfilt1, zfilt2, zfilt3, zfilt4;
  double gain;
  double T;
  int bankNo;
public:
  double cf;

  /** Initialise specifying: b(constant), sample freq., lower freq. limit
                             and the total count of filters
   */
  GT(double cf1, double T1, int which){
    //  std::cout <<"GT: init "<<this<<std::endl;
    cf=cf1;
    T=T1;
    bankNo=which;
    // first filter ...
    zfilt1.B = 2.0*Pi*1.019*24.7*(4.37*cf/1000.0 + 1.0);
    zfilt1.A0 = T;
    zfilt1.A1 = -(2.0*T*Cos(2.0*cf*Pi*T)/exp(zfilt1.B*T) + 
		  2.0*Sqrt(3.0 + Power(2.0,3.0/2.0))*T*Sin(2.0*cf*Pi*T)/exp(zfilt1.B*T))/2.0;
    zfilt1.A2 = 0.0;
    zfilt1.B0 = 1.0;
    zfilt1.B1 = -2.0*Cos(2.0*cf*Pi*T)/exp(zfilt1.B*T);
    zfilt1.B2 = exp(-2.0*zfilt1.B*T);
    
    // second filter ...
    zfilt2.B = 2.0*Pi*1.019*24.7*(4.37*cf/1000.0 + 1.0);
    zfilt2.A0 = T;
    zfilt2.A1 = -(2.0*T*Cos(2.0*cf*Pi*T)/exp(zfilt2.B*T) - 
		  2.0*Sqrt(3.0 + Power(2.0,3.0/2.0))*T*Sin(2.0*cf*Pi*T)/exp(zfilt2.B*T))/2.0;
    zfilt2.A2 = 0.0;
    zfilt2.B0 = 1.0;
    zfilt2.B1 = -2.0*Cos(2.0*cf*Pi*T)/exp(zfilt2.B*T);
    zfilt2.B2 = exp(-2.0*zfilt2.B*T);
    
    // third filter ...
    zfilt3.B = 2.0*Pi*1.019*24.7*(4.37*cf/1000.0 + 1.0);
    zfilt3.A0 = T;
    zfilt3.A1 = -(2.0*T*Cos(2.0*cf*Pi*T)/exp(zfilt3.B*T) + 
		  2.0*Sqrt(3.0 - Power(2.0,3.0/2.0))*T*Sin(2.0*cf*Pi*T)/exp(zfilt3.B*T))/2.0;
    zfilt3.A2 = 0.0;
    zfilt3.B0 = 1.0;
    zfilt3.B1 = -2.0*Cos(2.0*cf*Pi*T)/exp(zfilt3.B*T);
    zfilt3.B2 = exp(-2.0*zfilt3.B*T);
    
    // fourth filter ...
    zfilt4.B = 2.0*Pi*1.019*24.7*(4.37*cf/1000.0 + 1.0);
    zfilt4.A0 = T;
    zfilt4.A1 = -(2.0*T*Cos(2.0*cf*Pi*T)/exp(zfilt4.B*T) - 
		  2.0*Sqrt(3.0 - Power(2.0,3.0/2.0))*T*Sin(2.0*cf*Pi*T)/exp(zfilt4.B*T))/2.0;
    zfilt4.A2 = 0.0;
    zfilt4.B0 = 1.0;
    zfilt4.B1 = -2.0*Cos(2.0*cf*Pi*T)/exp(zfilt4.B*T);
    zfilt4.B2 = exp(-2.0*zfilt4.B*T);
    
    //std::cout << zfilt1<<'\n'<<zfilt2<<'\n'<<zfilt3<<'\n'<<zfilt4<<std::endl;
  }

  ~GT(){}

  /// Finds the gain for this particular filter
  double findGain(){
    double B=zfilt1.B;
    
    double_complex g;
    // Gain for the first cascade stage ...
    g = abs((-2.0*exp(double_complex(0.0, 4.0*cf*pi*T))*T + 
	     2.0*exp(-(B*T) + double_complex(0.0, 2.0*cf*pi*T))*T* 
	     (cos(2.0*cf*pi*T) - sqrt(3.0 - pow(2.0,(3.0/2.0)))* 
	      sin(2.0*cf*pi*T))) *
	    
	    (-2.0*exp(double_complex(0.0, 4.0*cf*pi*T))*T + 
	     2.0*exp(-(B*T) + double_complex(0.0, 2.0*cf*pi*T))*T* 
	     (cos(2.0*cf*pi*T) + sqrt(3.0 - pow(2.0,(3.0/2.0))) * 
	      sin(2.0*cf*pi*T))) *
	    
	    (-2.0*exp(double_complex(0.0 ,4.0*cf*pi*T))*T + 
	     2.0*exp(-(B*T) + double_complex(0.0, 2.0*cf*pi*T))*T* 
	     (cos(2.0*cf*pi*T) - 
	      sqrt(3.0 + pow(2.0,(3.0/2.0)))*sin(2.0*cf*pi*T))) * 
	    (-2.0*exp(double_complex(0,4.0*cf*pi*T))*T + 2.0*exp(-(B*T) + double_complex(0,2.0*cf*pi*T))*T* 
	     (cos(2.0*cf*pi*T) + sqrt(3.0 + pow(2.0,(3.0/2.0)))*sin(2.0*cf*pi*T))) / 
	    pow((-2.0 / exp(2.0*B*T) - 
		 2.0*exp(double_complex(0, 4.0*cf*pi*T)) +  
		 2.0*(1.0 + exp(double_complex(0, 4.0*cf*pi*T))
		      )/exp(B*T)),4.0)
	    );
    //std::cout <<g<<std::endl;
    return gain=g.real();
  }

  /// Fills the output pointer with sampleCount samples 
  void impulseResponse(double *output, int sampleCount){
    //Set up state data variables ...
    //  double gain=1.084e-13;
    zfilt1.filterInit(); zfilt2.filterInit();
    zfilt3.filterInit(); zfilt4.filterInit();
    findGain();
    
    double in=1.0;
    output[0]=zfilt4.filter(zfilt3.filter(zfilt2.filter(zfilt1.filter(in, gain))));
    in=0.0;
    // Step through sample by sample ...
    for (int i=1; i<sampleCount; i++)
      output[i]=zfilt4.filter(zfilt3.filter(zfilt2.filter(zfilt1.filter(in))));
  }

  /// Inits the filter for sample by sample mode
  void filterInit(void){
    zfilt1.filterInit();
    zfilt2.filterInit();
    zfilt3.filterInit();
    zfilt4.filterInit();
    findGain();
  }
  
  /// Fills the output pointer with sampleCount filtered samples
  /// Leaves the filter state intact
  void filterN(const double *input, double *output, int sampleCount){
    // Step through sample by sample ...
    for (int i=0; i<sampleCount; i++)
      output[i]=zfilt4.filter(zfilt3.filter(zfilt2.filter(zfilt1.filter(input[i], gain))));
  }
  
  /// Fills the output pointer with sampleCount filtered samples
  /// Re-inits the filters with each call
  void filter(double *input, double *output, int sampleCount){
    //Set up state data variables ...
    //  double gain=1.084e-13;
    zfilt1.filterInit(); zfilt2.filterInit();
    zfilt3.filterInit(); zfilt4.filterInit();
    findGain();
    
    // Step through sample by sample ...
    for (int i=0; i<sampleCount; i++)
      output[i]=zfilt4.filter(zfilt3.filter(zfilt2.filter(zfilt1.filter(input[i], gain))));
    /*
      output[0]=zfilt4.filter(zfilt3.filter(zfilt2.filter(zfilt1.filter(input[0], gain))));
      // Step through sample by sample ...
      for (int i=1; i<sampleCount; i++)
      output[i]=zfilt4.filter(zfilt3.filter(zfilt2.filter(zfilt1.filter(input[i]))));
    */
  }
};
#endif //GT_H_
