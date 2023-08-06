//SZN_FUNC.H
//适用于 VS C++17
#pragma once
#ifndef SZN_FUNC_H_
#define SZN_FUNC_H_
#ifndef _ITERATOR_DEBUG_LEVEL
//#define _ITERATOR_DEBUG_LEVEL=0
#endif // !_ITERATOR_DEBUG_LEVEL
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define Windows_ENABLE_KURZER

#define k2022 KUR::LB2022::KUR
#define k2023 KUR::LB2023

#ifdef ENABLE_SZN_OPE
#include <gl/glut.h>
#pragma comment(lib,"msvcrt.lib")
#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"opengl32.lib")
#include<glad.h>
#include<glfw3.h>
#endif // ENABLE_SZN_OPE
#ifdef __KURBOOST
//请添加boost库!!!
#define _WIN32_WINNT 0x0601
#include<boost/algorithm/string.hpp>
#include<boost/json.hpp>
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
#include<Windows.h>
#include <tchar.h>
#include <functional>
#include<strsafe.h>
#include <utility>
#include <memory>
#include <list>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
//#include<tuple>
#include<cwchar>
#include<type_traits>
#ifdef ENABLE_ZIP
#define Windows_ENABLE_KURZER
//Please add zip.cpp, unzip.cpp to the project directory
//请把zip.cpp和unzip.cpp添加到工程目录!!!
//https://www.codeproject.com/Articles/7530/Zip-Utils-Clean-Elegant-Simple-Cplusplus-Win
#include<Windows.h>
#include<TlHelp32.h>
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
#define Windows_ENABLE_KURZER
#endif
#ifdef Windows_SOCK_ENABLE_KURZER
#define Windows_ENABLE_KURZER
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <TlHelp32.h>
#define __KW_cw_malloc(GetLengthOfTarget,nptrType) (nptrType*)malloc(MultiByteToWideChar(0, 0, GetLengthOfTarget, -1, NULL, 0) * sizeof(nptrType))
#define __KW_cw(__char,__wchar) MultiByteToWideChar(0, 0,__char, -1, __wchar, MultiByteToWideChar(0, 0, __char, -1, NULL, 0))
#ifdef Windows_SOCK_ENABLE_KURZER
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
//请链接ws2_32.lib
#endif // Windows_SOCK_ENABLE_KURZER

#endif //Windows_SOCK_ENABLE_KURZER

#include<Bitset>
#include <assert.h>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")
#ifdef ENABLE_KEY
#include<conio.h>
#endif
#pragma warning (disable : 28183)
#pragma warning (disable : 26451)
#pragma warning (disable : 6387)
#pragma warning (disable : 4244)
#pragma warning (disable : 6011)//NULL指针引用
#define sptr_(name,type,_Pfn)shared_ptr<type>name(_Pfn)
#define sptrn(name,type,value)sptr_(name,type,new type(value))
#define _ptrtype(_Pfn)(typeid(*_Pfn).name())
#define newptr(name,type,value) type *name=new type(value)
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#define CLICK(time) mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);Sleep(time);mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0)
#define MOUSESET(x,y) MouseMove(x,y)
#define MOUSEOFFSET(_x,_y) 	{POINT __MOUSE_POINT_POSITION;::GetCursorPos(&__MOUSE_POINT_POSITION);MOUSESET(__MOUSE_POINT_POSITION.x+_x,__MOUSE_POINT_POSITION.y+_y);}
#define __DEBUGEXIT if (IsDebuggerPresent()) { return 0; }
#include "base.hpp"
typedef unsigned short int KCDSTR;
namespace KUR{
    namespace LB2022{
        namespace KUR{
            template<class Tchar> class CharT{
            public:
                using char_t = std::basic_string<Tchar,std::char_traits<Tchar>,std::allocator<Tchar>>;
                using out_t = std::basic_ostream<Tchar,std::char_traits<Tchar>>;
            };
        #define _KUR_TYPE_CHAR_(type) typename CharT<type>::char_t
        #define _KUR_TYPE_CHAR_USE(type) using TYPE_CHAR = typename CharT<type>::char_t
            const double pi = 3.14159265358979;
            const double e = 2.718281828459045;
            const double eps = 1e-10;
            void MouseMove(int x,int y){ double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1; double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1; double fx = x * (65535.0f / fScreenWidth); double fy = y * (65535.0f / fScreenHeight); mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,fx,fy,0,0); };
            tm* TIME(){ time_t now = time(0); tm* _tm = new tm; errno_t gmtm = gmtime_s(_tm,&now); return _tm; };
            wchar_t* ToWchar_N(const char* c){ size_t cSize = strlen(c) + 1; wchar_t* wc = new wchar_t[cSize]; mbstowcs(wc,c,cSize); return wc; };//unsafe
            ULONGLONG GetNowMilliSecond(){ return GetTickCount64(); };
            bool Divisible(unsigned long long _Val,unsigned long long _Num){ double a = (double)_Val / (double)_Num; if (a - (double)(unsigned long long)a < 1e-6){ return true; }; return false; };
            bool IsPrimeNumber(unsigned long long num){ if (num == 1 || num == 4){ return 0; }; if (num == 2 || num == 3){ return 1; }; if (num % 6 != 1 && num % 6 != 5){ return 0; }; unsigned long long tmp = (unsigned long long)sqrt(num); for (unsigned long long i = 5; i <= tmp; i += 6){ if (num % i == 0 || num % (i + 2) == 0){ return 0; }; }; return 1; };
            template<typename Tchar>typename CharT<Tchar>::char_t* FastRead_N(const Tchar* path){ std::basic_ifstream<Tchar,std::char_traits<Tchar>> fin(path,std::ios::binary); if (fin.good()){ using Type_char = typename CharT<Tchar>::char_t; std::vector<Tchar> buf(fin.seekg(0,std::ios::end).tellg()); fin.seekg(0,std::ios::beg).read(&buf[0],static_cast<std::streamsize>(buf.size())); fin.close(); Type_char* ret = new Type_char(); ret->assign(buf.begin(),buf.end()); return ret; } else{ return nullptr; }; };
            template<class Ry_ = void,class Ty_> void for_each(Ty_* First_,Ty_* Last_,Ry_(*_Pfn)(Ty_)){ while (First_ != Last_){ (*_Pfn)(*First_++); }; };
            std::string Subreplace(std::string resource_str,std::string sub_str,std::string new_str){ std::string dst_str = resource_str; std::string::size_type pos = 0; while ((pos = dst_str.find(sub_str)) != std::string::npos){ dst_str.replace(pos,sub_str.length(),new_str); }; return dst_str; };
            template<typename Tchar = char>std::vector<_KUR_TYPE_CHAR_(Tchar)> split(const _KUR_TYPE_CHAR_(Tchar)& s,const _KUR_TYPE_CHAR_(Tchar)& seperator){ std::vector<_KUR_TYPE_CHAR_(Tchar)> result; size_t posBegin = 0; size_t posSeperator = s.find(seperator); while (posSeperator != s.npos){ result.push_back(s.substr(posBegin,posSeperator - posBegin)); posBegin = posSeperator + seperator.size(); posSeperator = s.find(seperator,posBegin); }; if (posBegin != s.length())result.push_back(s.substr(posBegin)); return result; };
            //可能有bug
            TCHAR* WstrToTchar(std::wstring& ws){ return (TCHAR*)(&ws[0]); };
            //可能有bug
            std::string FileName(const char* path){ std::string P = path; auto v = split(P,"\\"); P = v[v.size() - 1]; return P; };
            //可能有bug
            std::wstring ToWstring_(const std::string& input){ std::wstring_convert<std::codecvt_utf8<wchar_t>> converter; return converter.from_bytes(input); };
            //可能有bug
            std::string ToString_(const std::wstring& input){ std::wstring_convert<std::codecvt_utf8<wchar_t>> converter; return converter.to_bytes(input); };
            void GetFileNames(std::string path,std::vector<std::string>& files){ using namespace std; intptr_t hFile = 0; struct _finddata_t fileinfo; std::string p; if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) != -1){ do{ if ((fileinfo.attrib & _A_SUBDIR)){ if (strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0)GetFileNames(p.assign(path).append("\\").append(fileinfo.name),files); } else{ files.push_back(p.assign(path).append("\\").append(fileinfo.name)); }; } while (_findnext(hFile,&fileinfo) == 0); _findclose(hFile); }; };
            //可能有bug
            std::string GetStringByWchar(const WCHAR* wszString){ std::string strString; if (wszString != NULL){ std::wstring ws(wszString); strString.assign(ws.begin(),ws.end()); }; return strString; };
            //可能有bug
            std::wstring GetWStringByChar(const char* szString){ std::wstring wstrString; if (szString != NULL){ std::string str(szString); wstrString.assign(str.begin(),str.end()); }; return wstrString; };
            int Rand_(int beg,int end){ srand((int)(time(NULL))); return ((rand() % (end - beg + 1)) + beg); };
            size_t FindAllCount(std::string str,std::string sub){ size_t count = 0; size_t index = 0; size_t len = str.length(); while ((index = str.find(sub,index)) < len){ count++; index++; }; return count; };
            void getFiles(std::string path,std::vector<std::string>& files){ using namespace std; intptr_t   hFile = 0; struct _finddata_t fileinfo; string p; if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) != -1){ do{ if ((fileinfo.attrib & _A_SUBDIR)){ if (strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0)getFiles(p.assign(path).append("\\").append(fileinfo.name),files); } else{ files.push_back(p.assign(path).append("\\").append(fileinfo.name)); }; } while (_findnext(hFile,&fileinfo) == 0); _findclose(hFile); }; };
            size_t GetFileSize(char* file_){ FILE* file = fopen(file_,"r"); if (file){ size_t size = filelength(fileno(file)); fclose(file); return size; } else{ return 0; }; };
            static short int GetBits(){ return sizeof(int*) * 8; };
            template<typename T>void PrintAll(const std::vector<T>& vec,const std::string&& _firdisplay = ""){ for_each(vec.begin(),vec.end(),[&,_firdisplay](const T& t){std::cout << _firdisplay << t << std::endl; }); };
            void PrintAll(const std::vector<std::wstring>& vec,const std::wstring&& _firdisplay = L""){ for_each(vec.begin(),vec.end(),[&,_firdisplay](const std::wstring& t){std::wcout << _firdisplay << t << std::endl; }); };
            std::string getCmdResult(const std::string& strCmd){ using namespace std; char buf[8191] = {0}; FILE* pf = NULL; if ((pf = _popen(strCmd.c_str(),"r")) == NULL){ return ""; }; string strResult; while (fgets(buf,sizeof buf,pf)){ strResult += buf; }; _pclose(pf); unsigned int iSize = (unsigned int)strResult.size(); if (iSize > 0 && strResult[iSize - 1] == '\n'){ strResult = strResult.substr(0,iSize - 1); }; return strResult; };
            template<typename T> T ButtonUpDetection(int charid,T(*Pfn_)(),int sleeptime = 1){ if (KEY_DOWN(charid)){ while (KEY_DOWN(charid)); Sleep(sleeptime); return (*Pfn_)(); }; return (T)0; };//Only one can exist at a time
            template<typename Tchar>size_t _stroprlen(const Tchar* str,void(*Pfn)(Tchar t) = nullptr){ const Tchar* end = str; if (Pfn == nullptr){ while (*end++); } else{ while (*end){ Pfn(*end++); }; }; return end - str - 1; };
		std::string CODE(char* path){ std::ifstream fin(path,std::ios::binary); unsigned char  s2; fin.read((char*)&s2,sizeof(s2)); int p = s2 << 8; fin.read((char*)&s2,sizeof(s2)); p += s2; std::string code; switch (p){ case 0xfffe:code = "Unicode"; break; case 0xfeff:code = "Unicode big endian"; break; case 0xefbb:code = "UTF-8"; break; default:code = "ANSI"; }; fin.close(); return code; };
                                                                                                                                                                                                                                        template<class T> class Stack;
                                                                                                                                                                                                                                        class Kstring;

                                                                                                                                                                                                                                    #ifdef ENABLE_MD5
                                                                                                                                                                                                                                        class MD5{
                                                                                                                                                                                                                                        private:
                                                                                                                                                                                                                                            unsigned int tempA,tempB,tempC,tempD,strlength;
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            constexpr static unsigned int ssssss[64] = {7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
                                                                                                                                                                                                                                            constexpr static unsigned int TTTTTT[64] = {0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};
                                                                                                                                                                                                                                            MD5(){ tempA = 0x67452301; tempB = 0xefcdab89; tempC = 0x98badcfe; tempD = 0x10325476; strlength = 0; };
                                                                                                                                                                                                                                            std::string operator()(std::string& src){ return encode(src); };
                                                                                                                                                                                                                                        private:
                                                                                                                                                                                                                                            unsigned int F(unsigned int b,unsigned int c,unsigned int d){ return (b & c) | ((~b) & d); };
                                                                                                                                                                                                                                            unsigned int G(unsigned int b,unsigned int c,unsigned int d){ return (b & d) | (c & (~d)); };
                                                                                                                                                                                                                                            unsigned int H(unsigned int b,unsigned int c,unsigned int d){ return b ^ c ^ d; };
                                                                                                                                                                                                                                            unsigned int I(unsigned int b,unsigned int c,unsigned int d){ return c ^ (b | (~d)); };
                                                                                                                                                                                                                                            unsigned int shift(unsigned int a,unsigned int n){ return (a << n) | (a >> (32 - n)); };
                                                                                                                                                                                                                                            std::string encode(std::string src){ using namespace std; vector<unsigned int> rec = padding(src); for (unsigned int i = 0; i < strlength / 16; i++){ unsigned int num[16]; for (int j = 0; j < 16; j++){ num[j] = rec[i * 16 + j]; }; iterateFunc(num,16); }; return format(tempA) + format(tempB) + format(tempC) + format(tempD); };
                                                                                                                                                                                                                                            void iterateFunc(unsigned int* X,int size = 16){ using namespace std; unsigned int a = tempA,b = tempB,c = tempC,d = tempD,rec = 0,g,k; for (int i = 0; i < 64; i++){ if (i < 16){ g = F(b,c,d); k = i; } else if (i < 32){ g = G(b,c,d); k = (1 + 5 * i) % 16; } else if (i < 48){ g = H(b,c,d); k = (5 + 3 * i) % 16; } else{ g = I(b,c,d); k = (7 * i) % 16; }; rec = d; d = c; c = b; b = b + shift(a + g + X[k] + TTTTTT[i],ssssss[i]); a = rec; }; tempA += a; tempB += b; tempC += c; tempD += d; };
                                                                                                                                                                                                                                            std::vector<unsigned int> padding(std::string src){ using namespace std; unsigned int num = (unsigned int)(((src.length() + 8) / 64) + 1); vector<unsigned int> rec(num * 16); strlength = num * 16; for (unsigned int i = 0; i < src.length(); i++){ rec[i >> 2] |= (int)(src[i]) << ((i % 4) * 8); }; rec[src.length() >> 2] |= (0x80 << ((src.length() % 4) * 8)); rec[rec.size() - 2] = (unsigned int)(src.length() << 3); return rec; };
                                                                                                                                                                                                                                            std::string format(unsigned int num){ using namespace std; const char str16[] = "0123456789abcdef"; string res = ""; unsigned int base = 1 << 8; for (int i = 0; i < 4; i++){ string tmp = ""; unsigned int b = (num >> (i * 8)) % base & 0xff; for (int j = 0; j < 2; j++){ tmp = str16[b % 16] + tmp; b /= 16; }; res += tmp; }; return res; };
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                        std::string md5(std::string& src){ MD5 md5_; return md5_(src); };
                                                                                                                                                                                                                                    #endif // ENABLE_MD5																																																	 //弃用 Deprecated,又慢又烂的垃圾实现
                                                                                                                                                                                                                                        class Kstring{
                                                                                                                                                                                                                                        private:
                                                                                                                                                                                                                                            size_t len = 0;
                                                                                                                                                                                                                                            char* data = NULL;
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            template<class T> friend class Stack;
                                                                                                                                                                                                                                            ~Kstring(){ delete[] data; len = 0; };
                                                                                                                                                                                                                                            Kstring(const char* str = NULL){ if (!str){ len = 0; data = new char[1]; *data = '\0'; } else{ len = strlen(str); data = new char[len + 1]; strcpy(data,str); }; };
                                                                                                                                                                                                                                            Kstring(const Kstring& str){ len = str.size(); data = new char[len + 1]; strcpy(data,str.str()); };
                                                                                                                                                                                                                                            Kstring operator+(const Kstring& str)const{ Kstring newstr; newstr.len = len + str.size(); newstr.data = new char[newstr.len + 1]; strcpy(newstr.data,data); strcat(newstr.data,str.data); return newstr; };
                                                                                                                                                                                                                                            Kstring& operator=(const Kstring& str){ if (this == &str){ return*this; }; delete[] data; len = str.len; data = new char[len + 1]; strcpy(data,str.str()); return*this; };
                                                                                                                                                                                                                                            Kstring& operator=(Kstring&& str)noexcept{ if (this == &str){ return *this; }; delete this->data; this->data = str.data; str.data = nullptr; return *this; };
                                                                                                                                                                                                                                            Kstring& operator+=(const Kstring& str){ *this = *this + str; return *this; };
                                                                                                                                                                                                                                            bool operator==(const Kstring& str) const{ if (len != str.len){ return false; } else{ return strcmp(data,str.data) ? false : true; }; };
                                                                                                                                                                                                                                            char& operator[](int n) const{ if (n >= int(len)){ return data[len - 1]; } else if (n < 0 && (-n) <= int(len)){ return data[int(len) + n]; } else if ((-n) > int(len)){ return data[0]; } else{ return data[n]; }; };
                                                                                                                                                                                                                                            size_t size() const{ return len; };
                                                                                                                                                                                                                                            const char* str() const{ return data; };
                                                                                                                                                                                                                                            char* address(){ return data; };
                                                                                                                                                                                                                                            friend Kstring& operator>>(const char* c,Kstring& str){ Kstring newstr = c; str += newstr; return str; };
                                                                                                                                                                                                                                            friend Kstring& operator<<(Kstring& str,const char* c){ c >> str; return str; };
                                                                                                                                                                                                                                            friend Kstring& operator<<(char* c,Kstring& str){ strcpy(c,str.str()); return str; };
                                                                                                                                                                                                                                            friend Kstring& operator>>(Kstring& str,char* c){ c << str; return str; };
                                                                                                                                                                                                                                            Kstring& operator>>(std::string& str){ str = this->str(); return *this; };
                                                                                                                                                                                                                                            Kstring& operator<<(std::string& str){ *this = str.c_str(); return *this; };
                                                                                                                                                                                                                                            friend std::ostream& operator<<(std::ostream& os,Kstring& str){ os << str.data; return os; };
                                                                                                                                                                                                                                            friend std::ostream& operator>>(Kstring& str,std::ostream& os){ os << str; };
                                                                                                                                                                                                                                            static void* operator new(std::size_t t){ Kstring* p = ::new Kstring(); return p; };
                                                                                                                                                                                                                                            char* Data(){ return data; };
                                                                                                                                                                                                                                            size_t to_find(const Kstring& str){ std::string st = (*this).str(); return st.find(str.str()); };
                                                                                                                                                                                                                                            std::string string_(){ std::string str = (*this).str(); return str; };
                                                                                                                                                                                                                                            std::string ToString(){ return this->string_(); };
                                                                                                                                                                                                                                            static void _ToLower(char* s){ size_t len = strlen(s); for (size_t i = 0; i < len; i++){ if (s[i] >= 'A' && s[i] <= 'Z'){ s[i] = tolower(s[i]); } } };
                                                                                                                                                                                                                                            static void _ToUpper(char* s){ size_t len = strlen(s); for (size_t i = 0; i < len; i++){ if (s[i] >= 'a' && s[i] <= 'z'){ s[i] = toupper(s[i]); } } };
                                                                                                                                                                                                                                            Kstring& ToLower(){ _ToLower(data); return *this; };
                                                                                                                                                                                                                                            Kstring& ToUpper(){ _ToUpper(data); return *this; };
                                                                                                                                                                                                                                            friend Kstring& operator<<(Kstring& ks,void(*p)(Kstring&)){ (*p)(ks); return ks; };
                                                                                                                                                                                                                                            char* begin(){ return data; };
                                                                                                                                                                                                                                            char* end(){ return data + len; };
                                                                                                                                                                                                                                            template<class T_>friend T_ operator<<(Kstring& s,T_(*_Pfn)(Kstring&)){ return (*_Pfn)(s); };
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                        //弃用 Deprecated,偶尔凑合着用用可以,因为内置char类型是kstring,所以垃圾
                                                                                                                                                                                                                                        template<class T = Kstring> class Stack{
                                                                                                                                                                                                                                        private:
                                                                                                                                                                                                                                            bool t = true;
                                                                                                                                                                                                                                            int top;
                                                                                                                                                                                                                                            int _max;
                                                                                                                                                                                                                                            T* p_data = NULL;
                                                                                                                                                                                                                                            T* p_revise = NULL;
                                                                                                                                                                                                                                            T* pop(){ T* item; if (top > 0){ item = (p_data + (--top)); t = true; return item; } else{ t = false; return NULL; }; };
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            friend class Kstring;
                                                                                                                                                                                                                                            operator int() const{ return int(*p_data); };
                                                                                                                                                                                                                                            operator double() const{ return double(*p_data); };
                                                                                                                                                                                                                                            operator char() const{ return char(*p_data); };
                                                                                                                                                                                                                                            operator bool() const{ return bool(*p_data); };
                                                                                                                                                                                                                                            operator float() const{ return float(*p_data); };
                                                                                                                                                                                                                                            friend Stack& operator>>(Stack& s,Stack& s_){ if (!s.isempty()){ s_.push(*s.pop()); } return s_; };
                                                                                                                                                                                                                                            friend Stack& operator<<(Stack& s,Stack& s_){ s_ >> s; return s; };
                                                                                                                                                                                                                                            friend Stack& operator<<(Stack& s,T t){ s.push(t); return s; };
                                                                                                                                                                                                                                            friend Stack& operator>>(T t,Stack& s){ s << t; return s; };
                                                                                                                                                                                                                                            friend Stack& operator>>(Stack& s,T& t){ s.pop(t); return s; };
                                                                                                                                                                                                                                            friend Stack& operator<<(T& t,Stack& s){ s >> t; return s; };
                                                                                                                                                                                                                                            friend std::ostream& operator<<(std::ostream& os,Stack& s){ T _; s.pop(_); os << _; return os; };
                                                                                                                                                                                                                                            friend std::ostream& operator>>(Stack& s,std::ostream& os){ os << s; return os; };
                                                                                                                                                                                                                                            Stack(const int max = 256){ top = 0; _max = max; p_data = new T[_max]; };
                                                                                                                                                                                                                                            Stack(const std::initializer_list<T>& v){ top = 0; _max = (int)v.size(); p_data = new T[_max]; for (T item : v){ *this << item; }; };
                                                                                                                                                                                                                                            ~Stack(){ delete[] p_data; p_data = NULL; };
                                                                                                                                                                                                                                            bool isempty(){ t = true; return top == 0; };
                                                                                                                                                                                                                                            bool isfull(){ t = true; return top == _max; };
                                                                                                                                                                                                                                            bool push(const T item){ if (top < _max){ *(p_data + (top++)) = item; t = true; return true; } else{ t = false; return false; }; };
                                                                                                                                                                                                                                            bool pop(T& item){ if (top > 0){ item = *(p_data + (--top)); t = true; return true; } else{ t = false; return false; }; };
                                                                                                                                                                                                                                            void Delete_Create(){ delete[] p_data; top = 0; p_data = new T[_max]; t = true; };
                                                                                                                                                                                                                                            void Delete_Create(const int len){ delete[] p_data; top = 0; p_data = new T[len]; t = true; };
                                                                                                                                                                                                                                            bool state(){ return t; };
                                                                                                                                                                                                                                            T* operator()(int n){ { if (n < 0 && -n < top){ p_revise = (p_data + top + n); } else if (n < top && n>0){ p_revise = (p_data + n); } else if (n > 0 && n > top){ p_revise = (p_data + top); } else if (n < 0 && -n > top || n == 0 || n < 0 && -n == top){ p_revise = p_data; }; }; return  p_revise; };
                                                                                                                                                                                                                                            T operator[](int n){ return check(n); };
                                                                                                                                                                                                                                            T check(const int n){ return *(*this)(n); };
                                                                                                                                                                                                                                            T* check_(const int n){ return (*this)(n); };
                                                                                                                                                                                                                                            T* address(){ return p_data; };
                                                                                                                                                                                                                                            void pop_(){ --top; };
                                                                                                                                                                                                                                            int length(){ return top; };
                                                                                                                                                                                                                                            int max_length(){ return _max; };
                                                                                                                                                                                                                                            void flip(){ KUR::Stack<T>t(_max); int top_ = top; int i = 0; while (i < top){ t << *(*this)(i); ++i; }; this->top = 0; while (!t.isempty()){ t >> *this; }; top = top_; };
                                                                                                                                                                                                                                            void replace(const T& from,const T& to){ for (int i = 0; i < top; ++i){ if ((*this)[i] == from){ *(*this)(i) = to; }; }; };
                                                                                                                                                                                                                                            T sum(){ T sum = 0;  for (T i = 0; i < top; ++i){ sum = sum + *(*this)(i); }; return sum; };
                                                                                                                                                                                                                                            void to_string(Stack<std::string>& str){ Kstring ks; std::string st; while (!(*this).isempty() && !str.isfull()){ *this >> ks; ks >> st; st >> str; }; };
                                                                                                                                                                                                                                            void to_kstring(Stack<Kstring>& kstr){ Kstring ks; std::string st; while (!(*this).isempty() && !kstr.isfull()){ *this >> st; ks << st; ks >> kstr; }; };
                                                                                                                                                                                                                                            static size_t TotalCharLength(Stack<Kstring>& kstr){ size_t t = 0; for (int i = 0; i < kstr.length(); i++){ t += kstr.check(i).size(); }return t; };
                                                                                                                                                                                                                                            static size_t TotalCharLength(Stack<std::string>& str){ size_t t = 0; for (int i = 0; i < str.length(); i++){ t += str.check(i).size(); }return t; };
                                                                                                                                                                                                                                            void print(){ while (!this->isempty()){ std::cout << *this << std::endl; }; };
                                                                                                                                                                                                                                            static std::string AsString(Stack<Kstring>& kstr){ std::string str; Kstring ks; kstr.flip(); while (!kstr.isempty()){ ks << kstr; str += ks.str(); }return str; };
                                                                                                                                                                                                                                            static KUR::Stack<size_t>* FindLineAsStack(std::string str,std::string sub,int Stacklen = 1024){ using namespace std; size_t len = str.length(); size_t index = 0; size_t count = 0; KUR::Stack<size_t>* res = new KUR::Stack<size_t>(Stacklen); while ((index = str.find(sub,index)) < len){ count++; index++; *res << index; }return res; };
                                                                                                                                                                                                                                            static Stack<std::string>* SubLineAsString(Kstring& res,std::string sub_1,std::string sub_2,int Stacklen = 1024){ using namespace std; auto s1 = KUR::Stack<int>::FindLineAsStack(res.Data(),sub_1,Stacklen); auto s2 = KUR::Stack<int>::FindLineAsStack(res.Data(),sub_2,Stacklen); auto length = min(s1->length(),s2->length()); int length_ = (int)sub_1.size(); KUR::Stack<std::string>* S = new KUR::Stack<std::string>(length); size_t t1 = 0,t2 = 0; std::string s = res.string_(); std::string s_; s1->flip(); s2->flip(); for (int i = 0; i < length; i++){ t1 << *s1; t1 += length_; t2 << *s2; s_ = s.substr(t1 - 1,t2 - t1); *S << s_; }return S; };
                                                                                                                                                                                                                                            std::string ToString(){ std::string str = "null"; return str; };
                                                                                                                                                                                                                                            void DeleteEachPoint(){ while (!this->isempty()){ delete* (p_data + (--top)); }; };
                                                                                                                                                                                                                                            friend Stack& operator<<(Stack& s,void(*_Pfn)(Stack<T>&)){ (*_Pfn)(s); return s; };
                                                                                                                                                                                                                                            template<class T_>friend T_ operator<<(Stack& s,T_(*_Pfn)(Stack<T>&)){ return (*_Pfn)(s); };
                                                                                                                                                                                                                                            T operator[](T(*_Pfn)(Stack<T>*)){ return (*_Pfn)(this); };
                                                                                                                                                                                                                                            void operator--(){ top--; };
                                                                                                                                                                                                                                            T* begin(){ return p_data; };
                                                                                                                                                                                                                                            T* end(){ return p_data + top; };
                                                                                                                                                                                                                                            T* Final(){ return p_data + _max; };
                                                                                                                                                                                                                                            void set(T* pdata = 0,T* previse = 0,int top_ = 0){ if (pdata != 0){ p_data = pdata; }; if (previse != 0){ previse = p_revise; }; if (top_ != 0){ top = top_ >= _max ? _max : top_; }; };
                                                                                                                                                                                                                                            friend Stack& operator>>(std::istream& ist,Stack& s){ T t; std::cin >> t; s.push(t); return s; };
                                                                                                                                                                                                                                            void SetFromCin(int start,int end_){ end_ = (end_ >= _max ? _max : end_); for (int i = 0; i < end_; i++){ std::cin >> *check_(i); }; };
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                        //Stack(char*)->Stack<std::string>;
                                                                                                                                                                                                                                        //弃用 Deprecated
                                                                                                                                                                                                                                        template<class T>void space(Stack<T>& s){ s.push(" "); };
                                                                                                                                                                                                                                        //弃用 Deprecated
                                                                                                                                                                                                                                        void space(Kstring& ks){ ks << " "; };
                                                                                                                                                                                                                                        //弃用 Deprecated
                                                                                                                                                                                                                                        void endl(Kstring& ks){ ks << "\n"; };
                                                                                                                                                                                                                                        //弃用 Deprecated
                                                                                                                                                                                                                                        template<class T>T end(Stack<T>* s){ return (*s)[(*s).length() - 1]; };
                                                                                                                                                                                                                                        template<typename T>class Node{
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            Node(){};
                                                                                                                                                                                                                                            Node(const T& _data){
                                                                                                                                                                                                                                                data = _data;
                                                                                                                                                                                                                                            };
                                                                                                                                                                                                                                            T data;
                                                                                                                                                                                                                                            Node<T>* next = nullptr;
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                        template<typename T>class OneLinkList{
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            size_t _size = 1;
                                                                                                                                                                                                                                            Node<T>* head = nullptr;
                                                                                                                                                                                                                                            OneLinkList(const T& _data){ this->head = new Node<T>(_data); };
                                                                                                                                                                                                                                            OneLinkList(){ this->head = new Node<T>(); };
                                                                                                                                                                                                                                            Node<T>* end(){ return this->get(this->_size - 1); };
                                                                                                                                                                                                                                            Node<T>* get(size_t pos){ Node<T>* _pos = this->head; pos = min(pos,this->_size); for (size_t i = 0; i < pos; i++){ _pos = _pos->next; }; return _pos; };
                                                                                                                                                                                                                                            OneLinkList<T>& insert(int pos,const T& _data){ Node<T>* new_node = new Node<T>(_data); Node<T>* this_node = this->get(pos); new_node->next = this_node->next; this_node->next = new_node; this->_size++; return *this; };
                                                                                                                                                                                                                                            OneLinkList<T>& insert(OneLinkList<T>& oll,size_t pos){ size_t _size = oll._size; Node<T>* _thisnode = this->get(pos); Node<T>* _ollend = oll.get(_size - 1); _ollend->next = _thisnode->next; _thisnode->next = oll.head; this->_size += _size; return *this; };
                                                                                                                                                                                                                                            OneLinkList<T>& insert(Node<T>* _begin,Node<T>* _end,size_t pos){ Node<T>* _pos = _begin; size_t count = 1; while (_pos != _end){ _pos = _pos->next; count++; }; Node<T>* _thisnode = this->get(pos); _end->next = _thisnode->next; _thisnode->next = _begin; this->_size += count; return *this; };
                                                                                                                                                                                                                                            OneLinkList<T>& remove(size_t pos,size_t _end = -1){ std::vector<Node<T>*>vec; size_t n = _end; if (_end == -1){ _end = this->_size; } else{ _end = min(_end,this->_size); }; if (pos == 0){ Node<T>* pos1 = this->head; for (size_t i = 0; i < _end; i++){ vec.push_back(pos1); pos1 = pos1->next; }; if (n != -1){ this->head = pos1->next; } else{ this->head = NULL; }; for (size_t i = 0; i < vec.size(); i++){ delete vec[i]; }; this->_size -= vec.size(); return *this; }; Node<T>* _pos = this->get(pos - 1); Node<T>* __pos = _pos; for (size_t i = pos; i < _end; i++){ _pos = _pos->next; vec.push_back(_pos); }; __pos->next = _pos->next; this->_size -= vec.size(); for (size_t i = 0; i < vec.size(); i++){ delete vec[i]; }; return *this; };
                                                                                                                                                                                                                                            template<typename Ty>OneLinkList<T>& for_each(Ty(*_Pfn)(KUR::Node<T>*)){ Node<T>* _pos = this->head; for (size_t i = 0; i < this->_size; i++){ (*_Pfn)(_pos); _pos = _pos->next; }; return *this; };
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                        template<typename Tchar>class BaseChar{
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            Tchar* tch = nullptr;
                                                                                                                                                                                                                                            void(*_ReclaimMemory)(BaseChar<Tchar>*) = nullptr;
                                                                                                                                                                                                                                            BaseChar(Tchar* ch){ this->tch = ch; };
                                                                                                                                                                                                                                            BaseChar(Tchar* ch,void(*_Rm)(BaseChar<Tchar>*)){ this->tch = ch; this->_ReclaimMemory = _Rm; };
                                                                                                                                                                                                                                            ~BaseChar(){ if (this->_ReclaimMemory == nullptr){ return; }; this->_ReclaimMemory(this); };
                                                                                                                                                                                                                                        };

                                                                                                                                                                                                                                        class KMiscellaneous{
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            static std::wstring WI(std::string address,std::string&& locale = "chs"){ std::locale chs(locale); std::wifstream ifs(address.c_str()); ifs.imbue(chs); std::wstring line,lines; while (std::getline(ifs,line)){ lines = lines + line + L"\n"; }; return lines; };
                                                                                                                                                                                                                                            //巨慢实现,不要用
                                                                                                                                                                                                                                            static std::string ReadData(std::string address){ std::ifstream ifs(address.c_str()); std::string line,lines; while (std::getline(ifs,line)){ lines = lines + line + "\n"; }; return lines; };
                                                                                                                                                                                                                                            //巨慢实现,不要用
                                                                                                                                                                                                                                            static std::vector<std::string>* ReadData_vector_N(std::string address){ std::vector<std::string>* vec = new std::vector<std::string>; std::ifstream ifs(address.c_str()); std::string line,lines; while (std::getline(ifs,line)){ vec->push_back(line); }; return vec; };
                                                                                                                                                                                                                                            static void SaveData(std::string& address,std::string& Source,std::string&& locale = "chs"){ std::fstream F_(address.c_str(),std::ios::out); F_.imbue((std::locale)locale); F_ << Source; F_.close(); };
                                                                                                                                                                                                                                            static void WO(std::string& address,std::wstring& ws,std::string&& locale = "chs"){ std::wfstream F_(address.c_str(),std::ios::out); F_.imbue((std::locale)locale); F_ << ws; F_.close(); };
                                                                                                                                                                                                                                            static int GetProgramBits(){ return sizeof(int*) * 8; };
                                                                                                                                                                                                                                            static void w_cout(std::wstring& s,std::string&& locale = "chs"){ std::locale chs(locale); std::wcout.imbue(chs); std::wcout << s; };
                                                                                                                                                                                                                                            //巨慢实现,不要用
                                                                                                                                                                                                                                            static std::string* encryption(const std::string key,const std::wstring& wstr){ using namespace std; const int max = 1024; const char* p = key.c_str(); const int p_len = (int)key.length(); const unsigned int MAX_ui = 4294967295; unsigned int seed = 0,seed_max = 0; for (int i = 0; i < p_len && seed_max < MAX_ui; i++){ seed = seed_max; seed_max += int(*(p + i)); }; srand(seed); wstring ws = wstr + L"   "; const wchar_t* ws_p = ws.c_str(); int ws_len = (int)(ws.length() - 2); int* ws_seed = new int[ws_len]; int* ws_int = new int[ws_len]; int* ws_res = new int[ws_len]; int* ws_res_len = new int[ws_len]; string keylen,ws_result; for (int i = 0; i < ws_len; i++){ ws_int[i] = int(ws_p[i]); ws_seed[i] = rand(); ws_res[i] = ws_int[i] + ws_seed[i]; ws_res_len[i] = (int)to_string(ws_res[i]).length(); keylen += to_string(ws_res_len[i]); ws_result += to_string(ws_res[i]); }; string key_ = "K" + keylen; string* resu = new string(ws_result + key_); delete[] ws_seed; delete[] ws_int; delete[] ws_res; delete[] ws_res_len; return resu; };
                                                                                                                                                                                                                                            //巨慢实现,不要用
                                                                                                                                                                                                                                            static std::wstring* decrypt(const std::string key,const std::string& lines){ using namespace std; const int max = 1024; const char* p = key.c_str(); const int p_len = (int)key.length(); const unsigned int MAX_ui = 4294967295; unsigned int seed = 0,seed_max = 0; for (int i = 0; i < p_len && seed_max < MAX_ui; i++){ seed = seed_max; seed_max += int(*(p + i)); }; srand(seed); const char* ch = lines.c_str(); const int lens = (int)(lines.length() - 2); KUR::Stack<char> C(lens); int lens_ = 0; for (int i = lens; i >= 0 && !(*(ch + i) == 'K'); i--){ lens_ = i - 1; *(ch + i) >> C; }; KUR::Stack<int> I(lens); char* ch_ = new char[lens_]; *ch_ = '1'; for (int i = 0; i < lens_; i++){ *(ch_ + i) = *(ch + i); }; int count = 0,Num,num; char k; char* NUM = new char[6]; int rand_ = 0; NUM[5] = '\0'; while (!C.isempty()){ k << C; Num = int(k) - 48; num = 0; for (int i = 0; i < 5; ++i){ NUM[i] = '\0'; }; for (int i = count; i < count + Num; ++i){ NUM[num] = *(ch_ + i); num++; }; count += Num; rand_ = rand(); I << (atoi(NUM) - rand_); }; KUR::Stack<wchar_t> W(lens); wchar_t W_; int w; while (!I.isempty()){ w << I; W_ = wchar_t(w); W << W_; }; wstring ws; wchar_t wt; while (!W.isempty()){ wt << W; ws += wt; }; delete[] ch_; delete[] NUM; wstring* ws_ = new wstring; *ws_ = ws; return ws_; };
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                        static inline float Q_rsqrt(float number){ float x2 = number * 0.5F,y = number; const float threehalfs = 1.5F; long i = *(long*)&y; i = 0x5f3759df - (i >> 1); y = *(float*)&i; y = y * (threehalfs - (x2 * y * y)); return y; };
                                                                                                                                                                                                                                    #ifdef _BITSET_
                                                                                                                                                                                                                                        template<class T = int> inline void cout_bit(T& t){ std::cout << std::bitset<sizeof(t) * 8>(t) << t; };//弃用 Deprecated
                                                                                                                                                                                                                                        //class TimeSetBit {
                                                                                                                                                                                                                                        //public:
                                                                                                                                                                                                                                        // static const unsigned year_offset = 7;
                                                                                                                                                                                                                                        // static const unsigned month = 4;
                                                                                                                                                                                                                                        // static const unsigned day = 5;
                                                                                                                                                                                                                                        // static const unsigned hour = 5;
                                                                                                                                                                                                                                        // static const unsigned minutes = 6;
                                                                                                                                                                                                                                        // static const unsigned second = 6;
                                                                                                                                                                                                                                        // static const unsigned length = 33;
                                                                                                                                                                                                                                        // static const unsigned short ulen = sizeof(unsigned short) * 8;
                                                                                                                                                                                                                                        // std::bitset<KUR::TimeSetBit::length>date;
                                                                                                                                                                                                                                        // TimeSetBit(unsigned year_, unsigned month_, unsigned day_, unsigned hour_, unsigned minutes_, unsigned second_) {
                                                                                                                                                                                                                                        //	 unsigned yearoff = year_ - 2020;
                                                                                                                                                                                                                                        //	 unsigned arr[6] = { yearoff, month_, day_, hour_, minutes_, second_ };
                                                                                                                                                                                                                                        //	 unsigned arr_size[6] = { year_offset,month,day,hour,minutes,second };
                                                                                                                                                                                                                                        //	 for (unsigned i = 0; i < 6; i++) {
                                                                                                                                                                                                                                        //		 unsigned a = arr[i];
                                                                                                                                                                                                                                        //		 unsigned b = arr_size[i];
                                                                                                                                                                                                                                        //		 for (unsigned j = 0; j < b; j++) {

                                                                                                                                                                                                                                        //		 };

                                                                                                                                                                                                                                        //	 };

                                                                                                                                                                                                                                        // };
                                                                                                                                                                                                                                        //};
                                                                                                                                                                                                                                    #endif // _BITSET_
                                                                                                                                                                                                                                    #ifdef ENABLE_KEY
                                                                                                                                                                                                                                        class Miniwin{
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            static char DetectKbhit(){ while (!kbhit()){ return getch(); }; return 0; };
                                                                                                                                                                                                                                            static void Refresh(){ system("cls"); };
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                    #endif // ENABLE_KEY
                                                                                                                                                                                                                                        class KFILE{
                                                                                                                                                                                                                                        private:
                                                                                                                                                                                                                                            static void error(int mode){ if (mode == 1){ std::cout << "\nCould not open file or wrong file path!!!\n"; }; };
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            static void FileCpoy(std::string from,std::string to){ using namespace std; fstream file; file.open(from.c_str(),ios::binary | ios::in | ios::ate); int length = (int)file.tellg(); char* Data = new char[length + 1]; file.seekg(0); file.read(Data,length); fstream file2; file2.open(to.c_str(),ios::binary | ios::out); file2.write(Data,length); };
                                                                                                                                                                                                                                            //弃用 Deprecated//巨慢实现,不要用
                                                                                                                                                                                                                                            static KUR::Stack<KUR::Kstring>* FileReadAsStack(const std::string res,int mode = 0){ using namespace std; const int max = 1024; char line[max]; FILE* fp1 = fopen(res.c_str(),"r"); if (fp1 == nullptr){ error(mode); if (mode == 0){ return  new KUR::Stack<KUR::Kstring>(); }; }; fseek(fp1,0,2); long readbuffer_ = ftell(fp1); rewind(fp1); int readbuffer = readbuffer_ / max + 1; KUR::Stack<KUR::Kstring>* S = new KUR::Stack<KUR::Kstring>(readbuffer); while (fgets(line,max,fp1) != NULL){ *S << line; }; return S; };//unsafe
                                                                                                                                                                                                                                            //弃用 Deprecated//巨慢实现,不要用
                                                                                                                                                                                                                                            static std::string FileReadAsString(const std::string res,int mode = 0){ KUR::Stack<KUR::Kstring>* K = FileReadAsStack(res,mode); std::string ress = KUR::Stack<std::string>::AsString(*K); delete K; return ress; };
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                        template<class Cl_Pn,class T>class Bind2c{//The parameter T is the first parameter of the function (class usage)
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            T val_;
                                                                                                                                                                                                                                            Cl_Pn* clp_;
                                                                                                                                                                                                                                            explicit Bind2c(Cl_Pn& clp,const T t):val_(t){ clp_ = &clp; };
                                                                                                                                                                                                                                            template<class C> void operator()(const C t_){ return (*clp_)(val_,t_); };
                                                                                                                                                                                                                                        };

                                                                                                                                                                                                                                        template<class _T,class _Bind>void for_each_bind(_T _Beg,_T _End,_Bind& _bind){ while (_Beg != _End){ _bind(*_Beg); _Beg++; }; };
                                                                                                                                                                                                                                    #define __KUR_PARAMS_BINDF template<class Cl_Pn, class T, typename...Args>																																																										 
                                                                                                                                                                                                                                        template<class Cl_Pn,class T,typename...Args>class Bindf{//The parameter T is the first parameter of the function (funciton usage)
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            T val_;
                                                                                                                                                                                                                                            Cl_Pn(*clpn)(Args...);
                                                                                                                                                                                                                                            explicit Bindf(Cl_Pn(*cl_pn)(Args...),const T t = 0):val_(t){ clpn = cl_pn; };
                                                                                                                                                                                                                                            template<typename...Args_>Cl_Pn operator()(const Args_&&...args){ return (*clpn)(val_,std::forward<Args>(args)...); };
                                                                                                                                                                                                                                            template<class Ty_> Cl_Pn operator()(const Ty_ t_){ return (*clpn)(val_,t_); };
                                                                                                                                                                                                                                            Cl_Pn operator()(){ return (*clpn)(val_); };
                                                                                                                                                                                                                                        };

                                                                                                                                                                                                                                        //template<class ...Args>
                                                                                                                                                                                                                                        //class Tmp {
                                                                                                                                                                                                                                        // template<size_t...>struct tuple_idx {};
                                                                                                                                                                                                                                        // template<size_t N, size_t... Is>struct tuple_bind : tuple_bind<N - 1, N - 1, Is...> {};
                                                                                                                                                                                                                                        // template<size_t... Is>
                                                                                                                                                                                                                                        // struct tuple_bind<0, Is...> { typedef tuple_idx<Is...> type; };
                                                                                                                                                                                                                                        //public:
                                                                                                                                                                                                                                        // virtual void process(Args... args) = 0;
                                                                                                                                                                                                                                        // template<size_t...Is>
                                                                                                                                                                                                                                        // void extendTupleToProcess(std::tuple<Args...> t, tuple_idx<Is...>&&) { process(std::get<Is>(t)...); };
                                                                                                                                                                                                                                        // void callProcess(std::tuple<Args...> t) { extendTupleToProcess(t, typename tuple_bind<sizeof ...(Args)>::type()); };
                                                                                                                                                                                                                                        //};
                                                                                                                                                                                                                                        //namespace nstuple {
                                                                                                                                                                                                                                        // template< size_t... _Indexes >
                                                                                                                                                                                                                                        // struct X_Index_tuple {};
                                                                                                                                                                                                                                        // template< std::size_t _Num, typename _Tuple = X_Index_tuple<> >
                                                                                                                                                                                                                                        // struct X_Build_index_tuple;
                                                                                                                                                                                                                                        // template< std::size_t _Num, size_t... _Indexes >
                                                                                                                                                                                                                                        // struct X_Build_index_tuple<_Num, X_Index_tuple< _Indexes... > > : X_Build_index_tuple< _Num - 1, X_Index_tuple< _Indexes..., sizeof...(_Indexes) > > {};
                                                                                                                                                                                                                                        // template< size_t... _Indexes >
                                                                                                                                                                                                                                        // struct X_Build_index_tuple< 0, X_Index_tuple< _Indexes... > > { typedef X_Index_tuple< _Indexes... > __type; };
                                                                                                                                                                                                                                        //}; // namespace nstuple
                                                                                                                                                                                                                                        template<typename Fn,typename Pm,size_t ... N>auto invokeImpl(Fn fn,Pm pa,std::index_sequence<N...>){ return fn(std::get<N>(pa) ...); };
                                                                                                                                                                                                                                        template<typename Fn,typename Pm>auto invoke(Fn fn,Pm t){ static constexpr auto size = std::tuple_size<Pm>::value; return invokeImpl(fn,t,std::make_index_sequence<size>{}); };

                                                                                                                                                                                                                                        template<typename...> struct List{};
                                                                                                                                                                                                                                        template<typename T,typename... U>
                                                                                                                                                                                                                                        struct List<T,U...>{ using Head = T; using Tail = List<U...>; };
                                                                                                                                                                                                                                        template<unsigned int,typename> struct TypeAt;
                                                                                                                                                                                                                                        template<> struct TypeAt<0,List<>>{ using type = List<>; };
                                                                                                                                                                                                                                        template<typename Head,typename... Tail>
                                                                                                                                                                                                                                        struct TypeAt<0,List<Head,Tail...>>{ using type = Head; };
                                                                                                                                                                                                                                        template<unsigned int idx,typename Head,typename... Tail>
                                                                                                                                                                                                                                        struct TypeAt<idx,List<Head,Tail...>>{ using type = typename TypeAt<idx - 1,List<Tail...>>::type; };

                                                                                                                                                                                                                                        //template<typename F, typename T, std::size_t... I>auto apply_impl(F&& f, T& t, std::index_sequence<I...>) -> decltype(f(std::get<I>(t)...)) {return f(std::get<I>(t)...); };
                                                                                                                                                                                                                                        //template<typename F, typename T>auto apply(F&& f, T& t) -> decltype(KUR::apply_impl(f, t, std::make_index_sequence<std::tuple_size<T>::value>())) {return KUR::apply_impl(f, t, std::make_index_sequence<std::tuple_size<T>::value>()); };

                                                                                                                                                                                                                                        //template<typename Callable, typename ...Args>class Bind {
                                                                                                                                                                                                                                        //public:
                                                                                                                                                                                                                                        // //using X_Tuple = std::tuple<Args...>;
                                                                                                                                                                                                                                        ////using X_Indices = nstuple::X_Build_index_tuple< std::tuple_size< X_Tuple >::value >::__type;
                                                                                                                                                                                                                                        // 
                                                                                                                                                                                                                                        // 
                                                                                                                                                                                                                                        // using _Types = KUR::List<Args...>;
                                                                                                                                                                                                                                        // using _At = KUR::TypeAt;
                                                                                                                                                                                                                                        // 
                                                                                                                                                                                                                                        // 
                                                                                                                                                                                                                                        // const unsigned int size = sizeof...(Args);
                                                                                                                                                                                                                                        // std::tuple<Args...>&& _tuple;
                                                                                                                                                                                                                                        // Callable&& _Pfn;
                                                                                                                                                                                                                                        // 
                                                                                                                                                                                                                                        // //template< size_t... _Ind >
                                                                                                                                                                                                                                        // //void _S_Invoke(X_Tuple&& xtuple, nstuple::X_Index_tuple< _Ind... >) {
                                                                                                                                                                                                                                        //	
                                                                                                                                                                                                                                        //	 //_Pfn(std::get< _Ind >(std::move(xtuple))...);
                                                                                                                                                                                                                                        //// }

                                                                                                                                                                                                                                        // explicit Bind(Callable(*func), Args&&...args) :_Pfn(std::forward<Callable>(*func)), _tuple(std::make_tuple(args...)) {};
                                                                                                                                                                                                                                        // template<typename...Args_> auto operator()(Args_&&...args) { auto tup = std::tuple_cat(_tuple, std::make_tuple(args...)); return KUR::apply(_Pfn, tup); };
                                                                                                                                                                                                                                        //};
                                                                                                                                                                                                                                        //巨慢实现,不要用,没有Sleep
                                                                                                                                                                                                                                        __KUR_PARAMS_BINDF Cl_Pn ButtonUpDetection(int CharId,KUR::Bindf<Cl_Pn,T,Args...>& _bf){ if (KEY_DOWN(CharId)){ while (KEY_DOWN(CharId)); return _bf(); }; return (Cl_Pn)0; };//Only one can exist at a time
                                                                                                                                                                                                                                    #ifndef USE_ASCII																																																										 
                                                                                                                                                                                                                                        HANDLE GetProcessHandle(int nID){ return OpenProcess(PROCESS_ALL_ACCESS,FALSE,nID); };//Get process handle by process name (with suffix .exe)
                                                                                                                                                                                                                                    #endif // !USE_ASCII
                                                                                                                                                                                                                                        class gdtime{//获取精确时间
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                            LARGE_INTEGER t1;
                                                                                                                                                                                                                                            LARGE_INTEGER t2;
                                                                                                                                                                                                                                            LARGE_INTEGER tc;
                                                                                                                                                                                                                                            gdtime(){
                                                                                                                                                                                                                                                QueryPerformanceFrequency(&tc);
                                                                                                                                                                                                                                                QueryPerformanceCounter(&t1);
                                                                                                                                                                                                                                            };
                                                                                                                                                                                                                                            double get(){
                                                                                                                                                                                                                                                QueryPerformanceCounter(&t2); return (double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
                                                                                                                                                                                                                                            };
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                        namespace ConsoleColor{
                                                                                                                                                                                                                                            class Color{
                                                                                                                                                                                                                                            public:
                                                                                                                                                                                                                                                Color(int r,int g,int b):r(r),g(g),b(b){};
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
                                                                                                                                                                                                                                            void rgb_init(){ HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);	DWORD dwInMode,dwOutMode; GetConsoleMode(hIn,&dwInMode); GetConsoleMode(hOut,&dwOutMode); dwInMode |= 0x0200;	dwOutMode |= 0x0004; SetConsoleMode(hIn,dwInMode); SetConsoleMode(hOut,dwOutMode); };
                                                                                                                                                                                                                                            void rgb_set(int wr,int wg,int wb,int br,int bg,int bb){ printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm",wr,wg,wb,br,bg,bb); };
                                                                                                                                                                                                                                            void rgb_rec(){ rgb_set(204,204,204,12,12,12); };
                                                                                                                                                                                                                                            void rgb_out(const char* ch,Color& w,Color& b){ rgb_set(w.r,w.g,w.b,b.r,b.g,b.b); std::cout << ch; rgb_rec(); };
                                                                                                                                                                                                                                            void rgb_out(const char* ch,Color& w){ Color col(12,12,12); rgb_out(ch,w,col); };
                                                                                                                                                                                                                                            void rgb_out(const char* ch,int wr,int wg,int wb,int br,int bg,int bb){ rgb_set(wr,wg,wb,br,bg,bb); std::cout << ch; rgb_rec(); };
                                                                                                                                                                                                                                            void rgb_out(const char* ch,int wr,int wg,int wb){ Color col(wr,wg,wb); rgb_out(ch,col); };
                                                                                                                                                                                                                                        #ifdef USE_ASCII
                                                                                                                                                                                                                                            void set_title(const char* ch){ SetConsoleTitle(LPCSTR(ch)); };
                                                                                                                                                                                                                                        #else
                                                                                                                                                                                                                                            void set_title(const wchar_t* ch){ SetConsoleTitle(LPCWSTR(ch)); };
                                                                                                                                                                                                                                        #endif
                                                                                                                                                                                                                                        #define K_COLOR using namespace KUR::ConsoleColor;KUR::ConsoleColor::rgb_init();KUR::ConsoleColor::Colors K_color;
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                    #define KUR_MAXPATH 512
                                                                                                                                                                                                                                        class Kwin{
                                                                                                                                                                                                                                        public:
                                                                                                                                                                                                                                        #ifdef USE_ASCII
                                                                                                                                                                                                                                            static void SetAutoRun(const char* ch){ HKEY hKey; LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,lpRun,0,KEY_WRITE,&hKey); if (lRet == ERROR_SUCCESS){ char pFileName[MAX_PATH] = {0}; DWORD dwRet = GetModuleFileName(NULL,pFileName,MAX_PATH); lRet = RegSetValueEx(hKey,ch,0,REG_SZ,(BYTE*)pFileName,dwRet); RegCloseKey(hKey); }; };
                                                                                                                                                                                                                                        #endif // USE_ASCII
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                    #ifdef ENABLE_ZIP
                                                                                                                                                                                                                                    #define Windows_ENABLE_KURZER
                                                                                                                                                                                                                                        void CZIP(std::vector<std::string>files,const char* topath){ std::string Src = files[0]; std::string Src1 = Src; auto name = split(Src,"\\"); Src = name[name.size() - 1]; std::wstring ws; ws.assign(Src.begin(),Src.end()); auto wtc = ToWchar_N(topath); std::wstring ws1 = wtc; HZIP hz = CreateZip(WstrToTchar(ws1),0); ws1.assign(Src1.begin(),Src1.end()); for (size_t i = 0; i < files.size(); i++){ auto file = files[i]; auto Name = ToWstring_(FileName(file.c_str())); std::wstring ws = ToWstring_(file); ZipAdd(hz,WstrToTchar(Name),WstrToTchar(ws)); }; CloseZip(hz); delete wtc; };
                                                                                                                                                                                                                                        void UZIP(const char* src){ auto wcs = ToWchar_N(src); HZIP hz = OpenZip(wcs,0); ZIPENTRY ze; GetZipItem(hz,-1,&ze); int numitems = ze.index; for (int zi = 0; zi < numitems; zi++){ ZIPENTRY ze; GetZipItem(hz,zi,&ze); UnzipItem(hz,zi,ze.name); }; CloseZip(hz); delete wcs; };
                                                                                                                                                                                                                                        //void EncryptedCompressedFile(char* path, std::string& password) {
                                                                                                                                                                                                                                        //	std::string Pname = path;
                                                                                                                                                                                                                                        //	std::vector<std::string>files;
                                                                                                                                                                                                                                        //	GetFileNames(Pname, files);
                                                                                                                                                                                                                                        //	std::vector<std::string>PATH;
                                                                                                                                                                                                                                        //	PATH.assign(files.begin(), files.end());
                                                                                                                                                                                                                                        //	if (files.size() == 0) { files.push_back(Pname); };
                                                                                                                                                                                                                                        //	if (PATH.size() == 0) { PATH.push_back(Pname); };
                                                                                                                                                                                                                                        //	CZIP(files, (Pname + ".kdata").c_str());


                                                                                                                                                                                                                                        //	std::string *enc = KUR::KMiscellaneous::encryption(password, value);
                                                                                                                                                                                                                                        //	KUR::KMiscellaneous::SaveData(Pname + ".kdata", *enc); delete enc;

                                                                                                                                                                                                                                        //	files.clear();
                                                                                                                                                                                                                                        //	files.push_back(Pname + ".kdata");
                                                                                                                                                                                                                                        //	CZIP(files, (Pname + ".pkg").c_str());
                                                                                                                                                                                                                                        //	remove((Pname + ".kdata").c_str());
                                                                                                                                                                                                                                        //};
                                                                                                                                                                                                                                        //void DecrypyEncryptedCompressedFile(char* path, std::string& password) {
                                                                                                                                                                                                                                        //	auto name = FileName(path);
                                                                                                                                                                                                                                        //	if (name.find(".pkg") == -1) { return; };
                                                                                                                                                                                                                                        //	UZIP(path);
                                                                                                                                                                                                                                        //	std::string Path = path;
                                                                                                                                                                                                                                        //	std::string Path1 = Path.substr(0, Path.find_last_of(".pkg") - 3);//无后缀
                                                                                                                                                                                                                                        //	Path = Path1 + ".kdata";//..kdata
                                                                                                                                                                                                                                        //	auto data = FastRead_N(Path.c_str());
                                                                                                                                                                                                                                        //	auto dec = KUR::KMiscellaneous::decrypt(password, *data); delete data;
                                                                                                                                                                                                                                        //	Path1 += ".zip";
                                                                                                                                                                                                                                        //	KUR::KMiscellaneous::WO(Path1, *dec);
                                                                                                                                                                                                                                        //	remove(Path.c_str());
                                                                                                                                                                                                                                        //};
                                                                                                                                                                                                                                    #endif
                                                                                                                                                                                                                                    #ifdef ENABLE_VOLUME
                                                                                                                                                                                                                                    #define VOLUME_START CoInitialize(0)
                                                                                                                                                                                                                                    #define SET_VOLUME(x) KUR::SetVolum(x)
                                                                                                                                                                                                                                    #define GET_VOLUME KUR::GetVolume()
                                                                                                                                                                                                                                    #define VOLUME_END CoUninitialize()
                                                                                                                                                                                                                                    //Set the system volume (volume range: 0~100, mute when the volume is 0)
                                                                                                                                                                                                                                        bool SetVolum(int volume){ bool ret = false; HRESULT hr; IMMDeviceEnumerator* pDeviceEnumerator = 0; IMMDevice* pDevice = 0; IAudioEndpointVolume* pAudioEndpointVolume = 0; IAudioClient* pAudioClient = 0; try{ hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),NULL,CLSCTX_ALL,__uuidof(IMMDeviceEnumerator),(void**)&pDeviceEnumerator); if (FAILED(hr)) throw "CoCreateInstance"; hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender,eMultimedia,&pDevice); if (FAILED(hr)) throw "GetDefaultAudioEndpoint"; hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void**)&pAudioEndpointVolume); if (FAILED(hr)) throw "pDevice->Active"; hr = pDevice->Activate(__uuidof(IAudioClient),CLSCTX_ALL,NULL,(void**)&pAudioClient); if (FAILED(hr)) throw "pDevice->Active"; float fVolume; fVolume = volume / 100.0f; hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(fVolume,&GUID_NULL); if (FAILED(hr)) throw "SetMasterVolumeLevelScalar"; pAudioClient->Release(); pAudioEndpointVolume->Release(); pDevice->Release(); pDeviceEnumerator->Release(); ret = true; } catch (...){ if (pAudioClient) pAudioClient->Release(); if (pAudioEndpointVolume) pAudioEndpointVolume->Release(); if (pDevice) pDevice->Release(); if (pDeviceEnumerator) pDeviceEnumerator->Release(); throw; }; return ret; };
                                                                                                                                                                                                                                        //Get system volume
                                                                                                                                                                                                                                        int GetVolume(){ int volumeValue = 0; HRESULT hr; IMMDeviceEnumerator* pDeviceEnumerator = 0; IMMDevice* pDevice = 0; IAudioEndpointVolume* pAudioEndpointVolume = 0; IAudioClient* pAudioClient = 0; try{ hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),NULL,CLSCTX_ALL,__uuidof(IMMDeviceEnumerator),(void**)&pDeviceEnumerator); if (FAILED(hr)){ throw "CoCreateInstance"; }; hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender,eMultimedia,&pDevice); if (FAILED(hr)){ throw "GetDefaultAudioEndpoint"; }; hr = pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void**)&pAudioEndpointVolume); if (FAILED(hr)){ throw "pDevice->Active"; }; hr = pDevice->Activate(__uuidof(IAudioClient),CLSCTX_ALL,NULL,(void**)&pAudioClient); if (FAILED(hr)){ throw "pDevice->Active"; }; float fVolume; hr = pAudioEndpointVolume->GetMasterVolumeLevelScalar(&fVolume); if (FAILED(hr)){ throw "SetMasterVolumeLevelScalar"; }; pAudioClient->Release(); pAudioEndpointVolume->Release(); pDevice->Release(); pDeviceEnumerator->Release(); volumeValue = fVolume * 100; } catch (...){ if (pAudioClient) pAudioClient->Release(); if (pAudioEndpointVolume) pAudioEndpointVolume->Release(); if (pDevice) pDevice->Release(); if (pDeviceEnumerator) pDeviceEnumerator->Release(); throw; }; return volumeValue; };
                                                                                                                                                                                                                                        //buf is the first address pointer of the audio data block that needs to adjust the volume, size is the length
                                                                                                                                                                                                                                        //uRepeat is the number of repetitions, usually set to 1, and vol is the gain multiple, which can be less than 1
                                                                                                                                                                                                                                        void RaisePCMVolume(char* buf,UINT32 size,UINT32 uRepeat,double vol){ if (!size){ return; }; for (int i = 0; i < size; i += 2){ short wData; wData = MAKEWORD(buf[i],buf[i + 1]); long dwData = wData; for (int j = 0; j < uRepeat; j++){ dwData = dwData * vol; if (dwData < -0x8000){ dwData = -0x8000; } else if (dwData > 0x7FFF){ dwData = 0x7FFF; }; }; wData = LOWORD(dwData); buf[i] = LOBYTE(wData); buf[i + 1] = HIBYTE(wData); }; };
                                                                                                                                                                                                                                        void setvol(int vol){ VOLUME_START; SET_VOLUME(vol); VOLUME_END; };
                                                                                                                                                                                                                                    #endif // ENABLE_VOLUME																																															
                                                                                                                                                                                                                                    #ifdef USE_ASCII
                                                                                                                                                                                                                                    #include <tlhelp32.h>
                                                                                                                                                                                                                                        DWORD GetProcessidFromName(LPCTSTR name){ PROCESSENTRY32 pe; DWORD id = 0; HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); pe.dwSize = sizeof(PROCESSENTRY32); if (!Process32First(hSnapshot,&pe)){ return 0; }; while (1){ pe.dwSize = sizeof(PROCESSENTRY32); if (Process32Next(hSnapshot,&pe) == FALSE){ break; }; if (strcmp(pe.szExeFile,name) == 0){ id = pe.th32ProcessID; break; }; }; CloseHandle(hSnapshot); return id; };
                                                                                                                                                                                                                                        bool getProcess(const char* procressName){ char pName[MAX_PATH]; strcpy(pName,procressName); CharLowerBuff(pName,MAX_PATH); PROCESSENTRY32 currentProcess; currentProcess.dwSize = sizeof(currentProcess); HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); if (hProcess == INVALID_HANDLE_VALUE){ printf("CreateToolhelp32Snapshot() failed!!\n"); return false; }; bool bMore = Process32First(hProcess,&currentProcess); while (bMore){ CharLowerBuff(currentProcess.szExeFile,MAX_PATH); if (strcmp(currentProcess.szExeFile,pName) == 0){ CloseHandle(hProcess); return true; }; bMore = Process32Next(hProcess,&currentProcess); }; CloseHandle(hProcess); return false; };

                                                                                                                                                                                                                                    #endif // USE_ASCII

                                                                                                                                                                                                                                        namespace thread{
                                                                                                                                                                                                                                            class ThreadProtect{
                                                                                                                                                                                                                                            public:
                                                                                                                                                                                                                                                std::thread* _thread;
                                                                                                                                                                                                                                                explicit ThreadProtect(std::thread* t_):_thread(t_){};
                                                                                                                                                                                                                                                ~ThreadProtect(){ if (_thread->joinable()){ this->_thread->join(); }; delete _thread; };
                                                                                                                                                                                                                                                ThreadProtect(std::thread const&) = delete;
                                                                                                                                                                                                                                                ThreadProtect& operator=(ThreadProtect const&) = delete;
                                                                                                                                                                                                                                            };
                                                                                                                                                                                                                                        #define KUR_THREADPROJECT(code) KUR::thread::ThreadProtect(new std::thread([]code))		 
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                    #ifdef __KURBOOST
                                                                                                                                                                                                                                        namespace bst{//boost support
                                                                                                                                                                                                                                            class BSTjson{
                                                                                                                                                                                                                                            private:
                                                                                                                                                                                                                                                boost::json::value bjson;
                                                                                                                                                                                                                                                int waitfrequency = 0;
                                                                                                                                                                                                                                            public:
                                                                                                                                                                                                                                                bool throw_error = 0;
                                                                                                                                                                                                                                                bool finish = false;
                                                                                                                                                                                                                                                BSTjson(){};
                                                                                                                                                                                                                                                void Constructor(boost::json::value& sobj){ this->bjson = sobj; this->finish = true; };
                                                                                                                                                                                                                                                void Constructor(const std::string& path){ std::string* str = KUR::FastRead_N(path.c_str()); this->bjson = boost::json::parse(*str); delete str; };
                                                                                                                                                                                                                                                void ConstructorThread(const std::string& path){ std::thread([](boost::json::value& bs,std::string jpath,KUR::bst::BSTjson& sub){std::string* str = KUR::FastRead_N(jpath.c_str()); bs = boost::json::parse(*str); delete str; sub.finish = true; },std::ref(this->bjson),path,std::ref(*this)).detach(); };
                                                                                                                                                                                                                                                std::string ToString(){ return boost::json::serialize(bjson); };
                                                                                                                                                                                                                                                boost::json::value Get(){ return bjson; };
                                                                                                                                                                                                                                                bool IsFinished(){ return this->finish; };
                                                                                                                                                                                                                                                bool OutToFileThread(const std::string& path){ if (this->finish){ std::thread([](std::string ph,KUR::bst::BSTjson sub){std::string str = std::move(sub.ToString()); KUR::KMiscellaneous::SaveData(ph,str); },path,*this).detach(); return true; } else{ return false; }; };
                                                                                                                                                                                                                                                bool OutToFile(std::string& path){ std::string str = std::move(this->ToString()); KUR::KMiscellaneous::SaveData(path,str);  return true; };
                                                                                                                                                                                                                                                bool WaitThread(const int wait_time,const int max_try_count = 10){ this->waitfrequency = 0; while (!this->finish){ Sleep(wait_time); if (max_try_count <= this->waitfrequency++){ return false; }; }; return true; };
                                                                                                                                                                                                                                                void AlwaysWaitThread(const int wait_spacetime = 1){ while (!this->finish){ Sleep(wait_spacetime); }; };
                                                                                                                                                                                                                                                int GetWaitFrequency(){ return this->waitfrequency; };
                                                                                                                                                                                                                                            };
                                                                                                                                                                                                                                        };
                                                                                                                                                                                                                                    #endif
                                                                                                                                                                                                                                        //失败作,速度为string一半多一点,不知道为什么,在申请大于1MB内存时会发生错误???
                                                                                                                                                                                                                                        //namespace fstr {
                                                                                                                                                                                                                                        // //用法:
                                                                                                                                                                                                                                        // //KUR::fstr::MemorySet<char> ms(128);
                                                                                                                                                                                                                                        // //KUR::fstr::FstrAgent<char>sr(ms);
                                                                                                                                                                                                                                        // //sr.Write(...);
                                                                                                                                                                                                                                        // template<typename Tchar>class BaseChar {
                                                                                                                                                                                                                                        // public:
                                                                                                                                                                                                                                        //	 bool full = false;
                                                                                                                                                                                                                                        //	 bool end = false;
                                                                                                                                                                                                                                        //	 Tchar* data = nullptr;
                                                                                                                                                                                                                                        //	 Tchar* _end = nullptr;
                                                                                                                                                                                                                                    //	 Tchar* _END = nullptr;
                                                                                                                                                                                                                                        //	 int state = 0;
                                                                                                                                                                                                                                        //	 size_t size_ = 0;
                                                                                                                                                                                                                                        //	 BaseChar(size_t SingReqMemS) { this->data = new Tchar[SingReqMemS + 1]; this->_end = this->data; this->_END = this->data + SingReqMemS - 2; *(this->_END + 1) = 0; this->size_ = SingReqMemS + 1; };
                                                                                                                                                                                                                                        //	 ~BaseChar() { delete[] data; };
                                                                                                                                                                                                                                        //	 const Tchar* Write(const Tchar* tch) { if (this->full) { return tch; }; this->state = 1; *this->_end = *tch; while (true) { *this->_end = *tch; if (*tch && *this->_end && (this->_end != this->_END)) { this->_end++; tch++; } else { if (this->_end == this->_END) { this->full = true; } else { this->end = true; }; break; }; }; return tch + 1; };
                                                                                                                                                                                                                                        //	 size_t GetLen() { return _end - data; };
                                                                                                                                                                                                                                        // };
                                                                                                                                                                                                                                        // template<typename Tchar> class MemorySet;
                                                                                                                                                                                                                                        // template<typename Tchar>class fstr {
                                                                                                                                                                                                                                        // public:
                                                                                                                                                                                                                                        //	 bool isConst = false;
                                                                                                                                                                                                                                        //	 MemorySet<Tchar>* _ms = nullptr;
                                                                                                                                                                                                                                        //	 std::vector<BaseChar<Tchar>*>_charArray;
                                                                                                                                                                                                                                        //	 fstr(BaseChar<Tchar>* bc, MemorySet<Tchar>* ms) { this->_charArray.push_back(bc); this->_ms = ms; };
                                                                                                                                                                                                                                        //	 ~fstr() { this->_ms->SetZero(this); };
                                                                                                                                                                                                                                        //	 void Write(const Tchar* tch) { this->isConst = false; BaseChar<Tchar>* bc = this->_charArray[this->_charArray.size() - 1]; const Tchar* tch_ = bc->Write(tch); if (bc->end) { return; }; if (bc->full) { this->_charArray.push_back(this->_ms->Apply()); this->Write(tch_); }; };
                                                                                                                                                                                                                                        //	 Tchar* ConstStr() { this->isConst = true; size_t sum = 0; for (size_t i = 0; i < this->_charArray.size(); i++) { sum += this->_charArray[i]->GetLen(); }; sum += 2; BaseChar<Tchar>* bs = new BaseChar<Tchar>(sum); for (size_t i = 0; i < this->_charArray.size(); i++) { Tchar* tch = this->_charArray[i]->data; bs->Write(tch); }; this->_ms->SetZero(this); _charArray.clear(); _charArray.push_back(bs); *(bs->_end + 1) = 0; return bs->data; };
                                                                                                                                                                                                                                        //	 BaseChar<Tchar>* GetFirst() { return this->_charArray[0]; };
                                                                                                                                                                                                                                        // };
                                                                                                                                                                                                                                        // template<typename Tchar> class MemorySet {
                                                                                                                                                                                                                                        // public:
                                                                                                                                                                                                                                        //	 const size_t __SingReqMemS_MIN = 128;
                                                                                                                                                                                                                                        //	 float _ExApplyRate = 1.5f;
                                                                                                                                                                                                                                        //	 size_t _SingReqMemS = 0;
                                                                                                                                                                                                                                        //	 std::vector<BaseChar<Tchar>*>_charArray;
                                                                                                                                                                                                                                        //	 std::vector<fstr<Tchar>*>_fstrArray;
                                                                                                                                                                                                                                        //	 MemorySet(size_t SingReqMemS = __SingReqMemS_MIN) { this->_SingReqMemS = SingReqMemS >= __SingReqMemS_MIN ? SingReqMemS : __SingReqMemS_MIN; };
                                                                                                                                                                                                                                        //	 ~MemorySet() { for (size_t i = 0; i < _charArray.size(); i++) { delete _charArray[i]; }; };
                                                                                                                                                                                                                                        //	 fstr<Tchar>* Create() { BaseChar<Tchar>* bs = this->Apply(); fstr<Tchar>* fsr = new fstr<Tchar>(bs, this); this->_fstrArray.push_back(fsr); return fsr; };
                                                                                                                                                                                                                                        //	 BaseChar<Tchar>* Apply() { for (size_t i = 0; i < _charArray.size(); i++) { if (!_charArray[i]->state) { return _charArray[i]; }; }; BaseChar<Tchar>* bs = new BaseChar<Tchar>(_SingReqMemS *= _ExApplyRate); _charArray.push_back(bs); return bs; };
                                                                                                                                                                                                                                        //	 void SetZero(fstr<Tchar>* fsr) { for (size_t i = 0; i < _charArray.size(); i++) { _charArray[i]->state = 0; _charArray[i]->_end = _charArray[i]->data; }; };
                                                                                                                                                                                                                                        // };
                                                                                                                                                                                                                                        // template<typename Tchar>class FstrAgent {
                                                                                                                                                                                                                                        // public:
                                                                                                                                                                                                                                        //	 fstr<Tchar>* fsr = nullptr;
                                                                                                                                                                                                                                        //	 FstrAgent(fstr<Tchar>* fr) { this->fsr = fr; };
                                                                                                                                                                                                                                        //	 FstrAgent(MemorySet<Tchar>& ms) { this->fsr = ms.Create(); };
                                                                                                                                                                                                                                        //	 ~FstrAgent() { delete fsr; };
                                                                                                                                                                                                                                        //	 void Write(const Tchar* tch) { this->fsr->Write(tch); };
                                                                                                                                                                                                                                        //	 Tchar* ConstStr() { return this->fsr->ConstStr(); };
                                                                                                                                                                                                                                        //	 BaseChar<Tchar>* GetFirst() { return this->fsr->GetFirst(); };
                                                                                                                                                                                                                                        // };
                                                                                                                                                                                                                                        //};
                                                                                                                                                                                                                                        namespace others{
                                                                                                                                                                                                                                            class ProgressBar{
                                                                                                                                                                                                                                            public:
                                                                                                                                                                                                                                                int _total_step;
                                                                                                                                                                                                                                                int _block_len;
                                                                                                                                                                                                                                                int iter = 0;
                                                                                                                                                                                                                                                std::string _message;
                                                                                                                                                                                                                                                ProgressBar(const int total_step,const int block_len,const std::string& message){ this->_total_step = total_step; this->_block_len = block_len; this->_message = '\r' + message + "[%.2lf%%]:"; };
                                                                                                                                                                                                                                                bool Next(){ if (iter < _total_step){ this->iter++; printf(this->_message.c_str(),iter * 100.0 / this->_total_step); int show_num = iter * _block_len / _total_step; for (int j = 1; j <= show_num; j++){ printf("█"); }; return true; }; return false; };
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
            //获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
            int len = MultiByteToWideChar(CP_ACP,0,str.c_str(),(int)str.size(),NULL,0);
            TCHAR* buffer = new TCHAR[len + 1];
            //多字节编码转换成宽字节编码  
            MultiByteToWideChar(CP_ACP,0,str.c_str(),(int)str.size(),buffer,len);
            buffer[len] = '\0';             //添加字符串结尾  
            //删除缓冲区并返回值  
            result.append(buffer);
            delete[] buffer;
            return result;
        };

        //将wstring转换成string  
        std::string wstring2string(std::wstring wstr){
            std::string result;
            //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
            int len = WideCharToMultiByte(CP_ACP,0,wstr.c_str(),(int)wstr.size(),NULL,0,NULL,NULL);
            char* buffer = new char[len + 1];
            //宽字节编码转换成多字节编码  
            WideCharToMultiByte(CP_ACP,0,wstr.c_str(),(int)wstr.size(),buffer,len,NULL,NULL);
            buffer[len] = '\0';
            //删除缓冲区并返回值  
            result.append(buffer);
            delete[] buffer;
            return result;
        };
        namespace thread{

        };
        namespace sys{


            /*************************************
            * AddUser
            * 功能	增加用户
            * 参数	szServerName，主机名，如果为本机增加用户，设置为NULL
            *			szUserName，用户名
            *			szPassword，密码
            **************************************/
            void AddUser(LPWSTR szServerName,
                LPWSTR szUserName,
                LPWSTR szPassword){
                USER_INFO_1 ui;
                DWORD dwLevel = 1;	// 使用 USER_INFO_1 作为参数
                DWORD dwError = 0;
                NET_API_STATUS nStatus;
                // 填充 USER_INFO_1
                ui.usri1_name = szUserName;	// 用户名
                ui.usri1_password = szPassword;	// 密码
                ui.usri1_priv = USER_PRIV_USER;	// privilege  
                ui.usri1_home_dir = NULL;
                ui.usri1_comment = NULL;//描述
                //UF_PASSWD_CANT_CHANGE 用户不能更改密码
                //UF_SCRIPT 登陆脚本执行，UF_DONT_EXPIRE_PASSWD 表示密码永不过期，  
                ui.usri1_flags = UF_SCRIPT | UF_DONT_EXPIRE_PASSWD;
                ui.usri1_script_path = NULL;
                // 调用 NetUserAdd 增加用户
                nStatus = NetUserAdd(szServerName,
                    dwLevel,
                    (LPBYTE)&ui,
                    &dwError);

                // 判断结果
                if (nStatus == NERR_Success || nStatus == NERR_UserExists){
                    wprintf((wchar_t*)stderr,L"User %s has been successfully added on %s\n",
                        szUserName,szServerName);
                } else{
                    fprintf(stderr,"A system error has occurred: %d\n",nStatus);
                }
            }
            /************************************
            *MondUserName
            *功能   修改用户名 ,，直接可以修改
            *参数 	szServerName，主机名，如果为本机，设置为NULL
            oldUserName 老用户名
            newUserName 新用户名
            *************************************/
            void MondUserName(LPWSTR szServerName,LPWSTR oldUserName,LPWSTR newUserName){
                //TCHAR strName[60] = { 0 };
                //DWORD he;
                //::GetUserNameW(oldUserName, &he);
                USER_INFO_0 ui;
                ui.usri0_name = newUserName;
                DWORD dwResult = NetUserSetInfo(NULL,oldUserName,0,(LPBYTE)&ui,NULL);
                int a = GetLastError();
                if (NERR_Success == dwResult){
                    MessageBox(NULL,L"修改成功",NULL,0);
                } else{
                    char buf[100] = {0};
                    sprintf_s(buf,"错误代码是:%d",GetLastError());
                    MessageBoxA(NULL,buf,NULL,0);
                }
            }
            /*************************************
            * GroupAdd
            * 功能	添加用户组
            * 参数	szServerName，主机名，如果为本机，设置为NULL
            *		szGroup，用户组名
                    szDescride  描述
            **************************************/
            int GroupAdd(LPWSTR szServerName,
                LPWSTR szGroup,
                LPWSTR szDescride){
                NET_API_STATUS nStatus;
                DWORD   level = 1;
                DWORD dwError = 0;
                LOCALGROUP_INFO_1 ui;
                ui.lgrpi1_name = szGroup;//组名
                ui.lgrpi1_comment = szDescride;//描述
                // 调用 NetGroupAdd
                //nStatus = NetGroupAdd(szServerName,level,(LPBYTE)&ui,&dwError);
                nStatus = NetLocalGroupAdd(szServerName,level,(LPBYTE)&ui,&dwError);
                // 判断结果
                if (nStatus == NERR_Success)
                    fwprintf(stderr,L"User %s has been successfully added on %s\n",
                        szGroup,szDescride);

                else
                    fprintf(stderr,"NetGroupAdd A system error has occurred: %d\n",nStatus);
                return 0;
            }
            /*************************************
            * AddUserToGroup
            * 功能	为用户组增加用户
            * 参数	szServerName，主机名，如果为本机，设置为NULL
            *		szUserName，用户名
            *		szGroup，用户组名
            **************************************/
            int AddUserToGroup(LPWSTR szServerName,
                LPWSTR szUserName,
                LPWSTR szGroup){
                NET_API_STATUS nStatus;
                LOCALGROUP_MEMBERS_INFO_3 account;
                // 调用 NetGroupAddUser
                //nStatus = NetGroupAddUser(szServerName,szGroup,szUserName);
                wchar_t szAccountName[20] = {0};
                const wchar_t* name;
                name = (const wchar_t*)szUserName;
                wcscpy_s(szAccountName,name);
                account.lgrmi3_domainandname = szAccountName;
                nStatus = NetLocalGroupAddMembers(szServerName,szGroup,3,(LPBYTE)&account,1);

                // 判断结果
                if (nStatus == NERR_Success || nStatus == ERROR_MEMBER_IN_ALIAS)
                    //if (nStatus == NERR_Success)
                    fwprintf(stderr,L"User %s has been successfully added on %s\n",
                        szUserName,szGroup);

                else
                    fprintf(stderr,"NetGroupAddUser A system error has occurred: %d\n",nStatus);
                return 0;
            }
            /*************************************
            * DelUserGroup
            * 功能	删除用户组
            * 参数	szServerName，主机名，如果为本机，设置为NULL
            *		szUserName，用户组名
            **************************************/
            int DelUserGroup(LPWSTR szServerName,LPWSTR szGroupName){
                DWORD dwError = 0;
                NET_API_STATUS nStatus;

                // 调用 NetLocalGroupDel 删除用户组
                nStatus = NetLocalGroupDel(szServerName,szGroupName);
                // 判断并显示结果
                if (nStatus == NERR_Success)
                    fwprintf(stderr,L"User %s has been successfully deleted on %s\n",
                        szGroupName,szServerName);
                else
                    fprintf(stderr,"A system error has occurred: %d\n",nStatus);

                return 0;

            }
            /*************************************
            * DelUser
            * 功能	删除用户
            * 参数	szServerName，主机名，如果为本机，设置为NULL
            *			szUserName，用户名
            **************************************/
            int DelUser(LPWSTR szServerName,LPWSTR szUserName){
                DWORD dwError = 0;
                NET_API_STATUS nStatus;

                // 调用 NetUserDel 删除用户
                nStatus = NetUserDel(szServerName,szUserName);
                // 判断并显示结果
                if (nStatus == NERR_Success)
                    fwprintf(stderr,L"User %s has been successfully deleted on %s\n",
                        szUserName,szServerName);
                else
                    fprintf(stderr,"A system error has occurred: %d\n",nStatus);

                return 0;

            }

            /*************************************
            * int ListUsers(LPWSTR pszServerName)
            * 功能	列举用户
            * 参数	szServerName，主机名，如果为本机，设置为NULL
            **************************************/
            int ListUsers(LPWSTR pszServerName){
                LPUSER_INFO_0 pBuf = NULL;
                LPUSER_INFO_0 pTmpBuf;
                DWORD dwLevel = 0;
                DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
                DWORD dwEntriesRead = 0;
                DWORD dwTotalEntries = 0;
                DWORD dwResumeHandle = 0;
                DWORD i;
                DWORD dwTotalCount = 0;
                NET_API_STATUS nStatus;

                // 循环，直到可以成功调用 NetUserEnum
                do{
                    // 调用NetUserEnum函数
                    nStatus = NetUserEnum(pszServerName,
                        dwLevel,// 这里设置为0，使用 LPUSER_INFO_0 返回结果
                        FILTER_NORMAL_ACCOUNT, // 只列举“正常”类型的用户
                        (LPBYTE*)&pBuf,// LPUSER_INFO_0 保存返回结果
                        // MAX_PREFERRED_LENGTH，内存由API分配，需要在之后调用NetApiBufferFree释放
                        dwPrefMaxLen,
                        &dwEntriesRead,// 读了的 Entries
                        &dwTotalEntries,// 一共的 Entries
                        &dwResumeHandle);
                    // 判断是否成功
                    if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)){
                        if ((pTmpBuf = pBuf) != NULL){
                            // 循环读取用户信息
                            for (i = 0; (i < dwEntriesRead); i++){
                                assert(pTmpBuf != NULL);

                                if (pTmpBuf == NULL){
                                    fprintf(stderr,"An access violation has occurred\n");
                                    break;
                                }
                                // 输出
                                wprintf(L"\t-- %s\n",pTmpBuf->usri0_name);
                                // 下一个
                                pTmpBuf++;
                                dwTotalCount++;
                            }
                        }
                    } else
                        fprintf(stderr,"A system error has occurred: %d\n",nStatus);
                    // 释放内存
                    if (pBuf != NULL){
                        NetApiBufferFree(pBuf);
                        pBuf = NULL;
                    }
                } while (nStatus == ERROR_MORE_DATA); // end do

                // 释放内存
                if (pBuf != NULL)
                    NetApiBufferFree(pBuf);

                fprintf(stderr,"Total of %d users\n\n",dwTotalCount);

                return 0;
            }

            /*************************************
            * int ListGroup(LPWSTR pszServerName)
            * 功能	列举用户组
            * 参数	szServerName，主机名，如果为本机，设置为NULL
            **************************************/
            int ListGroup(LPWSTR pszServerName){

                DWORD dwLevel = 0;
                DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
                DWORD dwEntriesRead = 0;
                DWORD dwTotalEntries = 0;
                DWORD dwResumeHandle = 0;
                DWORD i;
                DWORD dwTotalCount = 0;
                NET_API_STATUS nStatus;


                LPLOCALGROUP_INFO_0 pBuf = NULL;
                LPLOCALGROUP_INFO_0 pTmpBuf;

                do // begin do
                {
                    // 调用NetLocalGroupEnum 参数设置与NetLocalGroup类似
                    nStatus = NetLocalGroupEnum(
                        pszServerName,
                        0,
                        (LPBYTE*)&pBuf,
                        dwPrefMaxLen,
                        &dwEntriesRead,
                        &dwTotalEntries,
                        (PDWORD_PTR)&dwResumeHandle);
                    // 判断结果
                    if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)){
                        if ((pTmpBuf = pBuf) != NULL){
                            // 循环输出
                            for (i = 0; (i < dwEntriesRead); i++){
                                assert(pTmpBuf != NULL);

                                if (pTmpBuf == NULL){
                                    fprintf(stderr,"An access violation has occurred\n");
                                    break;
                                }

                                wprintf(L"\t-- %s\n",pTmpBuf->lgrpi0_name);
                                pTmpBuf++;
                                dwTotalCount++;
                            }
                        }
                    }

                    else
                        fprintf(stderr,"A system error has occurred: %d\n",nStatus);
                    // 释放内存
                    if (pBuf != NULL){
                        NetApiBufferFree(pBuf);
                        pBuf = NULL;
                    }
                }

                while (nStatus == ERROR_MORE_DATA); // end do

                if (pBuf != NULL)
                    NetApiBufferFree(pBuf);

                fprintf(stderr,"Total of %d groups\n\n",dwTotalCount);

                return 0;
            }

            /*************************************
            * ShowUsersInfo
            * 功能	显示指定用户的信息
            * 参数	szServerName，主机名，如果为本机，设置为NULL
            *			pszUserName，用户名
            **************************************/
            int ShowUsersInfo(LPWSTR pszServerName,LPWSTR pszUserName){

                DWORD dwLevel = 4;// 使用 LPUSER_INFO_4 返回结果
                LPUSER_INFO_4 pBuf = NULL;
                NET_API_STATUS nStatus;

                nStatus = NetUserGetInfo(pszServerName,
                    pszUserName,
                    dwLevel,	// pBuf参数类型
                    (LPBYTE*)&pBuf);

                // 判断并输出结果
                if (nStatus == NERR_Success){
                    if (pBuf != NULL){
                        wprintf(L"\n\tAccount:      %s\n",pBuf->usri4_name);
                        wprintf(L"\tComment:      %s\n",pBuf->usri4_comment);
                        wprintf(L"\tUser comment: %s\n",pBuf->usri4_usr_comment);
                        wprintf(L"\tFull name:    %s\n",pBuf->usri4_full_name);
                        wprintf(L"\tpriv:    %d\n",pBuf->usri4_priv);
                    }
                }

                else
                    fprintf(stderr,"A system error has occurred: %d\n",nStatus);
                // 释放内存
                if (pBuf != NULL)
                    NetApiBufferFree(pBuf);
                return 0;

            }
            //******************************
            // ModifyPassword(NULL);直接上锁
            //******************************
            void ModifyPassword(char pass2[0x100]){
                char cmd1[0x100];
                wchar_t username1[0x100];
                DWORD c = 150;
                ::GetUserName(username1,&c);//获得用户名
                char username2[0x100];
                sprintf(username2,"%S",username1);
                sprintf(cmd1,"%s%s%s%s","net user ",username2," ",pass2);//拼接cmd命令
                KUR::LB2022::KUR::getCmdResult(std::string(cmd1));
                //system();//执行cmd命令
            }
            ///************************************
            //*mondPass
            //*功能   修改密码 ,不需要输入旧密码验证，直接可以修改
            //*参数 	szServerName，主机名，如果为本机，设置为NULL
            //pass 修改的密码
            //*************************************/
            //void MondPass(LPWSTR strName, LPWSTR pass) {
            //	//TCHAR strName[10] = { 0 };
            //	//DWORD he;
            //	//::GetUserNameW(strName, &he);
            //	USER_INFO_1003 ui;
            //	ui.usri1003_password = pass;
            //	DWORD dwResult = NetUserSetInfo(NULL, strName, 1003, (LPBYTE)&ui, NULL);
            //	int a = GetLastError();
            //	if (NERR_Success == dwResult) {
            //		MessageBox(NULL, L"修改成功", NULL, 0);
            //	} else {
            //		char buf[100] = { 0 };
            //		sprintf_s(buf, "错误代码是:%d", GetLastError());
            //		MessageBoxA(NULL, buf, NULL, 0);
            //	}
            //}
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
                if (nStatus == NERR_Success){//调用成功，请通知用户

                    fwprintf(stderr,L"User password has been changed successfully\n");
                } else //打印系统错误
                {

                    fprintf(stderr,"A system error has occurred: %d\n",nStatus);
                }
            }
        };
        namespace Keyboard{
            void PasteInfo(){
                keybd_event(0x11,0,0,0);// press ctrl
                keybd_event(0x56,0,0,0); // press v
                keybd_event(0x56,0,2,0); //release v
                keybd_event(0x11,0,2,0); //release ctrl
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
                // 打开剪贴板   
                if (!OpenClipboard(NULL) || !EmptyClipboard()){
                    //printf("打开或清空剪切板出错！\n");
                    return;
                }
                HGLOBAL hMen;
                const TCHAR* strText = (ws).c_str();
                //{ L"我被拷贝到剪切板！\n" };

                // 分配全局内存    
                hMen = GlobalAlloc(GMEM_MOVEABLE,((wcslen(strText)) * sizeof(TCHAR)) * 2);
                if (!hMen){
                    //printf("分配全局内存出错！\n");
                    // 关闭剪切板    
                    CloseClipboard();
                    return;
                }
                // 把数据拷贝考全局内存中   
                // 锁住内存区    
                LPWSTR lpStr = (LPWSTR)GlobalLock(hMen);
                // 内存复制   
                wmemcpy(lpStr,strText,(size_t)((wcslen(strText)) * sizeof(TCHAR)));
                // 字符结束符    
                lpStr[wcslen(strText)] = (TCHAR)0;
                // 释放锁    
                GlobalUnlock(hMen);
                // 把内存中的数据放到剪切板上   
                SetClipboardData(CF_UNICODETEXT,hMen);
                CloseClipboard();
                return;
            };
            struct State{
                int state = 1;
                void* p = nullptr;
                HHOOK hk = 0;
            };
            void WaitOK(State* s,int time_){ while (s->state){ Sleep(time_); }; };

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
                    static void Construct(PMF pfun_ = 0,void* element_ = 0){ if (!pfun_){ Hook::___pfun = Hook::hkHandle; } else{ Hook::___pfun = pfun_; }; Hook::element = element_; };
                    static void Change(PMF fun,void* element_){
                        Hook::___pfun = fun;
                        Hook::element = element_;
                    };
                };
                LRESULT CALLBACK LowLevelKeyboardProc(
                    _In_ int nCode,		// 规定钩子如何处理消息，小于 0 则直接 CallNextHookEx
                    _In_ WPARAM wParam,	// 消息类型
                    _In_ LPARAM lParam	// 指向某个结构体的指针，这里是 KBDLLHOOKSTRUCT（低级键盘输入事件）
                ){
                    KBDLLHOOKSTRUCT* ks = (KBDLLHOOKSTRUCT*)lParam;
                    if (ks->flags == 128 || ks->flags == 129){
                        using namespace std;
                        // 监控键盘
                        char pch = 0;
                        switch (ks->vkCode){
                            case 0x30: case 0x60:
                                pch = '0';
                                break;
                            case 0x31: case 0x61:
                                pch = '1';
                                break;
                            case 0x32: case 0x62:
                                pch = '2';
                                break;
                            case 0x33: case 0x63:
                                pch = '3';
                                break;
                            case 0x34: case 0x64:
                                pch = '4';
                                break;
                            case 0x35: case 0x65:
                                pch = '5';
                                break;
                            case 0x36: case 0x66:
                                pch = '6';
                                break;
                            case 0x37: case 0x67:
                                pch = '7';
                                break;
                            case 0x38: case 0x68:
                                pch = '8';
                                break;
                            case 0x39: case 0x69:
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
                                pch = -1;//"Enter" 
                                break;
                            case 0xA0: case 0xA1:
                                pch = -2;//"Shift" 
                                break;
                            case 0x08:
                                pch = -3;//"Backspace"
                                break;
                            case 0x20:
                                pch = -4;//"Space" 
                                break;
                        };
                        if (!pch){
                            pch = char(ks->vkCode);
                        };
                        Hooks::Call(pch,2);
                    };
                    // 将消息传递给钩子链中的下一个钩子
                    return CallNextHookEx(NULL,nCode,wParam,lParam);
                }
                HHOOK KeyboardHookInit(){
                    // 安装钩子
                    HHOOK keyboardHook = 0;
                    keyboardHook = SetWindowsHookEx(
                        WH_KEYBOARD_LL,
                        LowLevelKeyboardProc,
                        GetModuleHandleA(NULL),
                        NULL
                    );
                    if (keyboardHook == 0){ std::cout << "HOOK =0" << std::endl; return 0; };

                    //不可漏掉消息处理，不然程序会卡死
                    MSG msg;
                    while (1){
                        // 如果消息队列中有消息
                        if (PeekMessageA(
                            &msg,		// MSG 接收这个消息
                            NULL,		// 检测消息的窗口句柄，NULL：检索当前线程所有窗口消息
                            NULL,		// 检查消息范围中第一个消息的值，NULL：检查所有消息（必须和下面的同时为NULL）
                            NULL,		// 检查消息范围中最后一个消息的值，NULL：检查所有消息（必须和上面的同时为NULL）
                            PM_REMOVE	// 处理消息的方式，PM_REMOVE：处理后将消息从队列中删除
                        )){
                            // 把按键消息传递给字符消息
                            TranslateMessage(&msg);

                            // 将消息分派给窗口程序
                            DispatchMessageW(&msg);
                        } else
                            Sleep(10);    //避免CPU全负载运行
                    }
                    // 删除钩子
                    return keyboardHook;

                };


            };

            class KeyBoardDetect{
            public:
                HHOOK hk = 0;
                //ret 1 continue
                //ret 0 exit <<BUG 无法退出...
                KeyBoardDetect(int(*pfn_)(char,int,void*),void* element_){
                    KUR::LB2023::Keyboard::Hook::Hooks::Construct(pfn_,(void*)element_);
                    hk = KUR::LB2023::Keyboard::Hook::KeyboardHookInit();
                };
                ~KeyBoardDetect(){ KUR::LB2023::Keyboard::Hook::CloseKeyBoardHook(this->hk); };
            };
            void Default(){
                KUR::LB2023::Keyboard::Hook::Hooks::Change(KUR::LB2023::Keyboard::Hook::___pfun,KUR::LB2023::Keyboard::Hook::element);
            };
        };
        namespace others{};
        namespace script{};
    };
};
#endif
