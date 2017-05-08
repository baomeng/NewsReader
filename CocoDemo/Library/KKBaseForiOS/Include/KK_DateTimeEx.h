//
//  KK_DateTimeEx.h
//  KKBaseForiOS
//
//  Created by  yuanjun on 11-11-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//
#ifndef  __KK_DATE_TIME_EX_H__
#define  __KK_DATE_TIME_EX_H__

#include "KK_Includes.h"
#include "KK_String.h"

class KK_DatetimeEx;

class KK_TimeEx
{
public:
    
	KK_TimeEx()	{ Clear(); }
	KK_TimeEx(long time_ex)	{ Clear(); SetTime(time_ex); }
	KK_TimeEx(long hour, long minute, long second, long tenthousandths = 0);
	KK_TimeEx(const KK_TimeEx& kktime_ex);
	KK_TimeEx(const char * str);
	KK_TimeEx(const wchar_t * wstr);
    
	virtual ~KK_TimeEx() { }
    
public:
	inline void Clear() { m_Time = 0; }
	inline void Now() { From_time_t(time(NULL)); }
    
	inline void From_time_t(time_t t)
	{
		KK_Assert(t >= 0);
		//tzset();
		
 		m_Time = t+[[NSTimeZone systemTimeZone] secondsFromGMT];
		m_Time = m_Time+86400;
		m_Time = m_Time%86400*10000;
		KK_Assert(m_Time >= 0);
	}
    
	inline bool IsClear() { return (m_Time == 0); }
    
	inline bool SetTime(const KK_TimeEx & kk_TimeEx)
	{
		m_Time = kk_TimeEx.m_Time;
		return true;
	}
	
	inline bool SetTime(int time_ex)
	{
		if (!IsValidTime(time_ex)) return false;
		m_Time = time_ex;
		return true;
	}
    
	inline bool SetTime(long hour, long minute, long second, long tenthousandths = 0)
	{
		if (!IsValidTime(hour, minute, second, tenthousandths)) return false;
		m_Time = hour * 36000000 + minute * 600000 + second * 10000 + tenthousandths;
		return true;
	}
    
	inline long GetTime(long& hour, long& minute, long& second) const
	{
		hour = Hours();
		minute = Minutes();
		second = Seconds();
		return -1;
	}
	inline long GetTime(long& hour, long& minute, long& second, long& tenthousandths) const
	{
		hour = Hours();
		minute = Minutes();
		second = Seconds();
		tenthousandths = Tenthousandths();
		return -1;
	}
	inline long GetTime() const { return m_Time; }
    
	inline long Hours() const { return m_Time/36000000; }
    
	inline long Minutes() const { return (m_Time/600000)%60; }
	inline long Seconds() const { return (m_Time/10000)%60; }
	inline long Tenthousandths() const { return m_Time%10000; }// Actually tenthousandths of seconds
    
public:
	KK_String   GetKKString() const;
	KK_StringA  GetKKStringA() const;
	KK_StringW  GetKKStringW() const;
	KK_StringU  GetKKStringU() const;
	KK_StringWA GetKKStringWA() const;
    
	void GetString(KK_StringA& str) const;
	void GetString(KK_StringW& wstr) const;
	void GetString(KK_StringU& ustr) const;
	
	bool SetTime(const char * str);
	bool SetTime(const wchar_t * wstr);
    
public:
	inline static bool IsValidTime(long time_ex){ return (time_ex >= 0 && time_ex < 864000000); }
	inline static bool IsValidTime(long hour, long minute, long second, long tenthousandths = 0){ return !(hour<0||hour>23||minute<0||minute>59||second<0||second>59||tenthousandths<0||tenthousandths>9999); }
    
public:
	inline long AddHours (long hours)//返回增加的天数
	{
        long lTime = m_Time;
		long newtime = (m_Time + (hours % 24) * 60 * 60 * 10000) % 864000000;
		m_Time = (newtime + 864000000) % 864000000;
        
		double nDays = hours / 24.00 + lTime / 864000000.00;
        if (nDays < 0) nDays = nDays - 1;
        
        return (long)nDays;
	}
    
	inline long AddMinutes (long minutes)//返回增加的天数
	{
        long lTime = m_Time;
		long newtime = (m_Time + (minutes % 1440) * 60 * 10000) % 864000000;
		m_Time = (newtime + 864000000) % 864000000;
        
        double nDays = minutes / 1440.00 + lTime / 864000000.00;
        if (nDays < 0) nDays = nDays - 1;
        
        return (long)nDays;
	}
    
    inline long AddSeconds(long seconds)//返回增加的天数
	{
        long lTime = m_Time;
		long newtime = (m_Time + (seconds % 86400) * 10000) % 864000000;
		m_Time = (newtime + 864000000) % 864000000;
        
        double nDays = seconds / 86400.00 + lTime / 864000000.00;
        if (nDays < 0) nDays = nDays - 1;
        
        return (long)nDays;
	}
	
    inline long AddTenthousandths(long tenthousandths)//返回增加的天数
	{
        long lTime = m_Time;
		long newtime = (m_Time + (tenthousandths % 864000000)) % 864000000;
		m_Time = (newtime + 864000000) % 864000000;
        
       	double nDays = tenthousandths / 864000000.00 + lTime / 864000000.00;
        if (nDays < 0) nDays = nDays - 1;
        
        return (long)nDays;
    }
//	inline long AddHours (long hours)//返回增加的天数
//     {
//     long lTime = m_Time;
//     long newtime = (m_Time + (hours % 24) * 60 * 60 * 10000) % 864000000;
//     m_Time = (newtime + 864000000) % 864000000;
//     
//     return (long)(hours / 24.00 + lTime / 864000000.00);
//     }
//     
//     inline long AddMinutes (long minutes)//返回增加的天数
//     {
//     long lTime = m_Time;
//     long newtime = (m_Time + (minutes % 1440) * 60 * 10000) % 864000000;
//     m_Time = (newtime + 864000000) % 864000000;
//     
//     return (long)(minutes / 1440.00 + lTime / 864000000.00);
//     }
//     
//     inline long AddSeconds(long seconds)//返回增加的天数
//     {
//     long lTime = m_Time;
//     long newtime = (m_Time + (seconds % 86400) * 10000) % 864000000;
//     m_Time = (newtime + 864000000) % 864000000;
//     
//     return (long)(seconds / 86400.00 + lTime / 864000000.00);
//     }
//     
//     inline long AddTenthousandths(long tenthousandths)//返回增加的天数
//     {
//     long lTime = m_Time;
//     long newtime = (m_Time + (tenthousandths % 864000000)) % 864000000;
//     m_Time = (newtime + 864000000) % 864000000;
//     
//     return (long)(tenthousandths / 864000000.00 + lTime / 864000000.00);//(tenthousandths + newtime为了防止溢出)
//     }
     
public:
	inline long operator - (const KK_TimeEx& kktime_ex)	{ return (m_Time - kktime_ex.GetTime() + 864000000) % 864000000; }
	inline KK_TimeEx operator - (long tenthousandths)//
	{
		KK_TimeEx kkTime_Ex(((m_Time - (tenthousandths % 864000000)) % 864000000 + 864000000) % 864000000);
		return kkTime_Ex;
	}
	inline const KK_TimeEx& operator -= (long tenthousandths)
	{
		long newtime = (m_Time - (tenthousandths % 864000000)) % 864000000 ;
		m_Time = (newtime + 864000000) % 864000000;
		return *this;
	}
    
	inline KK_TimeEx operator + (long tenthousandths)//
	{
		KK_TimeEx kkTime_Ex(((m_Time + (tenthousandths % 864000000)) % 864000000 + 864000000) % 864000000);
		return kkTime_Ex;
	}
	inline const KK_TimeEx& operator += (long tenthousandths)
	{
		long newtime = (m_Time + (tenthousandths % 864000000)) % 864000000;
		m_Time = (newtime + 864000000) % 864000000;
		return *this;
	}
    
	const KK_TimeEx& operator=(const KK_DatetimeEx& datatime_ex);
	inline const KK_TimeEx& operator=(const KK_TimeEx& time_ex)
	{
		m_Time = time_ex.GetTime();
		return *this;
	}
    
	inline bool operator>(const KK_TimeEx & tt) const { return m_Time > tt.GetTime(); }
	inline bool operator<(const KK_TimeEx& tt) const { return m_Time < tt.GetTime(); }
	inline bool operator<=(const KK_TimeEx & tt) const { return m_Time <= tt.GetTime(); }
	inline bool operator>=(const KK_TimeEx & tt) const { return m_Time >= tt.GetTime(); }
	inline bool operator==(const KK_TimeEx& tt) const { return m_Time == tt.GetTime(); }
    
protected:
	long m_Time;	// The time, in ten-thousandths of seconds since midnight	
};

/*----------KK_DateEx----------*/

class KK_DateEx
{
public:
	KK_DateEx()	{ Clear(); }
	KK_DateEx(long date)	{ Clear(); SetDate(date); }
	KK_DateEx(long year, long month, long day);
	KK_DateEx(const KK_DateEx& kkdate_ex);
	KK_DateEx(const char * str);
	KK_DateEx(const wchar_t * wstr);
    
	virtual ~KK_DateEx() { }
    
public:
	inline void Clear() { m_Date = 0; }
	void Today() { From_time_t(time(NULL)); }
	void From_time_t(time_t t);
	inline bool IsClear() { return (m_Date == 0); }
    
public:
	long Year() const;
	long Month() const;
	long Day() const;
	int Week() const;//返回星期数，周一为1，周日为0 //加inline会报错，怎么解决。
	static int Week(long year, long month, long day);
	int GetDayofYear()const; //返回一年的第几天
	static int GetDayofYear(long year, long month, long day);
	int GetWeekofYear()const; //返回一年的第几周，第一个星期由第一个周日开始。
	static int GetWeekofYear(long year, long month, long day);
	
	int GetMaxDayofMonth(); //返回当月的最大天数
	inline static int GetMaxDayofMonth(const int year,const int month)
	{
		if (!IsValidDate(year, month, 1)) return -1;
		if(2==month) return IsLeapYear(year) ? 29 : 28;				
		if(1==month||3==month||5==month||7==month||8==month||10==month||12==month) return 31;
		return 30; 
	}
	void StartOfMonth();
	void EndOfMonth();
    
	inline bool SetDate(const KK_DateEx & kkdate_ex) { m_Date = kkdate_ex.m_Date; return true;}
	bool SetDate(long date);
	bool SetDate(long year, long month, long day);
    
	void GetDate(long& year, long& month, long& day) const;
	inline long GetDate() const { return m_Date; }
    
	bool SetDate(const char * str);
	bool SetDate(const wchar_t * str);
    
	KK_String   GetKKString() const;
	KK_StringA  GetKKStringA() const;
	KK_StringW  GetKKStringW() const;
	KK_StringU  GetKKStringU() const;
	KK_StringWA GetKKStringWA() const;
    
	void GetString(KK_StringA& str) const;
	void GetString(KK_StringW& wstr) const;
	void GetString(KK_StringU& ustr) const;
    
public:
	static bool IsValidDate(long date);
	static bool IsValidDate(long year, long month, long day);
	inline static bool IsLeapYear(long year) { return year%4 == 0 && year%100 != 0 || year%400 == 0; }
    
public:
	bool AddDays(long days = 1);
	bool AddMonths(long months = 1);
	bool AddYears(long years = 1);
	
	inline long operator - (const KK_DateEx& date) { return m_Date - date.GetDate(); }
	KK_DateEx operator - (const long days);
	KK_DateEx operator + (const long days);
    
	const KK_DateEx& operator += (const long days);
	const KK_DateEx& operator -= (const long days);
    
	const KK_DateEx& operator = (const KK_DatetimeEx& kkdatetime_ex);
	inline const KK_DateEx& operator = (const KK_DateEx& kkdate_ex)
	{
		m_Date = kkdate_ex.GetDate();
		return *this;
	}
    
    inline bool operator<(const KK_DateEx& rv) const { return m_Date < rv.GetDate(); }
	inline bool operator>(const KK_DateEx& rv) const { return m_Date > rv.GetDate(); }
    inline bool operator<=(const KK_DateEx& rv) const { return m_Date <= rv.GetDate(); }
	inline bool operator>=(const KK_DateEx& rv) const { return m_Date >= rv.GetDate(); }
	inline bool operator==(const KK_DateEx& rv) const { return m_Date == rv.GetDate(); }
    
protected:
	long m_Date;	// The date : 0 == 1 Jan 0	
};

/*--------KK_DatetimeEx--------*/

class KK_DatetimeEx : public KK_DateEx, public KK_TimeEx
{
public:
	KK_DatetimeEx()
    { Clear(); }
    
	KK_DatetimeEx(long date, long time_ex)
    : KK_DateEx(date), KK_TimeEx(time_ex) {}
    
	KK_DatetimeEx(long year, long month, long day)
    { SetDate(year, month, day); KK_TimeEx::Clear(); }
    
	KK_DatetimeEx(long year, long month, long day, long hour, long minute, long second, long tenthousandths = 0)
    { SetDate(year, month, day); SetTime(hour, minute, second, tenthousandths); }
    
	KK_DatetimeEx(const KK_DatetimeEx& kkdatetime_ex)
    : KK_DateEx(kkdatetime_ex.m_Date), KK_TimeEx(kkdatetime_ex.m_Time) {}	// Copy Constructor
    
	KK_DatetimeEx(const KK_DateEx& kkdate_ex)
    { m_Date = kkdate_ex.GetDate(); m_Time = 0; }
    
	KK_DatetimeEx(const KK_TimeEx& kktime_ex)
    { m_Date = 0; m_Time = kktime_ex.GetTime(); }
	
	KK_DatetimeEx(const char * str);
	KK_DatetimeEx(const wchar_t * wstr);
    
	~KK_DatetimeEx() { }
    
public:
	inline __int64 operator - (const KK_DatetimeEx& datetime)
	{	
		__int64 minuend = (__int64)864000000 * GetDate() + GetTime();//被减数
		__int64 subtrahend = (__int64)864000000 * datetime.GetDate() + datetime.GetTime();//减数
		return minuend - subtrahend;		
	}
	
	inline KK_DatetimeEx operator - (__int64 datetime64)
	{
		__int64 minuend = (__int64)864000000 * GetDate() + GetTime();
		minuend = minuend - datetime64;
		KK_DatetimeEx kkdatetime_ex(minuend / 864000000, minuend % 864000000);
		return kkdatetime_ex;
	}
    
	inline KK_DatetimeEx operator + (__int64 datetime64)
	{
		__int64 addend = (__int64)864000000 * GetDate() + GetTime();
		addend = addend + datetime64;
		KK_DatetimeEx kkdatetime_ex(addend / 864000000, addend % 864000000);
		return kkdatetime_ex;
	}
    
	inline const KK_DatetimeEx& operator += (__int64 datetime64)
	{
		__int64 addend = (__int64)864000000 * GetDate() + GetTime();
		addend = addend + datetime64;
		m_Date = addend / 864000000;
		m_Time = addend % 864000000;
		return *this;
	}
    
	inline const KK_DatetimeEx& operator -= (__int64 datetime64)
	{
		__int64 minuend = (__int64)864000000 * GetDate() + GetTime();
		minuend = minuend - datetime64;
		m_Date = minuend / 864000000;
		m_Time = minuend % 864000000;
		return *this;
	}
    
	inline const KK_DatetimeEx& operator = (const KK_DatetimeEx& kkdatetime_ex)	// Timestamp Assignment operator
	{
		m_Date = kkdatetime_ex.m_Date;
		m_Time = kkdatetime_ex.m_Time;
		return *this;
	}
	inline const KK_DatetimeEx& operator = (const KK_DateEx& kkdate_ex)		// Date Assignment operator
	{
		m_Date = kkdate_ex.GetDate();
		return *this;
	}
	inline const KK_DatetimeEx& operator = (const KK_TimeEx& kktime_ex)		// Time Assignment operator
	{
		m_Time = kktime_ex.GetTime(); 
		return *this; 
	}
	
	inline bool operator<(const KK_DatetimeEx& rv) const
    { return (m_Date < rv.GetDate()) ||
        (m_Date == rv.GetDate() && m_Time < rv.GetTime()); }
	inline bool operator>(const KK_DatetimeEx& rv) const
    { return (m_Date > rv.GetDate()) ||
        (m_Date == rv.GetDate() && m_Time > rv.GetTime()); }
	inline bool operator<=(const KK_DatetimeEx& rv) const
    { return (m_Date < rv.GetDate()) ||
        (m_Date == rv.GetDate() && m_Time <= rv.GetTime()); }
	inline bool operator>=(const KK_DatetimeEx& rv) const
    { return (m_Date > rv.GetDate()) ||
        (m_Date == rv.GetDate() && m_Time >= rv.GetTime()); }
	inline bool operator==(const KK_DatetimeEx& rv) const
    { return (m_Date == rv.GetDate()) && (m_Time == rv.GetTime()); }
    
public:
	inline static void GetDHMS(const __int64 datetime64, long& day, long& hour, long& minute, long& second, long& tenthousandths)
	{
		day = datetime64 / 864000000;
		hour = datetime64 % 864000000/36000000;
		minute = datetime64 % 864000000 % 36000000 / 600000;
		second = datetime64 % 864000000 % 36000000 % 600000 / 10000;
		tenthousandths = datetime64 % 864000000 % 36000000 % 600000 % 10000;
	}
    
	inline static long GetDays64(const __int64 datetime64)	{ return datetime64 / 864000000; }
	inline static long GetHours64(const __int64 datetime64) { return datetime64 % 864000000/36000000; }
	inline static long GetMinute64(const __int64 datetime64) { return datetime64 % 864000000 % 36000000 / 600000; }
	inline static long GetSecond64(const __int64 datetime64) { return datetime64 % 864000000 % 36000000 % 600000 / 10000; }
	inline static long GetTenthousandths64(const __int64 datetime64) { return datetime64 % 864000000 % 36000000 % 600000 % 10000; }
	
public:
	inline void Clear() { KK_DateEx::Clear(); KK_TimeEx::Clear(); }
	inline void ClearDate() { KK_DateEx::Clear(); }
	inline void ClearTime() { KK_TimeEx::Clear(); }
	void Today() { KK_DateEx::Today(); KK_TimeEx::Clear(); }
	void Now() { From_time_t(time(NULL)); }
	inline bool IsClear() { return (KK_DateEx::IsClear() && KK_TimeEx::IsClear()); }
    
public:
	//覆盖KK_TimeEx的方法
	bool AddHours(long hours) { return AddDays(KK_TimeEx::AddHours(hours)); }
	bool AddMinutes(long minutes) { return AddDays(KK_TimeEx::AddMinutes(minutes)); }
	bool AddSeconds(long seconds) { return AddDays(KK_TimeEx::AddSeconds(seconds)); }
	bool AddTenthousandths(long tenthousandths) { return AddDays(KK_TimeEx::AddTenthousandths(tenthousandths)); }
    
    
public:
	bool To_time_t(time_t &t);
	void From_time_t(time_t t);
    
    
	inline void SetDatetime(const KK_DatetimeEx & dt)
	{
		SetDate(dt);
		SetTime(dt);
	}
	bool SetDatetime(long date, long time_ex);
	bool SetDatetime(long year, long month, long day, long hour, long minute, long second, long tenthousandths = 0);
	bool SetDatetime(const char * str);
	bool SetDatetime(const wchar_t * wstr);
    
	KK_String   GetKKString() const;
	KK_StringA  GetKKStringA() const;
	KK_StringW  GetKKStringW() const;
	KK_StringU  GetKKStringU() const;
	KK_StringWA GetKKStringWA() const;
    
	void GetString(KK_StringA& str) const;
	void GetString(KK_StringW& wstr) const;
	void GetString(KK_StringU& ustr) const;
    
public:
    
	static bool IsValidDatetime(long year, long month, long day, long hour = 0, long minute = 0, long second = 0, long tenthousandths = 0)
    { return IsValidDate(year, month, day) && IsValidTime(hour, minute, second, tenthousandths); }
	static bool IsValidDatetime(long date_ex, long time_ex)
    { return IsValidDate(date_ex) && IsValidTime(time_ex); }
	
};

#endif //__KK_DATE_TIME_EX_H__

