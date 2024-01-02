#pragma once
#include"_iostream.hpp"
#include"_string.hpp"
#include"base.hpp"
#include <cstdint>
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
    using float32 = float;
    using float64 = double;
    using byte1 = base::sbyte;
    using byte2 = base::sword;
    using byte4 = base::sdword;
    using byte8 = base::sqword;
    using ubyte1 = base::byte;
    using ubyte2 = base::word;
    using ubyte4 = base::dword;
    using ubyte8 = base::qword;
    template<typename T>class ByteArray{//max size = 8;
    public:
        using _type = T;
        using _base_byte = base::conditional_t<base::is_unsigned_v<T>,ubyte1,byte1>;
        //static_assert(base::is_integral<T>::value,"Integral type required.");也许需要操作其它类型,例如struct什么的,所以去掉了
        constexpr static const ubyte1 length = sizeof(T);
        T data;
        inline _base_byte* begin(){
            return static_cast<_base_byte*>(&data);
        };
        inline _base_byte* end(){
            return static_cast<_base_byte*>(&data + 1);
        };
        template<typename Ty = _base_byte>inline Ty& get_ref_bytes(const unsigned _idx){
        #ifdef KURZER_ENABLE_EXCEPTIONS
            if (_idx >= length)throw std::runtime_error("out of range !");
        #endif
            return *(Ty*)((_base_byte*)(&data) + _idx);
        };
        inline auto& operator[](const unsigned idx){
            return get_ref_bytes(idx);
        };
        template<typename Ty>inline Ty& refbytes(const unsigned _offset,const unsigned _base_offset = 0){
            static_assert(base::is_integral<Ty>::value,"Byte type required.");
        #ifdef KURZER_ENABLE_EXCEPTIONS
            if (_offset * sizeof(Ty) + _base_offset >= length)throw std::runtime_error("out of range !");
        #endif
            return reinterpret_cast<Ty&>(*((Ty*)((_base_byte*)&data + _base_offset) + _offset));
        };
    };
}
