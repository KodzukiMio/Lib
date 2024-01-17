#pragma once
#include <iostream> 
#include <string> 
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <type_traits>
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
    class stack{//����ʱջ,���ڲ�����������(�ɱ����ģ��̫�鷳��...),�������POD����!
    public:
        using size_type = ubyte8;
        using base_type = ubyte1;
        base::vector<base_type>_data;
        stack(const size_type init_size = 0){
            this->_data._construct(init_size);
        };
        stack(const stack&) = delete;
        stack& operator=(const stack&) = delete;
        stack(stack&&) = delete;
        stack& operator=(stack&&) = delete;
        inline void* rsp(){//ջ��ָ��
            return (void*)_data.end();
        };
        inline size_type size(){
            return this->_data.size();
        };
        inline size_type capacity(){
            return this->_data.capacity();
        };
        template<typename T>inline void push(const T _Val){
            static_assert(std::is_pod<T>::value,"Only POD types are allowed.");
            constexpr static const size_type _type_size = sizeof(T);
            size_type overflow = _type_size + this->_data.size();
            while (overflow >= this->_data.capacity())this->_data.expand();
            *(T*)this->rsp() = _Val;
            this->_data._pos += _type_size;
        };
        template<typename T>inline T pop(){
            static_assert(std::is_pod<T>::value,"Only POD types are allowed.");
            constexpr static const size_type _type_size = sizeof(T);
            if (this->_data._pos)this->_data._pos -= _type_size;
            KUR_DEBUG_ASSERT(if (this->size() <= 0){ throw std::runtime_error("out of range !"); };);
            return *(T*)this->rsp();
        };
        inline void pop(){
            if (--this->_data._pos <= 0)throw std::runtime_error("out of range !");
        };
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
        template<typename T>inline void copy_unsafe(const T& _Val)noexcept{
            KUR::_copy_to<T>(_Val,base::npos,_data);
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
        ByteContainer(){};//���ֶ���ʼ��data
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
            return this->refbytes<ByteN<0,void*>>(0,_offset);//����������,ֻ�����ֽڷ�Χ������
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
        ByteContainer& operator=(const T& _Val){
            this->data = _Val;
            return *this;
        };
        void print_hex()noexcept{//С�������
            std::ios_base::fmtflags original_flags = std::cout.flags();
            std::cout << std::hex;
            for (auto i : *this)std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(*i));
            std::cout.flags(original_flags);
        };
        inline void set(const byte1 _Val,const base::ull _first = 0,const base::ull _last = base::npos){//(_first,_last]�ķ�Χ�е�ÿ���ֽ�����Ϊ_Val
            base::R_Iterator<_base_byte> _begin = this->begin();
            base::R_Iterator<_base_byte> _end = this->begin();
            _begin._Pos -= _first;
            _end._Pos -= (_last == base::npos) ? length : _last;
            while (_begin != _end){
                **_begin = _Val;
                _begin.operator++();
            };
        };
    };
};
