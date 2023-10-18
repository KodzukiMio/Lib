//2023-2024
/*
199711L - C++98 | C++03
201103L - C++11
201402L - C++14
201703L - C++17
202002L - C++20
*/
#pragma once
#if __cplusplus < 201703L
#error need >= C++17
//����汾ȷʵ>=c++17,��ô���ڱ��������м���:/Zc:__cplusplus
//���磺cl /EHsc /Zc:__cplusplus main.cpp 
#endif
#ifndef _kurzerbase_
#define _kurzerbase_
#endif
/* #define options:
*  (�׳��쳣) KURZER_ENABLE_EXCEPTIONS
*
*/
#ifdef NDEBUG //release

#else //debug
#define KURZER_ENABLE_EXCEPTIONS
#endif // NDEBUG

#ifdef KURZER_ENABLE_EXCEPTIONS
#include<stdexcept>
#endif // KURZER_ENABLE_EXCEPTIONS

namespace KUR{
    namespace base{
        //size_t
        typedef unsigned long long ull;
        template<typename Tchar> inline static ull strlen(const Tchar* str){
            const Tchar* p = str;
            while (*p){
                ++p;
            };
            return p - str;
        };
        template<class Tchar> class CharT{
        public:
        #ifdef _XSTRING_
            using char_t = std::basic_string<Tchar,std::char_traits<Tchar>,std::allocator<Tchar>>;
        #endif // _XSTRING_
        #ifdef _IOSTREAM_
            using out_t = std::basic_ostream<Tchar,std::char_traits<Tchar>>;
        #endif // _IOSTREAM_
        };
        //typename base::enable_if<TRUE?>::type* = nullptr
        template<bool condition,class T = void>struct enable_if{};
        template<class T>struct enable_if<true,T>{ typedef T type; };
        //is_same<T1,T2>::value
        template <typename T,typename U>struct is_same{
            static constexpr bool value = false;
        };
        template <typename T>struct is_same<T,T>{
            static constexpr bool value = true;
        };
        template<typename T>struct is_character{
            static constexpr bool value = base::is_same<T,char>::value || base::is_same<T,wchar_t>::value || base::is_same<T,char16_t>::value || base::is_same<T,char32_t>::value;
        };
        template<typename _T0,typename _T1>using _Is_Same = typename base::enable_if<base::is_same<_T0,_T1>::value>::type*;

        //is_lvalue_reference<T>::value
        template<typename T>struct is_lvalue_reference{
            static const bool value = false;
        };
        template<typename T>struct is_lvalue_reference<T&>{
            static const bool value = true;
        };
        //is_rvalue_reference<T>::value
        template<typename T>struct is_rvalue_reference{
            static const bool value = false;
        };
        template<typename T>struct is_rvalue_reference<T&&>{
            static const bool value = true;
        };
        //remove_reference<T>::type
        template<typename T> struct remove_reference{ typedef T type; };
        template<typename T> struct remove_reference<T&>{ typedef T type; };
        template<typename T> struct remove_reference<T&&>{ typedef T type; };
        //move
        template <typename T>typename base::remove_reference<T>::type&& move(T&& _Ty){
            return static_cast<typename base::remove_reference<T>::type&&>(_Ty);
        }
        //forward<_Tp>(...);
        template<typename _Tp>constexpr _Tp&& forward(typename base::remove_reference<_Tp>::type& _Arg)noexcept{//left value
            return static_cast<_Tp&&>(_Arg);
        }
        template<typename _Tp>constexpr _Tp&& forward(typename base::remove_reference<_Tp>::type&& _Arg)noexcept{//right value
            static_assert(!base::is_lvalue_reference<_Tp>::value,"_Arg is not a right value.");
            return static_cast<_Tp&&>(_Arg);
        };
        template <typename _Ty>inline void swap(_Ty& _Left,_Ty& _Right){
            _Ty _Tmp = base::move(_Left);
            _Left = base::move(_Right);
            _Right = base::move(_Tmp);
        };

        template<typename _Tp,typename _CmpPfn2Args>inline void sort_bubble(_Tp* _Begin,_Tp* _End,_CmpPfn2Args _CmpPfn){//[_Begin,_End)    _Cmpfn(T*,T*)
            size_t n = _End - _Begin;
            for (size_t i = 0; i < n; ++i){
                bool _Flag = true;
                for (size_t j = 0; j < n - i - 1; ++j){
                    if (_CmpPfn(_Begin + j + 1,_Begin + j)){
                        auto _Tmp = _Begin[j];
                        _Begin[j] = _Begin[j + 1];
                        _Begin[j + 1] = _Tmp;
                        _Flag = false;
                    }
                }
                if (_Flag){
                    break;
                }
            }
        };
        template<typename _Tp,typename _CmpPfn2Args>inline void sort_select(_Tp* _Begin,_Tp* _End,_CmpPfn2Args _CmpPfn){//[_Begin,_End)    _Cmpfn(T*,T*)
            _Tp* _Tmp0 = _Begin;
            while (_Tmp0 < _End){
                _Tp* _Tmp1 = _Tmp0;
                _Tp* _Min_ = _Tmp1;
                while (_Tmp1 < _End){
                    if (_CmpPfn(_Min_,_Tmp1)){
                        _Min_ = _Tmp1;
                    }
                    ++_Tmp1;
                }
                _Tp _Tmp = *_Min_;
                *_Min_ = *_Tmp0;
                *_Tmp0 = _Tmp;
                ++_Tmp0;
            }
        }
        template<typename _Tp,typename _CmpPfn2Args>inline void sort_insert(_Tp* _Begin,_Tp* _End,_CmpPfn2Args _CmpPfn){//[_Begin,_End)    _Cmpfn(T*,T*)
            for (_Tp* _Sorted = _Begin + 1; _Sorted < _End; ++_Sorted){
                _Tp _Key = *_Sorted;
                _Tp* _Pos = _Sorted - 1;
                while (_Pos >= _Begin && _CmpPfn(&_Key,_Pos)){
                    *(_Pos + 1) = *_Pos;
                    --_Pos;
                }
                *(_Pos + 1) = _Key;
            }
        };
        template<typename _Tp,typename _CmpPfn2Args>inline void _merge(_Tp* _Begin,_Tp* _Middle,_Tp* _End,_CmpPfn2Args _CmpPfn,_Tp* _Tmp){
            auto _Beg = _Begin;
            auto _Pos = _Middle;
            _Tp* _Tp_ = _Tmp;
            while (_Beg < _Middle && _Pos < _End){
                if (_CmpPfn(_Beg,_Pos)){
                    *_Tp_++ = *_Beg++;
                } else{
                    *_Tp_++ = *_Pos++;
                }
            }
            while (_Beg < _Middle)*_Tp_++ = *_Beg++;
            while (_Pos < _End)*_Tp_++ = *_Pos++;
            while (_Begin < _End)*_Begin++ = *_Tmp++;
        };

        template<typename _Tp,typename _CmpPfn2Args>inline void _sort_merge(_Tp* _Begin,_Tp* _End,_CmpPfn2Args _CmpPfn,_Tp* _Tmp){
            if (_End - _Begin > 1){
                auto _Mid = _Begin + ((_End - _Begin) >> 1);
                _sort_merge(_Begin,_Mid,_CmpPfn,_Tmp);
                _sort_merge(_Mid,_End,_CmpPfn,_Tmp);
                _merge(_Begin,_Mid,_End,_CmpPfn,_Tmp);
            }
        };

        template<typename _Tp,typename _CmpPfn2Args>inline void sort_merge(_Tp* _Begin,_Tp* _End,_CmpPfn2Args _CmpPfn,_Tp* _Tmp = nullptr){//[_Begin,_End),_Cmpfn(T*,T*),_Tmp����Ϊ����ĳ���.
            if (!_Tmp){
                _Tmp = new _Tp[_End - _Begin];
            };
            if (_Tmp){
                _sort_merge(_Begin,_End,_CmpPfn,_Tmp);
                delete[] _Tmp;
                return;
            }
        #ifdef KURZER_ENABLE_EXCEPTIONS
            throw std::bad_alloc();
        #endif // KURZER_ENABLE_EXCEPTIONS
        };
        //_KUR_TEMPLATE_TYPE_IS(T1,T2)
    #define _KUR_TEMPLATE_TYPE_IS(Type,Is_Ty)template <typename Type,typename base::enable_if<base::is_same<Type,Is_Ty>::value>::type* = nullptr>
    //_KUR_TEMPLATE_T_IS(T2)
    #define _KUR_TEMPLATE_T_IS(Is_Ty) _KUR_TEMPLATE_TYPE_IS(T,Is_Ty) 
        template<typename Type>class Array{
        public:
            Type* _chunk = 0;
            ull _size = 0;
            ull _pos = 0;
            inline void expand(){
                Type* temp = _chunk;
                ull _esize = (_size >> 1) + _size;
                this->_chunk = Malloc(_esize);
            #ifdef KURZER_ENABLE_EXCEPTIONS
                if (!this->_chunk){
                    throw std::runtime_error("bad alloc !");
                };
            #endif
                for (ull i = 0; i < _size; ++i){
                    *(this->_chunk + i) = *(temp + i);
                };
                this->_size = _esize;
                delete[] temp;
            };
            Array(ull initSize = 0x10){
                create(initSize);
            };
            inline void create(ull initSize = 0x10){
                if (initSize < 0x2)initSize = 0x2;
                this->_size = initSize;
                this->_chunk = Malloc(initSize);
            };
            Array(const Array& other): _size(other._size),_pos(other._pos){
                _chunk = new Type[_size];
                for (ull i = 0; i < _pos; ++i){
                    _chunk[i] = other._chunk[i];
                };
            };
            Array& operator=(const Array& other){
                if (this != &other){
                    delete[] _chunk;
                    _size = other._size;
                    _pos = other._pos;
                    _chunk = new Type[_size];
                    for (ull i = 0; i < _pos; ++i){
                        _chunk[i] = other._chunk[i];
                    };
                };
                return *this;
            };
            inline Type* Malloc(ull nsize){
                if (!nsize)return nullptr;
                return new Type[nsize];
            };
            inline Type& operator[](ull index){
                return *(this->_chunk + index);
            }
            inline const Type& operator[](ull index) const{
                return *(this->_chunk + index);
            }
            inline Type* operator()(ull index){
            #ifdef KURZER_ENABLE_EXCEPTIONS
                if (index >= this->_pos){
                    throw std::runtime_error("Array<T> out of range !");
                    return nullptr;
                }
            #endif // KURZER_ENABLE_EXCEPTIONS
                return this->_chunk + index;
            };
            inline const Type* operator()(ull index) const{
            #ifdef KURZER_ENABLE_EXCEPTIONS
                if (index >= this->_pos){
                    throw std::runtime_error("Array<T> out of range !");
                    return nullptr;
                }
            #endif // KURZER_ENABLE_EXCEPTIONS
                return this->_chunk + index;
            };
            template<typename T> inline void push(const T value){
                if (this->_pos == this->_size){
                    expand();
                };
                *(this->_chunk + _pos) = value;
                ++_pos;
            };
            inline bool is_full(){
                return this->_pos + 1 == this->_size;
            };
            inline void Free(){
                if (_chunk){
                    delete[] _chunk;
                    _chunk = nullptr;
                }
                _size = 0;
                _pos = 0;
            }
            inline Type* begin(){ return _chunk; };
            inline Type* end(){ return this->_chunk + _pos; };
            inline Type* pop(){
                if (!_pos){
                    return nullptr;
                };
                return this->_chunk + --_pos;
            };
            inline ull Length()const{ return this->_pos; };
            ~Array(){
                if (_chunk){
                    delete[] _chunk;
                };
            };
            inline ull MaxSize(){ return this->_size; };
            inline Type* GetData(){ return this->_chunk; };
        };
        template<typename T>using array = Array<T>;
        template<typename Tchar,ull baseN = 0x10,typename base::enable_if<base::is_character<Tchar>::value>::type* = nullptr>class String{
        public:
            base::Array<Tchar> data;
            String(){};
            String(const Tchar* tch){
                this->Write(tch);
            };
            inline void Init(){
                data.Free();
                data.create(baseN);
            };
            inline Tchar& operator[](const ull pos){
                return data[pos];
            };
            inline const Tchar& operator[](const ull pos) const{
                return data[pos];
            };
            inline Tchar* operator()(const ull pos){
                return &data[pos];
            };
            inline const Tchar* operator()(const ull pos) const{
                return &data[pos];
            };
            inline Tchar* GetData()const{
                return data._chunk;
            };
            inline String& Write(const Tchar* tch){
                const Tchar* _tch = tch;
                while (*_tch){
                    this->data.push(*_tch);
                    ++_tch;
                }
                return *this;
            };
            inline String& Write(const String& str){
                const Tchar* data = str.GetData();
                ull _len = str.data.Length();
                for (ull i = 0; i < _len; ++i){
                    this->data.push(*(data + i));
                };
                return *this;
            };
            inline String& operator=(const Tchar* tch){
                this->Init();
                this->Write(tch);
                return *this;
            };
            inline String& operator=(const String& str){
                this->Init();
                this->Write(str);
                return *this;
            };
            inline String substr(ull start,ull length){
                String result;
                ull _end = start + length;
                for (ull i = start; i < _end; ++i){
                    result.data.push(this->data[i]);
                };
                return result;
            };
        #ifdef _IOSTREAM_
            friend typename base::CharT<Tchar>::out_t& operator<<(typename base::CharT<Tchar>::out_t& os,const String<Tchar>& str){
                ull _len = str.data.Length();
                for (ull i = 0; i < _len; ++i){
                    os << str[i];
                };
                return os;
            };
        #endif // _IOSTREAM_
        };
        using string = String<char,0x10>;
        using wstring = String<wchar_t,0x10>;
        template<typename T,ull _BaseN = 0x10> class Queue{
        private:
            ull _head = 0;
            ull _tail = 0;
        public:
            base::Array<T> _data = base::Array<T>(_BaseN);
            Queue(){};
            inline ull capacity(){
                return _data.MaxSize();
            }
            inline ull size(){
                return _data.Length();
            };
            inline bool is_empty(){
                return _head == _tail;
            }
            inline bool is_full(){
                return (_tail + 1) % capacity() == _head;
            }
            inline T& get(const ull pos){
                return _data[_head + pos];
            }
            inline T* get_unsafe(const ull pos){
                return _data(_head + pos);
            }
            inline T& operator[](const ull pos){
                return this->get(pos);
            };
            inline const T& operator[](const ull pos) const{
                return this->get(pos);
            };
            template<typename _Ty> inline void push(_Ty&& _Val){
                if (is_full()){
                    _data.expand();
                }
                _data[_tail] = base::forward<_Ty>(_Val);
                _tail = (_tail + 1) % capacity();
                ++_data._pos;
            }
            inline T* begin(){
                return _data._chunk + this->_head;
            };
            inline T* end(){
                return _data._chunk + _data._pos;
            };
            inline T* pop(){
                if (this->is_empty()){
                #ifdef KURZER_ENABLE_EXCEPTIONS
                    throw std::runtime_error("Queue is empty!");
                #else
                    return nullptr;
                #endif
                };
                T* ret = _data(_head);
                _head = (_head + 1) % capacity();
                return ret;
            }
        };
        template<typename T> using queue = Queue<T,0x10>;
        //template<typename T>class MultiDimensionalArray{
        //private:
        //    //TODO
        //public:
        //    ull size_ = 0;
        //    base::Array<T>* elements = nullptr;
        //    template<typename... Args>MultiDimensionalArray(Args... args){
        //        this->size_ = sizeof...(args);
        //        elements = new base::Array<T>(this->size_);
        //        (elements->push(args),...);
        //    };
        //    ~MultiDimensionalArray(){
        //        if (elements){
        //            delete elements;
        //        };
        //    };
        //};
    };
};