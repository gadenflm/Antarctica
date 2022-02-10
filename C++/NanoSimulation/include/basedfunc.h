#ifndef __BASEDFUNC_H__
#define __BASEDFUNC_H__

#include <cmath>

static const double EqThreshold = 1e-5;
inline bool IsFloatsEq(double para1, double para2)
{
   if (std::abs(para1 - para2) <= EqThreshold) 
   {
       return true;
   }
   else
   {
       return false;
   }
}

inline bool IsFloatsEq(float para1, float para2)
{
   if (std::abs(para1 - para2) <= EqThreshold) 
   {
       return true;
   }
   else
   {
       return false;
   }
}

inline double Get1Location(int Index, int TotalInd,double Start, double End)
{
    return (Start + Index * (End - Start) / TotalInd);
}

//Print Log and Error define:
#define SIMU_OK 0
#define SIMU_WRONG 1

#define SIMU_LOG(fmt,...)\
{ \
    printf("[Simulation Log] " #fmt " \t@%s,%d\n",##__VA_ARGS__,__FILE__,__LINE__);\
    fflush(stdout);  \
}

#define SIMU_ERR(fmt,...)\
{ \
    printf("[Simulation Error] " #fmt " \t@%s,%d\n",##__VA_ARGS__,__FILE__,__LINE__);\
    fflush(stdout);  \
}
#endif