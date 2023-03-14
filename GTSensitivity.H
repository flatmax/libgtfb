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
#ifndef GTSENSITIVITY_H_
#define GTSENSITIVITY_H_

#include <iomanip.h>
#include <math.h>

//Values determined from experiment using linear freq scale
#define FS 8192
#define HALFFS FS/2
#define ML 6.1100009523745203721e-02
#define BL -1.6546663009550019297e+02
#define STARTBINL 3940

#define MU -1.0656799951863307196e+00
#define BU 2.3845831655853703523e+02
#define STARTBINU 110

//Values determined from experiment using ERB scale
/* Lower edge FS=8192
a =    [1.4305e-02
        -1.2094e-01
        3.2325e+00
        -2.8171e+02]

octave:16> plot(erbx.^3*a(1)+erbx.^2*a(2)+erbx*a(3)+a(4))
octave:17> plot(erbx,erbx.^3*a(1)+erbx.^2*a(2)+erbx*a(3)+a(4))
*/

/*Upper edge FS=8192
b =

   -8.7337e-03
    2.5070e-01
   -6.5244e+00
    1.3632e+01
    1.9465e+01

octave:52> plot(erbx,erbx.^4*b(1)+erbx.^3*b(2)+erbx.^2*b(3)+erbx*b(4)+b(5))
octave:53> hold on
octave:54> plot(erbx(100:4096),w(1,100:4096))
 */

class GTSensitivity {
  //Lower freq. first order y=mx+b
  double *lower;
  //Higher freq. first order y=mx+b
  double *upper;
public:
  GTSensitivity(void){
    lower=upper=NULL;
    if (!(lower=new double[HALFFS])){
      cerr<<"GTSensitivity::GTSensitivity : lower malloc error"<<endl;
      exit(-1);
    }
    if (!(upper=new double[HALFFS])){
      cerr<<"GTSensitivity::GTSensitivity : lower malloc error"<<endl;
      exit(-1);
    }

    for (int i=0;i<HALFFS;i++){
      lower[i]=(double)i*ML+BL;
      upper[i]=(double)i*MU+BU;
    }
  }

  ~GTSensitivity(void){
    if (lower) delete [] lower;
    if (upper) delete [] upper;
  }

  double find(int whichBin, double CFBin, int sampFreq){
    //    if (whichBin==(int)rint(CFBin)) return 1.0;
    double factor=(double)FS/(double)sampFreq;
    if (whichBin<=CFBin){
      int index=STARTBINL-(int)rint((CFBin-(double)whichBin)*factor);
      if (index<0) index=0;
      return lower[index];
    }
    if (whichBin>CFBin){
      int index=STARTBINU+(int)rint(((double)whichBin-CFBin)*factor);
      if (index >=HALFFS) index=HALFFS-1;
      return upper[index];
    }
  }
};
#endif //GTSENSITIVITY_H_
