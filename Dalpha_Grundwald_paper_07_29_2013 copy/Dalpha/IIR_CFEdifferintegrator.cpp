#include "IIR_CFEdifferintegrator.h"
#include "Nrutil.h"
#include <fstream>


IIR_CFEdifferintegrator::IIR_CFEdifferintegrator(double alpha, int numCoeffinNumerator, double deltaT0)
{
	deltaT = deltaT0;
	I_alpha = alpha;
	PQ.initialize(alpha, numCoeffinNumerator);
	CFE_SIZE = numCoeffinNumerator;
	PQ.calculatePolynomials();
	PQ.rescale();
}

IIR_CFEdifferintegrator::~IIR_CFEdifferintegrator(void)
{
}


void IIR_CFEdifferintegrator::getWeights(void)
{
//	PQ.calculatePolynomials();
//	PQ.rescale();
	// Generate the coefficients for the digital FOC
	// Uses the continued fraction expansion method to
	// generate the coefficient set from powers of the exponent

	
	/*	double r[CFE_SIZE];
	r[0] = 1.0;
	r[1] = I_alpha;
	for(UINT j=2;j<CFE_SIZE;j++) 
		r[j] = r[j-1]*r[1];

	CoeffB[0] = r[0];
	CoeffB[1] = r[1];
	CoeffB[2] = (-2.11764706)*r[0] + 0.470588235*r[2];
	CoeffB[3] = (-1.78431373)*r[1] + 0.137254902*r[3];
	CoeffB[4] = 1.482352941*r[0] - 0.68627451*r[2] + 0.02745098*r[4];
	CoeffB[5] = 0.976472213*r[1] - 0.1568275*r[3] + 0.003921569*r[5];
	CoeffB[6] = (-0.3800905)*r[0] + 0.276118653*r[2] -0.0231272*r[4] + 0.000402212*r[6];
	CoeffB[7] = (-0.16664512)*r[1] +0.042131724*r[3] - 0.00221217*r[5] + (2.87294e-5)*r[7];
	CoeffB[8] = 0.025915261*r[0] - 0.02640105*r[2] + 0.003491933*r[4] - 0.00012798*r[6] + (1.30588e-6)*r[8];
	CoeffB[9] = 0.00427912*r[1] - 0.00152295*r[3] + 0.000126758*r[5] - (3.4824e-6)*r[7] + (2.90196e-8)*r[9];
	
	double sign = 1.0;
	for(UINT j= 0;j<CFE_SIZE;j++) 
	{
		CoeffA[j] = sign * CoeffB[j];
		sign = (-sign);
	}*/
	

}

void IIR_CFEdifferintegrator::printWeights(void)
{
	ofstream fs;
	fs.open("testIIRCFEweights.txt");
	for(int j=0; j<CFE_SIZE; j++)
	{
		fs << j << "\t" << PQ.Pnow[j] << "\t" << PQ.Qnow[j] << endl;
	}
	fs.close();
}


void IIR_CFEdifferintegrator::differintegrate(vector<double>& inputArray, int sizeInputArray)
{
	ShiftRegister2 CX;
	ShiftRegister2 CY;
		
	CX.SetSize(CFE_SIZE);
	CY.SetSize(CFE_SIZE);
	
	vector<double> UD(sizeInputArray);
	outputArray.resize(sizeInputArray);

	UD[0] = 0.0;
 
	int interval = 1; // Professor Bohannan says he has been using the program with TS = dT. - SD, 2/15/13
	for(int n=1;n<sizeInputArray;n++)
	{
		CX.Shift();
		CY.Shift();
		CX[0] = inputArray[n]; //X[0][n]
		UD[n] = 0.0;

		for(UINT j=0;j<CFE_SIZE;j++) 
			UD[n] += pow((deltaT/2.0),-I_alpha)*PQ.Pnow[j]*CX[j];

		for(UINT j=1;j<CFE_SIZE;j++) 
			UD[n] -= PQ.Qnow[j]*CY[j];

		outputArray[n] = CY[0] = UD[n];
	}
}
/*
void IIR_CFEdifferintegrator::rescale(double deltaT, double tSample)
{
	for(int n = 0; n< CFE_SIZE; n++)
	{
		double timeFac =pow((deltaT/2.0),-I_alpha);
		//double timeFac = pow((tSample/2.0), -I_alpha);
		outputArray[n]*= timeFac;
	
	}
}*/