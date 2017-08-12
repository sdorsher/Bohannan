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


#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#include <math.h>

#ifndef NULL
#define NULL 0
#endif

class Integrator{
private:
  double* data;
  double* weights;

  double q;
  double dt;
  int size;
  double decay;

  //represents the newest data point. new data is added to index plus one.
  int offset;

  bool stateChanged; //prevent recomputation if no new data
  double lastValue;  //store last computed value


public:
  //Initializes the Integrator
  //@param q The order of the differintegration operation
  //@param dt The time separation between data points
  //@param size The number of data points to store
  Integrator( double q, double dt, int size, double decayRate = 0.9999);

  //destructor recovers memory allocated to the object
  ~Integrator();

  // add a new value to the data set
  //@param value The new value to integrate
  void integrateValue(double value);

  //evaluates the integrators current value
  //avoids recomputing for successive calls when the value hasn't changed
  double getValue();

  void clearData();

private:
  //evaluates the integrators current value
  double differintegral();

  //loads the weight buffer according to q, dt, and size
  void initWeights();
};


#endif
