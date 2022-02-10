#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <vector>
#include <iostream>
#include "coeffi.h"

// BGF Class
class FBioGradField
{
public:
    FBioGradField(const int XPara = 0);
    ~FBioGradField();

    /**
     * @brief           计算 x = 0 处 t = 0 时刻产生一次BGF高斯脉冲，t = Time 时刻 BGF 各点的值
     * 
     * @param Time      所需计算的时间
     * @param XBegin    单轴X 起始位置
     * @param XEnd      单轴X 结束位置
     * @param Coeffi    仿真系数，见@coeffi.h
     */
    bool CalcOneDiffusAtZero(double Time, double XBegin, double XEnd, const FConstant& Coeffi); 

    
    bool CalcDiffusions(double Time, double XBegin, double XEnd);

    const std::vector<double>& GetValue();
private:
    // The number of BGF value in one dimension: SizeX
    int SizeX;
    std::vector<double> Value;

    // 2-dimension
    // std::vector<std::vector<double>> Value;
};

// Simulation Class:
class FSimulation
{
public:
    FSimulation(uint64_t IterTotalNumPara = 1000, double TimeStepPara = 0.01);
    ~FSimulation();

    bool QuickSimula();
private:
    // time :
    uint64_t    IterNum; 
    uint64_t    IterTotalNum;
    double      Time;
    double      TimeStep;    

};

#endif