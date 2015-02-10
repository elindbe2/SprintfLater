#include <iostream>
#include "StringUtilities.hpp"

template < typename T >
inline void AddToBuffer( char* buf, unsigned long & size, T const& val )
{
    memcpy(buf+size,&val,sizeof(val));
    size += sizeof(val);
}

template <>
inline void AddToBuffer( char* buf, unsigned long & size, char * const& val)
{
    strcpy(buf+size, val);
    size += strlen(val)+1; // include null terminator
}

template < typename T, typename... REST >
inline void AddToBuffer( char* buf, unsigned long & size, T const& val, REST... rest)
{
    AddToBuffer(buf, size, val);
    AddToBuffer(buf,size,rest...);
}

template <typename T>
inline T GetFromBuffer( char const * const data, unsigned long & buf_idx )
{
    auto ret = *reinterpret_cast<T const *>(data+buf_idx);
    buf_idx += sizeof(T);
    return ret;
}

std::string SPrintFromBuffer( char const * const fmt, char const * const data )
{
    std::string str(fmt);
    auto data_idx = 0ul;

    auto found = str.find("%");
    while( found != std::string::npos )
    {
       switch(str[found+1])
       {
            case '%':
            {
                continue;
                break;
            }

#define CREATE_TOKEN_CONVERTER( CHAR, TYPE )\
           case CHAR:\
           {\
               auto val = GetFromBuffer<TYPE>( data, data_idx);\
               str.replace(found,2,toString(val));\
               break;\
           }

           CREATE_TOKEN_CONVERTER( 'l', long )
           CREATE_TOKEN_CONVERTER( 'd', double )
           CREATE_TOKEN_CONVERTER( 'h', short )
       }
       found = str.find("%", found+1);
    }
    return str;
}

int main()
{
    long a = 222;
    double b = 1.0/3;
    short c = 255;
    char buf[256];
    memset(buf, 'e', 256);
    unsigned long size = 0;

    AddToBuffer(buf,size,a,b,c);

    std::cout << SPrintFromBuffer( "a = %l, b = %d, c = %h", buf ) << std::endl;

    return 0;
}
