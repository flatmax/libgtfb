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
#include <strstream.h>
#include <math.h>

#include "GTFB.H"
#include "../utils/octaveout.H"
#include <string.h>

main(){
  int count=20;
  double input[10000],output[10000];
  //  GTFB gtfb(60, 10025, count); // b, low, fs, count
  GTFB gtfb(1000, 10025, count); // low, fs, count

  memset(input, 0, 10000);
  input[0]=1.0;

  char name[255];
  char v='a';
  //  for (int i=1; i<=count; i++){
  int i=10;
  ostrstream na(name, 255);
  na << "results/gt"<<i<<".dat";
  
  //  gtfb.grab(i)->impulseResponse(output, 1000);
  gtfb.grab(i)->filter(input, output, 1000);
  octaveout<double>(name, v, 1000, output);
    v++;
    //}




    /*  for (int i=1; i<=5; i++){
    ostrstream na(name, 255);
    na << "results/gt"<<count+i<<".dat";
    //cout <<name<<endl;
    //cout <<v<<endl;
    gtfb1.grab(i)->impulseResponse(output, (int)rint(1000.0*(2000.0/10025.0)));
    octaveout<double>(name, v, (int)rint(1000.0*(2000.0/10025.0)), output);
    v++;
  }
  //  cout<<i<<' '<<gt.grab(i)->findGain()<<endl;
    */
}
