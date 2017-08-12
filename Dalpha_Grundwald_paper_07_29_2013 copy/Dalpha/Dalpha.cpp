#include "Dalpha.h"
#include "PolyRatioFromContFrac.h"
#include "AvgShiftRegister.h"
#include <iostream>
#include <fstream>



Dalpha::Dalpha(void)
{
}


Dalpha::~Dalpha(void)
{
}


void Dalpha::setProperties(double alpha0, double deltaT0, int Nmax0)
{
	alpha = alpha0;
	deltaT = deltaT0;
	Nmax = Nmax0;
	outputData.resize(Nmax);


}


void Dalpha::setPartition(int type, int numRegisters)
{
	int scaleFac = 2;

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
	case 3: // exponential per bin with 2 as scaling factor
		int scalingFactor;
		scalingFactor = 2;
		int expScalingFactorK;
		partitionSize = numRegisters;
		partition.resize(partitionSize);
		partition[0] = 0;
		for(int k=1; k<partitionSize; k++)
		{
			expScalingFactorK = 1;
			for(int j = 1; j<k; j++)
			{
				expScalingFactorK *=scalingFactor;
			}
			partition[k] = partition[k-1] + expScalingFactorK;
		}
		partitionType = "ExponentialPerBin";
		break;
	case 4: // Exponential end of bin sizes in partition-- the hope is that the fourier spectrum will be better. 2 as scaling factor
		partitionSize = numRegisters;
		partition.resize(partitionSize);
		partition[0]=0;
		partition[1] = 1;
		for(int k = 2; k<partitionSize; k++)
		{
			partition[k] = partition[k-1]*scaleFac;
		}
		partitionType="ExponentialEndPoints";
		break;
	case 5:
		partitionSize = numRegisters;
		partition.resize(partitionSize);
		partition[0]=0;
		partition[1]=1;
		for(int k = 2; k<partitionSize; k++)
		{
			partition[k]+=k*k;
		}
		partitionType="SquaredPerBin";
		break;
	case 6:
		partitionSize = numRegisters;
		partition.resize(partitionSize);
		partition[0] = 0;
		for(int k =1; k<partitionSize; k++)
		{
			partition[k] = (partition[k-1]+1)*scaleFac - 1;
		}
		partitionType ="ExpPerBinMinusOne";
		break;
	case 7: //CBB depressed exponential 
		partitionSize = numRegisters;
		partition.resize(partitionSize);
		partition[0]=0;
		partition[1] = 1;
		for(int k = 2; k<partitionSize; k++)
		{
			partition[k] = partition[k-1] + 1 + (int)(0.0001 * pow(k, 2.6));
		}
		partitionType="DepressedExponential";
		break;
	case 8:
		// Does not appear to calculate weights in reasonable time for larger choices of numbers of bins. Probably because the numbers blow up.
		//If it mattered they could be saved to a file.
		partitionSize = numRegisters;
		partition.resize(partitionSize);
		int count1, count2, total;
		count1 = 0;
		count2 = 1;
		total =1;
		partition[0]=0;
		for(int k =1; k<partitionSize; k++)
		{
			partition[k] = partition[k-1] + total;
			count1 = count2;
			count2 = total;
			total = count1 + count2;
		}
		partitionType = "Fibbonacci";
		break;
	case 9:
		partitionSize = numRegisters;
		partition.resize(partitionSize);
		int perBin;
		perBin = 1;
		partition[0]=0;
		for(int k=1; k<partitionSize; k++)
		{
			partition[k] = partition[k-1] + perBin;
			perBin++;
		}
		partitionType = "LinearPerBin";
		break;
	}
	historyRegister.setPartition(partition,partitionSize);
	if(false)
	{
		ofstream fs;
		fs.open("testPartition.txt");
		for(int k = 0; k < partitionSize; k++)
		{
			fs << k << "\t" << partition[k] << endl;
		}
		fs.close();
	}

}


void Dalpha::zeroHistory(void)
	// does not zero the weights
{
	for(int n = 0; n<Nmax; n++)
	{
		outputData[n] = 0.0;
	}
	historyRegister.zeroHistory();
}

void Dalpha::computeWeights(void)
{// derivative, for positive alpha
	// weights = (-1)^m*G(alpha+1)/m!/G(alpha-m+1)

	weights.resize(partitionSize);
	for(int n=0; n<partitionSize; n++)
	{
		weights[n] = 0.0;
	}

//	int maxIndex, minIndex;

	double weightk = 1.0;
	int curPartIndex=0;//current partition index for weights

	for(int n=0; n<=partition[partitionSize-1]; n++)	{
		if(n>partition[curPartIndex])
		{
			curPartIndex++;
		}
		weights[curPartIndex]+=weightk;
		weightk *= (n-alpha)/((double) (n+1));
	}

	if (false)
	//if(partitionType.compare("Linear"))
	{
		ofstream fs;
		fs.open("testWeights.txt");
		for(int k=0; k<partitionSize; k++)
		{
			fs<< k<< "\t" << weights[k] <<endl;
		}
		fs.close();
	}

}


//void Dalpha::differintegrate(CArray<double, double&>& parameters) //uses finite impulse response filter: Grunwald weights
void Dalpha::differintegrate(vector<double>& inputData)
{	 
	for(int Ntime=0; Ntime<Nmax; Ntime++)
	{
		updateHistory(inputData[Ntime]);
		outputData[Ntime]=differintegrateOnce();
	}
}

void Dalpha::updateHistory(double inputDatum)
{
	historyRegister.AvgShift(inputDatum);
}

double Dalpha::differintegrateOnce(void)
{
	double outputD=0;
	outputD=partSumOnce();
	outputD+=weights[0]*historyRegister.dataAt(0);
	return outputD;
}

double Dalpha::partSumOnce(void)
{
	double outputP=0.0;
		for(int n=1; n<partitionSize; n++)
		{
			int binMin;
			if(n == 0)
			{
				binMin = 0;
			} else{
				binMin = partition[n-1] + 1;
			}
			int binMax = partition[n];
			int nBin = binMax - binMin + 1;//number data points per bin in partition
			double currentWeight = weights[n] *((double) historyRegister.dataCountAt(n))/((double) nBin);
			outputP += currentWeight*historyRegister.dataAt(n);
		}
		double timeFac = pow(deltaT, -alpha);
		outputP*= timeFac;
		return outputP;
}

void Dalpha::print(string filename, vector<double>& inputData)
{
	ofstream fs;
	fs.open(filename);
	for(int N=0; N< Nmax; N++) 
	{
		fs << N*deltaT << "\t" << inputData[N] << "\t" << outputData[N] << endl;
	}
	fs.close();
}

double Dalpha::getWeight(int N)
{
	return weights[N];
}

int Dalpha::getBinMax(int N)
{
	return partition[N];
}

int Dalpha::getNumPerBin(int N)
{
	int num;
	if(N>=1)
	{
		num = partition[N]-partition[N-1];
	} else{
		num = partition[0]+1;
	}
	return num;
}

void Dalpha::setPresentValue(double y)
{
	historyRegister.setDataAt(0,y);

}