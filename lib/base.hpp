#pragma once
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
    typedef unsigned long long ull;
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
            Type* operator[](ull index){
                if (index < this->_pos){
                    return this->_chunk + index;
                }
                return nullptr;
            };
            const Type* operator[](ull index) const{
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
        };
        template<typename Tchar,ull baseN = 0x10>class String{
        public:
            base::Array<Tchar> data;
            String(){};
            String(const Tchar* tch){
                this->Write(tch);
            };
            ull strlen(const Tchar* str){
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
            Tchar operator[](const ull pos){
                return *(data[pos]);
            };
            Tchar* operator()(const ull pos){
                return data[pos];
            };
            Tchar* GetData()const{
                return data._chunk;
            };
            String& Write(const Tchar* tch){
                for (ull i = 0; i < this->strlen(tch); i++){
                    this->data.push(*(tch + i));
                }
                return *this;
            };
            String& Write(const String& str){
                const Tchar* data = str.GetData();
                for (ull i = 0; i < str.data.Length(); i++){
                    this->data.push(*(data + i));
                }
                return *this;
            };
            String& operator=(const Tchar* tch){
                this->Write(tch);
                return *this;
            };
            String& operator=(const String& str){
                this->Write(str);
                return *this;
            };
            base::Array<String> split(const String& delimiter){
                base::Array<String> result;
                ull start = 0;
                ull end = this->find(delimiter);
                while (end != -1){
                    result.push(this->substr(start,end - start));
                    start = end + delimiter.data.Length();
                    end = this->find(delimiter,start);
                };
                result.push(this->substr(start,this->data.Length() - start));
                return result;
            };
            ull find_impl(const String& str,ull start) const{
                if (str.data.Length() > this->data.Length()){
                    return -1;
                }
                for (ull i = start; i <= this->data.Length() - str.data.Length(); i++){
                    bool match = true;
                    for (ull j = 0; j < str.data.Length(); j++){
                        if (*(this->data[i + j]) != *(str.data[j])){
                            match = false;
                            break;
                        }
                    }
                    if (match) return i;
                }
                return -1;
            };
            ull find(const String& str,ull start = 0){
                return find_impl(str,start);
            };
            ull find(const String& str,ull start = 0) const{
                return find_impl(str,start);
            };
            String substr(ull start,ull length){
                String result;
                for (ull i = start; i < start + length; i++){
                    result.data.push(*(this->data[i]));
                };
                return result;
            };
        #ifdef _IOSTREAM_
            friend std::ostream& operator<<(std::ostream& os,const String& str){
                for (ull i = 0; i < str.data.Length(); ++i){
                    os << *(str.data[i]);
                };
                return os;
            };
        #endif // _IOSTREAM_
        };
        using string = String<char,0x10>;
        using wstring = String<wchar_t,0x10>;
    };
};