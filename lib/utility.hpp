#pragma once
#include"_iostream.hpp"
#include"_string.hpp"
#include"base.hpp"
#include <cstdint>
#include<initializer_list>
namespace KUR{
    namespace base{
        using sbyte = std::int8_t;
        using sword = std::int16_t;
        using sdword = std::int32_t;
        using sqword = std::int64_t;
        using byte = std::uint8_t;
        using word = std::uint16_t;
        using dword = std::uint32_t;
        using qword = std::uint64_t;
    }
    using fp32 = float;
    using fp64 = double;
    using byte1 = base::sbyte;
    using byte2 = base::sword;
    using byte4 = base::sdword;
    using byte8 = base::sqword;
    using ubyte1 = base::byte;
    using ubyte2 = base::word;
    using ubyte4 = base::dword;
    using ubyte8 = base::qword;
    inline byte1 tohex(char c){
        if (c >= '0' && c <= '9')return c - '0';
        if ((c >= 'A' && c <= 'F'))return c - 'A' + 10;
        if ((c >= 'a' && c <= 'f'))return c - 'a' + 10;
        return 0;
    };
    template<typename T>inline void _Copy_From(const T& _Val,const base::ull N,byte1* _data){
        base::ull _Len = base::minimum(N,sizeof(T));
        base::ull _Idx = -1;
        byte1* _Ptr = (byte1*)(&_Val);
        while ((++_Idx) < _Len)_data[_Idx] = _Ptr[_Idx];
    };
    template<base::ull N,typename Ty = void>class ByteN{
    private:
        byte1 _data[N] = {0};
    public:
        template<typename T>inline void operator=(const T& _Val){
            KUR::_Copy_From<T>(_Val,N,_data);
        };
        constexpr static const base::ull length = N;
        inline byte1* get_bytes(){
            return _data;
        };
    };
    template<>class ByteN<0,void*>{
    private:
        byte1 _data;
    public:
        inline byte1* get_bytes(){
            return &_data;
        };
        template<typename T>inline void operator=(const T& _Val){
            KUR::_Copy_From<T>(_Val,-1,&_data);
        };
    };
    template<typename T>class ByteArray{
    public:
        using _type = T;
        using _base_byte = base::conditional_t<base::is_unsigned_v<T>,ubyte1,byte1>;
        constexpr static const base::ull length = sizeof(T);
        T data;
        inline _base_byte* begin(){
            return static_cast<_base_byte*>(&data);
        };
        inline _base_byte* end(){
            return static_cast<_base_byte*>(&data + 1);
        };
        template<typename Ty = _base_byte>inline Ty& refbytes(const base::ull _offset,const base::ull _base_offset = 0){
            return (Ty&)(*((Ty*)((_base_byte*)&data + _base_offset) + _offset));
        };
        inline auto& operator[](const base::ull idx){
            return this->refbytes<_base_byte>(idx);
        };
        template<base::ull _RangeL,base::ull _RangeR>inline ByteN<_RangeR - _RangeL>& range(){
            return this->refbytes<ByteN<_RangeR - _RangeL>>(0,_RangeL);
        };
        inline ByteN<0,void*>& at(base::ull _offset){
            return  this->refbytes<ByteN<0,void*>>(0,_offset);
        };
        inline void print_range_hex(base::ull _range_l,base::ull _range_r){
            std::ios_base::fmtflags original_flags = std::cout.flags();
            std::cout << std::hex;
            _base_byte* _Ptr = (_base_byte*)(&data);
            while (_range_l < _range_r){
                std::cout << static_cast<int>((ubyte1)_Ptr[--_range_r]);
            };
            std::cout.flags(original_flags);
        };
    };
}
