/*  Author: Chad Bohannan
    email:  bohannan@cs.montana.edu
    Application Summary:: This software tool provides a space optimized
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

#include <iostream>
#include "IntegratorRecursive.h"

IntegratorRecursive::IntegratorRecursive( double Q, double DT, int SIZE, int gFactor){
	q= Q;
	dt = DT;
	size = SIZE < 1 ? 1 : SIZE;
	stepSize = 1;
	growthFactor = gFactor;
	child = NULL;

	weights = new double[size];
	weights[size - 1] = 1.0;
	k = K_INIT; // used to calculate weights. 0 for (0 to N), 1 for (0 to N-1)

	initWeights();

	data = new double[size];
	clearData();
}


IntegratorRecursive::IntegratorRecursive( double Q, double DT, int SIZE, int gFactor, int step, double lastWeight, int K){
	q= Q;
	dt = DT;
	k = K;
	size = SIZE;
	stepSize = step;
	growthFactor = gFactor;

	data = new double[size];
	for(int i = 0 ; i < size ; i++)
		data[i] = 0.0;

	weights = new double[size];
	weights[size - 1] = nextWeight(lastWeight);

	initWeights();

	offset = 0;
	child = NULL;
	runningAverage = 0.0;
	pointsInAverage = 0;

	stateChanged = true; // force initial computation
	lastValue = 0.0;
}


IntegratorRecursive::~IntegratorRecursive(){
	delete child;
	delete data;
	delete weights;
}

double IntegratorRecursive::nextWeight(double lastWeight)
{
	double temp = lastWeight;
	for(int j = 0 ; j < stepSize; j++){
		temp = temp * ((k - q)/(k + 1));
		k++;
	}
	return temp;
}


void IntegratorRecursive::initWeights(){
	for( int i = size - 2 ; i >= 0 ; i-- ){
		weights[i] = nextWeight(weights[i+1]);
	}

	/*
	printf( "starting at k = %d\n", k);
	for(int m = size-1; m >=  0; m-- ){
		printf( "weight at %d is %e \n", m, weights[m]);
	}//end for j
	*/
}


void IntegratorRecursive::setq(double Q){
	q = Q;
	k = K_INIT;
	initWeights();
	if( child != NULL ){
		child->setq(q, k, weights[0]);
	}//end if
	stateChanged = true;
}//end setq()


void IntegratorRecursive::setq(double Q, int K, double lastWeight){
	q = Q;
	k = K;
	weights[size - 1] = nextWeight(lastWeight);
	k++;
	initWeights();
	if( child != NULL ){
		child->setq(q, k, weights[0]);
	}//end if
	stateChanged = true;
}//end setq()


/*************************************************************
 * integrateValue(double value)
 * This method allows new data points to be added to the 
 * data structure. Until the current partition is full, data 
 * points are added to the local data array. When the local
 * space is filled, a child is created to begin containing
 * compressed old data.
 ************************************************************/
void IntegratorRecursive::integrateValue(double value){
  //printf( "integrating: %e, where k=%d\n", value, k);

	if(child != NULL){
		processOverflow();
	}//end if

	if(offset == (size-1) && child == NULL){
		child = new IntegratorRecursive( q, dt * growthFactor, size, growthFactor, stepSize * growthFactor, weights[0], k);
	}//end if

	data[offset++] = value;
	if(offset == size){
		offset = 0;
	}//end if

	stateChanged = true;
}//end integrateValue()


/*************************************************************
 * getValue()
 * Computes the qth integral over all recorded history
 ************************************************************/
double IntegratorRecursive::getValue(){
	double sum = 0.0;
	if(stateChanged){		
		if( child != NULL )
			sum =  child->getValue();
		sum += differintegral();
		lastValue = sum;
	}
	else
	{
		sum = lastValue; // prevent recomputation
	}
	return sum;
}//end getValue()


/*************************************************************
 * differintegral()
 * Computes the the integral over the range represented by
 * the current segment. Solves by summing the product of the 
 * recorded data point and its corrosponding weight.
 ************************************************************/
double IntegratorRecursive::differintegral(){
	double sum = 0.0;
	int dex = offset;
	stateChanged = false;

	for(int i = 0 ; i < size ; i++){
		if(dex >= size)
			dex = 0;
		sum += data[dex++] * weights[i];
	}
	return sum/pow( (dt*stepSize), q);
}//end differintegral()


/*************************************************************
 * processOverflow()
 * When new data is recieved and there is no room for data in 
 * the current partition, the data is aggregated to take less
 * space, then added to the child partition. 
 ************************************************************/
void IntegratorRecursive::processOverflow(){
	runningAverage += data[offset];
	pointsInAverage++;
	if( pointsInAverage >= growthFactor ){
		child->integrateValue( runningAverage / growthFactor );
		runningAverage = 0.0;	
		pointsInAverage = 0;
	}//end if points == factor
}//end if processOverflow()


void IntegratorRecursive::clearData()
{
	if(child != NULL)
		delete child;
	child = NULL;

	for(int i = 0 ; i < size ; i++)
		data[i] = 0.0;

	offset = 0;
	child = NULL;
	runningAverage = 0.0;
	pointsInAverage = 0;
	lastValue = 0.0;
	stateChanged = true; // force initial computation
}
