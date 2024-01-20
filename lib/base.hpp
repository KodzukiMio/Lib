//2023-2024
//适用于 C++17 及以上标准
#pragma once
#ifndef _kurzerbase_
#define _kurzerbase_
#endif
/* #define options:
*  KURZER_ENABLE_EXCEPTIONS
*/
#ifdef NDEBUG //release
#define KURZER_ENABLE_EXCEPTIONS 0
#else //debug
#define KURZER_ENABLE_EXCEPTIONS 1
#endif // NDEBUG
#include<stdexcept>
#ifdef __KUR_ENABLE_STRING             
#define __KUR_STRING
#endif
#ifdef __KUR_ENABLE_IOSTREAM
#define __KUR_IOSTREAM
#endif // ENABLE_IOSTREAM
#define KUR_DEBUG_ASSERT(CODE) if(KURZER_ENABLE_EXCEPTIONS){CODE}
namespace KUR{
    static_assert(sizeof(void*) > 2,"Does not support less than 32-bit.");
    [[maybe_unused]] constexpr static bool is_x86 = sizeof(void*) == 4;
    [[maybe_unused]] constexpr static bool is_x64 = sizeof(void*) == 8;
    namespace base{
        typedef unsigned long long ull;
        //release模式下平均性能为std::memcpy的93%.
        void* memcpy(void* _Dst,const void* _Src,ull _ByteSize){
            if (!_ByteSize || !_Dst || !_Src)return nullptr;
            ull _D_adr = (ull)_Dst;
            ull _S_adr = (ull)_Src;
            constexpr const ull _Ofx = sizeof(ull);
            while ((_S_adr % _Ofx) && _ByteSize){
                *((char*)_D_adr++) = *((char*)_S_adr++);
                --_ByteSize;
            };
            if (!_ByteSize)return _Dst;
            ull* _Aldr = (ull*)_D_adr;
            const ull* _Alsc = (const ull*)_S_adr;
            while (_ByteSize >= _Ofx){
                *_Aldr++ = *_Alsc++;
                _ByteSize -= _Ofx;
            };
            if (!_ByteSize)return _Dst;
            char* _Fdst = (char*)_Aldr;
            const char* _Fsrc = (const char*)_Alsc;
            while (_ByteSize){
                *_Fdst++ = *_Fsrc++;
                --_ByteSize;
            };
            return _Dst;
        };
        template<typename Tchar> struct CharT{
        #ifdef __KUR_STRING
            using char_t = std::basic_string<Tchar,std::char_traits<Tchar>,std::allocator<Tchar>>;
        #endif // _XSTRING_
        #ifdef  __KUR_IOSTREAM
            using out_t = std::basic_ostream<Tchar,std::char_traits<Tchar>>;
            using in_t = std::basic_istream<Tchar,std::char_traits<Tchar>>;
        #endif // _IOSTREAM_
        };
        template<ull S,int Log = 0>struct Log2: Log2<S / 2,Log + 1>{};
        template<int Log>struct Log2<1,Log>{
            static constexpr int value = Log;
        };
        typedef int _Sequence;
        struct sort_sequence{
            static constexpr const base::_Sequence upper = 0;
            static constexpr const base::_Sequence lower = 1;
        };
        template <class _Ty,_Ty _Val>struct integral_constant{
            static constexpr _Ty value = _Val;
            using value_type = _Ty;
            using type = integral_constant;
            constexpr operator value_type() const noexcept{
                return value;
            };
            constexpr value_type operator()() const noexcept{//function
                return value;
            };
        };
        template <bool _Val>using bool_constant = integral_constant<bool,_Val>;
        using true_type = bool_constant<true>;
        using false_type = bool_constant<false>;
        template <typename... _Types>using void_t = void;
        template <typename>inline constexpr bool _Always_false = false;
        template<bool condition,typename T = void>struct enable_if{};
        template<typename T>struct enable_if<true,T>{ typedef T type; };
        template <typename T,typename U>struct is_same{
            static constexpr bool value = false;
        };
        template <typename T>struct is_same<T,T>{
            static constexpr bool value = true;
        };
        template <typename _Ty>struct remove_cv{
            using type = _Ty;
            template <template <typename> class _Fn>using _Apply = _Fn<_Ty>;
        };
        template <typename _Ty>struct remove_cv<const _Ty>{
            using type = _Ty;
            template <template <typename> class _Fn>using _Apply = const _Fn<_Ty>;
        };
        template <typename _Ty>struct remove_cv<volatile _Ty>{
            using type = _Ty;
            template <template <typename> class _Fn>using _Apply = volatile _Fn<_Ty>;
        };
        template <typename _Ty>struct remove_cv<const volatile _Ty>{
            using type = _Ty;
            template <template <typename> class _Fn>using _Apply = const volatile _Fn<_Ty>;
        };
        template <class _Ty>using remove_cv_t = typename remove_cv<_Ty>::type;
        template<bool _True,typename _Itr_v,typename..._Next_t>struct _Disjunction{
            using type = _Itr_v;
        };
        template<typename _False,typename _Itr_v,typename..._Next_t>struct _Disjunction<false,_False,_Itr_v,_Next_t...>{
            using type = typename _Disjunction<_Itr_v::value,_Itr_v,_Next_t...>::type;
        };
        template <class... _Traits>struct disjunction: false_type{};
        template <class _Itr_t,class... _Next>struct disjunction<_Itr_t,_Next...>: _Disjunction<_Itr_t::value,_Itr_t,_Next...>::type{};
        template <class... _Traits>inline constexpr bool disjunction_v = disjunction<_Traits...>::value;
        template <class _Ty,class... _Types>inline constexpr bool _Is_any_of_v = disjunction_v<is_same<_Ty,_Types>...>;
        template<typename _Ty>struct is_character{
            static constexpr bool value = _Is_any_of_v<remove_cv_t<_Ty>,char,signed char,unsigned char,wchar_t,
            #ifdef __cpp_char8_t         
                char8_t,
            #endif
                char16_t,char32_t>;
        };
        template<typename _Ty>struct is_integral{
            static constexpr bool value = _Is_any_of_v<remove_cv_t<_Ty>,bool,char,signed char,unsigned char,wchar_t,
            #ifdef __cpp_char8_t         
                char8_t,
            #endif
                char16_t,char32_t,short,unsigned short,int,unsigned int,long,unsigned long,long long,unsigned long long>;
        };
        template<typename _Ty>inline constexpr bool is_integral_v = is_integral<_Ty>::value;
        template <class _Ty>inline constexpr bool is_floating_point_v = _Is_any_of_v<remove_cv_t<_Ty>,float,double,long double>;
        template<typename _Ty,bool = is_integral_v<_Ty>>struct _Sign_base{
            using _Uty = typename remove_cv<_Ty>::type;
            static constexpr bool _Signed = static_cast<_Uty>(-1) < static_cast<_Uty>(0);
            static constexpr bool _Unsigned = !_Signed;
        };
        template<typename _Ty>struct _Sign_base<_Ty,false>{
            static constexpr bool _Signed = is_floating_point_v<_Ty>;
            static constexpr bool _Unsigned = false;
        };
        template<typename _Ty>struct is_unsigned:bool_constant<_Sign_base<_Ty>::_Unsigned>{};
        template <class _Ty>inline constexpr bool is_unsigned_v = _Sign_base<_Ty>::_Unsigned;
        template<typename _Ty>struct is_signed:bool_constant<_Sign_base<_Ty>::_Signed>{};
        template <class _Ty>inline constexpr bool is_signed_v = _Sign_base<_Ty>::_Signed;
        template<typename _T0,typename _T1>using _Is_Same = typename base::enable_if<base::is_same<_T0,_T1>::value>::type*;
        template<typename T>struct is_lvalue_reference:base::false_type{};
        template<typename T>struct is_lvalue_reference<T&>:base::true_type{};
        template<typename T>struct is_rvalue_reference:base::false_type{};
        template<typename T>struct is_rvalue_reference<T&&>:base::true_type{};
        template<typename _Ty> struct remove_reference{ typedef _Ty type; };
        template<typename _Ty> struct remove_reference<_Ty&>{ typedef _Ty type; };
        template<typename _Ty> struct remove_reference<_Ty&&>{ typedef _Ty type; };
        template <typename T>typename base::remove_reference<T>::type&& move(T&& _Ty){
            return static_cast<typename base::remove_reference<T>::type&&>(_Ty);
        };
        template<typename _Tp>constexpr _Tp&& forward(typename base::remove_reference<_Tp>::type& _Arg)noexcept{//left value
            return static_cast<_Tp&&>(_Arg);
        };
        template<typename _Tp>constexpr _Tp&& forward(typename base::remove_reference<_Tp>::type&& _Arg)noexcept{//right value
            static_assert(!base::is_lvalue_reference<_Tp>::value,"_Arg is not a right value.");
            return static_cast<_Tp&&>(_Arg);
        };
        template <typename _Ty,typename = void>struct _Add_reference{ //no-ref
            using _Lvalue = _Ty;
            using _Rvalue = _Ty;
        };
        template <typename _Ty>struct _Add_reference<_Ty,void_t<_Ty&>>{ //ref
            using _Lvalue = _Ty&;
            using _Rvalue = _Ty&&;
        };
        template <typename _Ty>struct add_lvalue_reference{
            using type = typename _Add_reference<_Ty>::_Lvalue;
        };
        template <typename _Ty>using add_lvalue_reference_t = typename _Add_reference<_Ty>::_Lvalue;
        template <typename _Ty>struct add_rvalue_reference{
            using type = typename _Add_reference<_Ty>::_Rvalue;
        };
        template <typename _Ty>using add_rvalue_reference_t = typename _Add_reference<_Ty>::_Rvalue;
        template<typename _Ty>typename base::add_rvalue_reference<_Ty>::type declval() noexcept{
            static_assert(_Always_false<_Ty>,"declval should not be called!");
        };
        template<typename _Ty1,typename _Ty2,typename = void>struct allow_equal_operator:base::false_type{};
        template<typename _Ty1,typename _Ty2>struct allow_equal_operator<_Ty1,_Ty2,base::void_t<decltype(base::declval<_Ty1>() == base::declval<_Ty2>())>>:base::true_type{};
        template<bool _Test,typename _Ty1,typename _Ty2>struct conditional{
            using type = _Ty1;
        };
        template<typename _Ty1,typename _Ty2>struct conditional<false,_Ty1,_Ty2>{
            using type = _Ty2;
        };
        template <bool _Test,class _Ty1,class _Ty2>using conditional_t = typename conditional<_Test,_Ty1,_Ty2>::type;
        template<typename Tchar> inline static ull strlen(const Tchar* str){
            const Tchar* p = str;
            while (*p)++p;
            return p - str;
        };
        template<typename T>inline T maximum(T _Left,T _Right){
            return ((_Left) < (_Right)) ? (_Right) : (_Left);
        };
        template<typename T>inline T minimum(T _Left,T _Right){
            return ((_Left) < (_Right)) ? (_Left) : (_Right);
        };
        template<typename T>inline T gcd(const T& _Lv,const T& _Rv){
            return !_Rv ? _Lv : base::gcd(_Rv,_Lv % _Rv);
        };
        template <typename _Itr>void reverse(_Itr* _Begin,_Itr* _End){
            while (_Begin < _End){
                --_End;
                _Itr _Tmp = *_Begin;
                *_Begin = *_End;
                *_End = _Tmp;
                ++_Begin;
            };
        };
        template <typename _Ty>inline void swap(_Ty& _Left,_Ty& _Right){
            _Ty _Tmp = base::move(_Left);
            _Left = base::move(_Right);
            _Right = base::move(_Tmp);
        };
        template<typename _Ty,typename _Itr,typename...Args>inline _Itr find(_Itr _Begin,_Itr _End,_Ty _CmpPfn,Args... _CmpArgs){//_CmpPfn(itr,_CmpArgs...)
            while (_Begin != _End){
                if (_CmpPfn(*_Begin,base::forward<Args>(_CmpArgs)...))return _Begin;
                ++_Begin;
            };
            return _End;
        };
        template<typename _Ty,typename _Itr,typename base::enable_if<base::allow_equal_operator<_Ty,decltype(*base::declval<_Itr>())>::value>::type* = nullptr>inline _Itr find(_Itr _Begin,_Itr _End,const _Ty& _Cmp_Val){//need operator ==
            while (_Begin != _End){
                if (*_Begin == base::move(_Cmp_Val))return _Begin;
                ++_Begin;
            };
            return _End;
        };
        const constexpr ull npos = -1;
        template<typename Tchar,typename base::enable_if<base::is_character<Tchar>::value>::type* = nullptr>inline ull bf_search(const Tchar* _Sstr,const Tchar* _Fstr,const ull _First = 0,const ull _Last = base::npos){
            const Tchar* _Ssptr = _Sstr + _First;
            const Tchar* _Sptr = _Ssptr;
            const Tchar* _Fptr = _Fstr;
            ull _Pos = 0;
            if (_Last == base::npos){
                while (*_Ssptr){
                    _Sptr = _Ssptr;
                    _Fptr = _Fstr;
                    while ((*_Sptr == *_Fptr) && *_Fptr){
                        ++_Sptr;
                        ++_Fptr;
                    };
                    if (!*_Fptr){
                        _Pos = _Ssptr - _Sstr;
                        goto step;
                    };
                    _Ssptr = _Sptr + 1;
                };
            } else{
                while (*_Ssptr && ((_Ssptr - _Sstr) < _Last)){
                    _Sptr = _Ssptr;
                    _Fptr = _Fstr;
                    while ((*_Sptr == *_Fptr) && *_Fptr){
                        ++_Sptr;
                        ++_Fptr;
                    };
                    if (!*_Fptr){
                        _Pos = _Ssptr - _Sstr;
                        goto step;
                    };
                    _Ssptr = _Sptr + 1;
                };
            };
            return base::npos;
        step:
            if (_Pos >= (_Fstr - _Sstr))return base::npos;
            return _Pos;
        };
        template<typename Tchar,typename base::enable_if<base::is_character<Tchar>::value>::type* = nullptr>inline bool _str_isolength(const Tchar* _Sstr,const Tchar* _Fstr){//length _Sstr >= _Fstr
            while ((*_Sstr == *_Fstr) && *_Fstr){
                ++_Sstr;
                ++_Fstr;
            };
            if (!*_Fstr)return true;
            return false;
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
                    };
                };
                if (_Flag)break;
            };
        };
        template<typename _Tp,typename _CmpPfn2Args>inline void sort_select(_Tp* _Begin,_Tp* _End,_CmpPfn2Args _CmpPfn){//[_Begin,_End)    _Cmpfn(T*,T*)
            _Tp* _Tmp0 = _Begin;
            while (_Tmp0 < _End){
                _Tp* _Tmp1 = _Tmp0;
                _Tp* _Min_ = _Tmp1;
                while (_Tmp1 < _End){
                    if (_CmpPfn(_Tmp1,_Min_))_Min_ = _Tmp1;
                    ++_Tmp1;
                };
                _Tp _Tmp = *_Min_;
                *_Min_ = *_Tmp0;
                *_Tmp0 = _Tmp;
                ++_Tmp0;
            };
        };
        template<typename _Tp,typename _CmpPfn2Args>inline void sort_insert(_Tp* _Begin,_Tp* _End,_CmpPfn2Args _CmpPfn){//[_Begin,_End)    _Cmpfn(T*,T*)
            for (_Tp* _Sorted = _Begin + 1; _Sorted < _End; ++_Sorted){
                _Tp _Key = *_Sorted;
                _Tp* _Pos = _Sorted - 1;
                while (_Pos >= _Begin && _CmpPfn(&_Key,_Pos)){
                    *(_Pos + 1) = *_Pos;
                    --_Pos;
                };
                *(_Pos + 1) = _Key;
            };
        };
        template<typename _Tp,typename _CmpPfn2Args>inline void _merge(_Tp* _Begin,_Tp* _Middle,_Tp* _End,_CmpPfn2Args _CmpPfn,_Tp* _Tmp){
            auto _Beg = _Begin;
            auto _Pos = _Middle;
            _Tp* _Tp_ = _Tmp;
            while (_Beg < _Middle && _Pos < _End){
                if (_CmpPfn(_Beg,_Pos))*_Tp_++ = *_Beg++;
                else *_Tp_++ = *_Pos++;
            };
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
            };
        };
        template<typename _Tp,typename _CmpPfn2Args>inline void sort_merge(_Tp* _Begin,_Tp* _End,_CmpPfn2Args _CmpPfn,_Tp* _Tmp = nullptr){//[_Begin,_End),_Cmpfn(T*,T*),_Tmp length = arr
            bool _Mem = false;
            if (!_Tmp){
                _Tmp = new _Tp[_End - _Begin];
                _Mem = true;
            };
            if (_Tmp){
                _sort_merge(_Begin,_End,_CmpPfn,_Tmp);
                if (_Mem)delete[] _Tmp;
                return;
            };
            KUR_DEBUG_ASSERT(throw std::bad_alloc(););
        };

        template<typename Type,ull init_size>class Array;
        template<typename _Tp>inline _Tp _sort_count_size(_Tp* _Begin,_Tp* _End,_Tp& _Out_Lower){
            _Tp _Lower = *_Begin;
            _Tp _Upper = *_Begin;
            while (_Begin < _End){
                _Lower = base::minimum(*_Begin,_Lower);
                _Upper = base::maximum(*_Begin,_Upper);
                ++_Begin;
            };
            _Out_Lower = _Lower;
            return (_Upper - _Lower + 1);
        };
        template<typename _Tp,typename  base::enable_if<base::is_integral<_Tp>::value>::type* = nullptr>inline void sort_count(_Tp* _Begin,_Tp* _End,base::_Sequence _Seq = base::sort_sequence::upper){//[_Begin,_End)
            _Tp _Lower = 0;
            auto _Beg = _Begin;
            auto _Beg_R = _Begin;
            auto _Size = _sort_count_size<_Tp>(_Begin,_End,_Lower);
            Array<_Tp,0x10> _Tmp(_Size);
            _Tmp.init(0,_Size);
            while (_Begin < _End){
                ++_Tmp[(*_Begin - _Lower)];
                ++_Begin;
            };
            for (ull i = 0; i < _Size; ++i){
                if (_Tmp[i]){
                    while (_Tmp[i]){
                        *_Beg = i + _Lower;
                        ++_Beg;
                        --_Tmp[i];
                    };
                };
            };
            if (_Seq)base::reverse(_Beg_R,_End);
        };
        //_KUR_TEMPLATE_TYPE_IS(T1,T2)
    #define _KUR_TEMPLATE_TYPE_IS(Type,Is_Ty)template <typename Type,typename base::enable_if_t<base::is_same<Type,Is_Ty>::value>* = nullptr>
    //_KUR_TEMPLATE_T_IS(T2)
    #define _KUR_TEMPLATE_T_IS(Is_Ty) _KUR_TEMPLATE_TYPE_IS(T,Is_Ty) 
        template<typename T>class R_Iterator{//反向迭代器
        public:
            T* _begin = nullptr;
            T* _end = nullptr;
            T* _Pos = nullptr;
            R_Iterator(){};
            R_Iterator(const R_Iterator& other): _begin(other._begin),_end(other._end),_Pos(other._Pos){}
            R_Iterator(T* begin_,T* end_):_begin(begin_),_end(end_){
                this->_Pos = this->_end - 1;
            };
            inline T* begin(){
                return this->_end - 1;
            };
            inline T* end(){
                return this->_begin - 1;
            };
            inline R_Iterator& operator++(){
                KUR_DEBUG_ASSERT(if (this->_Pos < this->_begin){ throw std::runtime_error("R_Iterator bounds checking error !"); };);
                --this->_Pos;
                return *this;
            };
            inline T* operator*(){
                return this->_Pos;
            };
            inline bool operator!=(const R_Iterator& rv){
                return this->_Pos != rv._Pos;
            };
            inline bool operator==(const R_Iterator& rv){
                return this->_Pos == rv._Pos;
            };
            inline R_Iterator& operator=(const R_Iterator& rv){
                if (this == &rv)return *this;
                this->_begin = rv._begin;
                this->_end = rv._end;
                this->_Pos = rv._Pos;
                return *this;
            };
        };
        template<typename Type,ull init_size = 0x10>class Array{
        public:
            bool _allow_del = true;
            Type* _chunk = nullptr;
            ull _size = 0;
            ull _pos = 0;
            inline void expand(){
                if (!_chunk){
                    this->create(init_size);
                    return;
                };
                Type* temp = _chunk;
                ull _esize = (_size << 1);
                this->_chunk = _malloc(_esize);
                if (!this->_chunk){
                    this->_chunk = temp;
                    throw std::runtime_error("bad alloc !");
                };
                base::memcpy(_chunk,temp,_size * sizeof(Type));
                this->_size = _esize;
                delete[] temp;
            };
            inline void _construct(const ull initSize = 0)noexcept{
                if (initSize)create(initSize);
            };
            Array(const ull initSize = 0)noexcept{//Array<T>(0) ->not alloc memory
                this->_construct(initSize);
                this->_pos = initSize;
            };
            inline void create(ull initSize = 0x10)noexcept{
                if (initSize < 0x2)initSize = 0x2;
                this->_size = initSize;
                this->_chunk = _malloc(initSize);
            };
            inline void move_from(Array&& other)noexcept{
                if (_chunk)delete[] _chunk;
                this->_chunk = other._chunk;
                this->_size = other._size;
                this->_pos = other._pos;
                other._chunk = nullptr;
                other._size = 0;
                other._pos = 0;
            };
            Array(const Array& other)noexcept: _size(other._size),_pos(other._pos){
                _chunk = new Type[_size];
                for (ull i = 0; i < _pos; ++i)_chunk[i] = other._chunk[i];
            };
            Array(Array&& other) noexcept: _allow_del(other._allow_del),_chunk(other._chunk),_size(other._size),_pos(other._pos){
                other._chunk = nullptr;
                other._size = 0;
                other._pos = 0;
            };
            Array& operator=(Array&& other) noexcept{
                if (this != &other){
                    if (!this->capacity())return *this;
                    if (_chunk)delete[] _chunk;
                    _allow_del = other._allow_del;
                    _chunk = other._chunk;
                    _size = other._size;
                    _pos = other._pos;
                    other._chunk = nullptr;
                    other._size = 0;
                    other._pos = 0;
                };
                return *this;
            };
            Array& operator=(const Array& other)noexcept{
                if (this != &other){
                    if (_chunk)delete[] _chunk;
                    _size = other._size;
                    _pos = other._pos;
                    _chunk = new Type[_size];
                    for (ull i = 0; i < _pos; ++i)_chunk[i] = other._chunk[i];
                };
                return *this;
            };
            [[nodiscard]] inline Type* _malloc(ull nsize)noexcept{
                if (!nsize)return nullptr;
                return new Type[nsize];
            };
            inline Type& operator[](ull index){
                KUR_DEBUG_ASSERT(if (index >= _pos){ throw std::runtime_error("Array<T> out of range !"); };);
                return *(this->_chunk + index);
            };
            inline const Type& operator[](ull index) const{
                KUR_DEBUG_ASSERT(if (index >= _pos){ throw std::runtime_error("Array<T> out of range !"); };);
                return *(this->_chunk + index);
            };
            inline Type* operator()(ull index){
                KUR_DEBUG_ASSERT(
                    if (index >= this->_pos){
                        throw std::runtime_error("Array<T> out of range !");
                        return nullptr;
                    };);
                return this->_chunk + index;
            };
            inline const Type* operator()(ull index) const{
                KUR_DEBUG_ASSERT(
                    if (index >= this->_pos){
                        throw std::runtime_error("Array<T> out of range !");
                        return nullptr;
                    };);
                return this->_chunk + index;
            };
            inline void push(const Type value){
                if (this->_pos >= this->_size)expand();
                *(this->_chunk + _pos) = value;
                ++_pos;
            };
            inline void push_back(const Type value){
                if (this->_pos >= this->_size)expand();
                *(this->_chunk + _pos) = value;
                ++_pos;
            };
            inline bool is_full()noexcept{
                return this->_pos + 1 == this->_size;
            };
            inline Type* data()noexcept{
                return this->_chunk;
            };
            inline void _free()noexcept{
                if (_chunk){
                    delete[] _chunk;
                    _chunk = nullptr;
                };
                _size = 0;
                _pos = 0;
            };
            inline void clear()noexcept{
                this->_pos = 0;
            };
            inline Type& at(base::ull _Pos){
                KUR_DEBUG_ASSERT(if (_Pos >= _pos){ throw std::runtime_error("Array<T> out of range !"); };);
                return *(this->_chunk + _Pos);
            };
            inline Type* begin()noexcept{ return _chunk; };
            inline Type* end()noexcept{ return this->_chunk + _pos; };
            inline Type& last()noexcept{
                KUR_DEBUG_ASSERT(if (!_pos){ throw std::runtime_error("Array<T> out of range !"); };);
                return *(this->_chunk + _pos - 1);
            };
            [[nodiscard]] inline Type* pop()noexcept{
                if (!_pos)return nullptr;
                return this->_chunk + --_pos;
            };
            inline void pop_back(){
                KUR_DEBUG_ASSERT(if (!_pos){ throw std::runtime_error("Array<T> out of range !"); };);
                --_pos;
            };
            ~Array(){
                if (_chunk && _allow_del)delete[] _chunk;
            };
            void __force_release_memory(){//error deal
                if (this->_chunk)delete[] _chunk;
                _chunk = nullptr;
            };
            inline Type* erase(ull index){
                KUR_DEBUG_ASSERT(if (index >= _pos || !_pos){ throw std::runtime_error("Array<T> out of range !"); };);
                auto _pos0 = _pos - 1;
                for (ull i = index; i < _pos0; ++i)_chunk[i] = _chunk[i + 1];
                --_pos;
                return _chunk + index;
            };
            inline ull size()const noexcept{ return this->_pos; };
            inline ull capacity()noexcept{ return this->_size; };
            inline Type* get_data()noexcept{ return this->_chunk; };
            inline void init(Type _Init_Val,ull _Size = 0)noexcept{
                ull _Len = this->Length();
                if (_Size)_Len = _Size;
                for (ull i = 0; i < _Len; ++i)*(this->_chunk + i) = _Init_Val;
            };
            inline Type* find(const Type& value)noexcept{
                for (ull i = 0; i < _pos; ++i)if (_chunk[i] == value)return &_chunk[i];
                return end();
            };
            inline ull find_idx(const Type& value)noexcept{
                Type* _frsl = this->find(value);
                if (_frsl == this->end())return -1;
                return _frsl - this->begin();
            };
        };
        template<typename T>using array = Array<T>;
        template<typename Type,ull init_size = 0x10>using vector = Array<Type,init_size>;
        template<typename Tchar = char,ull baseN = 0x10,typename base::enable_if<base::is_character<Tchar>::value>::type* = nullptr>class String{
        public:
            base::Array<Tchar,baseN> data;
            String(){};
            using value_type = Tchar;
            String(const Tchar* tch){
                this->write(tch);
            };
            String(const base::Array<Tchar>& arr){
                this->Write(arr._chunk);
            };
            String(const bool* _bl,const ull _size){
                ull idx = 0;
                while (idx < _size)this->data.push(_bl[idx++]);
            };
            String(const String&& tch)noexcept{
                this->data = tch.data;
            };
            String(const String& tch){
                this->data = tch.data;
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
            inline Tchar* get_data()const{
                return data._chunk;
            };
            inline String& write(const Tchar* tch){
                const Tchar* _tch = tch;
                while (*_tch){
                    this->data.push(*_tch);
                    ++_tch;
                };
                return *this;
            };
            inline String& write(const String& str){
                const Tchar* data = str.GetData();
                ull _len = str.data.size();
                for (ull i = 0; i < _len; ++i)this->data.push(*(data + i));
                return *this;
            };
            inline String& operator=(const Tchar* tch){
                this->data.clear();
                this->write(tch);
                return *this;
            };
            inline String& operator=(const String& str){
                this->data.clear();
                this->write(str);
                return *this;
            };
            inline String substr(ull start,ull length){
                String result;
                ull _end = start + length;
                for (ull i = start; i < _end; ++i)result.data.push(this->data[i]);
                return result;
            };
            inline ull size(){
                return data.size();
            };
            inline Tchar* begin(){
                return this->data.begin();
            };
            inline Tchar* end(){
                return this->data.end();
            };
            inline bool empty(){
                return this->data._pos;
            };
            inline void push_back(Tchar* str){
                this->write(str);
            };
            inline void push_back(Tchar str){
                this->data.push(str);
            };
            inline void pop_back(){
                this->data.pop_back();
            };
            inline Tchar pop(){
                return *this->data.pop();
            };
        #ifdef __KUR_IOSTREAM
            friend typename base::CharT<Tchar>::out_t& operator<<(typename base::CharT<Tchar>::out_t& os,const String<Tchar>& str){
                ull _len = str.data.size();
                for (ull i = 0; i < _len; ++i)os << str[i];
                return os;
            };
        #endif
        };
        using string = String<char>;
        using wstring = String<wchar_t>;
        template<typename T,ull _BaseN = 0x10> class Queue{
        private:
            ull _head = 0;
            ull _tail = 0;
        public:
            base::Array<T> _data = base::Array<T>(_BaseN);
            Queue(){};
            inline ull capacity(){
                return _data.capacity();
            };
            inline ull size(){
                return _data.size();
            };
            inline bool is_empty(){
                return _head == _tail;
            };
            inline bool is_full(){
                return (_tail + 1) % capacity() == _head;
            };
            inline T& get(const ull pos){
                return _data[_head + pos];
            };
            inline T* get_unsafe(const ull pos){
                return _data(_head + pos);
            };
            inline T& operator[](const ull pos){
                return this->get(pos);
            };
            inline const T& operator[](const ull pos) const{
                return this->get(pos);
            };
            template<typename _Ty> inline void push(_Ty&& _Val){
                if (is_full())_data.expand();
                _data[_tail] = base::forward<_Ty>(_Val);
                _tail = (_tail + 1) % capacity();
                ++_data._pos;
            };
            inline T* begin(){
                return _data._chunk + this->_head;
            };
            inline T* end(){
                return _data._chunk + _data._pos;
            };
            inline T* pop(){
                if (this->is_empty()){
                    KUR_DEBUG_ASSERT(throw std::runtime_error("Queue is empty!");)else{
                        return nullptr;
                    };
                };
                T* ret = _data(_head);
                _head = (_head + 1) % capacity();
                return ret;
            };
        };
        template<typename T> using queue = Queue<T,0x10>;
        template<typename T,ull init_size = 0x10> class base_node{
        public:
            Array<T,init_size> data;
            template<typename...Args>inline void push(Args... arg){ data.push(T(arg...)); };//value | constructor
            template<typename Ty,typename...Args>inline T* find(Ty _CmpPfn,Args... _CmpArgs){//_CmpPfn(*itr,_CmpArgs...)
                return base::find(data.begin(),data.end(),_CmpPfn,base::forward<Args>(_CmpArgs)...);
            };
            template<typename Ty,typename base::enable_if<base::allow_equal_operator<Ty,T>::value>::type* = nullptr>inline T* find(const Ty _Cmp_Val){//need operator ==
                return base::find(data.begin(),data.end(),base::forward<const Ty>(_Cmp_Val));
            };
            inline auto eof(){ return this->data.end(); };
            template<typename...Args> base_node(Args... arg){ this->push(arg...); };
            base_node(){};
        };
        template<typename _Rt,typename... _Args>class _simple_function{
        public:
            using _Fn = _Rt(*)(_Args...);
            using _Ret = _Rt;
            _Fn _pfn = nullptr;
            _simple_function(_Fn _Pfn):_pfn(_Pfn){};
            _simple_function(){};
            _Rt operator()(_Args...args){
                return this->_pfn(base::forward<_Args>(args)...);
            };
        };
        template<typename T,ull init_size = 0x10,typename... _Args>class Tree:public _simple_function<bool,_Args...>{
        public:
            using value_type = T;
            class Node:public base::base_node<T,init_size>{
            public:
                bool _allow_del = true;
                Array<Node*,init_size>nodes;
                template<typename...Args> Node(Args... arg){ this->push(arg...); };
                inline void del_nodes(){
                    if (_allow_del)for (auto i : nodes)if (i)delete i;
                };
                ~Node(){
                    this->del_nodes();
                };
            };
            ull _size = 0;
            Node* _base_node = nullptr;
            void* _arg_data = nullptr;
            inline ull size(){ return this->_size; };
            Tree(bool(*_Pfn)(_Args...)): _simple_function<bool,_Args...>(_Pfn){};
            Tree(){};
            template<typename...Args> inline void init(Args... arg){
                _base_node = new Node(base::forward<Args>(arg)...);
                ++_size;
            }
            template<typename _Ty> inline void set_data(const _Ty* dat){
                _arg_data = (void*)dat;
            };
            inline bool call(base::Tree<T,init_size,_Args...>::Node* node){
                return this->_pfn(_arg_data,node);
            };
            template<typename...Args>inline void insert(Node* parent_node,Args... arg){
                if (parent_node){
                    parent_node->data.push(T(base::forward<Args>(arg)...));
                } else{
                    if (!_size){
                        this->init(base::forward<Args>(arg)...);
                    } else{
                        KUR_DEBUG_ASSERT(throw std::runtime_error("Parent node cannot be null"););
                        return;
                    };
                };
            };
            template<typename...Args>inline Node* insert_node(Node* parent_node,Args... arg){//平均每个节点占用314 Bytes,可以在构造时将init_size设置为较小的数值来减少初始大小.例如 using trees = KUR::base::tree_types<int,2>;//->131 Bytes
                if (parent_node){
                    Node* newNode = new Node(base::forward<Args>(arg)...);
                    if (newNode){
                        parent_node->nodes.push(newNode);
                        ++_size;
                        return newNode;
                    } else{
                        KUR_DEBUG_ASSERT(throw std::runtime_error("Could not alloc memory !"););
                    };
                } else{
                    if (!_size){
                        this->init(base::forward<Args>(arg)...);
                    } else{
                        KUR_DEBUG_ASSERT(throw std::runtime_error("Parent node cannot be null"););
                    };
                };
                return nullptr;
            };
            inline void traverse(Node* node,void(*_Pfn)(Node*)){//void _Pfn(KUR::base::_tree_search_t<T>* node)
                _Pfn(node);
                for (auto child : node->nodes)traverse(child,_Pfn);
            };
            Node* find_node(Node* node,const T& value,Node** parentNode){
                if (node->data.find(value) != node->eof())return node;
                for (auto child : node->nodes){
                    *parentNode = node;
                    Node* foundNode = find_node(child,value);
                    if (foundNode != nullptr)return foundNode;
                };
                return nullptr;
            };
            Node* find_node(Node* node,const T& value){
                if (node->data.find(value) != node->eof())return node;
                for (auto child : node->nodes){
                    Node* foundNode = find_node(child,value);
                    if (foundNode != nullptr)return foundNode;
                };
                return nullptr;
            };
            Node* find_parent_node(Node* start_node,Node* node){
                for (auto child : start_node->nodes){
                    if (child == node)return start_node;
                    return find_parent_node(child,node);
                };
                return nullptr;
            };
            template<typename _Ty,typename..._F_Args>Node* find_node(Node* node,_Ty _CmpPfn,_F_Args... _CmpArgs,Node** parentNode){
                if (_CmpPfn(node,base::forward<_F_Args>(_CmpArgs)...))return node;
                for (auto child : node->nodes){
                    *parentNode = node;
                    Node* foundNode = find_node(child,_CmpPfn,base::forward<_F_Args>(_CmpArgs)...);
                    if (foundNode != nullptr){
                        return foundNode;
                    };
                };
                return nullptr;
            };
            template<typename..._F_Args>Node* find_node(Node* node,bool(*_CmpPfn)(Node*,_F_Args...),_F_Args... _CmpArgs){//bool _Pfn(trees::search_t* node,...)
                if (_CmpPfn(node,base::forward<_F_Args>(_CmpArgs)...))return node;
                for (auto child : node->nodes){
                    Node* foundNode = find_node(child,_CmpPfn,base::forward<_F_Args>(_CmpArgs)...);
                    if (foundNode != nullptr)return foundNode;
                };
                return nullptr;
            };
            bool delete_node(const T& value){//not del root
                Node* parentNode = nullptr;
                Node* nodeToDelete = find_node(_base_node,value,&parentNode);
                if (nodeToDelete == nullptr || nodeToDelete == this->_base_node)return false;
                for (ull i = 0; i < nodeToDelete->nodes.size(); ++i)parentNode->nodes.push(nodeToDelete->nodes[i]);
                nodeToDelete->_allow_del = false;
                parentNode->nodes.erase(parentNode->nodes.find(nodeToDelete) - parentNode->nodes.begin());
                delete nodeToDelete;
                --_size;
                return true;
            };
            bool delete_node(Node* node){
                if (!node || node == this->_base_node)return false;
                Node* parent_node = this->find_parent_node(this->_base_node,node);
                if (!parent_node)return false;
                Node* node_to_delete = node;
                for (ull i = 0; i < node_to_delete->nodes.size(); ++i)parent_node->nodes.push(node_to_delete->nodes[i]);
                node_to_delete->_allow_del = false;
                parent_node->nodes.erase(parent_node->nodes.find(node_to_delete) - parent_node->nodes.begin());
                delete node_to_delete;
                --_size;
                return true;
            };
            ~Tree(){
                if (_base_node)delete _base_node;
            };
        };
        template<typename T>class _simple_auto_ptr{
        public:
            T* _Ptr = nullptr;
            _simple_auto_ptr(T* ptr):_Ptr(ptr){};
            ~_simple_auto_ptr(){
                if (_Ptr)delete _Ptr;
            };
            T& operator*(){
                return *_Ptr;
            };
        };
        template<typename T,ull init_size = 0x10>using bt_node = typename base::Tree<T,init_size,void*,T>::Node;
        template<typename T,ull init_size = 0x10>using _tree_search_t = typename base::Tree<T,init_size,void*,base::bt_node<T>*>::Node;
        template<typename T,ull init_size = 0x10,typename... _Args>auto make_tree(bool(*_Pfn)(_Args...)){//bool function(void*,base::bt_node<T>*) 
            return Tree<T,init_size,_Args...>(_Pfn);
        };
        template<typename T,ull init_size = 0x10>using tree = Tree<T,init_size,void*,base::bt_node<T>*>;
        template<typename T,ull init_size = 0x10>struct tree_types{ //recommendation
        public:
            using tree_t = base::tree<T,init_size>;//Tree type
            using node_t = base::_tree_search_t<T,init_size>;//Tree::Node type
        };
        template<typename T,ull init_size = 0x10>class trie_node{
        public:
            using value_type = T;
            using _type_node = typename base::Array<base::trie_node<T,init_size>,init_size>;
            _type_node next_nodes;
            T type_val = T(0);
            bool is_str = false;
            trie_node(const T& val):type_val(val){};
            trie_node(){};
            inline trie_node* find(const T& val){
                for (auto& itr : next_nodes)if (itr.type_val == val)return &itr;
                return nullptr;
            };
            inline trie_node* insert_node(const T& val){
                this->next_nodes.push(trie_node(val));
                return this->next_nodes.last();
            };
            inline bool is_no_end(){
                return next_nodes.size();
            };
        };
        template<typename T,ull init_size = 0x10,typename node_t = base::trie_node<T,init_size>,typename base::enable_if<base::is_character<T>::value>::type* = nullptr>class TrieTree{
        public:
            using value_type = T;
            using str_t = base::String<T,init_size>;
            using loop_t = base::Array<ull,init_size>;
            using loop_node_t = base::Array<node_t*,init_size>;
            using loop_all_str_t = base::Array<str_t,init_size>;
            node_t root;
            TrieTree(){};
            inline bool find(const str_t& fstr){
                ull pos = 0;
                ull msize = fstr.data.size();
                node_t* next_node = &root;
                while (next_node->is_no_end() && (pos < msize)){
                    next_node = next_node->find(fstr[pos]);
                    ++pos;
                    if (next_node)continue;
                    return false;
                };
                if (msize != pos || !next_node->is_str)return false;
                return true;
            };
            inline node_t* find_prefix(const str_t& fstr){
                ull pos = 0;
                ull msize = fstr.data.size();
                node_t* next_node = &root;
                while (next_node->is_no_end() && (pos < msize)){
                    next_node = next_node->find(fstr[pos]);
                    ++pos;
                    if (next_node)continue;
                    return nullptr;
                };
                if (msize != pos)return nullptr;
                return next_node;
            };
            inline node_t* search(node_t* beg_node,const T& val){
                if (!beg_node)return nullptr;
                if (val == beg_node->type_val)return beg_node;
                node_t* ptr = nullptr;
                for (auto& itr : beg_node->next_nodes)if (ptr = search(&itr,val))return ptr;
                return nullptr;
            };
            inline void insert(const str_t& fstr){
                ull msize = fstr.data.size();
                ull idx = 0;
                node_t* bnode = &this->root;
                node_t* last_node = bnode;
                while (idx < msize){
                    last_node = bnode;
                    bnode = bnode->find(fstr[idx]);
                    if (!bnode)bnode = last_node->insert_node(fstr[idx]);
                    ++idx;
                };
                bnode->is_str = true;
            };
            inline bool del_str(const str_t& fstr){
                if (!this->find(fstr))return false;
                if (fstr.data.size() == 1){
                    this->root.find(fstr[0])->is_str = false;
                    return true;
                };
                ull idx = 0;
                node_t* fptr = &this->root;
                base::Array<node_t*,init_size>tmp_vec;
                while (fptr = this->search(fptr,fstr[idx])){
                    ++idx;
                    tmp_vec.push(fptr);
                };
                idx = tmp_vec.size() - 1;
                for (;idx > 0;--idx)if (tmp_vec[idx]->next_nodes.size() > 1)break;
                node_t* tmp = tmp_vec[idx];
                if (!idx){
                    tmp_vec.last()->is_str = false;
                    goto step;
                };
                tmp->next_nodes.erase(tmp->find(((node_t*)tmp_vec[idx + 1])->type_val) - tmp->next_nodes.begin());
            step:
                return true;
            };
            bool _go_back(loop_node_t& path_array,node_t*& last_node){
                if (path_array.size() == 1)return false;
                last_node = *path_array.pop();
                while ((path_array.size() > 1) && ((*path_array.last())->next_nodes.size() == 1))last_node = *path_array.pop();
                return true;
            };
            inline bool _loop(node_t* node,loop_node_t& path_array){
                node_t* tnode = node;
                if (!path_array.size()){
                    path_array.push(tnode);
                    while (tnode->is_no_end() && !tnode->is_str){
                        tnode = &tnode->next_nodes[0];
                        path_array.push(tnode);
                    };
                    return true;
                };
                tnode = *path_array.last();
                if (tnode->is_no_end()){
                    tnode = &tnode->next_nodes[0];
                    path_array.push(tnode);
                    while (tnode->is_no_end() && !tnode->is_str){
                        tnode = &tnode->next_nodes[0];
                        path_array.push(tnode);
                    };
                    return true;
                } else{
                    node_t* last_node = nullptr;
                    node_t* lastnode = nullptr;
                    while (_go_back(path_array,last_node)){
                        lastnode = *path_array.last();
                        if (lastnode->next_nodes.size() > 1){
                            if (last_node != lastnode->next_nodes.last()){
                                path_array.push(last_node + 1);
                                return this->_loop(node,path_array);
                            };
                        };
                    };
                    if (path_array.size() == 1){
                        if (last_node != (*path_array[0]).next_nodes.last()){
                            path_array.push(last_node + 1);
                            return this->_loop(node,path_array);
                        };
                    };
                    return false;
                };
                return true;
            };
            inline str_t _build_str(loop_node_t& path_array){
                ull idx = 1;
                ull msize = path_array.size();
                str_t tmp;
                while (idx < msize)tmp.data.push(path_array[idx++]->type_val);
                return tmp;
            };
            inline str_t get_next(const str_t& prefix_str,loop_node_t& path_array,node_t* f_node = nullptr){
                node_t* snode = f_node;
                if (!snode)snode = this->find_prefix(prefix_str);
                if (!snode)return "";
                if (this->_loop(snode,path_array))return this->_build_str(path_array);
                return "";
            };
            inline void get_next_all(const str_t& prefix_str,loop_all_str_t& str_array){
                loop_node_t path_array;
                node_t* snode = this->find_prefix(prefix_str);
                if (!snode)return;
                while (this->_loop(snode,path_array))str_array.push(this->_build_str(path_array));
            };
            inline node_t* find_suffix_first(const str_t& suffix_str,node_t* base_node){
                //TODO:寻找第一个符合后缀的节点
                return nullptr;
            };
        };
        template<typename T>using trie_tree = TrieTree<T>;
        //using trie = KUR::base::trie_tree_types<char>;
        template<typename T,ull init_size = 0x10>struct trie_tree_types{ //recommendation
        public:
            using tree_t = typename base::TrieTree<T,init_size>;
            using node_t = typename tree_t::node_t;
            using value_type = typename tree_t::value_type;
            using str_t = typename tree_t::str_t;
            using loop_t = typename tree_t::loop_t;
            using loop_node_t = typename tree_t::loop_node_t;
            using loop_all_str_t = typename tree_t::loop_all_str_t;
        };
        template<typename T,ull init_size = 0x10>class trie_ac_node{
        public:
            using value_type = T;
            using _type_node = typename base::Array<base::trie_ac_node<T,init_size>,init_size>;
            _type_node next_nodes;
            T type_val = T(0);
            bool is_str = false;
            trie_ac_node* fail_node_ptr = nullptr;
            trie_ac_node(const T& val):type_val(val){};
            trie_ac_node(){};
            inline trie_ac_node* find(const T& val){
                for (auto& itr : next_nodes)if (itr.type_val == val)return &itr;
                return nullptr;
            };
            inline trie_ac_node* insert_node(const T& val){
                this->next_nodes.push(trie_ac_node(val));
                return this->next_nodes.last();
            };
            inline bool is_no_end(){
                return next_nodes.size();
            };
        };
        //未完成
        template<typename T,ull init_size = 0x10>class Aho_Corasick:public TrieTree<T,init_size,trie_ac_node<T,init_size>>{
        public:
            using _type = Aho_Corasick;
            using node_t = trie_ac_node<T,init_size>;
            using value_type = T;
            using str_t = base::String<T,init_size>;
            using loop_t = base::Array<ull,init_size>;
            using loop_node_t = base::Array<node_t*,init_size>;
            using loop_all_str_t = base::Array<str_t,init_size>;
            Aho_Corasick(){};
            inline void update(loop_node_t& path_array){
                str_t fstr = this->_build_str(path_array);
                base::reverse<str_t::value_type>(fstr.begin(),fstr.end());
                node_t* fnode = nullptr;
                while (!fnode && fstr.data.pop())fnode = this->find_suffix_first(fstr,&this->root);
                if (!fnode)fnode = &this->root;
                ((node_t*)(*path_array.last()))->fail_node_ptr = fnode;
            };
            inline void finish(){
                loop_node_t path_array;
                while (this->_loop(&this->root,path_array)){
                    loop_node_t tmp = path_array;
                    while (tmp.size()){
                        this->update(tmp);
                        tmp.pop();
                    };
                };
            };
            inline ull check(const str_t& fstr){//TODO:匹配字符串
                return -1;
            };
        };
        template<typename T>using aho_corasick = Aho_Corasick<T>;
    };
    using KUR::base::vector;
    using KUR::base::string;
    using KUR::base::wstring;
    using KUR::base::queue;
};
namespace kur = KUR;