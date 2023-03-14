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
#ifndef GTFB_H_
#define GTFB_H_

//#include "/home/flatmax/include/LinkList.H"
#include "LinkList.H"
#include "GT.H"

/// Implements a bank of Gamma Tone Filters
class GTFB : public LinkList<GT *> {
  int lowFreq, highFreq;
  double T;
public:
  double *CFEdges; // This is now redundant
  double *edgeFreq; // Edge frequencies between filter CFs
  /** Construct and initialise the GTFB
      Initialise with:
        lowF    - the lowest frequency of interest
	sampleF - the sample frequency (Fs Hz)
	count   - the desired number of filters
   */
  GTFB(int lowF, int sampleF, int count);
  ~GTFB();

  /// Finds the relevant cf variable
  double findCF(int which, int count);
  /// Gets the low frequency
  int getLowFreq(void){return lowFreq;}
  /// Gets the high frequency
  int getHighFreq(void){return highFreq;}
};

#endif //GTFB_H_
