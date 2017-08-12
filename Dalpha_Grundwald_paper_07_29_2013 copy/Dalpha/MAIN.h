#pragma once

#include <vector>

class MAIN
{
public:
	MAIN(void);
	~MAIN(void);
	double inputFunction(int N, double deltaT, double amplitude, double frequency, double phase = 0.0);
	// The input function to the integral. Set to be a sinusoid currently. 
	double inputStepFunction(int N, double deltaT, double amplitude, double frequency, double phase);
	//step function input for differential equation routine
	//double magnitude(double A1, double A2); //was commented out
	// amplitude reconstruction algorithm
//	double phase(double Bsin, double Bcos); //was commented out
	// Phase reconstruction algorithm
	void testCFEcoefficients(double alpha);
	//compare the continued fraction expansion polynomial coefficients from the recursive algorithm in PolyRatioFromContFrac
	//to those hard coded in CFEDifferintegrator. 
	void testSineFit(void);
	// tests the sine fitting routine in nrutils
	void testAvgShiftRegister(void);
	// tests the average shift register. 
	void generateAmpPhasePlot(double tSample, int sampleLength, int storedPoints, int CFEregisters, double alpha, int freqSpecType, int* shiftAvgType, bool outputTimeDomain, bool calcRec, bool calcTerm);
	//most current version. Does both Chad's approximations and Steven's approximations in one routine for comparison. 
	void generateComparisonPlot(void); //generate time Domain plot
	// generates data for a plot of unintegrated sinusoidal signal in the time domain
	// plus data for the integrated signal for the full integral, the integral with 10 registers, 
	// the integral with 81 registers (0 through 9 squared), 
	// and the integral with a squared partition and 10 registers
	void genAmpPhaseGrundwald(void);
	// generates amplitude and phase plots by sinusoidal fitting. Can use Grunwald, Dalpha, or 
	// FullGrunwald depending what object is instantiated at beginning
	void stepFnDiffEq(void);
	// solves a fractional order differential equation with a Mittag-Leffler solution by putting a step function
	// into it. 
};

