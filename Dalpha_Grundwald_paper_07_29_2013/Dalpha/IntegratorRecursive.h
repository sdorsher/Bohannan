/*  Author: Chad Bohannan
    email:  bohannan@cs.montana.edu
    Application Summary:: This software tool provides a space efficient
    fractional order integration to an arbitrary real value.

    Copyright (C) 2005 Chad Bohannan

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#ifndef _INTEGRATOR_RECURSIVE_H_
#define _INTEGRATOR_RECURSIVE_H_

#include <math.h>

#define NULL 0
#define K_INIT 0

class IntegratorRecursive{
private:
	double* data;
	double* weights;

	double q;
	double dt;
	int k;
	int size;
	int stepSize;
	int growthFactor;

	//represents the position of the newest data point. new data is added to index plus one.
	int offset;

	IntegratorRecursive* child;

	double runningAverage;
	int pointsInAverage;

	bool stateChanged; //prevent recomputation if no new data
	double lastValue;  //store last computed value


public:
	//Initializes the Integrator
	//@param q The order of the differintegration operation
	//@param dt The time separation between data points
	//@param size The number of data points per partition
	//@param growth_factor The rate at which the size of dT grows in each
	//        recursive definition of Integrator. Coverage is exponential. 
	IntegratorRecursive( double q, double dt, int size, int growthFactor);

protected:
	IntegratorRecursive( double q, double dt, int size, int growthFactor, int stepSize, double lastWeight, int K);

public:
	//destructor recovers memory allocated to the object
	~IntegratorRecursive();

	double nextWeight(double lastWeight);

	//Redefine the order of integration
	//@param q The new value of q to integrate to
	void setq(double Q);
	void setq(double Q, int K, double lastWeight);

	// add a new value to the data set
	//@param value The new value to integrate
	void integrateValue(double value);


	//calculate the qth order integration of the data set
	double getValue();

	//processes the local segment of the integration
	//sums the product of each data point by it's weight
	double differintegral();

	void clearData();

private:
	//loads the weight buffer according to q, dt, and size
	void initWeights();

	//averages the overflow and loads the child integrator
	void processOverflow();
};


#endif