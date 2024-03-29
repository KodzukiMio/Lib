#pragma once
#include <iostream> 
#include <string> 
#include <cstdint>
#include <iomanip>
#include <type_traits>
#define __KUR_ENABLE_IOSTREAM 1//用于开启base.hpp对stl的支持
#define __KUR_ENABLE_STRING 1//同上
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
    template<typename Ty = void,typename BaseT = Ty,typename Rty = std::conditional_t<std::is_floating_point_v<Ty>,std::conditional_t<sizeof(double) == sizeof(long double),double,long double>,int64_t>>inline Rty sumof(Ty* address,size_t length,size_t offset = sizeof(Ty)){
        static_assert(std::is_pod_v<BaseT>&std::is_pod_v<Ty>&std::is_pod_v<Rty>,"Error Types!");
        if (!length || !address || !offset)throw std::runtime_error("<sumof> error arguments !");
        Rty rax = 0;
        do{
            rax += *(BaseT*)address;
            address = (Ty*)((int8_t*)address + offset);
        } while (--length);
        return rax;
    };
    template<typename T = char>base::String<T> xor_crypt(base::String<T>& src,base::String<T>& key){//简易数据加密(不要用于重要数据)
        T* sbytes = src.data._chunk;
        ull ssize = src.size();
        T* kbytes = key.data._chunk;
        ull ksize = key.size();
        if (!ksize || !ssize || ksize > ssize) throw std::invalid_argument("key or src size must > 0 and key size <= src size.");
        base::String<T> result;
        result.data._construct(ssize);
        result.data._pos = ssize;
        for (ull i = 0; i < ssize; ++i)result[i] = sbytes[i] ^ kbytes[i % ksize];
        return result;
    };
    class stack{//运行时栈(自动扩容),用于递归优化和不定参数传参(可变参数模板太麻烦了...),请勿传入非POD类型!
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
        inline void* _rsp(){ //栈顶指针 + 1
            return (void*)_data.end();
        };
        inline base_type* rsp(){//栈顶指针
            return ((base_type*)_data.end() - 1);
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
            *(T*)this->_rsp() = _Val;
            this->_data._pos += _type_size;
        };
        template<typename T>inline T pop(){
            static_assert(std::is_pod<T>::value,"Only POD types are allowed.");
            constexpr static const size_type _type_size = sizeof(T);
            KUR_DEBUG_ASSERT(if (!this->_data._pos){ throw std::runtime_error("out of range !"); };);
            this->_data._pos -= _type_size;
            return *(T*)this->_rsp();
        };
        inline void pop(){
            if (--this->_data._pos != (size_type)(-1))throw std::runtime_error("out of range !");
        };
    };
    template<typename T>inline void _copy_to(const T& _Val,const base::ull N,byte1* _Data)noexcept{//用于覆盖内存,注意:不进行越界检查!
        KUR_DEBUG_ASSERT(
            base::ull _Len = base::minimum(N,sizeof(T));
        base::ull _Idx = -1;
        byte1 * _Ptr = (byte1*)(&_Val);
        while ((++_Idx) < _Len)_Data[_Idx] = _Ptr[_Idx];
        )else{
            base::memcpy(_Data,&_Val,base::minimum(N,sizeof(T)));
        };
    };
    //此类大多时候并不用于储存,而是用于类型转换操作数据;
    template<base::ull N,typename Ty = void>class ByteN{//静态范围
    private:
        byte1 _data[N] = {0};//储存和范围表示
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
    //ByteN<N>上范围操作特化,用于绕过模板限制,参数并没有实际意义.
    template<>class ByteN<0,void*>{
    private:
        byte1 _data;//用于获取ByteContainer原始offset地址,并不直接使用.
    public:
        inline byte1* get_bytes()noexcept{
            return &_data;
        };
        template<typename T>inline void operator=(const T& _Val)noexcept{
            KUR::_copy_to<T>(_Val,base::npos,&_data);//base::npos确保始终选择sizeof(T).
        };
    };
    //用于对类型T进行字节级别的操作;<T>不建议使用非POD类型.
    //类实例化大小与T一致(sizeof(T)==sizeof(ByteContainer<T>))=>true.
    template<typename T>class ByteContainer{
    public:
        using _type = T;
        using _base_byte = base::conditional_t<base::is_unsigned_v<T>,ubyte1,byte1>;//选择符号.
        constexpr static const base::ull length = sizeof(T);
        T data;
        ByteContainer(const T& _Val):data(_Val){};
        ByteContainer(){};//请手动初始化data
        inline _base_byte* _begin()noexcept{
            return (_base_byte*)(&data);
        };
        inline _base_byte* _end()noexcept{
            return (_base_byte*)(&data + 1);
        };
        //_offset是Ty类型的偏移量(sizeof(Ty)),_base_offset是字节偏移量(size=1)
        template<typename Ty = _base_byte>inline Ty& refbytes(const base::ull _offset,const base::ull _base_offset = 0){
            KUR_DEBUG_ASSERT(if (_offset * sizeof(Ty) + _base_offset >= length)throw std::runtime_error("Out of range !"););
            return (Ty&)(*((Ty*)((_base_byte*)&data + _base_offset) + _offset));//此函数并不创建新对象,只是返回其它转换后类型的引用
        };
        inline auto& operator[](const base::ull idx){
            return this->refbytes<_base_byte>(idx);//不创建对象,只返回字节的引用
        };
        template<base::ull _RangeL,base::ull _RangeR>inline ByteN<_RangeR - _RangeL>& range(){//[_RangeL,_RangeR)
            static_assert(_RangeR <= length,"Out of range !");
            return this->refbytes<ByteN<_RangeR - _RangeL>>(0,_RangeL);//不创建对象,只返回字节范围的引用
        };
        inline ByteN<0,void*>& at(const base::ull _offset){
            KUR_DEBUG_ASSERT(if (_offset >= length)throw std::runtime_error("Out of range !"););
            return this->refbytes<ByteN<0,void*>>(0,_offset);//不创建对象,只返回字节范围的引用
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
        void print_hex()noexcept{//小端序输出
            std::ios_base::fmtflags original_flags = std::cout.flags();
            std::cout << std::hex;
            for (auto i : *this)std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(*i));
            std::cout.flags(original_flags);
        };
        inline void set(const byte1 _Val,const base::ull _first = 0,const base::ull _last = base::npos){//(_first,_last]的范围中的每个字节设置为_Val
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
