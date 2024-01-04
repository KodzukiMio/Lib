#pragma once
#include <iostream> 
#include <string> 
#include <cstdint>
#include <cstring>
#define __KUR_ENABLE_IOSTREAM//���ڿ���base.hpp��stl��֧��
#define __KUR_ENABLE_STRING//ͬ��
#include "base.hpp"
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
    };
    using byte1 = base::sbyte;
    using byte2 = base::sword;
    using byte4 = base::sdword;
    using byte8 = base::sqword;
    using ubyte1 = base::byte;
    using ubyte2 = base::word;
    using ubyte4 = base::dword;
    using ubyte8 = base::qword;
    [[nodiscard]] inline byte1 hex_to_dec(char c){
        if (c >= '0' && c <= '9')return c - '0';
        if ((c >= 'a' && c <= 'f'))return c - 'a' + 10;
        if ((c >= 'A' && c <= 'F'))return c - 'A' + 10;
        KUR_DEBUG_ASSERT(throw std::runtime_error("Error Value !"););
        return 0;
    };
    template<typename T>inline void _copy_to(const T& _Val,const base::ull N,byte1* _Data){//���ڸ����ڴ�,ע��:������Խ����!
        KUR_DEBUG_ASSERT(
            base::ull _Len = base::minimum(N,sizeof(T));
        base::ull _Idx = -1;
        byte1 * _Ptr = (byte1*)(&_Val);
        while ((++_Idx) < _Len)_Data[_Idx] = _Ptr[_Idx];
        )else{
            std::memcpy(_Data,&_Val,base::minimum(N,sizeof(T)));
        };
    };
    //������ʱ�򲢲����ڴ���,������������ת����������;
    //ע��:ʹ��ʱȷ���˽��ڴ沼��,���򲻽���ʹ��.
    template<base::ull N,typename Ty = void>class ByteN{//��̬��Χ
    private:
        byte1 _data[N] = {0};//����ͷ�Χ��ʾ
    public:
        template<typename T>inline void operator=(const T& _Val){
            KUR::_copy_to<T>(_Val,N,_data);
        };
        constexpr static const base::ull length = N;
        inline byte1* get_bytes(){
            return _data;
        };
    };
    //ByteN<N>�Ϸ�Χ�����ػ�,�����ƹ�ģ������,������û��ʵ������.
    //���鲻Ҫֱ��ʹ������ػ��汾,����ȷʵ�����Լ�����ʲô.
    template<>class ByteN<0,void*>{
    private:
        byte1 _data;//���ڻ�ȡByteArrayԭʼoffset��ַ,����ֱ��ʹ��.
    public:
        inline byte1* get_bytes(){
            return &_data;
        };
        template<typename T>inline void operator=(const T& _Val){
            KUR::_copy_to<T>(_Val,base::npos,&_data);//base::nposȷ��ʼ��ѡ��sizeof(T).
        };
    };
    //���ڶ�����T�����ֽڼ���Ĳ���;<T>������ʹ�÷�POD����.
    //��ʵ������С��Tһ��(sizeof(T)==sizeof(ByteArray<T>))=>true.
    template<typename T>class ByteArray{
    public:
        using _type = T;
        using _base_byte = base::conditional_t<base::is_unsigned_v<T>,ubyte1,byte1>;//ѡ�����.
        constexpr static const base::ull length = sizeof(T);
        T data;
        inline _base_byte* begin(){
            return static_cast<_base_byte*>(&data);
        };
        inline _base_byte* end(){
            return static_cast<_base_byte*>(&data + 1);
        };
        //_offset��Ty���͵�ƫ����(sizeof(Ty)),_base_offset���ֽ�ƫ����(size=1)
        template<typename Ty = _base_byte>inline Ty& refbytes(const base::ull _offset,const base::ull _base_offset = 0){
            KUR_DEBUG_ASSERT(if (_offset * sizeof(Ty) + _base_offset >= length)throw std::runtime_error("Out of range !"););
            return (Ty&)(*((Ty*)((_base_byte*)&data + _base_offset) + _offset));//�˺������������¶���,ֻ�Ƿ�������ת�������͵�����
        };
        inline auto& operator[](const base::ull idx){
            return this->refbytes<_base_byte>(idx);//����������,ֻ�����ֽڵ�����
        };
        template<base::ull _RangeL,base::ull _RangeR>inline ByteN<_RangeR - _RangeL>& range(){
            static_assert(_RangeR <= length,"Out of range !");
            return this->refbytes<ByteN<_RangeR - _RangeL>>(0,_RangeL);//����������,ֻ�����ֽڷ�Χ������
        };
        inline ByteN<0,void*>& at(const base::ull _offset){
            KUR_DEBUG_ASSERT(if (_offset >= length)throw std::runtime_error("Out of range !"););
            return  this->refbytes<ByteN<0,void*>>(0,_offset);//����������,ֻ�����ֽڷ�Χ������
        };
        inline void print_range_hex(const base::ull _range_l,base::ull _range_r){//С�������
            std::ios_base::fmtflags original_flags = std::cout.flags();
            std::cout << std::hex;
            _base_byte* _Ptr = (_base_byte*)(&data);
            while (_range_l < _range_r)std::cout << static_cast<byte4>((ubyte1)_Ptr[--_range_r]);
            std::cout.flags(original_flags);
        };
    };
};
