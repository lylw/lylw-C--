#include "StringUtil.h"
#include <sstream>
#include <stdlib.h>

int16 StringUtil::str2int16(string str)
{
	return atoi(str.c_str());
}

int32 StringUtil::str2int32(string str)
{
	return atoi(str.c_str());
}

int64 StringUtil::str2int64(string str )
{
	return atol(str.c_str());
}

string StringUtil::tostr(int16 v)
{
	std::stringstream os;
	os << v;
	return os.str();
}

string StringUtil::tostr(int32 v)
{
	std::stringstream os;
	os << v;
	return os.str();
}

string StringUtil::tostr(int64 v)
{
	std::stringstream os;
	os << v;
	return os.str();
}

string StringUtil::tostr(uint16 v)
{
	std::stringstream os;
	os << v;
	return os.str();
}

string StringUtil::tostr(uint32 v)
{
	std::stringstream os;
	os << v;
	return os.str();
}

string StringUtil::tostr(uint64 v)
{
	std::stringstream os;
	os << v;
	return os.str();
}

string StringUtil::tostr(float v)
{
	std::stringstream os;
	os << v;
	return os.str();
}

string StringUtil::tostr(double v)
{
	std::stringstream os;
	os << v;
	return os.str();
}

std::string StringUtil::str2hex(const char* str , int len)
{
	static char hex[] = "0123456789ABCDEF";

	std::string strRet;
	for( int i = 0 ; i < len ; i ++ )
	{
		strRet += hex[ str[i] >> 4 ];
		strRet += hex[ str[i] & 0x0F ];
	}
	return strRet;
}

string StringUtil::filterString(const char* str, const char * const filter)
{
	std::string ret;
	bool isFilter = false ;
	for(const char *pchar = str ; *pchar != '\0' ; pchar++)
	{
		for( const char * buf = filter ; *buf != '\0' ; buf++)
		{
			if( *pchar == *buf )
			{
				isFilter = true;
			}
		}
		if( !isFilter )
			ret.append( pchar , 1 );
	}
	return ret;
}

void StringUtil::trimString(string & str, string val)
{
	string::size_type pos = str.find_first_not_of(val);
	if (pos != string::npos)
	{
		str.erase(0, pos);
	}
	
	pos = str.find_last_not_of(val) + val.size();
	if (pos < str.size())
	{
		str.erase(pos);
	}
}

bool StringUtil::splitString(const string& str, const string& delim, vector<string>& result)
{
	string::size_type pos = 0;
    string::size_type length = str.length();
    string elt;

    char quoteChar = '\0';
    while(pos < length)
    {
        if(quoteChar == '\0' && (str[pos] == '"' || str[pos] == '\''))
        {
            quoteChar = str[pos++];
            continue; // Skip the quote
        }
        else if(quoteChar == '\0' && str[pos] == '\\' && pos + 1 < length && 
                (str[pos + 1] == '\'' || str[pos + 1] == '"'))
        {
            ++pos;
        }
        else if(quoteChar != '\0' && str[pos] == '\\' && pos + 1 < length && str[pos + 1] == quoteChar)
        {
            ++pos;
        }
        else if(quoteChar != '\0' && str[pos] == quoteChar)
        {
            ++pos;
            quoteChar = '\0';
            continue; // Skip the end quote
        }
        else if(delim.find(str[pos]) != string::npos)
        {
            if(quoteChar == '\0')
            {
                ++pos;
                //if(elt.length() > 0)
                //{
                    result.push_back(elt);
                    elt = "";
                //}
                continue;
            }
        }
            
        if(pos < length)
        {
            elt += str[pos++];
        }
    }

    if(elt.length() > 0)
    {
        result.push_back(elt);
    }
    if(quoteChar != '\0')
    {
        return false; // Unmatched quote.
    }
    return true;
}

string StringUtil::joinString(const std::vector<std::string>& values, const std::string& delimiter)
{
	ostringstream out;
    for(unsigned int i = 0; i < values.size(); i++)
    {
        if(i != 0)
        {
            out << delimiter;
        }
        out << values[i];
    }
    return out.str();
}

void StringUtil::replaceString(string& str, string srcStr, string destStr)
{
	string::size_type index = str.find(srcStr);

	while(index != std::string::npos)
	{
		str.replace(index, srcStr.length(), destStr);
		index = str.find(srcStr, index + srcStr.length());
	}
}

string StringUtil::removeWhitespace(const std::string& s)
{
    string result;
    for(unsigned int i = 0; i < s.length(); ++ i)
    {
         if(!isspace(static_cast<unsigned char>(s[i])))
         {
             result += s[i];
         }
    }
    return result;
}

string StringUtil::toUpperString(string s)
{
	string result;
    result.reserve(s.size());
    for(unsigned int i = 0; i < s.length(); ++i)
    {
        if(isascii(s[i]))
        {
            result += tolower(static_cast<unsigned char>(s[i]));
        }
        else
        {
            result += s[i];
        }
    }
    return result;
}

string StringUtil::toLowerString(string s)
{
	string result;
	result.reserve(s.size());
	for(unsigned int i = 0; i < s.length(); ++i)
	{
		if(isascii(s[i]))
		{
			result += toupper(static_cast<unsigned char>(s[i]));
		}
		else
		{
			result += s[i];
		}
	}
	return result;
}

bool StringUtil::isAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool StringUtil::isDigit(char c)
{
	return c >= '0' && c <= '9';
}

string StringUtil::encodeXmlText(const char * pstr)
{
	int nlen = 0;
	const char *plast = pstr;
	string result;
	for( ; *pstr != '\0' ; pstr++ )
	{
		switch( *pstr )
		{
		case '<':
			if( 0 != nlen )
			{
				result.append( plast , nlen );
				nlen = 0;
			}
			result.append( "&lt;"  , sizeof("&lt;") - 1);
			break;
		case '>':
			if( 0 != nlen )
			{
				result.append( plast , nlen );
				nlen = 0;
			}
			result.append( "&gt;"  , sizeof("&gt;") - 1);
			break;
		case '&':
			if( 0 != nlen )
			{
				result.append( plast , nlen );
				nlen = 0;
			}
			result.append( "&amp;"  , sizeof("&amp;") - 1);
			break;
		case '\'':
			if( 0 != nlen )
			{
				result.append( plast , nlen );
				nlen = 0;
			}
			result.append( "&apos;"  , sizeof("&apos;") - 1);
			break;
		case '\"':
			if( 0 != nlen )
			{
				result.append( plast , nlen );
				nlen = 0;
			}
			result.append( "&quot;"  , sizeof("&quot;") - 1);
			break;
		default:
			if( 0 == nlen )
			{
				result = pstr;
			}
			nlen ++ ;
			break;
		}
	}
	if( 0 != nlen )
	{
		result.append( plast , nlen );
		nlen = 0;
	}

	return result;
}

string StringUtil::encodeUrl(string s)
{
	static  char hex[] = "0123456789ABCDEF";
	std::string dst;

	for (unsigned i = 0; i < s.length(); i++)
	{
		unsigned char ch = s[i];
		if (isalnum(ch))
		{
			dst += ch;
		}
		else
		{
			if (ch == ' ')
			{
				dst += '+';
			}
			else
			{
				unsigned char c = static_cast<unsigned char>(ch);
				dst += '%';
				dst += hex[c / 16];
				dst += hex[c % 16];
			}
		}
	}
	return dst;
}

void StringUtil::parseIPAddr(string ipAddr, string & ip, uint32 & port)
{
	std::vector<string> strList;
	StringUtil::splitString(ipAddr, ":", strList);
	ip   = strList.size() > 0 ? strList[0] : "0.0.0.0";
	port = strList.size() > 1 ? StringUtil::str2int32(strList[1]) : 9999;
}