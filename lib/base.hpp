//2023-2024
#pragma once
#ifndef _KURZERbase_
#define _KURZERbase_
#endif
namespace kurzer{

    template<class Tchar> class CharT{
    public:
    #ifdef _XSTRING_
        using char_t = std::basic_string<Tchar,std::char_traits<Tchar>,std::allocator<Tchar>>;
    #endif // _XSTRING_
    #ifdef _IOSTREAM_
        using out_t = std::basic_ostream<Tchar,std::char_traits<Tchar>>;
    #endif // _IOSTREAM_
    };
    //typename kurzer::enable_if<TRUE?>::type*=nullptr
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
        static constexpr bool value = kurzer::is_same<T,char>::value || kurzer::is_same<T,wchar_t>::value || kurzer::is_same<T,char8_t>::value || kurzer::is_same<T,char16_t>::value || kurzer::is_same<T,char32_t>::value;
    };
    //size_t
    typedef unsigned long long ull;

    //_KUR_TEMPLATE_TYPE_IS(T1,T2)
#define _KUR_TEMPLATE_TYPE_IS(Type,Is_Ty)template <typename Type,typename kurzer::enable_if<kurzer::is_same<Type,Is_Ty>::value>::type* = nullptr>
//_KUR_TEMPLATE_T_IS(T2)
#define _KUR_TEMPLATE_T_IS(Is_Ty) _KUR_TEMPLATE_TYPE_IS(T,Is_Ty) 
    namespace algorithm{

    };
    namespace base{

        template<typename Type>class Array{
        public:
            Type* _chunk = 0;
            ull _size = 0;
            ull _pos = 0;
            void expand(){
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
            void create(ull initSize = 0x10){
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
            Type* Malloc(ull nsize){
                if (!nsize)return nullptr;
                return new Type[nsize];
            };
            Type& operator[](ull index){
                return *(this->_chunk + index);
            }
            const Type& operator[](ull index) const{
                return *(this->_chunk + index);
            }
            Type* operator()(ull index){
                if (index < this->_pos){
                    return this->_chunk + index;
                }
                return nullptr;
            };
            const Type* operator()(ull index) const{
                if (index < this->_pos){
                    return this->_chunk + index;
                };
                return nullptr;
            };
            void push(const Type value){
                if (++this->_pos > this->_size){
                    expand();
                };
                *(this->_chunk + _pos - 1) = value;
            };
            void Free(){
                delete[] _chunk;
                _chunk = nullptr;
                _size = 0;
                _pos = 0;
            }
            Type* begin(){ return _chunk; };
            Type* end(){ return this->_chunk + _pos - 1; };
            Type* pop(){
                if (!_pos){
                    return nullptr;
                };
                --_pos;
                return this->_chunk + _pos;
            };
            ull Length()const{ return this->_pos; };
            ~Array(){
                delete[] _chunk;
            };
            ull Size(){ return this->_size; };
            Type* GetData(){ return this->_chunk; };
        };

        template<typename Tchar,ull baseN = 0x10,typename kurzer::enable_if<kurzer::is_character<Tchar>::value>::type* = nullptr>class String{
        public:
            base::Array<Tchar> data;
            String(){};
            String(const Tchar* tch){
                this->Write(tch);
            };
            static ull strlen(const Tchar* str){
                const Tchar* p = str;
                while (*p){
                    ++p;
                };
                return p - str;
            };
            void Clear(){
                data.Free();
                data.create(baseN);
            };
            Tchar& operator[](const ull pos){
                return data[pos];
            };
            const Tchar& operator[](const ull pos) const{
                return data[pos];
            };
            Tchar* operator()(const ull pos){
                return &data[pos];
            };
            const Tchar* operator()(const ull pos) const{
                return &data[pos];
            };
            Tchar* GetData()const{
                return data._chunk;
            };
            String& Write(const Tchar* tch){
                for (ull i = 0; i < this->strlen(tch); i++){
                    this->data.push(*(tch + i));
                };
                return *this;
            };
            String& Write(const String& str){
                const Tchar* data = str.GetData();
                for (ull i = 0; i < str.data.Length(); i++){
                    this->data.push(*(data + i));
                };
                return *this;
            };
            String& operator=(const Tchar* tch){
                this->Clear();
                this->Write(tch);
                return *this;
            };
            String& operator=(const String& str){
                this->Clear();
                this->Write(str);
                return *this;
            };
            String substr(ull start,ull length){
                String result;
                for (ull i = start; i < start + length; i++){
                    result.data.push(this->data[i]);
                };
                return result;
            };
        #ifdef _IOSTREAM_
            friend kurzer::CharT<Tchar>::out_t& operator<<(kurzer::CharT<Tchar>::out_t& os,const String<Tchar>& str){
                for (ull i = 0; i < str.data.Length(); ++i){
                    os << str[i];
                };
                return os;
            };
        #endif // _IOSTREAM_
        };

        using string = String<char,0x10>;
        using wstring = String<wchar_t,0x10>;

    };

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