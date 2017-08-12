#include "AvgShiftRegister.h"
#include "Nrutil.h"

#include <iostream>
using namespace std;

AvgShiftRegister::AvgShiftRegister(void) 
{
}

AvgShiftRegister::~AvgShiftRegister(void)
{
}

int AvgShiftRegister::dataSize()
{
	return data.size();
}

void AvgShiftRegister::setPartition(vector<int>& newPartition, int newPartitionSize)
{
	partition.resize(newPartitionSize);
	for(int n = 0; n<newPartitionSize; n++)
	{
		partition[n] = newPartition[n];
	}

	data.resize(newPartitionSize);
	midpoints.resize(newPartitionSize);
	dataCount.resize(newPartitionSize);
	filling = 0;
	for(int k=0; k<dataSize(); k++)
	{
		data[k]=0.0;
		dataCount[k]=0;
	}
}

void AvgShiftRegister::AvgShift(double newData) 
{
	//ofstream fs;  
	//fs.open("testAvgShift.txt");
	//fs << "k\t fill \t dataCt \t newD \t data\n";
	for(int k=min(data.size()-1,filling); k>=0; k--) 
	{		
		if(k==filling)
		{
			dataCount[k]++; //just added an element
			int rangeMax = partition[k];
			int rangeMin = (k==0) ? 0 : partition[k-1]+1;
			if((dataCount[k]==(rangeMax-rangeMin+1)))
				filling++;
		}

		double shiftValue = (k==0) ? newData : data[k-1];
		data[k]= (1.0-1.0/dataCount[k]) * data[k] + (1.0/dataCount[k]) * shiftValue;
	}
	//fs.close();
}

void AvgShiftRegister::getMidpoints(void)
{
	int rangeMin, rangeMax;
	for(int k=data.size()-1; k>0; k--)
	{
		rangeMax = partition[k];
		rangeMin = partition[k-1]+1;
		midpoints[k]=((double) (rangeMax-rangeMin))/2.0+ ((double) rangeMin);
	}
	midpoints[0]=((double) partition[0])/2.0;
}


void AvgShiftRegister::zeroHistory(void)
{// Returns AvgShiftRegister to an empty state

	for(UINT n = 0; n < partition.size(); n++)
	{
		data[n] = 0.0;
		dataCount[n] = 0;
	}
	filling = 0;
}

double AvgShiftRegister::dataAt(int n)
{
	return data[n];
}

double AvgShiftRegister::dataCountAt(int n)
{
	return dataCount[n];
}

double AvgShiftRegister::midpointAt(int n)
{
	int rangeMin, rangeMax;
	if(n == 0)
	{
		rangeMin = 0;
	}else{
		rangeMin = partition[n-1]+1;
	}
	rangeMax = partition[n];
	return ((double) (rangeMax-rangeMin))/2.0+ ((double) rangeMin); // calculate the midpoint of the bin for plotting purposes
}

void AvgShiftRegister::setDataAt(int n, double value)
{
	data[n] = value; 
}