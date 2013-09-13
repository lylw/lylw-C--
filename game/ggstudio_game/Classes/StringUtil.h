#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#ifdef _MSC_VER
#define cdf_snprintf _snprintf
#else
#define cdf_snprintf snprintf
#endif

#include "Common.h"
using namespace std;

class StringUtil
{
public:
    static int16  str2int16(string str);

    static int32  str2int32(string str);

    static int64  str2int64(string str);

    static string tostr(int16 v);

    static string tostr(int32 v);

    static string tostr(int64 v);

    static string tostr(uint16 v);

    static string tostr(uint32 v);

    static string tostr(uint64 v);

    static string tostr(float v);

    static string tostr(double v);

    static string str2hex(const char* str , int len);

    static string filterString(const char* str ,const char* filter);

    static void   trimString(string & str, string val);

    static bool   splitString(const string& str, const string& delim, vector<string>& result);

    static string joinString(const std::vector<std::string>& values, const std::string& delimiter);

    static void   replaceString(string& str, string srcStr, string destStr);

    static string toUpperString(string s);

    static string toLowerString(string s);

    static string removeWhitespace(const std::string& s);

    static string encodeXmlText(const char * pstr);

    static string encodeUrl(string s);

    static bool   isAlpha(char c);

    static bool   isDigit(char c);

    static void parseIPAddr(string ipAddr, string & ip, uint32 & port);

};

#endif

