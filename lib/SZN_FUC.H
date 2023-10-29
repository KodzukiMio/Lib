// 2021-2022
// 适用于 MSVC C++ 17+
#pragma once
#ifndef SZN_FUNC_H_
#define SZN_FUNC_H_
#ifndef _ITERATOR_DEBUG_LEVEL
// #define _ITERATOR_DEBUG_LEVEL=0
#endif // !_ITERATOR_DEBUG_LEVEL
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define Windows_ENABLE_base

#define k2022 LB2022::KUR
#define k2023 LB2023

#ifdef ENABLE_SZN_OPE
#include <gl/glut.h>
#pragma comment(lib, "msvcrt.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#include <glad.h>
#include <glfw3.h>
#endif // ENABLE_SZN_OPE
#ifdef __KURBOOST
// 请添加boost库!!!
#define _WIN32_WINNT 0x0601
#include <boost/algorithm/string.hpp>
#include <boost/json.hpp>
#endif // __KURBOOST
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atlstr.h>
#include <string>
#include <cmath>
#include <new>
#include <locale>
#include <memory>
#include <algorithm>
#include <vector>
#include <cstring>
#include <iterator>
#include <thread>
#include <codecvt>
#include <io.h>
#include <ctime>
#include <Windows.h>
#include <tchar.h>
#include <functional>
#include <strsafe.h>
#include <utility>
#include <memory>
#include <list>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
// #include<tuple>
#include <cwchar>
#include <type_traits>
#ifdef ENABLE_ZIP
#define Windows_ENABLE_base
// Please add zip.cpp, unzip.cpp to the project directory
// 请把zip.cpp和unzip.cpp添加到工程目录!!!
// https://www.codeproject.com/Articles/7530/Zip-Utils-Clean-Elegant-Simple-Cplusplus-Win
#include <Windows.h>
#include <TlHelp32.h>
#include <commctrl.h>
#include <tchar.h>
#include <stdio.h>
#include <zip/zip.h>
#include <zip/unzip.h>
#endif
#ifdef ENABLE_VOLUME
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <audioclient.h>
#endif // ENABLE_VOLUME

#ifdef ENABLE_COLOR
#define Windows_ENABLE_base
#endif
#ifdef Windows_SOCK_ENABLE_base
#define Windows_ENABLE_base
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <TlHelp32.h>
#define __KW_cw_malloc(GetLengthOfTarget, nptrType) (nptrType *)malloc(MultiByteToWideChar(0, 0, GetLengthOfTarget, -1, NULL, 0) * sizeof(nptrType))
#define __KW_cw(__char, __wchar) MultiByteToWideChar(0, 0, __char, -1, __wchar, MultiByteToWideChar(0, 0, __char, -1, NULL, 0))
#ifdef Windows_SOCK_ENABLE_base
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
// 请链接ws2_32.lib
#endif // Windows_SOCK_ENABLE_base

#endif // Windows_SOCK_ENABLE_base

#include <Bitset>
#include <assert.h>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")
#ifdef ENABLE_KEY
#include <conio.h>
#endif
#pragma warning(disable : 28183)
#pragma warning(disable : 26451)
#pragma warning(disable : 6387)
#pragma warning(disable : 4244)
#pragma warning(disable : 6011) // NULL指针引用
#define sptr_(name, type, _Pfn) shared_ptr<type> name(_Pfn)
#define sptrn(name, type, value) sptr_(name, type, new type(value))
#define _ptrtype(_Pfn) (typeid(*_Pfn).name())
#define newptr(name, type, value) type *name = new type(value)
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1 : 0)
#define CLICK(time)                                \
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); \
    Sleep(time);                                   \
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0)
#define MOUSESET(x, y) MouseMove(x, y)
#define MOUSEOFFSET(_x, _y)                                                     \
    {                                                                           \
        POINT __MOUSE_POINT_POSITION;                                           \
        ::GetCursorPos(&__MOUSE_POINT_POSITION);                                \
        MOUSESET(__MOUSE_POINT_POSITION.x + _x, __MOUSE_POINT_POSITION.y + _y); \
    }
#define __DEBUGEXIT          \
    if (IsDebuggerPresent()) \
    {                        \
        return 0;            \
    }
#include "base.hpp"
typedef unsigned short int KCDSTR;
namespace KUR{
    namespace LB2022{
        template <class Tchar>
        class CharT{
        public:
            using char_t = std::basic_string<Tchar,std::char_traits<Tchar>,std::allocator<Tchar>>;
            using out_t = std::basic_ostream<Tchar,std::char_traits<Tchar>>;
        };
    #define _KUR_TYPE_CHAR_(type) typename CharT<type>::char_t
    #define _KUR_TYPE_CHAR_USE(type) using TYPE_CHAR = typename CharT<type>::char_t
        const double pi = 3.14159265358979;
        const double e = 2.718281828459045;
        const double eps = 1e-10;
        void MouseMove(int x,int y){
            double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
            double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
            double fx = x * (65535.0f / fScreenWidth);
            double fy = y * (65535.0f / fScreenHeight);
            mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,fx,fy,0,0);
        };
        tm* TIME(){
            time_t now = time(0);
            tm* _tm = new tm;
            errno_t gmtm = gmtime_s(_tm,&now);
            return _tm;
        };
        wchar_t* ToWchar_N(const char* c){
            size_t cSize = strlen(c) + 1;
            wchar_t* wc = new wchar_t[cSize];
            mbstowcs(wc,c,cSize);
            return wc;
        }; // unsafe
        ULONGLONG GetNowMilliSecond(){ return GetTickCount64(); };
        bool Divisible(unsigned long long _Val,unsigned long long _Num){
            double a = (double)_Val / (double)_Num;
            if (a - (double)(unsigned long long)a < 1e-6){
                return true;
            };
            return false;
        };
        bool IsPrimeNumber(unsigned long long num){
            if (num == 1 || num == 4){
                return 0;
            };
            if (num == 2 || num == 3){
                return 1;
            };
            if (num % 6 != 1 && num % 6 != 5){
                return 0;
            };
            unsigned long long tmp = (unsigned long long)sqrt(num);
            for (unsigned long long i = 5; i <= tmp; i += 6){
                if (num % i == 0 || num % (i + 2) == 0){
                    return 0;
                };
            };
            return 1;
        };
        template <typename Tchar>
        typename CharT<Tchar>::char_t* FastRead_N(const Tchar* path){
            std::basic_ifstream<Tchar,std::char_traits<Tchar>> fin(path,std::ios::binary);
            if (fin.good()){
                using Type_char = typename CharT<Tchar>::char_t;
                std::vector<Tchar> buf(fin.seekg(0,std::ios::end).tellg());
                fin.seekg(0,std::ios::beg).read(&buf[0],static_cast<std::streamsize>(buf.size()));
                fin.close();
                Type_char* ret = new Type_char();
                ret->assign(buf.begin(),buf.end());
                return ret;
            } else{
                return nullptr;
            };
        };
        template <class Ry_ = void,class Ty_>
        void for_each(Ty_* First_,Ty_* Last_,Ry_(*_Pfn)(Ty_)){
            while (First_ != Last_){
                (*_Pfn)(*First_++);
            };
        };
        std::string Subreplace(std::string resource_str,std::string sub_str,std::string new_str){
            std::string dst_str = resource_str;
            std::string::size_type pos = 0;
            while ((pos = dst_str.find(sub_str)) != std::string::npos){
                dst_str.replace(pos,sub_str.length(),new_str);
            };
            return dst_str;
        };
        template <typename Tchar = char>
        std::vector<_KUR_TYPE_CHAR_(Tchar)> split(const _KUR_TYPE_CHAR_(Tchar)& s,const _KUR_TYPE_CHAR_(Tchar)& seperator){
            std::vector<_KUR_TYPE_CHAR_(Tchar)> result;
            size_t posBegin = 0;
            size_t posSeperator = s.find(seperator);
            while (posSeperator != s.npos){
                result.push_back(s.substr(posBegin,posSeperator - posBegin));
                posBegin = posSeperator + seperator.size();
                posSeperator = s.find(seperator,posBegin);
            };
            if (posBegin != s.length())
                result.push_back(s.substr(posBegin));
            return result;
        };
        // 可能有bug
        TCHAR* WstrToTchar(std::wstring& ws){ return (TCHAR*)(&ws[0]); };
        // 可能有bug
        std::string FileName(const char* path){
            std::string P = path;
            auto v = split(P,"\\");
            P = v[v.size() - 1];
            return P;
        };
        // 可能有bug
        std::wstring ToWstring_(const std::string& input){
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return converter.from_bytes(input);
        };
        // 可能有bug
        std::string ToString_(const std::wstring& input){
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return converter.to_bytes(input);
        };
        void GetFileNames(std::string path,std::vector<std::string>& files){
            using namespace std;
            intptr_t hFile = 0;
            struct _finddata_t fileinfo;
            std::string p;
            if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) != -1){
                do{
                    if ((fileinfo.attrib & _A_SUBDIR)){
                        if (strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0)
                            GetFileNames(p.assign(path).append("\\").append(fileinfo.name),files);
                    } else{
                        files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                    };
                } while (_findnext(hFile,&fileinfo) == 0);
                _findclose(hFile);
            };
        };
        // 可能有bug
        std::string GetStringByWchar(const WCHAR* wszString){
            std::string strString;
            if (wszString != NULL){
                std::wstring ws(wszString);
                strString.assign(ws.begin(),ws.end());
            };
            return strString;
        };
        // 可能有bug
        std::wstring GetWStringByChar(const char* szString){
            std::wstring wstrString;
            if (szString != NULL){
                std::string str(szString);
                wstrString.assign(str.begin(),str.end());
            };
            return wstrString;
        };
        int Rand_(int beg,int end){
            srand((int)(time(NULL)));
            return ((rand() % (end - beg + 1)) + beg);
        };
        size_t FindAllCount(std::string str,std::string sub){
            size_t count = 0;
            size_t index = 0;
            size_t len = str.length();
            while ((index = str.find(sub,index)) < len){
                count++;
                index++;
            };
            return count;
        };
        void getFiles(std::string path,std::vector<std::string>& files){
            using namespace std;
            intptr_t hFile = 0;
            struct _finddata_t fileinfo;
            string p;
            if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) != -1){
                do{
                    if ((fileinfo.attrib & _A_SUBDIR)){
                        if (strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0)
                            getFiles(p.assign(path).append("\\").append(fileinfo.name),files);
                    } else{
                        files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                    };
                } while (_findnext(hFile,&fileinfo) == 0);
                _findclose(hFile);
            };
        };
        size_t GetFileSize(char* file_){
            FILE* file = fopen(file_,"r");
            if (file){
                size_t size = filelength(fileno(file));
                fclose(file);
                return size;
            } else{
                return 0;
            };
        };
        static short int GetBits(){ return sizeof(int*) * 8; };
        template <typename T>
        void PrintAll(const std::vector<T>& vec,const std::string&& _firdisplay = ""){
            for_each(vec.begin(),vec.end(),[&,_firdisplay](const T& t){ std::cout << _firdisplay << t << std::endl; });
        };
        void PrintAll(const std::vector<std::wstring>& vec,const std::wstring&& _firdisplay = L""){
            for_each(vec.begin(),vec.end(),[&,_firdisplay](const std::wstring& t){ std::wcout << _firdisplay << t << std::endl; });
        };
        std::string getCmdResult(const std::string& strCmd){
            using namespace std;
            char buf[8191] = {0};
            FILE* pf = NULL;
            if ((pf = _popen(strCmd.c_str(),"r")) == NULL){
                return "";
            };
            string strResult;
            while (fgets(buf,sizeof buf,pf)){
                strResult += buf;
            };
            _pclose(pf);
            unsigned int iSize = (unsigned int)strResult.size();
            if (iSize > 0 && strResult[iSize - 1] == '\n'){
                strResult = strResult.substr(0,iSize - 1);
            };
            return strResult;
        };
        template <typename T>
        T ButtonUpDetection(int charid,T(*Pfn_)(),int sleeptime = 1){
            if (KEY_DOWN(charid)){
                while (KEY_DOWN(charid))
                    ;
                Sleep(sleeptime);
                return (*Pfn_)();
            };
            return (T)0;
        }; // Only one can exist at a time
        template <typename Tchar>
        size_t _stroprlen(const Tchar* str,void (*Pfn)(Tchar t) = nullptr){
            const Tchar* end = str;
            if (Pfn == nullptr){
                while (*end++)
                    ;
            } else{
                while (*end){
                    Pfn(*end++);
                };
            };
            return end - str - 1;
        };
        std::string CODE(char* path){
            std::ifstream fin(path,std::ios::binary);
            unsigned char s2;
            fin.read((char*)&s2,sizeof(s2));
            int p = s2 << 8;
            fin.read((char*)&s2,sizeof(s2));
            p += s2;
            std::string code;
            switch (p){
                case 0xfffe:
                    code = "Unicode";
                    break;
                case 0xfeff:
                    code = "Unicode big endian";
                    break;
                case 0xefbb:
                    code = "UTF-8";
                    break;
                default:
                    code = "ANSI";
            };
            fin.close();
            return code;
        };
        template <class T>
        class Stack;
        class Kstring;

    #ifdef ENABLE_MD5
        class MD5{
        private:
            unsigned int tempA,tempB,tempC,tempD,strlength;

        public:
            constexpr static unsigned int ssssss[64] = {7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
            constexpr static unsigned int TTTTTT[64] = {0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};
            MD5(){
                tempA = 0x67452301;
                tempB = 0xefcdab89;
                tempC = 0x98badcfe;
                tempD = 0x10325476;
                strlength = 0;
            };
            std::string operator()(std::string& src){ return encode(src); };

        private:
            unsigned int F(unsigned int b,unsigned int c,unsigned int d){ return (b & c) | ((~b) & d); };
            unsigned int G(unsigned int b,unsigned int c,unsigned int d){ return (b & d) | (c & (~d)); };
            unsigned int H(unsigned int b,unsigned int c,unsigned int d){ return b ^ c ^ d; };
            unsigned int I(unsigned int b,unsigned int c,unsigned int d){ return c ^ (b | (~d)); };
            unsigned int shift(unsigned int a,unsigned int n){ return (a << n) | (a >> (32 - n)); };
            std::string encode(std::string src){
                using namespace std;
                vector<unsigned int> rec = padding(src);
                for (unsigned int i = 0; i < strlength / 16; i++){
                    unsigned int num[16];
                    for (int j = 0; j < 16; j++){
                        num[j] = rec[i * 16 + j];
                    };
                    iterateFunc(num,16);
                };
                return format(tempA) + format(tempB) + format(tempC) + format(tempD);
            };
            void iterateFunc(unsigned int* X,int size = 16){
                using namespace std;
                unsigned int a = tempA,b = tempB,c = tempC,d = tempD,rec = 0,g,k;
                for (int i = 0; i < 64; i++){
                    if (i < 16){
                        g = F(b,c,d);
                        k = i;
                    } else if (i < 32){
                        g = G(b,c,d);
                        k = (1 + 5 * i) % 16;
                    } else if (i < 48){
                        g = H(b,c,d);
                        k = (5 + 3 * i) % 16;
                    } else{
                        g = I(b,c,d);
                        k = (7 * i) % 16;
                    };
                    rec = d;
                    d = c;
                    c = b;
                    b = b + shift(a + g + X[k] + TTTTTT[i],ssssss[i]);
                    a = rec;
                };
                tempA += a;
                tempB += b;
                tempC += c;
                tempD += d;
            };
            std::vector<unsigned int> padding(std::string src){
                using namespace std;
                unsigned int num = (unsigned int)(((src.length() + 8) / 64) + 1);
                vector<unsigned int> rec(num * 16);
                strlength = num * 16;
                for (unsigned int i = 0; i < src.length(); i++){
                    rec[i >> 2] |= (int)(src[i]) << ((i % 4) * 8);
                };
                rec[src.length() >> 2] |= (0x80 << ((src.length() % 4) * 8));
                rec[rec.size() - 2] = (unsigned int)(src.length() << 3);
                return rec;
            };
            std::string format(unsigned int num){
                using namespace std;
                const char str16[] = "0123456789abcdef";
                string res = "";
                unsigned int base = 1 << 8;
                for (int i = 0; i < 4; i++){
                    string tmp = "";
                    unsigned int b = (num >> (i * 8)) % base & 0xff;
                    for (int j = 0; j < 2; j++){
                        tmp = str16[b % 16] + tmp;
                        b /= 16;
                    };
                    res += tmp;
                };
                return res;
            };
        };
        std::string md5(std::string& src){
            MD5 md5_;
            return md5_(src);
        };
    #endif // ENABLE_MD5																																																	 //弃用 Deprecated,又慢又烂的垃圾实现
        class Kstring{
        private:
            size_t len = 0;
            char* data = NULL;

        public:
            template <class T>
            friend class Stack;
            ~Kstring(){
                delete[] data;
                len = 0;
            };
            Kstring(const char* str = NULL){
                if (!str){
                    len = 0;
                    data = new char[1];
                    *data = '\0';
                } else{
                    len = strlen(str);
                    data = new char[len + 1];
                    strcpy(data,str);
                };
            };
            Kstring(const Kstring& str){
                len = str.size();
                data = new char[len + 1];
                strcpy(data,str.str());
            };
            Kstring operator+(const Kstring& str) const{
                Kstring newstr;
                newstr.len = len + str.size();
                newstr.data = new char[newstr.len + 1];
                strcpy(newstr.data,data);
                strcat(newstr.data,str.data);
                return newstr;
            };
            Kstring& operator=(const Kstring& str){
                if (this == &str){
                    return *this;
                };
                delete[] data;
                len = str.len;
                data = new char[len + 1];
                strcpy(data,str.str());
                return *this;
            };
            Kstring& operator=(Kstring&& str) noexcept{
                if (this == &str){
                    return *this;
                };
                delete this->data;
                this->data = str.data;
                str.data = nullptr;
                return *this;
            };
            Kstring& operator+=(const Kstring& str){
                *this = *this + str;
                return *this;
            };
            bool operator==(const Kstring& str) const{
                if (len != str.len){
                    return false;
                } else{
                    return strcmp(data,str.data) ? false : true;
                };
            };
            char& operator[](int n) const{
                if (n >= int(len)){
                    return data[len - 1];
                } else if (n < 0 && (-n) <= int(len)){
                    return data[int(len) + n];
                } else if ((-n) > int(len)){
                    return data[0];
                } else{
                    return data[n];
                };
            };
            size_t size() const{ return len; };
            const char* str() const{ return data; };
            char* address(){ return data; };
            friend Kstring& operator>>(const char* c,Kstring& str){
                Kstring newstr = c;
                str += newstr;
                return str;
            };
            friend Kstring& operator<<(Kstring& str,const char* c){
                c >> str;
                return str;
            };
            friend Kstring& operator<<(char* c,Kstring& str){
                strcpy(c,str.str());
                return str;
            };
            friend Kstring& operator>>(Kstring& str,char* c){
                c << str;
                return str;
            };
            Kstring& operator>>(std::string& str){
                str = this->str();
                return *this;
            };
            Kstring& operator<<(std::string& str){
                *this = str.c_str();
                return *this;
            };
            friend std::ostream& operator<<(std::ostream& os,Kstring& str){
                os << str.data;
                return os;
            };
            friend std::ostream& operator>>(Kstring& str,std::ostream& os){ os << str; };
            static void* operator new(std::size_t t){
                Kstring* p = ::new Kstring();
                return p;
            };
            char* Data(){ return data; };
            size_t to_find(const Kstring& str){
                std::string st = (*this).str();
                return st.find(str.str());
            };
            std::string string_(){
                std::string str = (*this).str();
                return str;
            };
            std::string ToString(){ return this->string_(); };
            static void _ToLower(char* s){
                size_t len = strlen(s);
                for (size_t i = 0; i < len; i++){
                    if (s[i] >= 'A' && s[i] <= 'Z'){
                        s[i] = tolower(s[i]);
                    }
                }
            };
            static void _ToUpper(char* s){
                size_t len = strlen(s);
                for (size_t i = 0; i < len; i++){
                    if (s[i] >= 'a' && s[i] <= 'z'){
                        s[i] = toupper(s[i]);
                    }
                }
            };
            Kstring& ToLower(){
                _ToLower(data);
                return *this;
            };
            Kstring& ToUpper(){
                _ToUpper(data);
                return *this;
            };
            friend Kstring& operator<<(Kstring& ks,void (*p)(Kstring&)){
                (*p)(ks);
                return ks;
            };
            char* begin(){ return data; };
            char* end(){ return data + len; };
            template <class T_>
            friend T_ operator<<(Kstring& s,T_(*_Pfn)(Kstring&)){ return (*_Pfn)(s); };
        };
        // 弃用 Deprecated,偶尔凑合着用用可以,因为内置char类型是kstring,所以垃圾
        template <class T = Kstring>
        class Stack{
        private:
            bool t = true;
            int top;
            int _max;
            T* p_data = NULL;
            T* p_revise = NULL;
            T* pop(){
                T* item;
                if (top > 0){
                    item = (p_data + (--top));
                    t = true;
                    return item;
                } else{
                    t = false;
                    return NULL;
                };
            };

        public:
            friend class Kstring;
            operator int() const{ return int(*p_data); };
            operator double() const{ return double(*p_data); };
            operator char() const{ return char(*p_data); };
            operator bool() const{ return bool(*p_data); };
            operator float() const{ return float(*p_data); };
            friend Stack& operator>>(Stack& s,Stack& s_){
                if (!s.isempty()){
                    s_.push(*s.pop());
                }
                return s_;
            };
            friend Stack& operator<<(Stack& s,Stack& s_){
                s_ >> s;
                return s;
            };
            friend Stack& operator<<(Stack& s,T t){
                s.push(t);
                return s;
            };
            friend Stack& operator>>(T t,Stack& s){
                s << t;
                return s;
            };
            friend Stack& operator>>(Stack& s,T& t){
                s.pop(t);
                return s;
            };
            friend Stack& operator<<(T& t,Stack& s){
                s >> t;
                return s;
            };
            friend std::ostream& operator<<(std::ostream& os,Stack& s){
                T _;
                s.pop(_);
                os << _;
                return os;
            };
            friend std::ostream& operator>>(Stack& s,std::ostream& os){
                os << s;
                return os;
            };
            Stack(const int max = 256){
                top = 0;
                _max = max;
                p_data = new T[_max];
            };
            Stack(const std::initializer_list<T>& v){
                top = 0;
                _max = (int)v.size();
                p_data = new T[_max];
                for (T item : v){
                    *this << item;
                };
            };
            ~Stack(){
                delete[] p_data;
                p_data = NULL;
            };
            bool isempty(){
                t = true;
                return top == 0;
            };
            bool isfull(){
                t = true;
                return top == _max;
            };
            bool push(const T item){
                if (top < _max){
                    *(p_data + (top++)) = item;
                    t = true;
                    return true;
                } else{
                    t = false;
                    return false;
                };
            };
            bool pop(T& item){
                if (top > 0){
                    item = *(p_data + (--top));
                    t = true;
                    return true;
                } else{
                    t = false;
                    return false;
                };
            };
            void Delete_Create(){
                delete[] p_data;
                top = 0;
                p_data = new T[_max];
                t = true;
            };
            void Delete_Create(const int len){
                delete[] p_data;
                top = 0;
                p_data = new T[len];
                t = true;
            };
            bool state(){ return t; };
            T* operator()(int n){
                {
                    if (n < 0 && -n < top){
                        p_revise = (p_data + top + n);
                    } else if (n < top && n > 0){
                        p_revise = (p_data + n);
                    } else if (n > 0 && n > top){
                        p_revise = (p_data + top);
                    } else if (n < 0 && -n > top || n == 0 || n < 0 && -n == top){
                        p_revise = p_data;
                    };
                };
                return p_revise;
            };
            T operator[](int n){ return check(n); };
            T check(const int n){ return *(*this)(n); };
            T* check_(const int n){ return (*this)(n); };
            T* address(){ return p_data; };
            void pop_(){ --top; };
            int length(){ return top; };
            int max_length(){ return _max; };
            void flip(){
                Stack<T> t(_max);
                int top_ = top;
                int i = 0;
                while (i < top){
                    t << *(*this)(i);
                    ++i;
                };
                this->top = 0;
                while (!t.isempty()){
                    t >> *this;
                };
                top = top_;
            };
            void replace(const T& from,const T& to){
                for (int i = 0; i < top; ++i){
                    if ((*this)[i] == from){
                        *(*this)(i) = to;
                    };
                };
            };
            T sum(){
                T sum = 0;
                for (T i = 0; i < top; ++i){
                    sum = sum + *(*this)(i);
                };
                return sum;
            };
            void to_string(Stack<std::string>& str){
                Kstring ks;
                std::string st;
                while (!(*this).isempty() && !str.isfull()){
                    *this >> ks;
                    ks >> st;
                    st >> str;
                };
            };
            void to_kstring(Stack<Kstring>& kstr){
                Kstring ks;
                std::string st;
                while (!(*this).isempty() && !kstr.isfull()){
                    *this >> st;
                    ks << st;
                    ks >> kstr;
                };
            };
            static size_t TotalCharLength(Stack<Kstring>& kstr){
                size_t t = 0;
                for (int i = 0; i < kstr.length(); i++){
                    t += kstr.check(i).size();
                }
                return t;
            };
            static size_t TotalCharLength(Stack<std::string>& str){
                size_t t = 0;
                for (int i = 0; i < str.length(); i++){
                    t += str.check(i).size();
                }
                return t;
            };
            void print(){
                while (!this->isempty()){
                    std::cout << *this << std::endl;
                };
            };
            static std::string AsString(Stack<Kstring>& kstr){
                std::string str;
                Kstring ks;
                kstr.flip();
                while (!kstr.isempty()){
                    ks << kstr;
                    str += ks.str();
                }
                return str;
            };
            static Stack<size_t>* FindLineAsStack(std::string str,std::string sub,int Stacklen = 1024){
                using namespace std;
                size_t len = str.length();
                size_t index = 0;
                size_t count = 0;
                Stack<size_t>* res = new Stack<size_t>(Stacklen);
                while ((index = str.find(sub,index)) < len){
                    count++;
                    index++;
                    *res << index;
                }
                return res;
            };
            static Stack<std::string>* SubLineAsString(Kstring& res,std::string sub_1,std::string sub_2,int Stacklen = 1024){
                using namespace std;
                auto s1 = Stack<int>::FindLineAsStack(res.Data(),sub_1,Stacklen);
                auto s2 = Stack<int>::FindLineAsStack(res.Data(),sub_2,Stacklen);
                auto length = min(s1->length(),s2->length());
                int length_ = (int)sub_1.size();
                Stack<std::string>* S = new Stack<std::string>(length);
                size_t t1 = 0,t2 = 0;
                std::string s = res.string_();
                std::string s_;
                s1->flip();
                s2->flip();
                for (int i = 0; i < length; i++){
                    t1 << *s1;
                    t1 += length_;
                    t2 << *s2;
                    s_ = s.substr(t1 - 1,t2 - t1);
                    *S << s_;
                }
                return S;
            };
            std::string ToString(){
                std::string str = "null";
                return str;
            };
            void DeleteEachPoint(){
                while (!this->isempty()){
                    delete* (p_data + (--top));
                };
            };
            friend Stack& operator<<(Stack& s,void (*_Pfn)(Stack<T>&)){
                (*_Pfn)(s);
                return s;
            };
            template <class T_>
            friend T_ operator<<(Stack& s,T_(*_Pfn)(Stack<T>&)){ return (*_Pfn)(s); };
            T operator[](T(*_Pfn)(Stack<T>*)){ return (*_Pfn)(this); };
            void operator--(){ top--; };
            T* begin(){ return p_data; };
            T* end(){ return p_data + top; };
            T* Final(){ return p_data + _max; };
            void set(T* pdata = 0,T* previse = 0,int top_ = 0){
                if (pdata != 0){
                    p_data = pdata;
                };
                if (previse != 0){
                    previse = p_revise;
                };
                if (top_ != 0){
                    top = top_ >= _max ? _max : top_;
                };
            };
            friend Stack& operator>>(std::istream& ist,Stack& s){
                T t;
                std::cin >> t;
                s.push(t);
                return s;
            };
            void SetFromCin(int start,int end_){
                end_ = (end_ >= _max ? _max : end_);
                for (int i = 0; i < end_; i++){
                    std::cin >> *check_(i);
                };
            };
        };
        // Stack(char*)->Stack<std::string>;
        // 弃用 Deprecated
        template <class T>
        void space(Stack<T>& s){ s.push(" "); };
        // 弃用 Deprecated
        void space(Kstring& ks){ ks << " "; };
        // 弃用 Deprecated
        void endl(Kstring& ks){ ks << "\n"; };
        // 弃用 Deprecated
        template <class T>
        T end(Stack<T>* s){ return (*s)[(*s).length() - 1]; };
        template <typename T>
        class Node{
        public:
            T data;
            Node<T>* next = nullptr;
            Node() = default;
            explicit Node(const T& _data): data(_data){}
        };
        template <typename T>
        class OneLinkList{
        public:
            size_t _size = 1;
            Node<T>* head = nullptr;
            OneLinkList(const T& _data){ this->head = new Node<T>(_data); };
            OneLinkList(){ this->head = new Node<T>(); };
            ~OneLinkList(){
                while (_size > 0){
                    remove(0);
                };
            };
            Node<T>* end(){ return this->get(this->_size - 1); };
            Node<T>* get(size_t pos){
                Node<T>* _pos = this->head;
                pos = min(pos,this->_size);
                for (size_t i = 0; i < pos; i++){
                    _pos = _pos->next;
                };
                return _pos;
            };
            OneLinkList<T>& insert(int pos,const T& _data){
                Node<T>* new_node = new Node<T>(_data);
                Node<T>* this_node = this->get(pos);
                new_node->next = this_node->next;
                this_node->next = new_node;
                this->_size++;
                return *this;
            };
            OneLinkList<T>& insert(OneLinkList<T>& oll,size_t pos){
                size_t _size = oll._size;
                Node<T>* _thisnode = this->get(pos);
                Node<T>* _ollend = oll.get(_size - 1);
                _ollend->next = _thisnode->next;
                _thisnode->next = oll.head;
                this->_size += _size;
                return *this;
            };
            OneLinkList<T>& insert(Node<T>* _begin,Node<T>* _end,size_t pos){
                Node<T>* _pos = _begin;
                size_t count = 1;
                while (_pos != _end){
                    _pos = _pos->next;
                    count++;
                };
                Node<T>* _thisnode = this->get(pos);
                _end->next = _thisnode->next;
                _thisnode->next = _begin;
                this->_size += count;
                return *this;
            };
            OneLinkList<T>& remove(size_t pos,size_t _end = -1){
                std::vector<Node<T>*> vec;
                size_t n = _end;
                if (_end == -1){
                    _end = this->_size;
                } else{
                    _end = min(_end,this->_size);
                };
                if (pos == 0){
                    Node<T>* pos1 = this->head;
                    for (size_t i = 0; i < _end; i++){
                        vec.push_back(pos1);
                        pos1 = pos1->next;
                    };
                    if (n != -1){
                        this->head = pos1->next;
                    } else{
                        this->head = NULL;
                    };
                    for (size_t i = 0; i < vec.size(); i++){
                        delete vec[i];
                    };
                    this->_size -= vec.size();
                    return *this;
                };
                Node<T>* _pos = this->get(pos - 1);
                Node<T>* __pos = _pos;
                for (size_t i = pos; i < _end; i++){
                    _pos = _pos->next;
                    vec.push_back(_pos);
                };
                __pos->next = _pos->next;
                this->_size -= vec.size();
                for (size_t i = 0; i < vec.size(); i++){
                    delete vec[i];
                };
                return *this;
            };
            template <typename Ty>
            OneLinkList<T>& for_each(Ty(*_Pfn)(Node<T>*)){
                Node<T>* _pos = this->head;
                for (size_t i = 0; i < this->_size; i++){
                    (*_Pfn)(_pos);
                    _pos = _pos->next;
                };
                return *this;
            };
        };
        template <typename Tchar>
        class BaseChar{
        public:
            Tchar* tch = nullptr;
            void (*_ReclaimMemory)(BaseChar<Tchar>*) = nullptr;
            BaseChar(Tchar* ch){ this->tch = ch; };
            BaseChar(Tchar* ch,void (*_Rm)(BaseChar<Tchar>*)){
                this->tch = ch;
                this->_ReclaimMemory = _Rm;
            };
            ~BaseChar(){
                if (this->_ReclaimMemory == nullptr){
                    return;
                };
                this->_ReclaimMemory(this);
            };
        };

        class KMiscellaneous{
        public:
            static std::wstring WI(std::string address,std::string&& locale = "chs"){
                std::locale chs(locale);
                std::wifstream ifs(address.c_str());
                ifs.imbue(chs);
                std::wstring line,lines;
                while (std::getline(ifs,line)){
                    lines = lines + line + L"\n";
                };
                return lines;
            };
            // 巨慢实现,不要用
            static std::string ReadData(std::string address){
                std::ifstream ifs(address.c_str());
                std::string line,lines;
                while (std::getline(ifs,line)){
                    lines = lines + line + "\n";
                };
                return lines;
            };
            // 巨慢实现,不要用
            static std::vector<std::string>* ReadData_vector_N(std::string address){
                std::vector<std::string>* vec = new std::vector<std::string>;
                std::ifstream ifs(address.c_str());
                std::string line,lines;
                while (std::getline(ifs,line)){
                    vec->push_back(line);
                };
                return vec;
            };
            static void SaveData(std::string& address,std::string& Source,std::string&& locale = "chs"){
                std::fstream F_(address.c_str(),std::ios::out);
                F_.imbue((std::locale)locale);
                F_ << Source;
                F_.close();
            };
            static void WO(std::string& address,std::wstring& ws,std::string&& locale = "chs"){
                std::wfstream F_(address.c_str(),std::ios::out);
                F_.imbue((std::locale)locale);
                F_ << ws;
                F_.close();
            };
            static int GetProgramBits(){ return sizeof(int*) * 8; };
            static void w_cout(std::wstring& s,std::string&& locale = "chs"){
                std::locale chs(locale);
                std::wcout.imbue(chs);
                std::wcout << s;
            };
            // 巨慢实现,不要用
            static std::string* encryption(const std::string key,const std::wstring& wstr){
                using namespace std;
                const int max = 1024;
                const char* p = key.c_str();
                const int p_len = (int)key.length();
                const unsigned int MAX_ui = 4294967295;
                unsigned int seed = 0,seed_max = 0;
                for (int i = 0; i < p_len && seed_max < MAX_ui; i++){
                    seed = seed_max;
                    seed_max += int(*(p + i));
                };
                srand(seed);
                wstring ws = wstr + L"   ";
                const wchar_t* ws_p = ws.c_str();
                int ws_len = (int)(ws.length() - 2);
                int* ws_seed = new int[ws_len];
                int* ws_int = new int[ws_len];
                int* ws_res = new int[ws_len];
                int* ws_res_len = new int[ws_len];
                string keylen,ws_result;
                for (int i = 0; i < ws_len; i++){
                    ws_int[i] = int(ws_p[i]);
                    ws_seed[i] = rand();
                    ws_res[i] = ws_int[i] + ws_seed[i];
                    ws_res_len[i] = (int)to_string(ws_res[i]).length();
                    keylen += to_string(ws_res_len[i]);
                    ws_result += to_string(ws_res[i]);
                };
                string key_ = "K" + keylen;
                string* resu = new string(ws_result + key_);
                delete[] ws_seed;
                delete[] ws_int;
                delete[] ws_res;
                delete[] ws_res_len;
                return resu;
            };
            // 巨慢实现,不要用
            static std::wstring* decrypt(const std::string key,const std::string& lines){
                using namespace std;
                const int max = 1024;
                const char* p = key.c_str();
                const int p_len = (int)key.length();
                const unsigned int MAX_ui = 4294967295;
                unsigned int seed = 0,seed_max = 0;
                for (int i = 0; i < p_len && seed_max < MAX_ui; i++){
                    seed = seed_max;
                    seed_max += int(*(p + i));
                };
                srand(seed);
                const char* ch = lines.c_str();
                const int lens = (int)(lines.length() - 2);
                Stack<char> C(lens);
                int lens_ = 0;
                for (int i = lens; i >= 0 && !(*(ch + i) == 'K'); i--){
                    lens_ = i - 1;
                    *(ch + i) >> C;
                };
                Stack<int> I(lens);
                char* ch_ = new char[lens_];
                *ch_ = '1';
                for (int i = 0; i < lens_; i++){
                    *(ch_ + i) = *(ch + i);
                };
                int count = 0,Num,num;
                char k;
                char* NUM = new char[6];
                int rand_ = 0;
                NUM[5] = '\0';
                while (!C.isempty()){
                    k << C;
                    Num = int(k) - 48;
                    num = 0;
                    for (int i = 0; i < 5; ++i){
                        NUM[i] = '\0';
                    };
                    for (int i = count; i < count + Num; ++i){
                        NUM[num] = *(ch_ + i);
                        num++;
                    };
                    count += Num;
                    rand_ = rand();
                    I << (atoi(NUM) - rand_);
                };
                Stack<wchar_t> W(lens);
                wchar_t W_;
                int w;
                while (!I.isempty()){
                    w << I;
                    W_ = wchar_t(w);
                    W << W_;
                };
                wstring ws;
                wchar_t wt;
                while (!W.isempty()){
                    wt << W;
                    ws += wt;
                };
                delete[] ch_;
                delete[] NUM;
                wstring* ws_ = new wstring;
                *ws_ = ws;
                return ws_;
            };
        };
        static inline float Q_rsqrt(float number){
            float x2 = number * 0.5F,y = number;
            const float threehalfs = 1.5F;
            long i = *(long*)&y;
            i = 0x5f3759df - (i >> 1);
            y = *(float*)&i;
            y = y * (threehalfs - (x2 * y * y));
            return y;
        };
    #ifdef _BITSET_
        template <class T = int>
        inline void cout_bit(T& t){
            std::cout << std::bitset<sizeof(t) * 8>(t) << t;
        }; // 弃用 Deprecated
    #endif     // _BITSET_
    #ifdef ENABLE_KEY
        class Miniwin{
        public:
            static char DetectKbhit(){
                while (!kbhit()){
                    return getch();
                };
                return 0;
            };
            static void Refresh(){ system("cls"); };
        };
    #endif // ENABLE_KEY
        class KFILE{
        private:
            static void error(int mode){
                if (mode == 1){
                    std::cout << "\nCould not open file or wrong file path!!!\n";
                };
            };

        public:
            static void FileCpoy(std::string from,std::string to){
                using namespace std;
                fstream file;
                file.open(from.c_str(),ios::binary | ios::in | ios::ate);
                int length = (int)file.tellg();
                char* Data = new char[length + 1];
                file.seekg(0);
                file.read(Data,length);
                fstream file2;
                file2.open(to.c_str(),ios::binary | ios::out);
                file2.write(Data,length);
            };
            // 弃用 Deprecated//巨慢实现,不要用
            static Stack<Kstring>* FileReadAsStack(const std::string res,int mode = 0){
                using namespace std;
                const int max = 1024;
                char line[max];
                FILE* fp1 = fopen(res.c_str(),"r");
                if (fp1 == nullptr){
                    error(mode);
                    if (mode == 0){
                        return new Stack<Kstring>();
                    };
                };
                fseek(fp1,0,2);
                long readbuffer_ = ftell(fp1);
                rewind(fp1);
                int readbuffer = readbuffer_ / max + 1;
                Stack<Kstring>* S = new Stack<Kstring>(readbuffer);
                while (fgets(line,max,fp1) != NULL){
                    *S << line;
                };
                return S;
            }; // unsafe
            // 弃用 Deprecated//巨慢实现,不要用
            static std::string FileReadAsString(const std::string res,int mode = 0){
                Stack<Kstring>* K = FileReadAsStack(res,mode);
                std::string ress = Stack<std::string>::AsString(*K);
                delete K;
                return ress;
            };
        };
        template <class Cl_Pn,class T>
        class Bind2c{ // The parameter T is the first parameter of the function (class usage)
        public:
            T val_;
            Cl_Pn* clp_;
            explicit Bind2c(Cl_Pn& clp,const T t): val_(t){ clp_ = &clp; };
            template <class C>
            void operator()(const C t_){ return (*clp_)(val_,t_); };
        };

        template <class _T,class _Bind>
        void for_each_bind(_T _Beg,_T _End,_Bind& _bind){
            while (_Beg != _End){
                _bind(*_Beg);
                _Beg++;
            };
        };
    #define __KUR_PARAMS_BINDF template <class Cl_Pn, class T, typename... Args>
        template <class Cl_Pn,class T,typename... Args>
        class Bindf{ // The parameter T is the first parameter of the function (funciton usage)
        public:
            T val_;
            Cl_Pn(*clpn)(Args...);
            explicit Bindf(Cl_Pn(*cl_pn)(Args...),const T t = 0): val_(t){ clpn = cl_pn; };
            template <typename... Args_>
            Cl_Pn operator()(const Args_ &&...args){ return (*clpn)(val_,std::forward<Args>(args)...); };
            template <class Ty_>
            Cl_Pn operator()(const Ty_ t_){ return (*clpn)(val_,t_); };
            Cl_Pn operator()(){ return (*clpn)(val_); };
        };
        template <typename Fn,typename Pm,size_t... N>
        auto invokeImpl(Fn fn,Pm pa,std::index_sequence<N...>){ return fn(std::get<N>(pa)...); };
        template <typename Fn,typename Pm>
        auto invoke(Fn fn,Pm t){
            static constexpr auto size = std::tuple_size<Pm>::value;
            return invokeImpl(fn,t,std::make_index_sequence<size>{});
        };

        template <typename...>
        struct List{};
        template <typename T,typename... U>
        struct List<T,U...>{
            using Head = T;
            using Tail = List<U...>;
        };
        template <unsigned int,typename>
        struct TypeAt;
        template <>
        struct TypeAt<0,List<>>{
            using type = List<>;
        };
        template <typename Head,typename... Tail>
        struct TypeAt<0,List<Head,Tail...>>{
            using type = Head;
        };
        template <unsigned int idx,typename Head,typename... Tail>
        struct TypeAt<idx,List<Head,Tail...>>{
            using type = typename TypeAt<idx - 1,List<Tail...>>::type;
        };
        // 巨慢实现,不要用,没有Sleep
        __KUR_PARAMS_BINDF Cl_Pn ButtonUpDetection(int CharId,Bindf<Cl_Pn,T,Args...>& _bf){
            if (KEY_DOWN(CharId)){
                while (KEY_DOWN(CharId))
                    ;
                return _bf();
            };
            return (Cl_Pn)0;
        }; // Only one can exist at a time
    #ifndef USE_ASCII
        HANDLE GetProcessHandle(int nID){
            return OpenProcess(PROCESS_ALL_ACCESS,FALSE,nID);
        }; // Get process handle by process name (with suffix .exe)
    #endif     // !USE_ASCII
        class gdtime{ // 获取精确时间
        public:
            LARGE_INTEGER t1;
            LARGE_INTEGER t2;
            LARGE_INTEGER tc;
            gdtime(){
                QueryPerformanceFrequency(&tc);
                QueryPerformanceCounter(&t1);
            };
            double get(){
                QueryPerformanceCounter(&t2);
                return (double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
            };
        };
        namespace ConsoleColor{
            class Color{
            public:
                Color(int r,int g,int b): r(r),g(g),b(b){};
                int r = 0;
                int g = 0;
                int b = 0;
            };
            class Colors{
            public:
                Color CUSTOMIZE = Color(0,0,0);
                Color Default_ft = Color(204,204,204);
                Color Default_bg = Color(12,12,12);
                Color black = Color(0,0,0);
                Color blue = Color(0,55,218);
                Color green = Color(19,161,14);
                Color red = Color(197,15,31);
                Color purple = Color(136,23,152);
                Color yellow = Color(193,156,0);
                Color white = Default_ft;
                Color grey = Color(118,118,118);
                Color light_blue = Color(97,214,214);
                Color light_green = Color(22,198,22);
                Color light_red = Color(231,72,86);
                Color lavender = Color(180,0,158);
                Color light_yellow = Color(249,241,165);
                Color bright_white = Color(255,255,255);
            };
            void rgb_init(){
                HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
                HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
                DWORD dwInMode,dwOutMode;
                GetConsoleMode(hIn,&dwInMode);
                GetConsoleMode(hOut,&dwOutMode);
                dwInMode |= 0x0200;
                dwOutMode |= 0x0004;
                SetConsoleMode(hIn,dwInMode);
                SetConsoleMode(hOut,dwOutMode);
            };
            void rgb_set(int wr,int wg,int wb,int br,int bg,int bb){ printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm",wr,wg,wb,br,bg,bb); };
            void rgb_rec(){ rgb_set(204,204,204,12,12,12); };
            void rgb_out(const char* ch,Color& w,Color& b){
                rgb_set(w.r,w.g,w.b,b.r,b.g,b.b);
                std::cout << ch;
                rgb_rec();
            };
            void rgb_out(const char* ch,Color& w){
                Color col(12,12,12);
                rgb_out(ch,w,col);
            };
            void rgb_out(const char* ch,int wr,int wg,int wb,int br,int bg,int bb){
                rgb_set(wr,wg,wb,br,bg,bb);
                std::cout << ch;
                rgb_rec();
            };
            void rgb_out(const char* ch,int wr,int wg,int wb){
                Color col(wr,wg,wb);
                rgb_out(ch,col);
            };
        #ifdef USE_ASCII
            void set_title(const char* ch){
                SetConsoleTitle(LPCSTR(ch));
            };
        #else
            void set_title(const wchar_t* ch){
                SetConsoleTitle(LPCWSTR(ch));
            };
        #endif
        #define K_COLOR                   \
    using namespace ConsoleColor; \
    ConsoleColor::rgb_init();     \
    ConsoleColor::Colors K_color;
        };
    #define KUR_MAXPATH 512
        class Kwin{
        public:
        #ifdef USE_ASCII
            static void SetAutoRun(const char* ch){
                HKEY hKey;
                LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
                long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,lpRun,0,KEY_WRITE,&hKey);
                if (lRet == ERROR_SUCCESS){
                    char pFileName[MAX_PATH] = {0};
                    DWORD dwRet = GetModuleFileName(NULL,pFileName,MAX_PATH);
                    lRet = RegSetValueEx(hKey,ch,0,REG_SZ,(BYTE*)pFileName,dwRet);
                    RegCloseKey(hKey);
                };
            };
        #endif // USE_ASCII
        };
    #ifdef ENABLE_ZIP
    #define Windows_ENABLE_base
        void CZIP(std::vector<std::string> files,const char* topath){
            std::string Src = files[0];
            std::string Src1 = Src;
            auto name = split(Src,"\\");
            Src = name[name.size() - 1];
            std::wstring ws;
            ws.assign(Src.begin(),Src.end());
            auto wtc = ToWchar_N(topath);
            std::wstring ws1 = wtc;
            HZIP hz = CreateZip(WstrToTchar(ws1),0);
            ws1.assign(Src1.begin(),Src1.end());
            for (size_t i = 0; i < files.size(); i++){
                auto file = files[i];
                auto Name = ToWstring_(FileName(file.c_str()));
                std::wstring ws = ToWstring_(file);
                ZipAdd(hz,WstrToTchar(Name),WstrToTchar(ws));
            };
            CloseZip(hz);
            delete wtc;
        };
        void UZIP(const char* src){
            auto wcs = ToWchar_N(src);
            HZIP hz = OpenZip(wcs,0);
            ZIPENTRY ze;
            GetZipItem(hz,-1,&ze);
            int numitems = ze.index;
            for (int zi = 0; zi < numitems; zi++){
                ZIPENTRY ze;
                GetZipItem(hz,zi,&ze);
                UnzipItem(hz,zi,ze.name);
            };
            CloseZip(hz);
            delete wcs;
        };
    #endif
    #ifdef ENABLE_VOLUME
    #define VOLUME_START CoInitialize(0)
    #define SET_VOLUME(x) SetVolum(x)
    #define GET_VOLUME GetVolume()
    #define VOLUME_END CoUninitialize()
        bool SetVolum(int volume){
            bool ret = false;
            HRESULT hr;
            IMMDeviceEnumerator* pDeviceEnumerator = 0;
            IMMDevice* pDevice = 0;
            IAudioEndpointVolume* pAudioEndpointVolume = 0;
            IAudioClient* pAudioClient = 0;
            try{
                hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),NULL,CLSCTX_ALL,__uuidof(IMMDeviceEnumerator),(void**)&pDeviceEnumerator);
                if (FAILED(hr))
                    throw "CoCreateInstance";
                hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender,eMultimedia,&pDevice);
                if (FAILED(hr))
                    throw "GetDefaultAudioEndpoint";
                hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void**)&pAudioEndpointVolume);
                if (FAILED(hr))
                    throw "pDevice->Active";
                hr = pDevice->Activate(__uuidof(IAudioClient),CLSCTX_ALL,NULL,(void**)&pAudioClient);
                if (FAILED(hr))
                    throw "pDevice->Active";
                float fVolume;
                fVolume = volume / 100.0f;
                hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(fVolume,&GUID_NULL);
                if (FAILED(hr))
                    throw "SetMasterVolumeLevelScalar";
                pAudioClient->Release();
                pAudioEndpointVolume->Release();
                pDevice->Release();
                pDeviceEnumerator->Release();
                ret = true;
            } catch (...){
                if (pAudioClient)
                    pAudioClient->Release();
                if (pAudioEndpointVolume)
                    pAudioEndpointVolume->Release();
                if (pDevice)
                    pDevice->Release();
                if (pDeviceEnumerator)
                    pDeviceEnumerator->Release();
                throw;
            };
            return ret;
        };

        int GetVolume(){
            int volumeValue = 0;
            HRESULT hr;
            IMMDeviceEnumerator* pDeviceEnumerator = 0;
            IMMDevice* pDevice = 0;
            IAudioEndpointVolume* pAudioEndpointVolume = 0;
            IAudioClient* pAudioClient = 0;
            try{
                hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),NULL,CLSCTX_ALL,__uuidof(IMMDeviceEnumerator),(void**)&pDeviceEnumerator);
                if (FAILED(hr)){
                    throw "CoCreateInstance";
                };
                hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender,eMultimedia,&pDevice);
                if (FAILED(hr)){
                    throw "GetDefaultAudioEndpoint";
                };
                hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void**)&pAudioEndpointVolume);
                if (FAILED(hr)){
                    throw "pDevice->Active";
                };
                hr = pDevice->Activate(__uuidof(IAudioClient),CLSCTX_ALL,NULL,(void**)&pAudioClient);
                if (FAILED(hr)){
                    throw "pDevice->Active";
                };
                float fVolume;
                hr = pAudioEndpointVolume->GetMasterVolumeLevelScalar(&fVolume);
                if (FAILED(hr)){
                    throw "SetMasterVolumeLevelScalar";
                };
                pAudioClient->Release();
                pAudioEndpointVolume->Release();
                pDevice->Release();
                pDeviceEnumerator->Release();
                volumeValue = fVolume * 100;
            } catch (...){
                if (pAudioClient)
                    pAudioClient->Release();
                if (pAudioEndpointVolume)
                    pAudioEndpointVolume->Release();
                if (pDevice)
                    pDevice->Release();
                if (pDeviceEnumerator)
                    pDeviceEnumerator->Release();
                throw;
            };
            return volumeValue;
        };

        void RaisePCMVolume(char* buf,UINT32 size,UINT32 uRepeat,double vol){
            if (!size){
                return;
            };
            for (int i = 0; i < size; i += 2){
                short wData;
                wData = MAKEWORD(buf[i],buf[i + 1]);
                long dwData = wData;
                for (int j = 0; j < uRepeat; j++){
                    dwData = dwData * vol;
                    if (dwData < -0x8000){
                        dwData = -0x8000;
                    } else if (dwData > 0x7FFF){
                        dwData = 0x7FFF;
                    };
                };
                wData = LOWORD(dwData);
                buf[i] = LOBYTE(wData);
                buf[i + 1] = HIBYTE(wData);
            };
        };
        void setvol(int vol){
            VOLUME_START;
            SET_VOLUME(vol);
            VOLUME_END;
        };
    #endif // ENABLE_VOLUME
    #ifdef USE_ASCII
    #include <tlhelp32.h>
        DWORD GetProcessidFromName(LPCTSTR name){
            PROCESSENTRY32 pe;
            DWORD id = 0;
            HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
            pe.dwSize = sizeof(PROCESSENTRY32);
            if (!Process32First(hSnapshot,&pe)){
                return 0;
            };
            while (1){
                pe.dwSize = sizeof(PROCESSENTRY32);
                if (Process32Next(hSnapshot,&pe) == FALSE){
                    break;
                };
                if (strcmp(pe.szExeFile,name) == 0){
                    id = pe.th32ProcessID;
                    break;
                };
            };
            CloseHandle(hSnapshot);
            return id;
        };
        bool getProcess(const char* procressName){
            char pName[MAX_PATH];
            strcpy(pName,procressName);
            CharLowerBuff(pName,MAX_PATH);
            PROCESSENTRY32 currentProcess;
            currentProcess.dwSize = sizeof(currentProcess);
            HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
            if (hProcess == INVALID_HANDLE_VALUE){
                printf("CreateToolhelp32Snapshot() failed!!\n");
                return false;
            };
            bool bMore = Process32First(hProcess,&currentProcess);
            while (bMore){
                CharLowerBuff(currentProcess.szExeFile,MAX_PATH);
                if (strcmp(currentProcess.szExeFile,pName) == 0){
                    CloseHandle(hProcess);
                    return true;
                };
                bMore = Process32Next(hProcess,&currentProcess);
            };
            CloseHandle(hProcess);
            return false;
        };

    #endif // USE_ASCII

        namespace thread{
            class ThreadProtect{
            public:
                std::thread* _thread;
                explicit ThreadProtect(std::thread* t_): _thread(t_){};
                ~ThreadProtect(){
                    if (_thread->joinable()){
                        this->_thread->join();
                    };
                    delete _thread;
                };
                ThreadProtect(std::thread const&) = delete;
                ThreadProtect& operator=(ThreadProtect const&) = delete;
            };
        #define KUR_THREADPROJECT(code) thread::ThreadProtect(new std::thread([] code))
        };
    #ifdef __KURBOOST
        namespace bst{ // boost support
            class BSTjson{
            private:
                boost::json::value bjson;
                int waitfrequency = 0;

            public:
                bool throw_error = 0;
                bool finish = false;
                BSTjson(){};
                void Constructor(boost::json::value& sobj){
                    this->bjson = sobj;
                    this->finish = true;
                };
                void Constructor(const std::string& path){
                    std::string* str = FastRead_N(path.c_str());
                    this->bjson = boost::json::parse(*str);
                    delete str;
                };
                void ConstructorThread(const std::string& path){
                    std::thread([](boost::json::value& bs,std::string jpath,bst::BSTjson& sub){std::string* str = FastRead_N(jpath.c_str()); bs = boost::json::parse(*str); delete str; sub.finish = true; },
                        std::ref(this->bjson),path,std::ref(*this))
                        .detach();
                };
                std::string ToString(){ return boost::json::serialize(bjson); };
                boost::json::value Get(){ return bjson; };
                bool IsFinished(){ return this->finish; };
                bool OutToFileThread(const std::string& path){
                    if (this->finish){
                        std::thread([](std::string ph,bst::BSTjson sub){std::string str = std::move(sub.ToString()); KMiscellaneous::SaveData(ph,str); },
                            path,*this)
                            .detach();
                        return true;
                    } else{
                        return false;
                    };
                };
                bool OutToFile(std::string& path){
                    std::string str = std::move(this->ToString());
                    KMiscellaneous::SaveData(path,str);
                    return true;
                };
                bool WaitThread(const int wait_time,const int max_try_count = 10){
                    this->waitfrequency = 0;
                    while (!this->finish){
                        Sleep(wait_time);
                        if (max_try_count <= this->waitfrequency++){
                            return false;
                        };
                    };
                    return true;
                };
                void AlwaysWaitThread(const int wait_spacetime = 1){
                    while (!this->finish){
                        Sleep(wait_spacetime);
                    };
                };
                int GetWaitFrequency(){ return this->waitfrequency; };
            };
        };
    #endif
        namespace others{
            class ProgressBar{
            public:
                int _total_step;
                int _block_len;
                int iter = 0;
                std::string _message;
                ProgressBar(const int total_step,const int block_len,const std::string& message){
                    this->_total_step = total_step;
                    this->_block_len = block_len;
                    this->_message = '\r' + message + "[%.2lf%%]:";
                };
                bool Next(){
                    if (iter < _total_step){
                        this->iter++;
                        printf(this->_message.c_str(),iter * 100.0 / this->_total_step);
                        int show_num = iter * _block_len / _total_step;
                        for (int j = 1; j <= show_num; j++){
                            printf("█");
                        };
                        return true;
                    };
                    return false;
                };
            };
        };
    };

    namespace LB2023{
        /* UNICODE */
    #ifndef UNICODE
    #define UNICODE
    #endif
        std::wstring string2wstring(std::string str){
            std::wstring result;
            int len = MultiByteToWideChar(CP_ACP,0,str.c_str(),(int)str.size(),NULL,0);
            TCHAR* buffer = new TCHAR[len + 1];
            MultiByteToWideChar(CP_ACP,0,str.c_str(),(int)str.size(),buffer,len);
            buffer[len] = '\0';
            result.append(buffer);
            delete[] buffer;
            return result;
        };
        std::string wstring2string(std::wstring wstr){
            std::string result;
            int len = WideCharToMultiByte(CP_ACP,0,wstr.c_str(),(int)wstr.size(),NULL,0,NULL,NULL);
            char* buffer = new char[len + 1];
            WideCharToMultiByte(CP_ACP,0,wstr.c_str(),(int)wstr.size(),buffer,len,NULL,NULL);
            buffer[len] = '\0';
            result.append(buffer);
            delete[] buffer;
            return result;
        };
        namespace thread{

        };
        namespace sys{
            //******************************
            // ModifyPassword(NULL);直接上锁
            //******************************
            void ModifyPassword(char pass2[0x100]){
                char cmd1[0x100];
                wchar_t username1[0x100];
                DWORD c = 150;
                ::GetUserName(username1,&c);
                char username2[0x100];
                sprintf(username2,"%S",username1);
                sprintf(cmd1,"%s%s%s%s","net user ",username2," ",pass2);
                LB2022::getCmdResult(std::string(cmd1));
                // system();//执行cmd命令
            }

            /************************************
            *MondifyPassword
            *功能   修改密码 ，需要输入旧密码验证才能修改
            *参数
            szServerName  主机名，如果为本机，设置为NULL
            userName      该NetUserChangePassword函数改变指定用户的密码。如果此参数为NULL，则使用调用方的登录名
            oldPassword   用户的旧密码
            newpassword   用户的新密码
            *************************************/

            void MondifyPassword(LPWSTR szServerName,LPWSTR userName,LPWSTR oldPassword,LPWSTR newpassword){
                DWORD dwError = 0;
                DWORD dwLevel = 5;
                NET_API_STATUS nStatus;
                if (dwLevel != 5){
                    fwprintf(stderr,L"Usage: %s \\\\ServerName UserName OldPassword NewPassword\n",L"");
                    exit(1);
                }
                nStatus = NetUserChangePassword(szServerName,userName,oldPassword,newpassword);
                std::cout << "nStatus=" << nStatus << std::endl;
                if (nStatus == NERR_Success){

                    fwprintf(stderr,L"User password has been changed successfully\n");
                } else{

                    fprintf(stderr,"A system error has occurred: %d\n",nStatus);
                }
            }
        };
        namespace Keyboard{
            void PasteInfo(){
                keybd_event(0x11,0,0,0); // press ctrl
                keybd_event(0x56,0,0,0); // press v
                keybd_event(0x56,0,2,0); // release v
                keybd_event(0x11,0,2,0); // release ctrl
            };
            void SetClipBoard(const char* str,int alloc_size){
                HWND hWnd = NULL;
                OpenClipboard(hWnd);
                EmptyClipboard();
                HANDLE hHandle = GlobalAlloc(GMEM_FIXED,alloc_size);
                char* pData = (char*)GlobalLock(hHandle);
                strcpy(pData,str);
                SetClipboardData(CF_TEXT,hHandle);
                GlobalUnlock(hHandle);
                CloseClipboard();
            };
            void SetClipBoardW(const std::wstring& ws){
                if (!OpenClipboard(NULL) || !EmptyClipboard()){
                    return;
                }
                HGLOBAL hMen;
                const TCHAR* strText = (ws).c_str();
                hMen = GlobalAlloc(GMEM_MOVEABLE,((wcslen(strText)) * sizeof(TCHAR)) * 2);
                if (!hMen){
                    CloseClipboard();
                    return;
                }
                LPWSTR lpStr = (LPWSTR)GlobalLock(hMen);
                wmemcpy(lpStr,strText,(size_t)((wcslen(strText)) * sizeof(TCHAR)));
                lpStr[wcslen(strText)] = (TCHAR)0;
                GlobalUnlock(hMen);
                SetClipboardData(CF_UNICODETEXT,hMen);
                CloseClipboard();
                return;
            };
            struct State{
                int state = 1;
                void* p = nullptr;
                HHOOK hk = 0;
            };
            void WaitOK(State* s,int time_){
                while (s->state){
                    Sleep(time_);
                };
            };

            namespace Hook{
                struct Pack{
                    char cval = 0;
                    int ival = 0;
                };
                int hkHandle(char,int,void*){ return 1; };

                BOOL CloseKeyBoardHook(HHOOK hk){
                    return UnhookWindowsHookEx(hk);
                };
                typedef int (*PMF)(char,int,void*);
                PMF ___pfun;
                void* element;
                static class Hooks{
                public:
                    static int Call(char c,int i){
                        return ___pfun(c,i,element);
                    };
                    static void Construct(PMF pfun_ = 0,void* element_ = 0){
                        if (!pfun_){
                            Hook::___pfun = Hook::hkHandle;
                        } else{
                            Hook::___pfun = pfun_;
                        };
                        Hook::element = element_;
                    };
                    static void Change(PMF fun,void* element_){
                        Hook::___pfun = fun;
                        Hook::element = element_;
                    };
                };
                LRESULT CALLBACK LowLevelKeyboardProc(
                    _In_ int nCode,
                    _In_ WPARAM wParam,
                    _In_ LPARAM lParam){
                    KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
                    if (ks->flags == 128 || ks->flags == 129){
                        using namespace std;
                        char pch = 0;
                        switch (ks->vkCode){
                            case 0x30:
                            case 0x60:
                                pch = '0';
                                break;
                            case 0x31:
                            case 0x61:
                                pch = '1';
                                break;
                            case 0x32:
                            case 0x62:
                                pch = '2';
                                break;
                            case 0x33:
                            case 0x63:
                                pch = '3';
                                break;
                            case 0x34:
                            case 0x64:
                                pch = '4';
                                break;
                            case 0x35:
                            case 0x65:
                                pch = '5';
                                break;
                            case 0x36:
                            case 0x66:
                                pch = '6';
                                break;
                            case 0x37:
                            case 0x67:
                                pch = '7';
                                break;
                            case 0x38:
                            case 0x68:
                                pch = '8';
                                break;
                            case 0x39:
                            case 0x69:
                                pch = '9';
                                break;
                            case 0x6A:
                                pch = '*';
                                break;
                            case 0x6B:
                                pch = '+';
                                break;
                            case 0x6D:
                                pch = '-';
                                break;
                            case 0x6E:
                                pch = '.';
                                break;
                            case 0x6F:
                                pch = '/';
                                break;
                            case 0x0D:
                                pch = -1; //"Enter"
                                break;
                            case 0xA0:
                            case 0xA1:
                                pch = -2; //"Shift"
                                break;
                            case 0x08:
                                pch = -3; //"Backspace"
                                break;
                            case 0x20:
                                pch = -4; //"Space"
                                break;
                        };
                        if (!pch){
                            pch = char(ks->vkCode);
                        };
                        Hooks::Call(pch,2);
                    };
                    return CallNextHookEx(NULL,nCode,wParam,lParam);
                }
                HHOOK KeyboardHookInit(){
                    HHOOK keyboardHook = 0;
                    keyboardHook = SetWindowsHookEx(
                        WH_KEYBOARD_LL,
                        LowLevelKeyboardProc,
                        GetModuleHandleA(NULL),
                        NULL);
                    if (keyboardHook == 0){
                        std::cout << "HOOK =0" << std::endl;
                        return 0;
                    };
                    MSG msg;
                    while (1){
                        if (PeekMessageA(
                            &msg,
                            NULL,
                            NULL,
                            NULL,
                            PM_REMOVE)){
                            TranslateMessage(&msg);
                            DispatchMessageW(&msg);
                        } else
                            Sleep(10);
                    }
                    return keyboardHook;
                };

            };

            class KeyBoardDetect{
            public:
                HHOOK hk = 0;
                // ret 1 continue
                // ret 0 exit <<BUG 无法退出...
                KeyBoardDetect(int (*pfn_)(char,int,void*),void* element_){
                    LB2023::Keyboard::Hook::Hooks::Construct(pfn_,(void*)element_);
                    hk = LB2023::Keyboard::Hook::KeyboardHookInit();
                };
                ~KeyBoardDetect(){ LB2023::Keyboard::Hook::CloseKeyBoardHook(this->hk); };
            };
            void Default(){
                LB2023::Keyboard::Hook::Hooks::Change(LB2023::Keyboard::Hook::___pfun,LB2023::Keyboard::Hook::element);
            };
        };
        namespace others{
            template <size_t N = 64>
            class MUIALU{
            public:
                bool carry = false;
                std::bitset<N> reg;
                void SetReg(std::bitset<N> value){
                    reg = value;
                };
                void Add(std::bitset<N>& rvalue,std::bitset<N>& value){
                    carry = false;
                    for (size_t i = 0; i < N; ++i){
                        bool h = rvalue[i];
                        bool l = value[i];
                        rvalue[i] = h ^ l ^ carry;
                        carry = (h & (l | carry)) | (l & carry);
                    }
                };
                void Add(std::bitset<N>& value){
                    this->Add(reg,value);
                };
                void Subtract(std::bitset<N>& rvalue,std::bitset<N> value){
                    value.flip();
                    for (std::size_t i = 0; i < value.size(); ++i){
                        if (value[i]){
                            value[i] = 0;
                        } else{
                            value[i] = 1;
                            break;
                        }
                    }
                    this->Add(rvalue,value);
                };
                void Subtract(std::bitset<N> value){
                    this->Subtract(reg,value);
                };
                void Multiply(std::bitset<N> value){
                    std::bitset<N> result;
                    for (size_t i = 0; i < N; ++i){
                        if (value[i]){
                            std::bitset<N> temp = reg << i;
                            this->Add(result,temp);
                        }
                    }
                    reg = result;
                };
            };

            int UBIDB2(std::string& num){ // u dec / 2
                int tmp = 0;
                std::string stmp;
                for (auto c : num){
                    tmp = (c - '0') + tmp * 10;
                    if (!stmp.empty() || (tmp >> 1)){
                        stmp.push_back((tmp >> 1) + '0');
                    }
                    tmp = tmp % 2;
                }
                num = stmp.empty() ? "0" : stmp;
                return tmp;
            }
            std::string UBITB(std::string& dec){ // u dec -> bin
                std::string stmp = dec;
                std::string bin;
                while (stmp.size() > 1 || (stmp[0] - '0') >= 1){
                    bin.push_back(UBIDB2(stmp) + '0');
                }
                std::reverse(bin.begin(),bin.end());
                return bin.empty() ? "0" : bin;
            };

            std::string UBIPBI(std::string& p0,std::string& p1){ // u dec + dec
                std::string tm;
                std::string tn;
                std::string ret;
                if (p0.size() > p1.size()){
                    tm = p0;
                    tn = p1;
                } else{
                    tm = p1;
                    tn = p0;
                }
                std::reverse(tm.begin(),tm.end());
                std::reverse(tn.begin(),tn.end());

                int carry = 0;
                for (size_t i = 0; i < tn.size(); ++i){
                    int sum = tm[i] - '0' + tn[i] - '0' + carry;
                    carry = sum / 10;
                    ret.push_back(sum % 10 + '0');
                }
                for (size_t i = tn.size(); i < tm.size(); ++i){
                    int sum = tm[i] - '0' + carry;
                    carry = sum / 10;
                    ret.push_back(sum % 10 + '0');
                }
                if (carry > 0){
                    ret.push_back(carry + '0');
                }
                std::reverse(ret.begin(),ret.end());
                return ret;
            }
            std::string UBTBI(const std::string& bin){ // u bin -> dec
                std::string ret = "0";
                std::string r2 = "1";
                for (int i = bin.size() - 1; i >= 0; --i){
                    if (bin[i] == '1'){
                        ret = UBIPBI(ret,r2);
                    }
                    r2 = UBIPBI(r2,r2);
                }
                return ret;
            }
            /*
            constexpr const int r2m = 666;
            int main(){
            MUIALU<r2m>alu;//200->664.386
            std::string str0,str1;
            std::cin >> str0 >> str1;
            std::bitset<r2m>b0(UBITB(str0));
            std::bitset<r2m>b1(UBITB(str1));
            alu.SetReg(b0);
            alu.Multiply(b1);
            std::cout << UBTBI(alu.reg.to_string());
            return 0;
            }
            */
        };
        namespace script{
        };
    };
};
#endif
