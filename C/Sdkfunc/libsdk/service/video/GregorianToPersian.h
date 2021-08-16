#ifndef __HI_GREGOYIAN_TO_PERSIAN_H__
#define __HI_GREGOYIAN_TO_PERSIAN_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

////////////////////////////////////////////////////////////////////////////////

typedef enum {
    SDK_G2P_FALSE = 0,
    SDK_G2P_TRUE  = 1,
} SDK_G2P_BOOL;


typedef struct _sdk_DateData_s
{
    unsigned int year;
    unsigned int month;
    unsigned int day;
}SDK_DateData_S;


////////////////////////////////////////////////////////////////////////////////

int GregorianToPersian( unsigned int gYear, unsigned int gMonth, unsigned int gDay, unsigned int * pYear, unsigned int * pMonth, unsigned int * pDay);

////////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_GREGOYIAN_TO_PERSIAN_H__ */

