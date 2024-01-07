#pragma once
#include <iostream> 
#include <string> 
#include <cstdint>
#include <cstring>
#include <iomanip>
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
    inline char byte1_to_hexc(byte1 ch){
        if (ch < 10)return ch;
        return ch - 10 + 'A';
    };
    template<typename T>inline void _copy_to(const T& _Val,const base::ull N,byte1* _Data)noexcept{//���ڸ����ڴ�,ע��:������Խ����!
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
    template<base::ull N,typename Ty = void>class ByteN{//��̬��Χ
    private:
        byte1 _data[N] = {0};//����ͷ�Χ��ʾ
    public:
        template<typename T>inline void operator=(const T& _Val)noexcept{
            KUR::_copy_to<T>(_Val,N,_data);
        };
        constexpr static const base::ull length = N;
        inline byte1* get_bytes()noexcept{
            return _data;
        };
    };
    //ByteN<N>�Ϸ�Χ�����ػ�,�����ƹ�ģ������,������û��ʵ������.
    template<>class ByteN<0,void*>{
    private:
        byte1 _data;//���ڻ�ȡByteContainerԭʼoffset��ַ,����ֱ��ʹ��.
    public:
        inline byte1* get_bytes()noexcept{
            return &_data;
        };
        template<typename T>inline void operator=(const T& _Val)noexcept{
            KUR::_copy_to<T>(_Val,base::npos,&_data);//base::nposȷ��ʼ��ѡ��sizeof(T).
        };
    };
    //���ڶ�����T�����ֽڼ���Ĳ���;<T>������ʹ�÷�POD����.
    //��ʵ������С��Tһ��(sizeof(T)==sizeof(ByteContainer<T>))=>true.
    template<typename T>class ByteContainer{
    public:
        using _type = T;
        using _base_byte = base::conditional_t<base::is_unsigned_v<T>,ubyte1,byte1>;//ѡ�����.
        constexpr static const base::ull length = sizeof(T);
        T data;
        ByteContainer(const T& _Val):data(_Val){};
        ByteContainer(){};
        inline _base_byte* _begin()noexcept{
            return (_base_byte*)(&data);
        };
        inline _base_byte* _end()noexcept{
            return (_base_byte*)(&data + 1);
        };
        //_offset��Ty���͵�ƫ����(sizeof(Ty)),_base_offset���ֽ�ƫ����(size=1)
        template<typename Ty = _base_byte>inline Ty& refbytes(const base::ull _offset,const base::ull _base_offset = 0){
            KUR_DEBUG_ASSERT(if (_offset * sizeof(Ty) + _base_offset >= length)throw std::runtime_error("Out of range !"););
            return (Ty&)(*((Ty*)((_base_byte*)&data + _base_offset) + _offset));//�˺������������¶���,ֻ�Ƿ�������ת�������͵�����
        };
        inline auto& operator[](const base::ull idx){
            return this->refbytes<_base_byte>(idx);//����������,ֻ�����ֽڵ�����
        };
        template<base::ull _RangeL,base::ull _RangeR>inline ByteN<_RangeR - _RangeL>& range(){//[_RangeL,_RangeR)
            static_assert(_RangeR <= length,"Out of range !");
            return this->refbytes<ByteN<_RangeR - _RangeL>>(0,_RangeL);//����������,ֻ�����ֽڷ�Χ������
        };
        inline ByteN<0,void*>& at(const base::ull _offset){
            KUR_DEBUG_ASSERT(if (_offset >= length)throw std::runtime_error("Out of range !"););
            return  this->refbytes<ByteN<0,void*>>(0,_offset);//����������,ֻ�����ֽڷ�Χ������
        };
        inline base::R_Iterator<_base_byte> get_itr(){
            return base::R_Iterator<_base_byte>(this->_begin(),this->_end());
        };
        inline base::R_Iterator<_base_byte> begin()noexcept{
            return get_itr();
        };
        inline base::R_Iterator<_base_byte> end()noexcept{
            base::R_Iterator<_base_byte> itr = get_itr();
            itr._Pos = itr.end();
            return itr;
        };
        void print_hex()noexcept{//С�������
            std::ios_base::fmtflags original_flags = std::cout.flags();
            std::cout << std::hex;
            for (auto i : *this)std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(*i));
            std::cout.flags(original_flags);
        };
        inline void set(const byte1 _Val,const base::ull _first = 0,const base::ull _last = base::npos){
            base::R_Iterator<_base_byte> _begin = this->begin();
            base::R_Iterator<_base_byte> _end = this->begin();
            _begin._Pos -= _first;
            _end._Pos -= _last;
            while (_begin != _end){
                **_begin = _Val;
                _begin.operator++();
            };
        };
    };
    /*
    #include "lib/utility.hpp"
    struct Data{
    int q;
    long c;
    };
    int main(){
    kur::ByteContainer<kur::ByteN<sizeof(Data)>>b;
    b.set(0xaf,0,sizeof(Data));
    b.range<1,3>() = 0xabcd;
    kur::byte2 p = 3;
    b.at(p) = (kur::byte1)(0xee);
    b.print_hex();
    Data data = b.refbytes<Data>(0,0);
    std::cout << ':' << std::hex << data.c << ':' << data.q;
    //afafafafeeabcdaf:afafafaf:eeabcdaf
    return 0;
    };
    */
};
