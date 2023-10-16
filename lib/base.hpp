//2023-2024
#pragma once
#ifndef _kurzerbase_
#define _kurzerbase_
#endif
namespace KUR{
    namespace base{
        //size_t
        typedef unsigned long long ull;
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
        //is_character<T>::value
        template<typename T>struct is_character{
            static constexpr bool value = base::is_same<T,char>::value || base::is_same<T,wchar_t>::value || base::is_same<T,char8_t>::value || base::is_same<T,char16_t>::value || base::is_same<T,char32_t>::value;
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
                if (index < this->_pos){
                    return this->_chunk + index;
                }
                return nullptr;
            };
            inline const Type* operator()(ull index) const{
                if (index < this->_pos){
                    return this->_chunk + index;
                };
                return nullptr;
            };
            inline void push(const Type value){
                if (++this->_pos > this->_size){
                    expand();
                };
                *(this->_chunk + _pos - 1) = value;
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
            inline Type* end(){ return this->_chunk + _pos; };//end +1;
            inline Type* pop(){
                if (!_pos){
                    return nullptr;
                };
                --_pos;
                return this->_chunk + _pos;
            };
            inline ull Length()const{ return this->_pos; };
            ~Array(){
                delete[] _chunk;
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
            inline static ull strlen(const Tchar* str){
                const Tchar* p = str;
                while (*p){
                    ++p;
                };
                return p - str;
            };
            inline void Clear(){
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
                ull _len = this->strlen(tch);
                for (ull i = 0; i < _len; ++i){
                    this->data.push(*(tch + i));
                };
                return *this;
            };
            inline String& Write(const String& str){
                const Tchar* data = str.GetData();
                for (ull i = 0; i < str.data.Length(); ++i){
                    this->data.push(*(data + i));
                };
                return *this;
            };
            inline String& operator=(const Tchar* tch){
                this->Clear();
                this->Write(tch);
                return *this;
            };
            inline String& operator=(const String& str){
                this->Clear();
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
            friend base::CharT<Tchar>::out_t& operator<<(base::CharT<Tchar>::out_t& os,const String<Tchar>& str){
                for (ull i = 0; i < str.data.Length(); ++i){
                    os << str[i];
                };
                return os;
            };
        #endif // _IOSTREAM_
        };
        using string = String<char,0x10>;
        using wstring = String<wchar_t,0x10>;
        template<typename T>class Queue{
        public:
            base::Array<T>_data;
            ull _front = 0;
            ull _rear = 0;
            Queue(ull _BaseN = 0x10){
                _data.create(_BaseN);
            }
            template<typename _Ty>inline void push(const _Ty&& _Val){
                this->_push(base::forward<const _Ty>(_Val));
            }
            template<typename _Ty>inline void push(_Ty&& _Val){
                this->_push(base::forward<_Ty>(_Val));
            }
            inline void _push(const T& _Val_L){

            }
            inline void _push(const T&& _Val_R){

            }
        };
        template<typename T> using queue = Queue<T>;
        template<typename T>class MultiDimensionalArray{
        private:
            //TODO
        public:
            ull size_ = 0;
            base::Array<T>* elements = nullptr;
            template<typename... Args>MultiDimensionalArray(Args... args){
                this->size_ = sizeof...(args);
                elements = new base::Array<T>(this->size_);
                (elements->push(args),...);
            };
            ~MultiDimensionalArray(){
                if (elements){
                    delete elements;
                };
            };
        };
    };
};