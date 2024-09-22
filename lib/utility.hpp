#pragma once
#include <iostream> 
#include <string> 
#include <cstdint>
#include <iomanip>
#include <type_traits>
#include <functional>
#define __KUR_ENABLE_IOSTREAM 1//用于开启base.hpp对stl的支持
#define __KUR_ENABLE_STRING 1//同上
#include "base.hpp"
namespace kur{
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
    using namespace base;
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
        )else base::memcpy(_Data,&_Val,base::minimum(N,sizeof(T)));
    };
    //此类大多时候并不用于储存,而是用于类型转换操作数据;
    template<base::ull N,typename Ty = void>class ByteN{//静态范围
    private:
        byte1 _data[N] = {0};//储存和范围表示
    public:
        template<typename T>inline void operator=(const T& _Val)noexcept{
            kur::_copy_to<T>(_Val,N,_data);
        };
        constexpr static const base::ull length = N;
        inline byte1* get_bytes()noexcept{
            return _data;
        };
        template<typename T>inline void copy_unsafe(const T& _Val)noexcept{
            kur::_copy_to<T>(_Val,base::npos,_data);
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
            kur::_copy_to<T>(_Val,base::npos,&_data);//base::npos确保始终选择sizeof(T).
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
        inline uint64_t hash_value(){
            return (base::hash<_type>(&this->data,sizeof(this->data))).get();
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
            for (auto i : *this)std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(i));
            std::cout.flags(original_flags);
        };
        inline void set(const byte1 _Val,const base::ull _first = 0,const base::ull _last = base::npos){//(_first,_last]的范围中的每个字节设置为_Val
            base::R_Iterator<_base_byte> _begin = this->begin();
            base::R_Iterator<_base_byte> _end = this->begin();
            _begin._Pos -= _first;
            _end._Pos -= (_last == base::npos) ? length : _last;
            while (_begin != _end){
                *_begin = _Val;
                _begin.operator++();
            };
        };
    };
    //假的mov to,from
#define MMOVE(to,from)  ((*(kur::ByteN<sizeof(from)>*)to) = (*(kur::ByteN<sizeof(from)>*)from));
#pragma warning(disable : 26495)
    template<typename Key,typename Value>class hash_node_unit{
    public:
        Key first;
        Value second;
        hash_node_unit(){
            first = Key();
            second = Value();
        };
        hash_node_unit(const Key& _Key,const Value& _Val){
            this->first = _Key;
            this->second = _Val;
        };
    };
#pragma warning(default : 26495)
    template<typename Key,typename Value>class bucket_unit{
    public:
        using unit_type = hash_node_unit<Key,Value>;
        using node_type = typename  base::list<hash_node_unit<Key,Value>>::node_type;
        using unit_mul_type = base::vector<node_type*>;
        using element_type = bucket_unit<Key,Value>;
        base::list<unit_type>unit;
        element_type* next = nullptr;
        base::list_node<element_type*>* root = nullptr;
        bucket_unit(){};
        inline node_type* insert(const Key& _Key,const Value& _Val){
            return this->unit.push_back(unit_type(_Key,_Val));
        };
        inline node_type* get_object(const Key& _Key){
            node_type* tmp = this->unit.address;
            while (tmp){
                if (tmp->data.first == _Key)return tmp;
                tmp = tmp->next;
            };
            return nullptr;
        };
        inline unit_mul_type get_mul_object(const Key& _Key){
            node_type* tmp = this->unit.address;
            unit_mul_type munit;
            while (tmp){
                if (tmp->data.first == _Key)munit.push_back(tmp);
                tmp = tmp->next;
            };
            return munit;
        }
        inline Value* get(const node_type* node){
            if (node)return &(node->data.second);
            return nullptr;
        }
        inline Value* get(const Key& _Key){
            node_type* tmp = this->get_object(_Key);
            if (tmp)return &(tmp->data.second);
            return nullptr;
        }
    };
    template<typename Key,typename Value>class bucket_iterator{
    public:
        using unit_type = bucket_unit<Key,Value>;
        using element_type = hash_node_unit<Key,Value>;
        using base_type = typename base::list<unit_type*>;
        using node_type = typename base::list_node<hash_node_unit<Key,Value>>;
        base_type* _src = nullptr;
        unit_type* _pos = nullptr;
        node_type* _tmp = nullptr;
        bucket_iterator(node_type* tmp):_tmp(tmp){};
        bucket_iterator(base_type* src):_src(src),_pos(nullptr),_tmp(nullptr){};//end
        bucket_iterator(base_type* src,unit_type* pos):_src(src),_pos(pos){//begin
            if (pos)_tmp = pos->unit.address;
        };
        inline bool operator!=(const bucket_iterator& rv){
            return !(*this == rv);
        };
        inline bool operator==(const bucket_iterator& rv){
            return this->_tmp == rv._tmp;
        };
        inline bucket_iterator& operator=(const bucket_iterator& rv){
            if (this == &rv)return *this;
            this->_src = rv._src;
            this->_pos = rv._pos;
            this->_tmp = rv._tmp;
            return *this;
        };
        inline element_type& operator*(){
            return this->_tmp->data;
        };
        inline element_type* operator->(){
            return &this->_tmp->data;
        };
        inline bucket_iterator& operator++(){
            if (!this->_tmp->next->is_end){
                this->_tmp = this->_tmp->next;
            } else{
                if (this->_pos && this->_pos->next){
                    this->_pos = this->_pos->next;
                    this->_tmp = this->_pos->unit.address;
                } else{
                    this->_pos = nullptr;
                    this->_tmp = nullptr;
                };
            }
            return *this;
        };
        inline bucket_iterator operator++(int){
            auto temp = *this;
            ++(*this);
            return temp;
        };
    };
    template<typename Key,typename Value>class bucket{
    public:
        using unit_type = bucket_unit<Key,Value>;
        using iterator_type = bucket_iterator<Key,Value>;
        using node_type = typename base::list_node<hash_node_unit<Key,Value>>;
        using unit_mul_type = base::vector<node_type*>;
        using ref_itrnode_type = typename base::list_node<unit_type*>;
        float factor = 0;
        uint64_t limit = 0;
        uint64_t _size = 0;
        unit_type* _last = nullptr;
        base::list<unit_type*>iterator_list;
        base::Array<unit_type>data;
        bucket(const uint64_t default_size = 0x10,const float _factor = 0.7f){
            data.init(unit_type(),default_size);
            this->set_factor(_factor);
        }
        inline iterator_type begin(){
            return iterator_type(&this->iterator_list,this->iterator_list.address->data);
        }
        inline iterator_type end(){
            return iterator_type(&this->iterator_list);
        }
    #pragma warning(disable : 4244)
        void set_factor(const float _factor){
            if (_factor <= 0 || _factor > 1)throw std::runtime_error("error argument value.");
            this->factor = _factor;
            this->limit = this->factor * this->data.capacity();
        }
        template<typename T>inline uint64_t get_pos(const base::String<T>& _Key,bool){
            return (_Key.hash_value() % this->data.capacity());
        }
        template<typename T> inline uint64_t get_pos(const std::basic_string<T>& _Key){
            return(std::hash<std::basic_string<T>>()(_Key) % this->data.capacity());
        }
        template<typename K = Key,typename std::enable_if<std::is_trivially_copyable<K>::value,void>::type* = nullptr>inline uint64_t get_pos(const Key& _Key){
            return (ByteContainer<Key>(_Key).hash_value() % this->data.capacity());
        }
        template<typename K = Key,typename std::enable_if<!std::is_trivially_copyable<K>::value&& base::has_hash_value<K>::value,void>::type* = nullptr>inline uint64_t get_pos(const Key& _Key){
            return _Key.hash_value() % this->data.capacity();
        }
    #pragma warning(default : 4244)
        node_type* set(unit_type* tmp,const Key& _Key,const Value& _Val){
            if (this->_last){
                if (!tmp->next){
                    this->_last->next = tmp;
                    this->_last = tmp;
                    tmp->next = nullptr;
                    this->iterator_list.push_back(tmp);
                    tmp->root = this->iterator_list.last;
                }
            } else{
                this->_last = tmp;
                this->_last->next = nullptr;
                this->iterator_list.push_back(tmp);
                tmp->root = this->iterator_list.last;
            }
            ++this->_size;
            return tmp->insert(_Key,_Val);
        };
        inline unit_type* get_node(const Key& _Key){
            return &this->data[this->get_pos(_Key)];
        };
        inline node_type* set(const Key& _Key,const Value& _Val){
            return this->set(this->get_node(_Key),_Key,_Val);
        };
        inline void erase(unit_type* tmp,node_type* getv){
            if (getv == nullptr)return;
            if (tmp->unit.size() > 1){
                tmp->unit.erase(getv);
            } else{
                if (tmp->root != this->iterator_list.address)tmp->root->pre->data->next = tmp->next;
                this->iterator_list.erase(tmp->root);
                tmp->unit.clear();
                tmp->next = nullptr;
            }
            --this->_size;
        }
        inline void erase(const Key& _Key){
            unit_type* tmp = &this->data[this->get_pos(_Key)];
            if (tmp == nullptr)return;
            node_type* getv = tmp->get_object(_Key);
            this->erase(tmp,getv);
        }
        inline void erase_multiple(const Key& _Key){
            unit_type* tmp = &this->data[this->get_pos(_Key)];
            if (tmp == nullptr)return;
            unit_mul_type getv = tmp->get_mul_object(_Key);
            for (size_t idx = 0;idx < getv._pos;++idx)this->erase(tmp,getv[idx]);
        }
        inline void erase_multiple(const unit_mul_type& unit_mul,const size_t idx){
            if (idx != -1 && idx >= unit_mul.size() || !unit_mul.size())return;
            unit_type* tmp = &this->data[this->get_pos(unit_mul[0]->data.first)];
            if (tmp == nullptr)return;
            if (idx == (size_t)(-1)){
                for (ull index = 0;index < unit_mul._pos;++index)this->erase(tmp,unit_mul[index]);
            } else this->erase(tmp,unit_mul[idx]);
        }
        inline uint64_t size(){
            return this->_size;
        };
        inline Value* get(const Key& _Key){
            return this->data[this->get_pos(_Key)].get(_Key);
        };
        inline node_type* get_object(const Key& _Key){
            return this->data[this->get_pos(_Key)].get_object(_Key);
        };
        inline unit_mul_type get_mul_object(const Key& _Key){
            return this->data[this->get_pos(_Key)].get_mul_object(_Key);
        };
    };
    template<typename Key,typename Value>class unordered_map;
    template<typename Key,typename Value>class mul_map_iterator;
    template<typename Key,typename Value>class mul_iterator{
    public:
        using parent_type = mul_map_iterator<Key,Value>;
        using element_type = hash_node_unit<Key,Value>;
        parent_type* root = nullptr;
        ull offset = 0;
        mul_iterator(){};
        mul_iterator(parent_type* root_,const ull ofx = 0){
            this->root = root_;
            this->offset = ofx;
        };
        mul_iterator(const mul_iterator& other,const ull ofx = 0){
            this->root = other.root;
            this->offset = other.offset + ofx;
        }
        inline bool operator!=(const mul_iterator& rv){
            return !(*this == rv);
        };
        inline bool operator==(const mul_iterator& rv){
            return this->offset == rv.offset && this->root == rv.root;
        };
        inline mul_iterator& operator=(const mul_iterator& rv){
            if (this == &rv)return *this;
            this->root = rv.root;
            this->offset = rv.offset;
            return *this;
        };
        inline element_type& operator*(){
            return this->root->data[this->offset]->data;
        };
        inline element_type* operator->(){
            return &this->root->data[this->offset]->data;
        }
        inline mul_iterator& operator++(){
            ++this->offset;
            return *this;
        };
        inline mul_iterator operator++(int){
            auto temp = *this;
            ++(*this);
            return temp;
        };
    };
    template<typename Key,typename Value>class mul_map_iterator{
    public:
        using node_type = typename  base::list<hash_node_unit<Key,Value>>::node_type;
        using unit_mul_type = base::vector<node_type*>;
        using x_iterator = typename unit_mul_type::itreator;
        using iterator = mul_iterator<Key,Value>;
        kur::unordered_map<Key,Value>* root = nullptr;
        unit_mul_type data;
        mul_map_iterator(){};
        mul_map_iterator(const unit_mul_type&& move_unit){
            data.move_from(move_unit);
        };
        inline ull size(){
            return this->data._pos;
        };
        inline iterator begin(){
            return iterator(this);
        }
        inline iterator end(){
            return iterator(this,data._pos);
        }
        inline iterator erase(const iterator& itr){
            this->root->erase_multiple(this->data,itr.offset);
            return iterator(itr,1);
        }
        inline iterator get(ull idx){
            if (idx >= this->data._pos)throw std::runtime_error("index out of range.");
            return iterator(this,idx);
        }
        inline hash_node_unit<Key,Value>& operator[](const ull idx){
            return this->data[idx]->data;
        };
    };
    /*对于自定义数据结构做为Key的要求:重载==操作符,默认构造函数,带参构造函数,uint64_t hash_value()const函数.
    * 请勿在使用迭代器时插入或删除元素,会使得迭代器失效.
    * 示例:
     class A{public:int x;std::string s;A() = default;A(int y,const std::string& q): x(y),s(q){};
     bool operator==(A& other)const{ return this->x == other.x && this->s == other.s;};
     uint64_t hash_value()const{return kur::ByteContainer<int>(x).hash_value() + std::hash<std::string>()(s);}};
    */
    template<typename Key,typename Value>class unordered_map{
    public:
        using bk_type = bucket<Key,Value>;
        using iterator_type = bucket_iterator<Key,Value>;
        using element_type = typename base::list<hash_node_unit<Key,Value>>::node_type;
        using node_type = typename base::list_node<hash_node_unit<Key,Value>>;
        using unit_mul_type = base::vector<node_type*>;
        using mul_x_iterator = mul_map_iterator<Key,Value>;
        using mul_iterator = typename mul_x_iterator::iterator;
        float factor = 0;//负载因子
        bk_type* _bucket = nullptr;//桶
        unordered_map(const uint64_t base_size = 0x20,const float _factor = 0.7f){
            this->_bucket = new bk_type(base_size,_factor);
            this->factor = _factor;
        };
        inline iterator_type begin(){//迭代器:开始
            return this->_bucket->begin();
        };
        inline iterator_type end(){//迭代器:结束
            return this->_bucket->end();
        };
        void clear(){//清空
            uint64_t _length = this->_bucket->data.capacity();
            delete this->_bucket;
            this->_bucket = new bk_type(_length,this->factor);
        };
        void expand(){//扩容
            bk_type* tmp = this->_bucket;
            this->_bucket = new bk_type(tmp->data.capacity() << 1,this->factor);
            for (auto& itr : *tmp)this->_bucket->set(itr.first,itr.second);
            delete tmp;
        };
        inline void rehash(){
            if (this->load_factor() > this->factor)this->expand();
        };
        inline void reserve(uint64_t new_capacity){
            if (new_capacity > this->_bucket->data.capacity()){
                bk_type* tmp = this->_bucket;
                this->_bucket = new bk_type(new_capacity,this->factor);
                for (auto& itr : *tmp)this->_bucket->set(itr.first,itr.second);
                delete tmp;
            };
        };
        inline uint64_t size(){//以使用元素个数
            return this->_bucket->size();
        };
        inline uint64_t capacity(){
            return this->_bucket->_size;
        };
        inline bool is_empty(){//是否为空
            return !this->_bucket->_size;
        };
        inline void set_factor(const float _factor){//设置负载因子,扩容操作在下一次insert前检测触发
            this->_bucket->set_factor(_factor);
            this->factor = _factor;
        };
        inline uint64_t get_limit(){//获取最大负载上限
            return this->_bucket->limit;
        };
        inline float load_factor(){//当前负载率
            return (float)this->size() / (float)this->get_limit();
        };
        inline float bucket_utilization(){//真实空间使用率
            return  (float)this->size() / (float)this->_bucket->data.capacity();
        };
        template<typename... Args>inline void create_multiple(const Key& _Key,const Args&&... _Vals){
            auto tmp = this->_bucket->get_node(_Key);
            while (this->_bucket->_size + sizeof...(_Vals) >= this->_bucket->limit)this->expand();
            (this->_bucket->set(tmp,_Key,base::forward<const Args>(_Vals)),...);
        }
        inline element_type* create(const Key& _Key,const Value& _Val){//插入键值,key->value,value,...
            element_type* tmp = this->_bucket->set(_Key,_Val);
            if (this->_bucket->_size >= this->_bucket->limit)this->expand();
            return tmp;
        };
        inline void erase(const Key& _Key){//删除单个键值对,如果键对应多个值只会删除第一个插入的值
            this->_bucket->erase(_Key);
        };
        inline mul_iterator erase(const mul_iterator& itr){//删除多个键值
            this->erase_multiple(itr.root->data,itr.offset);
            return mul_iterator(itr,1);
        };
        inline element_type* get(const Key& _Key){//不存在时返回nullpter
            return this->_bucket->get_object(_Key);
        };
        inline iterator_type find(const Key& _Key){//不存在时返回尾部迭代器
            element_type* tmp = this->get(_Key);
            if (tmp)return iterator_type(tmp);
            else return this->end();
        };
        inline kur::unordered_map<Key,Value>& insert(const Key& _Key,const Value& _Val){//插入唯一键值
            element_type* tmp = this->get(_Key);
            if (!tmp)this->create(_Key,_Val);
            else tmp->data.second = _Val;
            return *this;
        };
        inline kur::unordered_map<Key,Value>& insert_multiple(const Key& _Key,const Value& _Val){//为键插入多个单值
            this->create(_Key,_Val);
            return *this;
        };
        inline void erase_multiple(const Key& _Key){//删除多个键
            this->_bucket->erase_multiple(_Key);
        };
        inline void erase_multiple(unit_mul_type& unit_mul,const size_t idx){//删除多键迭代器中的对象
            this->_bucket->erase_multiple(unit_mul,idx);
            unit_mul[idx] = nullptr;
        };
        inline typename bk_type::unit_mul_type _get_multiple_unit(const Key& _Key){//内部函数,获取多键原始表示
            return this->_bucket->get_mul_object(_Key);
        };
        inline mul_x_iterator get_multiple(const Key& _Key){//获取多键值迭代器
            mul_x_iterator itr;
            itr.root = this;
            itr.data = this->_bucket->get_mul_object(_Key);
            return itr;
        };
        inline void erase(const mul_x_iterator& mitr){//和erase_multiple一样
            this->_bucket->erase_multiple(mitr.data,-1);
        };
        inline void erase_multiple(const mul_x_iterator& unit_mul){//删除多个键
            this->erase(unit_mul);
        };
        inline Value& operator[](const Key& _Key){//获取单个键值对
            element_type* tmp = this->get(_Key);
            if (tmp)return tmp->data.second;
            else return this->create(_Key,Value())->data.second;
        };
        inline mul_x_iterator operator()(const Key& _Key){//获取多个键值对
            return this->get_multiple(_Key);
        };
        template<typename... Args>inline kur::unordered_map<Key,Value>& operator()(const Key& _Key,const Args&&... _Vals){//多值插入
            ((this->create_multiple(_Key,base::forward<const Args>(_Vals))),...);
            return *this;
        };
        ~unordered_map(){
            if (this->_bucket)delete this->_bucket;
        };
    };
};
