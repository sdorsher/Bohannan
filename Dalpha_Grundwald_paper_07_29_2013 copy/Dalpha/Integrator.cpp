/*****************************************************************************************
    Author: Chad Bohannan
    Email:  bohannan@cs.montana.edu
    Summary:: The Integrator object encapsulates the fractional derivative funtion and
        manages an internal ring buffer to provide a simple digital signal processing
        element. Expiring data points are reapplied to the front of the

    Copyright (C) 2005 Chad Bohannan

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.
******************************************************************************************/

#include <iostream>
#include "Integrator.h"

Integrator::Integrator( double Q, double DT, int SIZE, double decayRate){
  q= Q;
  dt = DT;
  size = SIZE;
  decay = decayRate;

  data = new double[size];
  
  weights = new double[size];
  weights[size - 1] = 1.0;
  
  initWeights();
  clearData();
}


Integrator::~Integrator(){
  delete data;
  delete weights;
}

/*****************************************************************************
 * Iterative method for building the weighting array.
 * Always uses a previous weight point, then multiplies by the 
 * ratio (k-q)/(k+1) for each new point.
 ****************************************************************************/
void Integrator::initWeights(){
  int k = 0;
  for( int i = size - 2 ; i >= 0 ; i-- )
  {
    weights[i] = weights[i+1] * ((k - q)/(k + 1));
    k++;
  }

  return;
}

void Integrator::clearData()
{	
  for(int i = 0 ; i < size ; i++)
    data[i] = 0.0;
  offset = 0;

  stateChanged = true; // force initial computation
  lastValue = 0.0;	
}

/*****************************************************************************
 * integrateValue(double value)
 * This method allows new data points to be added to the  data structure. Data
 * is written to the points are added to the local data array. When the local
 * space is filled, the exponential approximation causes the tail data to 
 * decay exponentially by the fractional difference between the oldest
 * weights.
 ****************************************************************************/
void Integrator::integrateValue(double value){
  //the last datum that is being overwritten is reincorporated at the beginning
  //data[offset++] = data[offset] * weights[0] + value;
  //float decay = weights[0] / weights[1];
  data[(offset + 1) % size] += data[offset] * decay;
  data[offset] = value;
  offset = ++offset % size;
  stateChanged = true;
}//end integrateValue()


/*****************************************************************************
 * getValue()
 * Computes the qth integral over all recorded history
 ****************************************************************************/
double Integrator::getValue(){
  double sum;

  if(stateChanged){		
    sum = differintegral();
    lastValue = sum;
  }
  else
    sum = lastValue; // prevent recomputation

  return sum;
}//end getValue()


/***************************************************************************
 * differintegral()
 * Computes the the integral over the range represented by the current 
 * segment. Solves by summing the product of the recorded data point and 
 * its corrosponding weight.
 **************************************************************************/
double Integrator::differintegral(){
  double sum = 0.0;
  int dex = offset;
  stateChanged = false;

  for(int i = 0 ; i < size ; i++){
    if(dex >= size)
      dex = 0;
      sum += data[dex++] * weights[i];
  }
  return sum/pow( (dt), q);
}//end differintegral()
