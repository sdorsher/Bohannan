#include "Grundwald.h"
#include "PolyRatioFromContFrac.h"
#include "AvgShiftRegister.h"
#include <iostream>
#include <fstream>

Grundwald::Grundwald(void)
{
}

Grundwald::~Grundwald(void)
{
}

void Grundwald::setProperties(double alpha0, double deltaT0, int Nmax0)
{
	alpha = alpha0;
	deltaT = deltaT0;
	Nmax = Nmax0;
	outputData.resize(Nmax);
}

void Grundwald::setPartition(int type, int numRegisters)
{
	int scalingFactor = 2;
	int powScalingFactorK = 1;
	int stepSize =10;

	switch (type)
	{
	case 1: //linear, increments of one. Full integral if numRegisters = Nmax
		partitionSize = numRegisters;
		partition.resize(partitionSize);
		for(int k = 0; k<partitionSize; k++)
		{
			partition[k] = k;
		}
		partitionType="Linear";
		break;
	case 2: // squared intervals, 2k+1 per bin
		partitionSize = numRegisters;
		partition.resize(partitionSize);
		for(int k = 0; k<partitionSize; k++)
		{
			partition[k] = k*k;
		}
		partitionType="Squared";
		break;
	case 3: // power law per bin with 2 as scaling factor
		partitionSize = numRegisters;
		partition.resize(partitionSize);
		partition[0] = 0;
		for(int k=1; k<partitionSize; k++)
		{
			powScalingFactorK = 1;
			for(int j = 1; j<=k; j++)
			{
				powScalingFactorK *=scalingFactor;
			}
			partition[k] = partition[k-1] + powScalingFactorK;
		}
		partitionType = "PowerLawPerBin";
		break;
		
	case 4: //linear steps of 10
		partitionSize = numRegisters;
		partition.resize(partitionSize);
		for(int k=0; k<partitionSize; k++)
		{
			partition[k]=stepSize*k;
		}
		partitionType = "LinearStep10";
		break;
	}
	historyRegister.setPartition(partition,partitionSize);
}


void Grundwald::zeroHistory(void)
	// does not zero the weights
{
	for(int n = 0; n<Nmax; n++)
	{
		outputData[n] = 0.0;
	}
	historyRegister.zeroHistory();
}

void Grundwald::computeWeights(void)
{// for a bin with only one element in it, the Grundwald weight is (j-alpha)/(j+1) which is multiplied times the product of the 
	// previous terms before adding f_j. If f_i through f_j are identical in some partition which we will label with k such that 
	// f_k is the f_i=f_(i+1)= f_(j-1)=f_j elements in the partition and F_(k-1) is the product of the previous terms. Let 
	// weightsPrev[k] represent the way that the weights from i through j multiply F_(k-1) to produce a new weight and weightsThis[k]
	// represents the way that the weights from i through j multiply f_k to produce a new weight. Then F_k, the product of all terms 
	// including this one, is equal to F_k = weightsThis[k]*f_k + weightsPrev[k]*F[k-1]. weightsPrev[k]= the product of all Grundwald
	// weights from i through j. weightsThis[k] can be obtained by recursively adding one to a Grundwald weight then multiplying by 
	// the next weight, starting from the highest index j within the partition k.  

	weightsThis.resize(partitionSize);
	weightsPrev.resize(partitionSize);
	for(int k=0; k<partitionSize; k++)
	{
		weightsPrev[k] = 1.0;
		weightsThis[k] = 1.0;
	}

//	int maxIndex, minIndex;

	int minBin, maxBin;
	double weightj;
	int curPartIndex=0;//current partition index for weights
	for(int k=partitionSize-1; k>=0; k--)
	{
		if (k==0){
			minBin =0;
		} else{
			minBin = partition[k-1] +1;
		}
			
		maxBin= partition[k];
		for(int j=maxBin; j>=minBin; j--)
		{
			if (j!=0)
			{
				weightj = (j-alpha-1.0)/((double) j);
			} else {
				weightj = 1.0;
			}
			weightsPrev[k] *= weightj;
			weightsThis[k] *= weightj;
			weightsThis[k] += 1.0;
		}
		weightsThis[k] -= 1.0;
	}
}


//void Dalpha::differintegrate(CArray<double, double&>& parameters) //uses finite impulse response filter: Grunwald weights
void Grundwald::differintegrate(vector<double>& inputData)
{
	 double prevProduct=0.0; // product of all data that came before excluding current data point. 
	 // starts at zero for Nmax-1 since the first term is just f_k. 
	for(int Ntime=0; Ntime< Nmax; Ntime++)
	{
		historyRegister.AvgShift(inputData[Ntime]);
		outputData[Ntime]=0.0;
		for(int n=partitionSize-1; n>=0; n--)
		{
			// account for the fact that not all bins are occupied in the weighting scheme
			int minBin;
			if(n==0)
			{
				minBin= 0;
			} else{
				minBin = partition[n-1]+1;
			}	
			int maxBin = partition[n];
			int nBin = maxBin - minBin +1; //number data points per bin in partition
			double avgWeightPrev = pow(weightsPrev[n], (((double) historyRegister.dataCountAt(n))/((double) nBin)));
			double avgWeightThis =  pow(weightsThis[n], (((double) historyRegister.dataCountAt(n))/((double) nBin)));
			prevProduct = prevProduct *avgWeightPrev + avgWeightThis * historyRegister.dataAt(n);	
		}
		outputData[Ntime]= prevProduct*pow(deltaT,-alpha);
	}
}

void Grundwald::print(string filename, vector<double>& inputData)
{
	ofstream fs;
	fs.open(filename);
	for(int N=0; N< Nmax; N++) 
	{
		fs << N*deltaT << "\t" << inputData[N] << "\t" << outputData[N] << endl;
	}
	fs.close();
}

