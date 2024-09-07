//2023-2024
//适用于 C++17 及以上标准
#pragma once
#ifndef _kurzerbase_
#define _kurzerbase_
#endif
/* #define options:
*  KURZER_ENABLE_EXCEPTIONS
*/
#pragma warning(disable : 26819)
#ifdef NDEBUG //release
#define KURZER_ENABLE_EXCEPTIONS 0
#else //debug
#define KURZER_ENABLE_EXCEPTIONS 1
#endif // NDEBUG
#include<stdexcept>
#include<stdint.h>
#include<immintrin.h>
#ifdef __KUR_ENABLE_STRING             
#define __KUR_STRING
#endif

#ifdef __KUR_ENABLE_IOSTREAM
#define __KUR_IOSTREAM
#endif // ENABLE_IOSTREAM
#define KUR_DEBUG_ASSERT(CODE) if(KURZER_ENABLE_EXCEPTIONS){CODE}
namespace kur{
    typedef unsigned long long ull;
    static_assert(sizeof(void*) > 2,"Does not support less than 32-bit.");
    [[maybe_unused]] constexpr static bool is_x86 = sizeof(void*) == 4;
    [[maybe_unused]] constexpr static bool is_x64 = sizeof(void*) == 8;
    template<typename T>concept TriviallyCopyableAndStandardLayout = std::is_trivially_copyable_v<T> && std::is_standard_layout_v<T>;
    namespace base{
        typedef unsigned long long ull;
        //x64-release模式下平均性能为std::memcpy的93%.
        void* memcpy(void* _Dst,const void* _Src,ull _ByteSize){
            if (!_ByteSize || !_Dst || !_Src)return nullptr;
            ull _D_adr = (ull)_Dst;
            ull _S_adr = (ull)_Src;
            constexpr static const ull _Ofx = sizeof(ull);
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
        //x64-release下平均性能为std::memcmp的90%.
        inline int memcmp(const void* ls,const void* rs,ull count){
            const unsigned char* l8p = (const unsigned char*)ls,* r8p = (const unsigned char*)rs;
            for (ull rem = count % sizeof(ull); rem > 0; ++l8p,++r8p,--rem)if (*l8p != *r8p) return *l8p - *r8p;
            const ull* lptr = (ull*)(l8p),* rptr = (ull*)(r8p);
            for (ull batch = count / sizeof(ull); batch > 0; ++lptr,++rptr,--batch)if (*lptr != *rptr){
                l8p = (const unsigned char*)lptr,r8p = (const unsigned char*)rptr;
                for (ull ofx = 0; ofx < sizeof(ull); ++ofx,++l8p,++r8p)if (*l8p != *r8p) return *l8p - *r8p;
            }
            return 0;
        }
        //x64-release下平均性能为std::memcmp的108%.需要avx2指令集.
        int memcmp_avx2(const void* ptr1,const void* ptr2,size_t num){
            const uint8_t* p1 = static_cast<const uint8_t*>(ptr1),* p2 = static_cast<const uint8_t*>(ptr2);
            while (num >= 32){
                if (_mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_loadu_si256((__m256i*)p1),_mm256_loadu_si256((__m256i*)p2))) != -1)
                    for (int i = 0; i < 32; ++i,++p1,++p2)if (*p1 != *p2)return *p1 - *p2;
                p1 += 32,p2 += 32,num -= 32;
            }
            while (num > 0){
                if (*p1 != *p2)return *p1 - *p2;
                ++p1,++p2,--num;
            }
            return 0;
        }
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
        template <typename T,typename... Args>auto test_constructor(int) -> decltype(T(base::declval<Args>()...),base::true_type{}){
            return {};
        };
        template <typename T,typename... Args>base::false_type test_constructor(...){
            return {};
        };
        template <typename T,typename... Args>struct has_constructor{
            static constexpr bool value = decltype(base::test_constructor<T,Args...>(0))::value;
        };
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
        template<typename Ty>inline Ty rol(Ty _Val,unsigned int _Shift){
            static_assert(base::is_integral_v<Ty>,"Ty must be integral type.");
            constexpr static const unsigned int _NumBits = sizeof(Ty) << 3;
            _Shift %= _NumBits;
            if (!_Shift) return _Val;
            return (_Val << _Shift) | (_Val >> (_NumBits - _Shift));
        };
        template<typename Ty>inline Ty ror(Ty _Val,unsigned int _Shift){
            static_assert(base::is_integral_v<Ty>,"Ty must be integral type.");
            constexpr static const unsigned int _NumBits = sizeof(Ty) << 3;
            _Shift %= _NumBits;
            if (!_Shift) return _Val;
            return (_Val >> _Shift) | (_Val << (_NumBits - _Shift));
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
            return !*_Fstr;
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
            inline T& operator*(){
                return *this->_Pos;
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
        uint64_t MurmurHash64(const void* key,int32_t len,uint32_t seed = 0xfabb2ead){//0xfabb2ead -> fab bread
            const uint64_t m = 0xc6a4a7935bd1e995;
            const int32_t r = 47;
            uint64_t h = seed ^ (len * m);
            const uint64_t* data = (const uint64_t*)key;
            const uint64_t* end = data + (len >> 3);
            while (data != end){
                uint64_t k = *data++;
                k *= m;
                k ^= k >> r;
                k *= m;
                h ^= k;
                h *= m;
            }
            const unsigned char* data2 = (const unsigned char*)data;
            switch (len & 7){
                case 7:
                    h ^= uint64_t(data2[6]) << 48;
                case 6:
                    h ^= uint64_t(data2[5]) << 40;
                case 5:
                    h ^= uint64_t(data2[4]) << 32;
                case 4:
                    h ^= uint64_t(data2[3]) << 24;
                case 3:
                    h ^= uint64_t(data2[2]) << 16;
                case 2:
                    h ^= uint64_t(data2[1]) << 8;
                case 1:
                    h ^= uint64_t(data2[0]);
                    h *= m;
            };
            h ^= h >> r;
            h *= m;
            h ^= h >> r;
            return h;
        }
        template<typename T> class hash{
        public:
            void* address = nullptr;
            int32_t length = 0;
            inline hash(T* _address,uint64_t _len):address((void*)_address){
                if (!(_address && _len))throw std::runtime_error("error argument value.");
                if (_len > INT32_MAX)_len %= INT32_MAX;
                this->length = (int32_t)_len;
            };
            inline uint64_t get(){
                return base::MurmurHash64(this->address,this->length);
            };
        };
        template<typename T>class has_hash_value{
        private:
            template<typename U,uint64_t(U::*)() const> struct SFINAE{};
            template<typename U> static char Test(SFINAE<U,&U::hash_value>*){ return 0; };
            template<typename U> static int Test(...){ return 0; };
        public:
            static constexpr bool value = sizeof(Test<T>(0)) == 1;
        };
        template<typename T>class iterator{
        public:
            T* _begin = nullptr;
            T* _end = nullptr;
            T* _pos = nullptr;
            iterator(T* begin_,T* end_){
                this->_begin = begin_;
                this->_end = end_;
                this->_pos = this->_begin;
            };
            iterator(const iterator& other){
                this->_begin = other._begin;
                this->_end = other._end;
                this->_pos = other._pos;
            };
            inline T* begin(){
                return this->_begin;
            };
            inline T* end(){
                return this->_end;
            };
            inline T& operator*(){
                return *this->_pos;
            };
            inline iterator& operator++(){
                KUR_DEBUG_ASSERT(if (this->_pos >= this->_end){ throw std::runtime_error("list_iterator<T> bounds checking error !"); };);
                ++this->_pos;
                return *this;
            };
            inline iterator operator++(int){
                KUR_DEBUG_ASSERT(if (this->_pos >= this->_end){ throw std::runtime_error("list_iterator<T> bounds checking error !"); };);
                iterator tmp(*this);
                ++this->_pos;
                return tmp;
            };
            inline iterator& operator--(){
                KUR_DEBUG_ASSERT(if (this->_pos < this->_begin){ throw std::runtime_error("list_iterator<T> bounds checking error !"); };);
                --this->_pos;
                return *this;
            };
            inline iterator operator--(int){
                KUR_DEBUG_ASSERT(if (this->_pos < this->_begin){ throw std::runtime_error("list_iterator<T> bounds checking error !"); };);
                iterator tmp(*this);
                --this->_pos;
                return tmp;
            };
            inline bool operator!=(const iterator& rv){
                return this->_pos != rv._pos;
            };
            inline bool operator==(const iterator& rv){
                return this->_pos == rv._pos;
            };
            inline iterator& operator=(const iterator& rv){
                if (this == &rv)return *this;
                this->_begin = rv._begin;
                this->_end = rv._end;
                this->_pos = rv._pos;
                return *this;
            };
        };
        template<typename T>class list_node{
        public:
            bool is_end = false;
            list_node* next = nullptr;
            list_node* pre = nullptr;
            T data;
            list_node(){};
            list_node(const T& val){
                this->data = val;
            };
        };
        template<typename T>class list_iterator{
        public:
            using unit_type = list_node<T>;
            unit_type* _pos;
            bool is_reverse = false;
            list_iterator(unit_type* pos,bool _reverse = false): _pos(pos),is_reverse(_reverse){};
            list_iterator(const unit_type* pos,bool _reverse = false): _pos(const_cast<unit_type*>(pos)),is_reverse(_reverse){};
            inline T& operator*(){
                return this->_pos->data;
            };
            inline bool operator!=(const list_iterator& rv){
                return this->_pos != rv._pos;
            };
            inline bool operator==(const list_iterator& rv){
                return this->_pos == rv._pos;
            };
            inline list_iterator& operator=(const list_iterator& rv){
                if (this == &rv)return *this;
                this->_pos = rv._pos;
                return *this;
            };
            inline list_iterator& operator++(){
                if (this->is_reverse)this->_pos = this->_pos->pre;
                else this->_pos = this->_pos->next;
                return *this;
            };
            inline list_iterator& operator--(){
                if (this->is_reverse)this->_pos = this->_pos->next;
                else this->_pos = this->_pos->pre;
                return *this;
            };
            inline list_iterator operator++(int){
                list_iterator tmp(*this);
                if (this->is_reverse)this->_pos = this->_pos->pre;
                else this->_pos = this->_pos->next;
                return tmp;
            };
            inline list_iterator operator--(int){
                list_iterator tmp(*this);
                if (this->is_reverse)this->_pos = this->_pos->next;
                else this->_pos = this->_pos->pre;
                return tmp;
            };
        };

        template<typename T>class list{
        public:
            using node_type = list_node<T>;
            using iterator_type = list_iterator<T>;
            node_type* address = nullptr;
            node_type* last = nullptr;
            node_type end_node;
            uint64_t _size = 0;
            list(): address(nullptr),last(&end_node),_size(0){
                end_node.pre = nullptr;
                end_node.next = nullptr;
                end_node.is_end = true;
            };
            iterator_type begin(){
                return  iterator_type(this->address);
            };
            const iterator_type begin() const{
                return iterator_type(this->address);
            }
            iterator_type end(){
                return  iterator_type(&end_node);
            }
            const iterator_type end()const{
                return  iterator_type(&end_node);
            }
            iterator_type rbegin(){
                return iterator_type(this->last,true);
            }
            iterator_type rend(){
                return iterator_type(&end_node,true);
            }
            void clear(){
                this->_release();
                this->_size = 0;
                this->address = nullptr;
                this->end_node.next = nullptr;
                this->end_node.pre = nullptr;
                this->last = &end_node;
            }
            node_type* push_back(const T& val){//性能为std::list的82%
                if (this->address){
                    node_type* new_last = new node_type(val);
                    new_last->next = &end_node;
                    new_last->pre = last;
                    last->next = new_last;
                    last = new_last;
                } else{
                    address = new node_type(val);
                    address->next = &end_node;
                    address->pre = &end_node;
                    last = address;
                };
                ++_size;
                return this->last;
            };
            inline uint64_t size()const{
                return this->_size;
            };
            inline node_type* get(const uint64_t pos){
                uint64_t _pos = pos;
                node_type* tmp = this->address;
                while (tmp && _pos--)tmp = tmp->next;
                return tmp;
            }
            void erase(const node_type* tmp){
                if (!tmp)throw std::runtime_error("ptr is nullptr !.");
                if (this->_size > 1){
                    if (tmp == this->address){
                        node_type* tmp_ = this->address;
                        this->address = this->address->next;
                        this->address->pre = nullptr;
                        --this->_size;
                        delete tmp_;
                        return;
                    }
                    tmp->next->pre = tmp->pre;
                    tmp->pre->next = tmp->next;
                    delete tmp;
                } else{
                    delete this->address;
                    this->end_node.pre = nullptr;
                    this->address = nullptr;
                }
                --this->_size;
            }
            inline void erase(const uint64_t pos){
                if (pos >= this->_size)throw std::runtime_error("index out of range.");
                this->erase(this->get(pos));
            }
            inline bool empty(){
                return !this->_size;
            };
            void insert_back(node_type* tmp,const T& val){
                if (this->_size == 0){
                    this->push_back(val);
                    return;
                }
                if (tmp){
                    node_type* nxt = tmp->next;
                    if (nxt == &this->end_node || nxt == nullptr){
                        this->push_back(val);
                        return;
                    }
                    node_type* new_node = new node_type(val);
                    tmp->next = new_node;
                    new_node->pre = tmp;
                    new_node->next = nxt;
                    nxt->pre = new_node;
                } else throw std::runtime_error("list<T> out of range.");
                ++this->_size;
            };
            void insert_back(const uint64_t pos,const T& val){
                this->insert_back(this->get(pos),val);
            };
            void traverse(void(*_Pfn)(node_type&)){
                node_type* tmp = this->address;
                while (tmp && tmp != &this->end_node){
                    _Pfn(*tmp);
                    tmp = tmp->next;
                }
            };
            void _release(){
                while (address != nullptr && address != &end_node){
                    auto temp = address;
                    address = address->next;
                    delete temp;
                }
            }
            ~list(){
                this->_release();
            };
        };
        template<typename Type,ull init_size = 0x10>class Array{
        public:
            bool _allow_del = true;
            Type* _chunk = nullptr;
            ull _size = 0;
            ull _pos = 0;
            inline void expand(bool _copy = true,bool _free = true){
                if (!_chunk){
                    this->create(init_size);
                    return;
                };
                Type* temp = _chunk;
                ull _esize = _size ? (_size << 1) : 2;
                this->_chunk = _malloc(_esize);
                if (!this->_chunk){
                    this->_chunk = nullptr;
                    delete[] temp;
                    throw std::runtime_error("bad alloc !");
                };
                if (_copy)base::memcpy(_chunk,temp,_size * sizeof(Type));
                this->_size = _esize;
                if (_free)delete[] temp;
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
                if (_chunk && _pos > 0)for (ull i = 0; i < _pos; ++i)_chunk[i] = other._chunk[i];
                this->_allow_del = other._allow_del;
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
                    base::memcpy(_chunk,other._chunk,_pos * sizeof(Type));
                };
                return *this;
            };
            [[nodiscard]] inline Type* _malloc(ull nsize)noexcept{
                if (!nsize)return nullptr;
                return new Type[nsize];
            };
            inline Type& operator[](ull index){//性能为std::vector的108.7%
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
            inline void push_back(const Type value){//性能为std::vector的111.8%
                if (this->_pos >= this->_size)expand();
                *(this->_chunk + _pos) = value;
                ++_pos;
            };
            inline bool empty(){
                return !this->_pos;
            }
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
            inline Type* last_()noexcept{
                KUR_DEBUG_ASSERT(if (!_pos){ throw std::runtime_error("Array<T> out of range !"); };);
                return this->_chunk + _pos - 1;
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
            inline Type* erase(ull index){//性能为std::vector<T>::erase的292.2%(3倍)
                KUR_DEBUG_ASSERT(if (index >= _pos){ throw std::runtime_error("Array<T> out of range !"); };);
                if constexpr (std::is_trivially_destructible<Type>::value)base::memcpy(this->_chunk + index,this->_chunk + index + 1,(this->_pos - index) * sizeof(Type));
                else for (ull idx = index;idx < this->_pos;++idx)this->_chunk[idx] = this->_chunk[idx + 1];
                --_pos;
                return _chunk + index;
            };
            inline void insert(const ull index,const Type& value){//性能为std::vector<T>::insert的101.3%
                KUR_DEBUG_ASSERT(if (index >= _pos){ throw std::runtime_error("Array<T> out of range !"); };);
                if (this->_pos + 1 >= this->_size)expand();
                if constexpr (std::is_trivially_destructible<Type>::value)std::memmove(this->_chunk + index + 1,this->_chunk + index,(this->_pos - index) * sizeof(Type));
                else for (ull idx = _pos; idx > index; --idx)this->_chunk[idx] = this->_chunk[idx - 1];
                *(this->_chunk + index) = value;
                ++_pos;
            }
            inline ull size()const noexcept{ return this->_pos; };
            inline ull capacity()noexcept{ return this->_size; };
            inline Type* get_data()noexcept{ return this->_chunk; };
            inline void init(Type _Init_Val,ull _Size = 0)noexcept{
                if (!this->_chunk)this->create(_Size ? _Size : 0x10);
                ull _Len = this->capacity();
                if (_Size)_Len = _Size;
                for (ull i = 0; i < _Len; ++i)*(this->_chunk + i) = _Init_Val;
                this->_pos = this->_size;
            };
            inline Type* find(const Type& value)noexcept{//对于POD类型,查找速度是std::find的2170%,其余则性能相同
                for (const Type* beg = this->_chunk,*ends = this->_chunk + _pos;beg != ends;++beg)
                    if constexpr (std::is_trivially_destructible<Type>::value)if (base::memcmp(beg,&value,sizeof(Type)))return (Type*)beg;
                    else if (*beg == value)return (Type*)beg;
                return end();
            };
            inline ull get_offset(const Type* value)noexcept{
                return value - this->_chunk;
            };
            inline ull find_idx(const Type& value)noexcept{
                Type* _frsl = this->find(value);
                if (_frsl == this->end())return -1;
                return _frsl - this->begin();
            };
            inline uint64_t hash_value()const{
                return (base::hash<Type>(this->_chunk,this->_pos * sizeof(Type))).get();
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
            inline String& operator+=(const Tchar* tch){//性能是std::string的104.5%
                this->write(tch);
                return *this;
            }
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
                const Tchar* data = str.get_data();
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
            inline uint64_t hash_value()const{
                return this->data.hash_value();
            };
            inline bool operator==(const String& str)const{
                return this->data._size == str.data._size && !base::memcmp(this->data._chunk,str.data._chunk,this->data._size * sizeof(Tchar));
            }
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
        template<typename Ty1,typename Ty2>class simple_pair{
        public:
            static_assert(base::disjunction_v<base::has_constructor<Ty1>,base::has_constructor<Ty2>>,"Ty1 and Ty2 require default constructor.");
            Ty1 first;
            Ty2 second;
            simple_pair():first(),second(){};
            simple_pair(Ty1& _first,Ty2& _second):first(_first),second(_second){};
            simple_pair(const simple_pair& _pair){
                this->first = _pair.first;
                this->second = _pair.second;
            }
        };
        template<typename Ty1,typename Ty2>using pair = simple_pair<Ty1,Ty2>;
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
        template<typename T>class heap{
        public:
            base::Array<T,0>_data;
            bool(*_Pfn)(const T&,const T&);
            using base_type = typename base::remove_cv_t<T>;
            heap(bool(*_CmpFn)(const base_type&,const base_type&),ull init_size = 0x10){
                this->_data.create(init_size + 1);
                this->_data._pos = 1;//第一个元素置空
                this->_Pfn = _CmpFn;
            }
            inline ull size(){
                return this->_data.size() - 1;
            }
            inline const base_type& peek(){
                return this->_data[1];
            }
            inline bool is_empty(){
                return !this->size();
            }
            inline void clear(){
                this->_data.clear();
                this->_data._pos = 1;
            }
            inline heap<T>& insert(T val){
                this->_data.push_back(val);
                ull _Pos = this->size();
                while (_Pos > 1){
                    const ull _Half_Pos = _Pos >> 1;
                    if (this->_Pfn(this->_data[_Half_Pos],val))break;
                    base::swap(this->_data[_Pos],this->_data[_Half_Pos]);
                    _Pos = _Half_Pos;
                }
                return *this;
            }
            inline T get(){
                if (this->is_empty())throw std::runtime_error("heap is empty");
                const T ret = this->_data[1];
                this->_data[1] = *this->_data.pop();
                const ull size = this->size();
                ull now = 1,next = 2;
                while (next <= size){
                    if (next < size && this->_Pfn(this->_data[next + 1],this->_data[next]))++next;
                    if (this->_Pfn(this->_data[next],this->_data[now]))base::swap(this->_data[next],this->_data[now]);
                    else break;
                    now = next;
                    next = (now << 1);
                }
                return ret;
            };
        };
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
            template<typename...Args>inline Node* insert_node(Node* parent_node,Args... arg){//平均每个节点占用314 Bytes,可以在构造时将init_size设置为较小的数值来减少初始大小.例如 using trees = kur::base::tree_types<int,2>;//->131 Bytes
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
            inline void traverse(Node* node,void(*_Pfn)(Node*)){//void _Pfn(kur::base::_tree_search_t<T>* node)
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
    };
    using kur::base::vector;
    using kur::base::string;
    using kur::base::wstring;
    using kur::base::queue;
};