#include "MAIN.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "Dalpha.h"
#include "DFT.h"
#include "AmpPhaseFromSine.h"
#include "Nrutil.h"
#include "Grundwald.h"
#include "FullGrundwald.h"
#include "Fractance2.h"
#include "FractOp2.h"
#include "Integrator.h"
#include "IntegratorRecursive.h"
#include "FEDiffereintegrator.h"
#include "IIR_CFEdifferintegrator.h"
#include <stdio.h>
#include <time.h>

#include "../kiss_fft130/kiss_fft.h"

using namespace std; 

void main(void)
{
	MAIN myObj = MAIN();
	
	//myObj.testCFEcoefficients(0.5);
//	 myObj.testSineFit();
	//	myObj.testAvgShiftRegister();
	
//	int shiftAvgType[6] = {1,9,5,3,8,0};

//	myObj.generateAmpPhasePlot(10.0, 1000, 10, 10, 0.5, 1, shiftAvgType, true,false,false);//always have alpha between -1 and 1
//use 20 registers for both! 20=log_2(10^6). 10^6 timesteps is max can run in a few hours. 
	//best shift avg is 26, best cfe is 40. 100000,26,40
	// compare 10 to 10, 26 to 40 
	//25 is maximum number of partition bins for shiftavg3 
	//  1 -10 registers of no binning, 9 linear per bin, 3 exponential per bin, 5 squared per bin, 8 fibbonacci per bin

	//myObj.generateComparisonPlot(); //timeDomain obsolete
	//myObj.genAmpPhaseGrundwald(); //has time domain partially written-- obsolete. three different methods depending what is commented
	myObj.stepFnDiffEq();
}


MAIN::MAIN(void)
{
}

MAIN::~MAIN(void)
{
}

double MAIN::inputFunction(int N, double deltaT, double amplitude, double frequency, double phase)
{
	return amplitude * sin(phase + 2.0 * PI * frequency * deltaT * N);
}

double MAIN::inputStepFunction(int N, double deltaT, double amplitude, double frequency, double phase)
{
	if((fmod(N*deltaT*frequency,2.0*PI)>PI-phase)&&(fmod(N*deltaT*frequency,2.0*PI)<2.0*PI-phase))
	{
		return 1.0;
	}
	else
	{
		return 0.0;
	}
}

double magnitude(double A1, double A2)
{
	double amp = sqrt(A1*A1 + A2*A2);
	return amp;
}

double phase(double Bsin, double Bcos)
{

	double phase0 = -atan2(Bsin, Bcos);
	phase0 = phase0 + PI/2.0; //could be plus
	if(phase0>PI)
	{
		phase0-=2.0*PI;
	}
	if(phase0<-PI)
	{
		phase0+=2.0*PI;
	}
	phase0 *= 360.0/(2.0*PI);
	return phase0;
}

void analyzeSignal(vector<double>& data, double deltaT, double frequency, double* peakAmp, double* peakPhase)
{
	double epsilon[2];
	SineFit(frequency, deltaT, 0, data.size(), data, epsilon);
	double temp1 = magnitude(epsilon[0], epsilon[1]);
	double temp2 = phase(epsilon[0], epsilon[1]);
	*peakAmp = temp1; //magnitude(epsilon[0], epsilon[1]);
	*peakPhase = temp2; //phase(epsilon[0], epsilon[1]);
}

void MAIN::testCFEcoefficients(double alpha)
{// want a plot of index versus coefficients for a variety of alpha

	PolyRatioFromContFrac recursiveCFE = PolyRatioFromContFrac();
	CFEDiffereintegrator constantCFE = CFEDiffereintegrator(alpha);
	recursiveCFE.initialize(alpha,10); // 10, from P_0 to P_9 (ten coefficients)// 3792 is max without infinities
	recursiveCFE.calculatePolynomials();
	recursiveCFE.rescale();
	recursiveCFE.printCoeff();
	constantCFE.getWeights();
	constantCFE.printWeights();
}







void MAIN::testSineFit(void)
{ //Works with sin and cos reversed from the original notation-- 0 is sine and 1 is cosine. For test of ampCos = 1.0, ampSin = 0
	// works for ampCos = 0, ampSin = 1; works for 1 and 1. about 1 in 10^8 precision for amplitude recovery. tested with 
	// frequency equal to one and deltaT equal to 0.01.

	// apparently off by minus sign in phase. cos(ft -pi/2) = sin(ft)
	UINT Nmax =1000;
	vector<double> sinusoid;
	sinusoid.resize(Nmax);

	double epsilon[2];

	double amplitudeCos = 1.0;
	double amplitudeSin = 1.0;
	double frequency = 1.0;
	double pi = 3.14159265;
	double deltaT = 0.01;

	for(UINT n=0; n<Nmax; n++)
	{
		sinusoid[n] = amplitudeCos *cos(2.0*pi*frequency*deltaT* n)+amplitudeSin*sin(2.0*pi*frequency*deltaT*n);
	}

	UINT delay_index = 0;
	UINT capture_index = Nmax;

	SineFit(frequency, deltaT, delay_index, capture_index, sinusoid, epsilon);
	
	ofstream fs;
	fs.open("sineFitTest.txt");
	fs << amplitudeCos << "\t" << amplitudeSin << "\t" << epsilon[1] << "\t" << epsilon[0] << endl;
	
	double amplitudeAct = magnitude(amplitudeSin, amplitudeCos);
	double amplitudeEmp = magnitude(epsilon[0], epsilon[1]);
	double phaseAct = phase(amplitudeSin, amplitudeCos);
	double phaseEmp = phase(epsilon[0], epsilon[1]);
	fs << amplitudeAct << "\t" << phaseAct <<"\t" << amplitudeEmp <<"\t" << phaseEmp << endl;
	fs.close();
}

void MAIN::testAvgShiftRegister()
	//want plot of midpoints versus value for initial value input, number of time steps. for one, zero, 
	// sinusoidal sequence with linear and nonlinear partition
{// works for both squared and linear partition, for partial and full fill
	
	AvgShiftRegister AX = AvgShiftRegister();

	int Nmax = 10;
	int numShifts =200;
	vector<int> partition1(Nmax);
	for (int n = 0; n< Nmax; n++)
	{
		partition1[n] = n;
	}

	AX.setPartition(partition1, Nmax);
	for(int n = 0; n< numShifts; n++)
	{
		AX.AvgShift(1.0);
	}
	ofstream fs;
	fs.open("avgShiftReg_linear.txt");
	
	for(int n=0; n< Nmax; n++)
	{
		fs<< AX.midpointAt(n) << "\t" << AX.dataAt(n) << "\t" << AX.dataCountAt(n) << endl;
	}
	fs.close();
	

	AvgShiftRegister AY = AvgShiftRegister();

	int Nmax2= 10;
	vector<int> partition2(Nmax2);
	for(int n=0; n< Nmax2; n++)
	{
		partition2[n] = n*n;
	}

	AY.setPartition(partition2, Nmax2);

	for(int n=0; n< numShifts; n++)
	{
		AY.AvgShift(1.0);
	}

	fs.open("avgShiftReg_squared.txt");
	for(int n=0; n< Nmax2; n++)
	{
		fs<< AY.midpointAt(n) << "\t" << AY.dataAt(n) << "\t" << AY.dataCountAt(n) << endl;
	}
	fs.close();
	
	AvgShiftRegister AZ = AvgShiftRegister();
	int Nmax3 = 10;
	vector<int> partition3(Nmax3);
	for(int n=1; n<=Nmax3; n++)
	{
		partition3[n-1] = 10*n -1;
	}		
	
	AZ.setPartition(partition3, Nmax3);

	for(int n=0; n<numShifts; n++)
	{
		AZ.AvgShift(1.0);
	}

	fs.open("avgShiftReg_linear10.txt");
	for(int n=0; n< Nmax3; n++)
	{
		fs<< AZ.midpointAt(n) << "\t" << AZ.dataAt(n) << "\t" << AZ.dataCountAt(n) << endl;
	}
	fs.close();
	
}

void MAIN::generateAmpPhasePlot(double tSample, int sampleLength, int storedPoints, int CFEregisters, double alpha, int freqSpecType, int* shiftAvgType, bool outputTimeDomain, bool calcRec, bool calcTerm)
{//most current version, does both Chad's approximations and Steven's approximations at once
	//Maple requires two output files to plot because only ten columns can be used. CSV is best for importing data.



	double deltaT = tSample/((double) sampleLength);
	double sampleFreq =1.0/deltaT;
	double nyquistFreq = sampleFreq/2.0;
	double minFreq = nyquistFreq/((double) sampleLength);

	vector<double> freqParams;
	switch(freqSpecType)
	{
	case 1: //logMinToMaxFreq-- frequencies in logarithmic scale from the minimum to the maximum	
		int freqsPerDecade;
		freqsPerDecade =5;
		int numFreqs;
		numFreqs = floor(log10((double) sampleLength)*((double) freqsPerDecade));
		for (int i =0; i<=numFreqs; i++)
		{
			freqParams.push_back(minFreq*pow(10.0,((double) i)/((double) freqsPerDecade)));
		}
	break;
	case 2: //twoDecades
		for(int i = 1; i < 10; i++)
			freqParams.push_back(0.01 * i);
		for(int i = 1; i < 10; i++)
			freqParams.push_back(0.1 * i);
	break;
	case 3: // threeDecades
		for(int i = 1; i < 10; i++)
			freqParams.push_back(0.001 * i);
		for(int i = 1; i < 10; i++)
			freqParams.push_back(0.01 * i);
		for(int i = 1; i < 10; i++)
			freqParams.push_back(0.1 * i);
	break;
	case 4: // fourDecades
			for(int i = 1; i < 10; i++)
				freqParams.push_back(0.0001 * i);
			for(int i = 1; i < 10; i++)
				freqParams.push_back(0.001 * i);
			for(int i = 1; i < 10; i++)
				freqParams.push_back(0.01 * i);
			for(int i = 1; i < 10; i++)
				freqParams.push_back(0.1 * i);
	break;
	}




	//Continued Fraction Expansion FIR filter for comparison
//	CFEDiffereintegrator cfeIIR(alpha);
	IIR_CFEdifferintegrator cfeIIR(alpha, CFEregisters, deltaT);
	//40 is maximum

	//exponential tail approximation
	Integrator expIntgrator(alpha, deltaT, storedPoints, 0.99); 

	//recursive growth approximation
	IntegratorRecursive recIntegrator(alpha, deltaT, storedPoints, 2);

	std::cout << "Objects instantiated\n";

	int numPartShown=6; //Number of partitions shown. 6 is max Maple can handle because 1 freq + 1 exp + 1 rec + 1 full = 10 
	// must equal dimensions of shiftAvgType input array. 
	//shiftAvgType must have 0's only at end

	int lastShiftAvgData = 0; // assumes at least one data point is present. detects first zero and truncates
	for(int i = 1; i< numPartShown; i++)
	{
		if(shiftAvgType[i]==0) { break;}
		else { lastShiftAvgData++;}
	}


	ofstream fspt;
	char filenamept[255];
	sprintf(filenamept, "binNumVersusbinMax.csv");
	fspt.open(filenamept);
	fspt << "binNum,binMax" << endl;

	ofstream fsbt;
	char filenamebt[255];
	sprintf(filenamebt, "binNumVnumPerBin.csv");
	fsbt.open(filenamebt);
	fsbt << "binNum,numPerBin" << endl;

	ofstream fssumwt;
	char filenamesumwt[255];
	sprintf(filenamesumwt, "binNumVsumW_%1.2fa.csv",alpha);
	fssumwt.open(filenamesumwt);
	fssumwt <<"binNum,sumGLweight" << endl;

	ofstream fssumwp;
	char filenamesumwp[255];
	sprintf(filenamesumwp, "binMaxVsumW_%1.2fa.csv",alpha);
	fssumwp.open(filenamesumwp);
	fssumwp << "binMax,sumGLweight" << endl;

	ofstream fst0;
	char filenamet0[255];
	sprintf(filenamet0, "DaOfT_%dsamples_%dstored_%1.2fa_%1.2fdT.csv", sampleLength, storedPoints, alpha, deltaT);
	fst0.open(filenamet0);
	fst0 << fixed <<showpoint << setprecision(4);
	fst0<< "time,f(t),IIR_CFE,Grunwald,exponential,recursive,";
	for(int i = 0; i <= lastShiftAvgData; i++)
	{
		fst0 << "shiftAvg" << shiftAvgType[i];
		if(i == lastShiftAvgData)
		{
			fst0 << endl;
		}
		else
			fst0 << ",";
	}

	//AvgShiftRegister approximation
	vector<Dalpha> DshiftAvgVect; 
	for(int i = 0; i <= lastShiftAvgData; i++)
	{
		Dalpha DshiftAvg = Dalpha();
		DshiftAvg.setProperties(alpha,deltaT,sampleLength);
		DshiftAvg.setPartition(shiftAvgType[i], storedPoints);
		std::cout << "Partition " << shiftAvgType[i] << " set" << endl;
		DshiftAvg.computeWeights();
		std::cout << "Weights computed" << endl;
		DshiftAvgVect.push_back(DshiftAvg); 
	}

	for(int k=0; k<storedPoints; k++)
	{
		fspt << k << ",";
		fsbt << k << ",";
		fssumwt << k << ",";
		for(int i=0; i<=lastShiftAvgData; i++)
		{
			fspt << DshiftAvgVect[i].getBinMax(k);
			fsbt << DshiftAvgVect[i].getNumPerBin(k);
			fssumwt << DshiftAvgVect[i].getWeight(k);
			fssumwp << DshiftAvgVect[i].getBinMax(k);
			fssumwp << "," << DshiftAvgVect[i].getWeight(k);
			if(i==lastShiftAvgData)
			{
				fspt<< endl;
				fsbt << endl;
				fssumwt << endl;
				fssumwp <<endl;
			}else{
				fspt << ",";
				fsbt << ",";
				fssumwt << ",";
				fssumwp << ",";
			}
		}
	}

	//output must be divided between two files because Maple can handle only up to ten columns in a given input matrix for plotting
	ofstream fs2;
	char filename2[255];
	sprintf(filename2, "freqPhase_%dsamples_%dstored_%1.2fa_%1.2fdT.csv", sampleLength, storedPoints, alpha, deltaT);
	fs2.open(filename2);
	fs2 << fixed << showpoint << setprecision(4);

	fs2 << "freq,CFEPhase,fullPhase,expPhase,recPhase,";
	for(int i = 0; i <= lastShiftAvgData; i++)
	{
		fs2 << "shiftAvgPhase" << shiftAvgType[i];
		if(i == lastShiftAvgData)
		{
			fs2 << endl;
		}
		else
			fs2 << ",";
	}

	ofstream fs3;
	char filename3[255];
	sprintf(filename3, "freqAmplitude_%dsamples_%dstored_%1.2fa_%1.2fdT.csv", sampleLength, storedPoints, alpha, deltaT);
	fs3.open(filename3);
	fs3 << fixed << showpoint << setprecision(4);

	fs3 << "freq,CFEAmp,fullAmp,expAmp,recAmp,";
	for(int i = 0; i <= lastShiftAvgData; i++)
	{
		fs3 << "shiftAvgAmp" << shiftAvgType[i];
		if(i == lastShiftAvgData)
		{
			fs3 << endl;
		}
		else
			fs3 << ",";
	}

	ofstream fs;
	char filename[255];
	sprintf(filename, "freqPhaseAmplitude_%dsamples_%dstored_%1.2fa_%1.2fdT.csv", sampleLength, storedPoints, alpha, deltaT);
	fs.open(filename);
	fs << fixed << showpoint << setprecision(4);

	fs << "inputFreq,CFEAmp,CFEPhase,fullAmp,fullPhase,expAmp,expPhase,recAmp,recPhase,";
	for(int i = 0; i <= lastShiftAvgData; i++)
	{
		fs << "shiftAvgAmp" << shiftAvgType[i] << ",shiftAvgPhase" << shiftAvgType[i];
		if(i == lastShiftAvgData) 
			fs << endl;
		else
			fs << ",";
	}
	vector<double> inputData(sampleLength);
	vector<double> outputVectorRec(sampleLength);
	vector<double> outputVectorExp(sampleLength);
	vector<double> outputVectorFull(sampleLength);

	cout << "numFreqs: " << freqParams.size() << endl;
			
	double peakAmp, peakPhase;
	for(unsigned int i = 0; i < freqParams.size(); i++) 
	{
		double frequency = freqParams[i]; //frequency
		double phase = 0.0; //phase of 0
		double amplitude = 1.0; //amplitude of 1

		for(int n=0; n<sampleLength; n++)
		{
			inputData[n] = inputFunction(n, deltaT, amplitude, frequency);
		}

		fs << frequency << ",";
		cout << frequency << ",";
		fs2 << frequency << ",";
		fs3 << frequency << ",";
		//CFE IIR computation
		//cfeIIR.getWeights();
		//cfeIIR.printWeights();
		cfeIIR.differintegrate(inputData,sampleLength);
		//cfeIIR.rescale(deltaT, tSample);
		analyzeSignal(cfeIIR.outputArray, deltaT, frequency, &peakAmp, &peakPhase);
		fs << peakAmp << "," << peakPhase << ",";
		cout << peakAmp << "," << peakPhase << ",";
		fs2 << peakPhase << ",";
		fs3 << peakAmp << ",";

		std::cout << "CFE complete" <<endl;

		//faster full computation 
		if(sampleLength<=10000)
		{
		Integrator fullIntgrator(alpha, deltaT, sampleLength, 0.99); 
		fullIntgrator.clearData();
		for(int j = 0; j < sampleLength; j++)
		{
			fullIntgrator.integrateValue(inputData[j]);
			outputVectorFull[j] = fullIntgrator.getValue();
		}
		analyzeSignal(outputVectorFull, deltaT, frequency, &peakAmp, &peakPhase);
		fs << peakAmp << "," << peakPhase << ",";
		cout << peakAmp << "," << peakPhase << ",";
		fs2 << peakPhase << ",";
		fs3 << peakAmp << ",";
		}else{	
		fs<< "0,0,";
		fs2 << "0,";
		fs3 << "0,";
		}
		std::cout<< "full complete" <<endl;

		//exponential decay approx
if(calcTerm) {
		expIntgrator.clearData();
		for(int j = 0; j < sampleLength; j++)
		{
			expIntgrator.integrateValue(inputData[j]);
			outputVectorExp[j] = expIntgrator.getValue();
		}
		analyzeSignal(outputVectorExp, deltaT, frequency, &peakAmp, &peakPhase);
}else{
	peakAmp=0.0;
	peakPhase=0.0;
}
		fs << peakAmp << "," << peakPhase << ","; 
		cout << peakAmp << "," << peakPhase << ",";
		fs2 << peakPhase << ",";
		fs3 << peakAmp << ",";
//}

		//recusive time compression approx
if(calcRec)
{
		recIntegrator.clearData();
		for(int j = 0; j < sampleLength; j++)
		{
			recIntegrator.integrateValue(inputData[j]);
			outputVectorRec[j] = recIntegrator.getValue();
		}
		analyzeSignal(outputVectorRec, deltaT, frequency, &peakAmp, &peakPhase);
}else{
	peakAmp=0.0;
	peakPhase=0.0;
}
		fs << peakAmp << "," << peakPhase << ","; 
		cout << peakAmp << "," << peakPhase << ",";
		fs2 << peakPhase << ",";
		fs3 << peakAmp << ",";

		//AvgShiftRegister approx
		for(int d = 0; d < DshiftAvgVect.size(); d++)
		{
			DshiftAvgVect[d].zeroHistory();
			DshiftAvgVect[d].differintegrate(inputData);
			analyzeSignal(DshiftAvgVect[d].outputData, deltaT, frequency, &peakAmp, &peakPhase);
			fs  << peakAmp << "," << peakPhase;
			cout << peakAmp << "," << peakPhase;
			fs2 << peakPhase;
			fs3 << peakAmp;

			if( d == DshiftAvgVect.size()-1)
			{
				fs << endl; 
				cout << endl << endl;
				fs2 << endl;
				fs3 << endl;
			}
			else 
			{
				fs <<","; 
				cout << ",";
				fs2 << ",";
				fs3 << ",";
			}			
		}

		if((outputTimeDomain)&&(i==floor(log10(nyquistFreq/minFreq)*5.0/2.0)))
		{
	for(int n = 0; n< sampleLength; n++)
	{
		fst0 << deltaT*n << "," << inputData[n] << "," << cfeIIR.outputArray[n] << ",";
		fst0 << outputVectorFull[n] << "," << outputVectorExp[n] << ",";
		fst0 << outputVectorRec[n] << ",";
		for(int d = 0; d < DshiftAvgVect.size(); d++)
		{
			fst0 << DshiftAvgVect[d].outputData[n];
			if( d == DshiftAvgVect.size()-1)
			{
				fst0 << endl; 
			}else{
				fst0 << ",";
			}
		}
	}
	}
		fs.flush();
		fs2.flush();
		fs3.flush();
		fst0.flush();
	}
	fs.close();
	fs2.close();
	fs3.close();
	fst0.close();
	fspt.close();
	fsbt.close();
	fssumwt.close();
	
}

	


void MAIN::generateComparisonPlot(void) 
{//want time domain plots to check frequency reconstruction


	// need to fix this so that it handles the new setFunctionParameters method
	const int Nmax = 1000;
	double deltaT = 0.01;
	double alpha = 0.5;
	int approxPartitionMax = 9;

	vector<double> inputData;
	inputData.resize(Nmax);

	for(int n= 0; n<Nmax; n++)
	{
		inputData[n] = inputFunction(n, deltaT, 1.0, 1.0);
	}

	Dalpha Dlinear = Dalpha();
	Dlinear.setProperties(alpha, deltaT, Nmax);
	Dlinear.setPartition(1, 9);
	Dlinear.differintegrate(inputData);
	Dlinear.print("Dalpha_d9_linearPart.csv", inputData);

	approxPartitionMax =81;
	Dalpha DlinearLong = Dalpha();
	DlinearLong.setProperties(alpha, deltaT, Nmax);
	DlinearLong.setPartition(1, 81);
	DlinearLong.differintegrate(inputData);
	Dlinear.print("Dalpha_d81_linearPart.csv", inputData);

	approxPartitionMax = 81; //goal is to have a partition size of 10 to match linear. PartitionMax = (partitionSize-1)^2
	Dalpha Dsquared = Dalpha();
	Dsquared.setProperties(alpha, deltaT, Nmax);
	Dsquared.setPartition(2, 9);
	Dsquared.differintegrate(inputData);
	Dsquared.print("Dalpha_d81_squaredPart.csv", inputData);

	approxPartitionMax = Nmax;
	Dalpha Dfull = Dalpha();
	Dfull.setProperties(alpha, deltaT, Nmax);
	Dfull.setPartition(1, Nmax);
	Dfull.differintegrate(inputData);
	Dfull.print("Dalpha_full_linearPart.csv", inputData);
}

void MAIN::genAmpPhaseGrundwald(void)
{// want a plot of phase reconstruction determination-- both time domain signal and up close for phase determination

	double fullAmp, shiftAvgAmp, fullPhase, shiftAvgPhase, theoryAmp, theoryPhase, powerlawAmp, powerlawPhase;

	// simplified, no average shift register. Grundwald recursion
	//FullGrundwald Dfull = FullGrundwald();
	//FullGrundwald DshiftAvg = FullGrundwald();
	//FullGrundwald Dpowerlaw = FullGrundwald();

	// average shift register. grundwald recursion
	//Grundwald Dfull = Grundwald();
	//Grundwald DshiftAvg = Grundwald(); 
	//Grundwald Dpowerlaw = Grundwald();
	
	// direct sum with weights using Grundwald integral formula. Average shift register
	Dalpha Dfull = Dalpha();
	Dalpha DshiftAvg = Dalpha();
	Dalpha Dpowerlaw = Dalpha();

	
	ofstream fs;
	//fs.open("freqPhaseAmpFullGrundwald");
	//fs.open("freqPhaseAmpGrundwald.csv");
	fs.open("freqPhaseAmp.csv");
	fs << fixed << setprecision(4);

	fs << "frequency" << "," << "theoryAmp" << "," << "theoryPhase" << "," << "fullAmp" << "," 
	   << "fullPhase" << "," << "shiftAvgAmp" <<"," << "shiftAvgPhase" << "," << "powerlawAmp" << "," << "powerlawPhase" << endl;


	double pi = 3.14159265;
	double alpha = 0.5; // index of fractional integral
	int Nmax = 1000; // data points in time series
	double deltaT = 0.01; // seconds

	Dfull.setProperties(alpha, deltaT, Nmax);
	DshiftAvg.setProperties(alpha,deltaT,Nmax);
	Dpowerlaw.setProperties(alpha,deltaT,Nmax);

	Dfull.setPartition(4, 10);
	DshiftAvg.setPartition(7, 10);
	Dpowerlaw.setPartition(8,10); // power law index, best at high frequencies so far. not good at low frequencies
	
	vector<double> CAfull;
	vector<double> CAshiftAvg;
	vector<double> CApowerlaw;
	vector<double> inputData;

	double parameters[3];
	double aFull[2];
	double aShiftAvg[2];
	double aPowerlaw[2];

	CAfull.resize(Nmax);
	CAshiftAvg.resize(Nmax);
	CApowerlaw.resize(Nmax);
	inputData.resize(Nmax);

	Dfull.computeWeights();
	DshiftAvg.computeWeights();
	Dpowerlaw.computeWeights();

	double freqSample = 1.0/deltaT;
	double freqMin = freqSample/((double) Nmax);

	double freqNyquist = freqSample /2.0;
	int freqPerDecade = 5;
	int numFreqs = (int) floor(log10(freqNyquist/freqMin)*freqPerDecade);
		
	// max is 13 for 1000, 16 for 5000
	//for(int freqNum = 15; freqNum <=15; freqNum++) 
	for(int freqNum = 0; freqNum < numFreqs; freqNum++) 
	{	
		parameters[0] = freqMin*pow(10.0,((double) freqNum)/((double) freqPerDecade)); //frequency
		parameters[1] = 0.0; //phase of 0
		parameters[2] = 1.0; //amplitude of 1
		double frequency = parameters[0];

		for(int n=0; n<Nmax; n++)
		{
			inputData[n] = inputFunction(n, deltaT, parameters[2], parameters[0]);
		}

		Dfull.zeroHistory();
		DshiftAvg.zeroHistory();
		Dpowerlaw.zeroHistory();

		DshiftAvg.differintegrate(inputData);
		Dpowerlaw.differintegrate(inputData);
		Dfull.differintegrate(inputData);
		
		UINT minBin = Nmax/2;
		for(int n=0; n<Nmax; n++)
		{
			CAfull[n] = Dfull.outputData[n];
			CAshiftAvg[n]= DshiftAvg.outputData[n];
			CApowerlaw[n]= Dpowerlaw.outputData[n];
		}
	
		UINT includeRange = Nmax-minBin;

		int n= 500;

		SineFit(frequency, deltaT, minBin, includeRange, CAfull, aFull);
		SineFit(frequency, deltaT, minBin, includeRange, CAshiftAvg, aShiftAvg);
		SineFit(frequency, deltaT, minBin, includeRange, CApowerlaw, aPowerlaw);

		
		if(Nmax== 1000)
		{
			if((freqNum ==10)||(freqNum==12)||(freqNum ==13))
			{
				ofstream fstime;
				switch(freqNum)
				{
				case 10:
					fstime.open("timeSeriesFullGrund_10_1000.csv");
					break;
				case 12:
					fstime.open("timeSeriesFullGrund_12_1000.csv");
					break;
				case 13:
					fstime.open("timeSeriesFullGrund_13_1000.csv");
					break;
				}
				for(int i = 0; i<Nmax; i++)
				{
					fstime << deltaT*i << "," << inputData[i] << "," << CAfull[i] << endl;
				}
				fstime.close();
			}
		}
		if(Nmax== 5000)
		{
			if((freqNum ==13)||(freqNum==15)||(freqNum ==16))
			{
				ofstream fstime;
				switch(freqNum)
				{
				case 13:
					fstime.open("timeSeriesGrundwald_13_5000.csv");
					break;
				case 15:
					fstime.open("timeSeriesGrundwald_15_5000.csv");
					break;
				case 16:
					fstime.open("timeSeriesGrundwald_16_5000.csv");
					break;
				}
				for(int i = 0; i<Nmax; i++)
				{
					fstime << deltaT*i << "," << inputData[i] << "," << CAfull[i] << "," << CAshiftAvg[i] << "," << CApowerlaw[i] << endl;
				}
				fstime.close();
			}
		}
		fullAmp = magnitude(aFull[0], aFull[1]);
		fullPhase = phase(aFull[0], aFull[1]);
		
		shiftAvgAmp = magnitude(aShiftAvg[0], aShiftAvg[1]);
		shiftAvgPhase = phase(aShiftAvg[0], aShiftAvg[1]);

		powerlawAmp = magnitude(aPowerlaw[0], aPowerlaw[1]);
		powerlawPhase = phase(aPowerlaw[0], aPowerlaw[1]);

		//alpha is divided by four because the phase is equal to pi*alpha/2 which is 2*pi*alpha/4
		theoryPhase = (alpha/4.0)*360.0;
		theoryAmp = pow((2.0*pi*frequency),alpha);

		fs << frequency << "," << theoryAmp << "," << theoryPhase << "," << fullAmp;
		fs << "," << fullPhase << "," << shiftAvgAmp <<"," << shiftAvgPhase << "," << powerlawAmp << "," << powerlawPhase<< endl;
	}

	fs.close();

}

void MAIN::stepFnDiffEq(void)
{
	Dalpha Ialpha; //avg Grunwald integral
	Dalpha Palpha; //avg Grunwald part sum
	Dalpha IalphaFull; //full Grunwald integral
	Dalpha PalphaFull; //full Grunwald part sum

	double deltaT = 0.05;
	double Nmax = 1000;
	double alpha = -1.3; //must be negative
	double rho = 1.0; //scaling constant
	double kappa = 1.0; // scaling constant

	// rho*y^alpha + kappa *y = f(t), f(t) is square wave
	double frequency = 0.3;
	double amplitude=1.0;
	double phase =0.0;

	Ialpha.setProperties(alpha,deltaT,Nmax);
	Palpha.setProperties(alpha,deltaT,Nmax);
	Ialpha.setPartition(3,20);
	Palpha.setPartition(3,20);
	Ialpha.zeroHistory();
	Palpha.zeroHistory();
	Ialpha.computeWeights();
	Palpha.computeWeights();


	IalphaFull.setProperties(alpha,deltaT,Nmax);
	PalphaFull.setProperties(alpha,deltaT,Nmax);
	IalphaFull.setPartition(1,Nmax);
	PalphaFull.setPartition(1,Nmax);
	IalphaFull.zeroHistory();
	PalphaFull.zeroHistory();
	IalphaFull.computeWeights();
	PalphaFull.computeWeights();

	double Ialphaf; //input integral of previous n-1 time steps
	double Palphay; //partial sum y of previous n-1 time steps
	double IalphaFullf;
	double PalphaFully;


	ofstream fs;
	fs.open("stepFnDiffEq.csv");
	fs << "time,input,GL,EXP20" <<endl;

	for(int n=0; n<Nmax; n++)
	{
		double inputSignal = inputStepFunction(n,deltaT,amplitude,frequency,phase);
		Ialpha.updateHistory(inputSignal); //update the history with the new value of the input signal
		Ialphaf=Ialpha.differintegrateOnce();
		IalphaFull.updateHistory(inputSignal);
		IalphaFullf=Ialpha.differintegrateOnce();


		Palpha.updateHistory(0.0); //shift the history back one, disregarding the value in the zeroth bin
		Palphay=Palpha.partSumOnce();
		double y0= Ialphaf-Palphay;
		y0/=1.0+pow(deltaT,-alpha)*(kappa/rho);
		Palpha.setPresentValue(y0); //set the present value of y to y0 in Palpha's history so next time it is shifted
		// it will update properly

		PalphaFull.updateHistory(0.0); //shift the history back one, disregarding the value in the zeroth bin
		PalphaFully=PalphaFull.partSumOnce();
		double y0Full= IalphaFullf-PalphaFully;
		y0Full/=1.0+pow(deltaT,-alpha)*(kappa/rho);
		PalphaFull.setPresentValue(y0Full); //set the present value of y to y0 in Palpha's history so next time it is shifted
		// it will update properly

		fs << n*deltaT << "," << inputSignal<< "," << y0Full << "," << y0 << endl;
	}

	fs.close();
}