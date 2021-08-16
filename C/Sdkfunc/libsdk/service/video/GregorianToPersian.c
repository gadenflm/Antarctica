////////////////////////////////////////////////////////////////////////////////
#include "GregorianToPersian.h"

static unsigned int encode( SDK_DateData_S * d ) 
{
    unsigned int  ret = ((d->year << 16) & 0xFFFF0000) | ((d->month << 8) & 0xFF00) | d->day;
    return ret;
}

// static  void decode(unsigned int encode, SDK_DateData_S * d )
// {
//     d->year = encode >> 16 & 0xFFFF;
//     d->month = encode >> 8 & 0xFF;
//     d->day = encode & 0xFF;
// }

SDK_G2P_BOOL IsLeapyear(unsigned int year,SDK_G2P_BOOL persian)
{
    if (persian)
    {
        int res = (int)((year + 37) * 31 / 128.0 * 100) % 100;
        if (res < 31)
        {
            return SDK_G2P_FALSE;
        }
        res = (int)((year + 38) * 31 / 128.0 * 100) % 100;
        if (res >= 31)
        {
            return SDK_G2P_FALSE;
        }
        return SDK_G2P_TRUE;
    }
    else
    {
        if ((year % 100 != 0 && year % 4 == 0) || year % 400 == 0)
        {
            return SDK_G2P_TRUE;
        }

        return SDK_G2P_FALSE;
    }
}

unsigned int DaysInMonth(unsigned int year,unsigned int month,SDK_G2P_BOOL persian)
{
    if (persian)
    {
        unsigned int days[12] = {31,31,31,31,31,31,30,30,30,30,30,29};
        if (IsLeapyear(year,SDK_G2P_TRUE))
        {
            days[11] = 30;
        }
        return days[(month - 1) % 12];
    }
    else
    {
        unsigned int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
        if (IsLeapyear(year,SDK_G2P_FALSE))
        {
            days[1] = 29;
        }
        return days[(month - 1) % 12];
    }
}

unsigned int DaysToNextMonth(unsigned int year,unsigned int month,unsigned int day,SDK_G2P_BOOL persian)
{
    unsigned int days = DaysInMonth(year,month,persian);
    return days - day + 1;
}

unsigned int DaysToNextYear(unsigned int year,unsigned int month,unsigned int day,SDK_G2P_BOOL persian)
{
    unsigned int days = 0;
    int i;
    
    for ( i = month; i < 13; ++i)
    {
        days += DaysToNextMonth(year,i,day,persian);
        day = 1;
    }

    return days;
}

// 获取公历gYear-gMonth-gDay到2016-1-1的天数
int GregorianDaysTo(unsigned int gYear,unsigned int gMonth,unsigned int gDay)
{
    // 2016,1,1
    SDK_G2P_BOOL negative = SDK_G2P_FALSE;
    SDK_DateData_S early = {2016,1,1};
    SDK_DateData_S later = {gYear,gMonth,gDay};
    if (encode(&early) > encode(&later))
    {
        SDK_DateData_S temp;
        temp = early;early = later;later = temp;
        negative = SDK_G2P_TRUE;
    }
    int days = 0;
    while (encode(&early) != encode(&later))
    {
        if (early.year < later.year)
        {
            // 处理年份不相同，先计算年份相同时的days（年份不同，先计算early到later同年1月1号需要的days）
            if (early.month != 1 || early.day != 1)
            {
                days += DaysToNextYear(early.year,early.month,early.day,SDK_G2P_FALSE);
                ++early.year;
                early.month = early.day = 1;
            }
            else
            {
                days += 365;
                if (IsLeapyear(early.year,SDK_G2P_FALSE))
                {
                    ++days;
                }
                ++early.year;
            }
        }
        else if (early.month < later.month)
        {
            // 同年，计算到同月的days（月份不同，先计算early到later同月1号需要的days）
            if (early.day != 1)
            {
                days += DaysToNextMonth(early.year,early.month,early.day,SDK_G2P_FALSE);
                ++early.month;
                early.day = 1;
            }
            else
            {
                days += DaysInMonth(early.year,early.month,SDK_G2P_FALSE);
                ++early.month;
            }
        }
        else
        {
            // 同年同月，计算到同日的days
            days += later.day - early.day;
            early.day = later.day;
        }
    }

    if (negative)
    {
        days = -days;
    }

    return days;
}

// 波斯历stDateData减一天
void DecreaseOneDay( SDK_DateData_S * d )
{
    if (d->day > 1)
    {
        --d->day;
    }
    else
    {
        if (d->month > 1)
        {
            --d->month;
            d->day = DaysInMonth(d->year,d->month,SDK_G2P_TRUE);
        }
        else
        {
            --d->year;
            d->month = 12;
            d->day = 29;
            if (IsLeapyear(d->year,SDK_G2P_TRUE))
            {
                d->day = 30;
            }
        }
    }
}

// 波斯历stDateData加一天
void IncreaseOneDay( SDK_DateData_S * d )
{
    int monthDay = DaysInMonth(d->year,d->month,SDK_G2P_TRUE);
    if (d->day < monthDay)
    {
        ++d->day;
    }
    else
    {
        if (d->month < 12)
        {
            ++d->month;
            d->day = 1;
        }
        else
        {
            ++d->year;
            d->month = d->day = 1;
        }
    }
}

// 获取波斯历1394-10-11增加days后的日期由pYear-pMonth-pDay传回
void PersianAddDays(unsigned int * pYear,unsigned int * pMonth,unsigned int * pDay,int days)
{
    //1394,10,11,
    SDK_DateData_S  temp = {1394,10,11};
    if (days > 0)
    {
        while (days > 0)
        {
            if (days >= 365)
            {
                // 处理days大于1年
                if (IsLeapyear(temp.year,SDK_G2P_TRUE))
                {
                    DecreaseOneDay(&temp);
                }
                ++temp.year;
                days -= 365;
            }
            else if (days >= 29)
            {
                // 处理days接近大于1个月
                int monthDay = DaysInMonth(temp.year,temp.month,SDK_G2P_TRUE);
                days -= 29;
                temp.day += 29;
                if (temp.day > monthDay)
                {
                    ++temp.month;
                    if (temp.month > 12)
                    {
                        ++temp.year;
                        temp.month = 1;
                    }
                    temp.day -= monthDay;
                }
            }
            else
            {
                // 处理days小于1个月
                IncreaseOneDay(&temp);
                --days;
            }
        }
    }else
    {
        days = -days;
        while (days > 0)
        {
            if (days >= 365)
            {
                // 处理days大于1年
                if (IsLeapyear(temp.year-1,SDK_G2P_TRUE))
                {
                    IncreaseOneDay(&temp);
                }
                --temp.year;
                days -= 365;
            }
            else if (days >= 29)
            {
                // 处理days接近大于1个月
                int year = temp.year,month=temp.month;
                if (temp.month <= 1)
                {
                    --year;
                    month = 12;
                }
                else
                {
                    --month;
                }
                int monthDay = DaysInMonth(year,month,SDK_G2P_TRUE);
                days -= 29;
                if (temp.day > 29)
                {
                    temp.day -= 29;
                }
                else
                {
                    --temp.month;
                    if (temp.month < 1)
                    {
                        --temp.year;
                        temp.month = 12;
                    }
                    temp.day += monthDay-29;
                }
            }
            else
            {
                // 处理days小于1个月
                DecreaseOneDay(&temp);
                --days;
            }
        }
    }
    *pYear = temp.year; *pMonth = temp.month; *pDay = temp.day;
}

int GregorianToPersian( unsigned int gYear, unsigned int gMonth, unsigned int gDay, unsigned int * pYear, unsigned int * pMonth, unsigned int * pDay)
{
    if (gDay > DaysInMonth(gYear,gMonth,SDK_G2P_FALSE))
    {
        return SDK_G2P_FALSE;
    }
    int adds = GregorianDaysTo(gYear,gMonth,gDay);
    PersianAddDays(pYear,pMonth,pDay,adds);
    return SDK_G2P_TRUE;
}