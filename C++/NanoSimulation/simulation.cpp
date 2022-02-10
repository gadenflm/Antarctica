#include "simulation.h"
#include "basedfunc.h"
#include "coeffi.h"
#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

/****** BGF Class *******/

FBioGradField::FBioGradField(const int XPara)
{
	if (XPara <= 0) 
	{
		SIMU_ERR("Wrong BGF Size");
	}	
	SizeX = XPara;
	
	Value.resize(SizeX);
	for (int i = 0; i < SizeX; ++i)
			Value[i] = 0;
}

const vector<double>& FBioGradField::GetValue()
{
	return Value;
}

bool FBioGradField::CalcOneDiffusAtZero(double Time, double XBegin, double XEnd, const FConstant& Coeffi)
{
	if (Time <= 0 || XBegin >= XEnd)
	{
		SIMU_ERR("Wrong parameters!");
		return SIMU_WRONG;
	}	

	for (int i = 0; i < SizeX; ++i)
	{
		double LocX = Get1Location(i, SizeX, XBegin, XEnd);
		Value[i] = 1 / sqrt(4*SIMU_PI*Coeffi.DiffuCoeffi*Time) * exp(-pow(LocX - Coeffi.BloodFlowVeloc*Time, 2) / (4*Coeffi.DiffuCoeffi*Time));
	}

	return SIMU_OK;
}

/****** Simulation Class ******/

FSimulation::FSimulation(uint64_t IterTotalNumPara, double TimeStepPara)
{
	IterNum = 0;
	IterTotalNum = IterTotalNumPara;
	Time = 0; 
}

FSimulation::~FSimulation()
{
	
}