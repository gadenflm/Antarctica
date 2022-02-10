#ifndef __COEFFI_H__
#define __COEFFI_H__

typedef struct st_constant
{
	double DiffuCoeffi;
	double BloodFlowVeloc;
} FConstant;

const FConstant NormalConst = {
	.DiffuCoeffi = 1.0f,
	.BloodFlowVeloc = 1.0f
};

const double SIMU_PI = 3.14159265f;
#endif