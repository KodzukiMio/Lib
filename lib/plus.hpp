#pragma once
#ifndef _kurzerplus_
#define _kurzerplus_
#endif
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <memory>
#include <string>
#include <unordered_map>
#include <bitset>
#include <algorithm>
#include <map>
#include <vector>
namespace KUR{
    namespace plus{
        //将 _tVal 的低位设置到_sVal指定的起始位begbits上,覆盖长度为 len 的位段.要求T为任意整型,超出范围或类型错误抛出异常.
        template<typename T>inline void setbits(T& _sVal,size_t begbits,size_t len,T _tVal){
            static_assert(std::is_integral<T>::value,"Integral type required.");
            if (len <= 0 || begbits < 0 || begbits >= (sizeof(T) << 3) || len >((sizeof(T) << 3) - begbits))throw std::out_of_range("Bit range is out of bounds");
            T mask = ((T(1) << len) - 1) << begbits;
            _sVal &= ~mask;
            _sVal |= (_tVal << begbits) & mask;
        };
        //从 _sVal中提取从第begbits位开始,长度为len的位序列并返回.要求T为任意整型,超出范围或类型错误抛出异常.
        template<typename T>inline T getbits(T _sVal,size_t begbits,size_t len){
            static_assert(std::is_integral<T>::value,"Integral type required.");
            if (len <= 0 || begbits < 0 || begbits >= (sizeof(T) << 3) || len >((sizeof(T) << 3) - begbits))throw std::out_of_range("Bit range is out of bounds");
            return (_sVal >> begbits) & ((T(1) << len) - 1);
        };
        class Object{
        public:
            const size_t type_id;
            virtual ~Object() = default;
            Object(size_t _id):type_id(_id){};
            virtual std::unique_ptr<Object> clone() const = 0;
        };
        template<typename T>class Holder: public Object{
        public:
            static_assert(std::is_copy_constructible<T>::value,"Type T must be copy constructible for Holder");
            T data;
            Holder(const T& _data,size_t _type_id): Object(_type_id),data(_data){};
            std::unique_ptr<Object> clone() const override{
                return std::make_unique<Holder<T>>(*this);
            };
        };
        template<typename Ty = std::string> class Storage{
        private:
            size_t id_idx = 1;
        public:
            std::unordered_map<Ty,std::unique_ptr<Object>> storageMap;
            std::unordered_map<std::string,size_t> types_id;
            std::unordered_map<size_t,std::string> id_types;
            template<typename T>inline void push(const Ty& tag,const T& obj){
                std::string _type = typeid(obj).name();
                size_t& _id = types_id[_type];
                if (!_id){
                    id_types[id_idx] = _type;
                    _id = id_idx;
                    ++id_idx;
                };
                storageMap[tag] = std::make_unique<Holder<T>>(obj,_id);
            };
            inline auto find(const Ty& tag){
                auto it = storageMap.find(tag);
                if (it == storageMap.end()) throw std::runtime_error("Object not found");
                return it;
            };
            template<typename T>inline T& get(const Ty& tag){
                auto holder = static_cast<Holder<T>*>(find(tag)->second.get());
                if (!holder) throw std::bad_cast();
                return holder->data;
            }
            inline Object* get_object(const Ty& tag){
                return static_cast<Object*>(find(tag)->second.get());
            };
            inline std::string get_type(const Ty& tag){
                return id_types[find(tag)->second.get()->type_id];
            };
            inline void del(const Ty& tag){
                storageMap.erase(find(tag));
            };
        };
        class ArrayTuple{
        private:
            size_t _size = 0;
        public:
            Storage<size_t>store;
            template<typename T>inline void push(const T& obj){
                store.push(_size,obj);
                ++_size;
            };
            inline void push(const char* obj){//store as std::string
                store.push(_size,std::string(obj));
                ++_size;
            };
            inline void pop(){
                --_size;
                store.del(_size);
            };
            template<typename T>inline T& get(const size_t idx){
                return store.template get<T>(idx);
            };
            inline size_t size(){
                return this->_size;
            };
            inline std::string get_type(const size_t idx){
                return this->store.get_type(idx);
            };
        };
        int UBIDB2(std::string& num){ // u dec / 2,return remainder
            int tmp = 0;
            std::string stmp;
            for (auto c : num){
                tmp = (c - '0') + tmp * 10;
                if (!stmp.empty() || (tmp >> 1))stmp.push_back((tmp >> 1) + '0');
                tmp &= 1;
            };
            num = stmp.empty() ? "0" : std::move(stmp);
            return tmp;
        };
        std::string UBITB(std::string stmp){ // u dec -> bin
            std::string bin;
            while (stmp.size() > 1 || (stmp[0] - '0') >= 1)bin.push_back(UBIDB2(stmp) + '0');
            std::reverse(bin.begin(),bin.end());
            return bin.empty() ? "0" : bin;
        };
        std::string _UBIPBI(std::string& L,std::string& R){// u dec + dec,reverse
            std::string str;
            size_t s0 = L.size();
            size_t s1 = R.size();
            size_t smx = s0 > s1 ? s0 : s1;
            size_t idx = 0;
            int carry = 0;
            while (idx < smx){
                if (idx < s0)carry += L[idx] - '0';
                if (idx < s1)carry += R[idx] - '0';
                str.push_back('0' + carry % 10);
                carry /= 10;
                ++idx;
            };
            if (carry)str.push_back('1');
            return str;
        };
        std::string UBIPBI(std::string L,std::string R){// u dec + dec
            std::reverse(L.begin(),L.end());
            std::reverse(R.begin(),R.end());
            std::string str = _UBIPBI(L,R);
            std::reverse(str.begin(),str.end());
            return str;
        };
        std::string _UBISBI(std::string& L,std::string& R){//u dec - dec, L >= R,reverse
            std::string ret = "";
            size_t idx = 0;
            int carry = 0;
            size_t _Lsize = L.size(),_Rsize = R.size();
            while (idx < _Lsize){
                carry = L[idx] - '0' - carry;
                if (idx < _Rsize)carry -= (R[idx] - '0');
                ret.push_back((carry + 0xA) % 0xA + '0');
                if (carry < 0)carry = 1;
                else carry = 0;
                ++idx;
            };
            while (ret.back() == '0' && ret.size() > 1)ret.pop_back();
            return ret;
        };
        std::string UBISBI(std::string L,std::string R){//u dec - dec, L >= R
            std::reverse(L.begin(),L.end());
            std::reverse(R.begin(),R.end());
            std::string ret = _UBISBI(L,R);
            std::reverse(ret.begin(),ret.end());
            return ret;
        };
        std::string _UBIMBI(std::string& L,std::string& R){//u dec * dec,reverse
            std::string ret = "0";
            size_t idx = 0;
            int carry = 0;
            std::string tmp = "";
            size_t _Lsize = L.size(),_Rsize = R.size();
            while (idx < _Lsize){
                int _nt = L[idx] - '0';
                for (size_t idxr = 0;idxr < _Rsize;++idxr){
                    carry = _nt * (R[idxr] - '0') + carry;
                    tmp.push_back(carry % 0xA + '0');
                    carry /= 0xA;
                };
                if (carry)tmp.push_back(carry + '0');
                carry = 0;
                int ofx = (int)(idx + 1);
                std::string _zero = "";
                while (--ofx)_zero.push_back('0');
                tmp.insert(0,_zero);
                ret = _UBIPBI(ret,tmp);
                tmp.clear();
                ++idx;
            };
            while (ret.size() > 1 && ret.back() == '0')ret.pop_back();
            return ret;
        };
        std::string UBIMBI(std::string L,std::string R){//u dec * dec
            std::reverse(L.begin(),L.end());
            std::reverse(R.begin(),R.end());
            std::string ret = _UBIMBI(L,R);
            std::reverse(ret.begin(),ret.end());
            return ret;
        };
        bool UBIDBI_CMP(std::string& L,std::string& R){//L >= R ?
            if (L.size() > R.size())return true;
            if (L.size() < R.size())return false;
            size_t idx = 0;
            size_t _idx = L.size() - 1;
            while (idx < _idx){
                if (L[idx] ^ R[idx])return L[idx] > R[idx];
                ++idx;
            };
            if (idx == _idx)return L[idx] >= R[idx];
            return false;
        };
        std::string UBIDBI(std::string L,std::string R,std::string& _remainder){//u dec / dec
            size_t Lsize = L.size(),Rsize = R.size(),idx,tmp = 0;
            idx = Lsize - Rsize;
            std::string ret;
            while (tmp < idx && ++tmp)R.push_back('0');
            tmp = 0;
            while (tmp <= idx){
                int carry = 0;
                while (UBIDBI_CMP(L,R)){
                    L = UBISBI(L,R);
                    ++carry;
                };
                ret.push_back(carry + '0');
                ++tmp;
                R.pop_back();
            };
            _remainder = L;
            tmp = 0;
            while (ret[tmp] == '0')++tmp;
            if (tmp)return ret.erase(0,tmp);
            return ret;
        };
        std::string UBTBI(const std::string& bin){ // u bin -> dec
            std::string ret = "0";
            std::string r2 = "1";
            for (size_t i = bin.size() - 1; i != (size_t)(-1); --i){
                if (bin[i] == '1')ret = UBIPBI(ret,r2);
                r2 = UBIPBI(r2,r2);
            };
            return ret;
        };
        template <size_t N = 64>class MUIALU{
        public:
            bool carry = false;
            std::bitset<N> reg;
            std::bitset<N> _tmp;
            MUIALU(){};
            MUIALU& SetReg(std::bitset<N>& value){
                reg = value;
                return *this;
            };
            MUIALU& Add(std::bitset<N>& rvalue,std::bitset<N>& value){
                carry = false;
                for (size_t i = 0; i < N; ++i){
                    bool h = rvalue[i];
                    bool l = value[i];
                    rvalue[i] = h ^ l ^ carry;
                    carry = (l & carry) | (h & (l | carry));
                };
                return *this;
            };
            MUIALU& Add(std::bitset<N>& value){
                return this->Add(reg,value);
            };
            inline void s2c(std::bitset<N>& value){
                value.flip();
                for (std::size_t i = 0; i < N; ++i){
                    if (value[i]){
                        value[i] = 0;
                    } else{
                        value[i] = 1;
                        break;
                    };
                };
            };
            MUIALU& Subtract(std::bitset<N>& rvalue,std::bitset<N> value){
                this->s2c(value);
                return this->Add(rvalue,value);
            };
            MUIALU& Subtract(std::bitset<N>& value){
                return this->Subtract(reg,value);
            };
            MUIALU& Multiply(std::bitset<N>& value){
                std::bitset<N> result;
                for (size_t i = 0; i < N; ++i){
                    if (value[i]){
                        std::bitset<N> temp = reg << i;
                        this->Add(result,temp);
                    };
                };
                reg = result;
                return *this;
            };
            size_t find_one(std::bitset<N>& value){
                for (size_t i = N - 1; i > 0; --i)if (value[i])return i + 1;
                return 0;
            };
            inline void setbits(std::string&& num,std::bitset<N>& value){
                size_t _sl = num.size();
                for (size_t i = 0; i < _sl && i < value.size(); ++i)value.set(i,num[_sl - 1 - i] == '1');
                for (size_t i = _sl; i < value.size(); ++i)value.reset(i);
            };
            inline bool greater_than(std::bitset<N>& L,std::bitset<N>& R){
                size_t idx = N - 1;
                while (idx > 0){
                    if (L[idx] ^ R[idx])return L[idx];
                    --idx;
                };
                return true;
            };
            std::bitset<N> Division(std::bitset<N>& rvalue,std::bitset<N>& value,std::bitset<N>& remainder){
                size_t dividend_length = this->find_one(rvalue);
                size_t divisor_length = this->find_one(value);
                if (divisor_length == 0 || divisor_length > dividend_length)return std::bitset<N>(0);
                std::bitset<N> quotient = 0;
                for (int i = dividend_length - 1; i >= 0; --i){
                    remainder <<= 1;
                    remainder[0] = rvalue[i];
                    if (greater_than(remainder,value)){
                        this->Subtract(remainder,value);
                        quotient.set(i);
                    };
                };
                return quotient;
            };
            MUIALU& operator=(std::string& num){
                this->setbits(std::move(UBITB(num)),_tmp);
                return this->SetReg(_tmp);
            };
            MUIALU& operator+=(std::string& num){
                this->setbits(std::move(UBITB(num)),_tmp);
                return this->Add(_tmp);
            };
            MUIALU& operator-=(std::string& num){
                this->setbits(std::move(UBITB(num)),_tmp);
                return this->Subtract(_tmp);
            };
            MUIALU& operator*=(std::string& num){
                this->setbits(std::move(UBITB(num)),_tmp);
                return this->Multiply(_tmp);
            };
            MUIALU& operator/=(std::string& num){
                this->setbits(std::move(UBITB(num)),_tmp);
                std::bitset<N>remai;
                this->reg = this->Division(this->reg,_tmp,remai);
                return *this;
            };
            MUIALU& operator=(MUIALU& num){
                this->SetReg(num.reg);
                return *this;
            };
            MUIALU& operator+=(MUIALU& num){
                this->Add(num.reg);
                return *this;
            };
            MUIALU& operator-=(MUIALU& num){
                this->Subtract(num.reg);
                return *this;
            };
            MUIALU& operator*=(MUIALU& num){
                this->Multiply(num.reg);
                return *this;
            };
            MUIALU& operator/=(MUIALU& num){
                std::bitset<N>remai;
                this->reg = this->Division(this->reg,num.reg,remai);
                return *this;
            };
            std::string get(){
                return UBTBI(this->reg.to_string());
            };
        };
        template<size_t _Bits = 256,size_t _Decimal_places = 16>class Number{
        public:
            using vtype = MUIALU<_Bits>;
            using stype = Number<_Bits,_Decimal_places>;
            vtype value;
            std::string _cst_num = "1";
            const size_t _bits = _Bits;
            const size_t _dec_places = _Decimal_places;
            bool msb = 0;
            size_t findsign(std::string& orinum){
                size_t _mx = orinum.size();
                size_t idx = (orinum[0] == '+' || orinum[0] == '-');
                while (idx < _mx)if (orinum[idx++] == '.')return idx - 1;
                return 0;
            };
            void _increase(std::string& orinum){
                size_t idx = this->findsign(orinum);
                if (idx)orinum = orinum.erase(idx,1);
                if (idx)idx = orinum.size() - idx + 1;
                while (idx < _Decimal_places){
                    orinum.push_back('0');
                    ++idx;
                };
            };
            inline void _Construct_S_Num(){
                size_t idx = 0;
                while (idx < _Decimal_places){
                    _cst_num.push_back('0');
                    ++idx;
                };
            };
            inline void init(std::string& num){
                if (num[0] == '-')msb = 1;
                if (num[0] == '+' || msb)value = num.erase(0,1);
                else value = num;
                this->_Construct_S_Num();
            };
            inline void _init(std::string num){
                this->_increase(num);
                this->init(num);
            };
            Number(std::string& num){
                this->_init(num);
            };
            Number(const char* nums){
                std::string num = nums;
                this->_init(num);
            };
            Number(){};
            Number& operator=(std::string& num){
                this->_init(num);
                return *this;
            };
            Number& operator=(const char* nums){
                std::string num = nums;
                this->_init(num);
                return *this;
            };
            Number& operator=(Number& num){
                this->value = num.value;
                this->msb = num.msb;
                this->_cst_num = num._cst_num;
                return *this;
            };
            Number& operator=(Number num){
                this->value = num.value;
                this->msb = num.msb;
                this->_cst_num = num._cst_num;
                return *this;
            };
            friend Number operator+(Number& L,Number& R){
                Number ret;
                if (!(L.msb ^ R.msb)){
                    ret.value = L.value;
                    ret.value += R.value;
                    if (L.msb)ret.msb = 1;
                    return ret;
                };
                if (L.msb){
                    if (L.value.greater_than(L.value.reg,R.value.reg)){
                        ret.value = L.value;
                        ret.value -= R.value;
                        ret.msb = 1;
                    } else{
                        ret.value = R.value;
                        ret.value -= L.value;
                    };
                    return ret;
                } else{
                    if (L.value.greater_than(L.value.reg,R.value.reg)){
                        ret.value = L.value;
                        ret.value -= R.value;
                    } else{
                        ret.value = R.value;
                        ret.value -= L.value;
                        ret.msb = 1;
                    };
                    return ret;
                };
                return ret;
            };
            friend Number operator-(Number& L,Number& R){
                R.msb = !R.msb;
                return (L + R);
            };
            friend Number operator*(Number& L,Number& R){
                stype ret = L;
                ret.msb = L.msb ^ R.msb;
                ret.value *= R.value;
                ret.value /= R._cst_num;
                return ret;
            };
            friend Number operator/(Number& L,Number& R){
                stype ret = L;
                bool flag = false;
                if (!L.value.greater_than(L.value.reg,R.value.reg))flag = true;
                if (flag)ret.value *= L._cst_num;
                ret.value *= L._cst_num;
                ret.value /= R.value;
                ret.msb = L.msb ^ R.msb;
                if (flag)ret.value /= L._cst_num;
                return ret;
            };
            Number& operator+=(Number& num){
                (*this) = (*this) + num;
                return *this;
            };
            Number& operator-=(Number& num){
                (*this) = (*this) - num;
                return *this;
            };
            Number& operator*=(Number& num){
                (*this) = (*this) * num;
                return *this;
            };
            Number& operator/=(Number& num){
                (*this) = (*this) / num;
                return *this;
            };
            bool operator==(Number& num){
                if (num.msb ^ this->msb)return false;
                if (num.value.reg == num.value.reg)return true;
                return false;
            };
            std::string get(){
                std::string str = "";
                str += this->value.get();
                int len = str.size() - _Decimal_places;
                size_t _len = str.size() - _Decimal_places;
                if (len > 0 && str[_len - 1] == '-')str.insert(_len,"0.");
                else if (len > 0)str.insert(_len,1,'.');
                else{
                    int _zl = _Decimal_places - str.size();
                    size_t _zll = _Decimal_places - str.size();
                    std::string zl = "0.";
                    if (_zl >= 0){
                        while (_zll > 0){
                            zl.push_back('0');
                            --_zll;
                        };
                    };
                    str.insert(0,zl);
                };
                if (this->msb)str.insert(0,1,'-');
                return str;
            };
            friend std::ostream& operator<<(std::ostream& os,Number& num){
                os << num.get();
                return os;
            };
            friend std::ostream& operator<<(std::ostream& os,Number num){
                os << num.get();
                return os;
            };
        };
        template<typename T,typename ...Args>inline std::string to_string(const T _Val,Args... args){
            return std::to_string(_Val);
        };

        template<>inline std::string to_string<double,int>(const double _Val,int len){//len must >= 0
            std::string ret;
            long long v0 = (long long)_Val;
            ret = std::to_string(v0);
            double v1 = _Val - v0;
            if (!len)return ret;
            ret.push_back('.');
            for (int i = 0;i < len;++i){
                v1 *= 10;
                ret.push_back('0' + (int)v1);
                v1 -= (int)v1;
            };
            return ret;
        };
        enum class TokenType{
            NOPS,

            Lv0,
            Identifier,
            Number,
            Function,
            // Highest priority (Level 1)
            Lv1,
            OpenParen,            // "(" 
            CloseParen,           // ")" 
            OpenBracket,          // "[" 
            CloseBracket,         // "]" 
            Dot,                  // "." 
            Arrow,                // "->"
            // Level 2
            Lv2,
            Increment,            // "++" 
            Decrement,            // "--" 
            // Level 3
            Lv3,
            Not,                  // "~"
            Emark,                // "!"
            Multiply,             //*  
            Divide,               // /  
            Modulo,               //"%"  
            // Level 4
            Lv4,
            Add,                 //"+"  
            Subtract,            //"-"  
            // Level 5
            Lv5,
            ShiftLeft,           //"<<"  
            ShiftRight,          //" >> "  
            // Level 6
            Lv6,
            LessThan,            //<"  
            GreaterThan,         //" > "  
            LessThanEqual,       //" <="   
            GreaterThanEqual,	  //>="
            // Level 7
            Lv7,
            EqualTo,// "=="
            NotEqualTo,// "!="
            // Level 8
            Lv8,
            And,// "&"
            Xor,// "^"
            Or,// "|"
            LogicalAnd,// "&&"
            LogicalOr,// "||"

            Lv9,
            EqlAssign,// "="
            AddAssign,// "+="
            SubtractAssign,// "-="
            MultiplyAssign,// "*="
            DivideAssign,// "/="
            ModuloAssign,// "%="

            Lv10,
            SingleQuote,//'
            Qmark,//?
            DoubleQuote,//"
            Comma,
            Colon,
            OpenBrace,
            CloseBrace,
        };
        std::map<char,TokenType> symbolTable1 = {
            {'+',TokenType::Add},
            {'-',TokenType::Subtract},
            {'*',TokenType::Multiply},
            {'/',TokenType::Divide},
            {'%',TokenType::Modulo},
            {'(',TokenType::OpenParen},
            {')',TokenType::CloseParen},
            {'&',TokenType::And},
            {'|',TokenType::Or},
            {'^',TokenType::Xor},
            {'<',TokenType::LessThan},
            {'>',TokenType::GreaterThan},
            {',',TokenType::Comma},
            {'[',TokenType::OpenBracket},
            {']',TokenType::CloseBracket},
            {'{',TokenType::OpenBrace},
            {'}',TokenType::CloseBrace},
            {':',TokenType::Colon},
            {'.',TokenType::Dot},
            {'"',TokenType::DoubleQuote},
            {'\'',TokenType::SingleQuote},
            {'?',TokenType::Qmark},
            {'~',TokenType::Not},
            {'!',TokenType::Emark},
            {'=',TokenType::EqlAssign},
        };
        std::map<std::string,TokenType> symbolTable2 = {
            {"==",TokenType::EqualTo},
            {"!=",TokenType::NotEqualTo},
            {"<=",TokenType::LessThanEqual},
            {">=",TokenType::GreaterThanEqual},
            {"&&",TokenType::LogicalAnd},
            {"||",TokenType::LogicalOr},
            {"<<",TokenType::ShiftLeft},
            {">>",TokenType::ShiftRight},
            {"++",TokenType::Increment},
            {"--",TokenType::Decrement},
            {"->",TokenType::Arrow},
            {"+=",TokenType::AddAssign},
            {"-=",TokenType::SubtractAssign},
            {"*=",TokenType::MultiplyAssign},
        };
        inline bool issymbol1(char c){
            return symbolTable1.find(c) != symbolTable1.end();
        };
        inline bool issymbol2(const std::string& str){
            return symbolTable2.find(str) != symbolTable2.end();
        };
        inline bool isvar1(char c){
            return std::isalpha(c) || c == '_';
        }
        inline bool isvar2(char c){
            return std::isdigit(c) || isvar1(c);
        }
        inline bool isdig(TokenType type){
            return type == TokenType::Number;
        };
        inline bool isvar(TokenType type){
            return type == TokenType::Identifier;
        };
        inline bool isopr(TokenType type){
            return !isdig(type) && !isvar(type);
        };
        inline bool isbrace(TokenType type){
            if (type == TokenType::CloseBrace || type == TokenType::OpenBrace)return true;
            return false;
        };
        inline bool isparen(TokenType type){
            if (type == TokenType::CloseParen || type == TokenType::OpenParen)return true;
            return false;
        };
        std::vector<std::string> tokenize(const std::string& expr0){
            std::vector<std::string> tokens;
            std::string number;
            std::string var;
            std::string tcop;
            char tmp;
            std::string expr;
            for (size_t i = 0;i < expr0.size();++i){
                tmp = expr0[i];
                if (tmp == ' ')continue;
                else expr.push_back(tmp);
            }
            for (size_t i = 0; i < expr.size(); ++i){
                tmp = expr[i];
                if (isvar1(tmp)){
                    var.push_back(tmp);
                    continue;
                };
                if (!var.empty()){
                    if (isvar2(tmp)){
                        var.push_back(tmp);
                    } else{
                        tokens.push_back(var);
                        var.clear();
                    }
                }
                if (!number.empty() && !std::isdigit(tmp) && tmp != '.'){
                    tokens.push_back(number);
                    number.clear();
                }
                if (i + 1 < expr.size()){
                    tcop = {tmp,expr[i + 1]};
                    if (issymbol2(tcop)){
                        tokens.push_back(tcop);
                        i++;
                        continue;
                    }
                }
                if ((std::isdigit(tmp) || tmp == '.' || (tmp == '-' && i + 1 < expr.size() && isdigit(expr[i + 1]) && i >= 1 && !isdigit(expr[i - 1]) && expr[i - 1] != ')') || (tmp == '-' && !i)) && var.empty()){
                    number.push_back(tmp);
                    continue;
                }
                if (issymbol1(tmp)){
                    tokens.push_back(std::string(1,tmp));
                    continue;
                }
                //err type...
            }
            if (!number.empty())tokens.push_back(number);
            if (!var.empty())tokens.push_back(var);
            return tokens;
        };
        size_t get_lvl(TokenType type){
            if (type < TokenType::Lv10){
                if (type < TokenType::Lv9){
                    if (type < TokenType::Lv8){
                        if (type < TokenType::Lv7){
                            if (type < TokenType::Lv6){
                                if (type < TokenType::Lv5){
                                    if (type < TokenType::Lv4){
                                        if (type < TokenType::Lv3){
                                            if (type < TokenType::Lv2){
                                                if (type < TokenType::Lv1){
                                                    return (size_t)TokenType::Lv0;
                                                }
                                                return (size_t)TokenType::Lv1;
                                            }
                                            return (size_t)TokenType::Lv2;
                                        }
                                        return (size_t)TokenType::Lv3;
                                    }
                                    return (size_t)TokenType::Lv4;
                                }
                                return (size_t)TokenType::Lv5;
                            }
                            return (size_t)TokenType::Lv6;
                        }
                        return (size_t)TokenType::Lv7;
                    }
                    return (size_t)TokenType::Lv8;
                }
                return (size_t)TokenType::Lv9;
            }
            return (size_t)TokenType::Lv10;
        };
        class Token{
        public:
            std::string val;
            TokenType type;
            size_t lvl = 0;
            bool is_int = 0;
            Token(const std::string& _val,TokenType _type):val(_val),type(_type){
                this->lvl = get_lvl(type);
            };
        };
        std::vector<Token> handle_token_msg(const std::vector<std::string>& src_tokens){
            std::vector<Token> tokens;
            bool is_number;
            for (const auto& str : src_tokens){
                if (str.empty()) continue;
                if (str.length() == 2 && issymbol2(str))tokens.emplace_back(str,symbolTable2[str]);
                else if (str.length() == 1 && issymbol1(str[0]))tokens.emplace_back(str,symbolTable1[str[0]]);
                else{
                    is_number = false;
                    if (str[0] == '-' || str[0] == '+' || std::isdigit(str[0]))is_number = true;
                    TokenType type = is_number ? TokenType::Number : TokenType::Identifier;
                    tokens.emplace_back(str,type);
                }
            }
            return tokens;
        };
        inline std::vector<Token> build_token_msg(const std::string& expr){
            return handle_token_msg(tokenize(expr));
        };
        template<typename NumType,typename Ty = std::string>inline NumType get_var_value(std::string& name,Storage<Ty>& store){
            return static_cast<NumType>(store.get(name));
        };
        std::vector<Token> convert_token_rpn(const std::vector<Token>& tokens){
            std::vector<Token>opr;
            std::vector<Token>out;
            for (size_t i = 0;i < tokens.size();++i){
                const Token& tk = tokens[i];
                if (isdig(tk.type) || isvar(tk.type))out.push_back(tk);
                else if (tk.type == TokenType::OpenParen)opr.push_back(tk);
                else if (tk.type == TokenType::CloseParen){
                    while (!opr.empty() && opr.back().type != TokenType::OpenParen){
                        out.push_back(opr.back());
                        opr.pop_back();
                    };
                    if (!opr.empty())opr.pop_back();
                } else if (isopr(tk.type)){
                    while (!opr.empty() && tk.lvl >= opr.back().lvl && opr.back().type != TokenType::OpenParen){
                        out.push_back(opr.back());
                        opr.pop_back();
                    };
                    opr.push_back(tk);
                };
            };
            while (opr.size()){
                out.push_back(opr.back());
                opr.pop_back();
            };
            return out;
        };
        using __ntype_int = long long;
        using __ntype_dec = double;
        class NumberType{
        public:
            __ntype_dec val = 0;
            bool is_int = 0;
            bool no_flag = false;
            std::string valname;
            TokenType type = TokenType::NOPS;
            NumberType(bool flag = 0):no_flag(flag){};
            NumberType(__ntype_dec _val,bool _is_int,TokenType _type):val(_val),is_int(_is_int),type(_type){};
            NumberType(__ntype_dec _val,bool _is_int,TokenType _type,const std::string& _valname):val(_val),is_int(_is_int),type(_type),valname(_valname){};
            inline bool is_val(){
                return valname.size();
            };
            inline __ntype_dec get(){
                if (is_int)return (__ntype_dec)((__ntype_int)val);
                return val;
            };
        };
        static const double eps = 1e-10;
        class Calculate{
        public:
            static inline bool is_integer(const std::string& str){
                for (size_t i = 0;i < str.size();++i)if (str[i] == '.')return false;
                return true;
            };
            static inline bool is_decimal(const std::string& str){
                return !is_integer(str);
            };
            template<typename T>static inline int get_number_type(){
                if (std::is_integral<T>::value)return 1;
                if (std::is_floating_point<T>::value)return 2;
                return  0;
            };
            static int get_opr_c(TokenType type){
                switch (type){
                    case KUR::plus::TokenType::NOPS:
                        return 0;
                    case KUR::plus::TokenType::Increment://--
                        return 1;
                    case KUR::plus::TokenType::Decrement://++
                        return 1;
                    case KUR::plus::TokenType::Not: //~
                        return 1;
                    case KUR::plus::TokenType::Emark: //!
                        return 1;
                    default:
                        return 2;
                };
            };
            template<typename Ret>static Ret calculate(TokenType type,NumberType L,NumberType R,Storage<std::string>& store){
                auto& v = L;
                if (isvar(L.type))v = store.get<NumberType>(L.valname);
                switch (type){
                    case KUR::plus::TokenType::NOPS:
                        break;
                    case KUR::plus::TokenType::Identifier:
                        return (Ret)store.get<Ret>(L.valname);
                    case KUR::plus::TokenType::Number:
                        return (Ret)L.get();
                    case KUR::plus::TokenType::Function:
                        break;
                    case KUR::plus::TokenType::Increment:
                        if (isvar(L.type))return (Ret)++v.val;
                        else return (Ret)(L.val + 1);
                    case KUR::plus::TokenType::Decrement:
                        if (isvar(L.type))return (Ret)--v.val;
                        else return (Ret)(L.val - 1);
                    case KUR::plus::TokenType::Not:      //TODO,variable;
                        return (Ret)(~(__ntype_int)L.val);
                    case KUR::plus::TokenType::Emark:
                        return (Ret)!L.val;
                    case KUR::plus::TokenType::Multiply:
                        return (Ret)(L.val * R.val);
                    case KUR::plus::TokenType::Divide:
                        return (Ret)(L.val / R.val);
                    case KUR::plus::TokenType::Modulo:
                        return (Ret)((__ntype_int)L.val % (__ntype_int)R.val);
                    case KUR::plus::TokenType::Add:
                        return (Ret)(L.val + R.val);
                    case KUR::plus::TokenType::Subtract:
                        return (Ret)(L.val - R.val);
                    case KUR::plus::TokenType::ShiftLeft:
                        return (Ret)((__ntype_int)L.val << (__ntype_int)R.val);
                    case KUR::plus::TokenType::ShiftRight:
                        return (Ret)((__ntype_int)L.val >> (__ntype_int)R.val);
                    case KUR::plus::TokenType::LessThan:
                        return (Ret)(L.val < R.val);
                    case KUR::plus::TokenType::GreaterThan:
                        return (Ret)(L.val > R.val);
                    case KUR::plus::TokenType::LessThanEqual:
                        return (Ret)(L.val <= R.val);
                    case KUR::plus::TokenType::GreaterThanEqual:
                        return (Ret)(L.val >= R.val);
                    case KUR::plus::TokenType::EqualTo:
                        if (L.is_int && !R.is_int)return (Ret)((__ntype_int)L.val == R.val);
                        else if (R.is_int && !L.is_int)return (Ret)(L.val == (__ntype_int)R.val);
                        else if (R.is_int && L.is_int)return (Ret)((__ntype_int)L.val == (__ntype_int)R.val);
                        else return (Ret)(L.val == R.val);
                    case KUR::plus::TokenType::NotEqualTo:
                        if (L.is_int)return (Ret)((__ntype_int)L.val != (__ntype_int)R.val);
                        else return (Ret)(L.val != R.val);
                    case KUR::plus::TokenType::And:
                        return (Ret)((__ntype_int)L.val & (__ntype_int)R.val);
                    case KUR::plus::TokenType::Xor:
                        return (Ret)((__ntype_int)L.val ^ (__ntype_int)R.val);
                    case KUR::plus::TokenType::Or:
                        return (Ret)((__ntype_int)L.val | (__ntype_int)R.val);
                    case KUR::plus::TokenType::LogicalAnd:
                        if (L.is_int)return (Ret)((__ntype_int)L.val && (__ntype_int)R.val);
                        else return (Ret)(L.val && R.val);
                    case KUR::plus::TokenType::LogicalOr:
                        if (L.is_int)return (Ret)((__ntype_int)L.val || (__ntype_int)R.val);
                        else return (Ret)(L.val || R.val);
                    case KUR::plus::TokenType::EqlAssign://L must be var
                        v.is_int = R.is_int;
                        return (Ret)(v.val = R.val);
                    case KUR::plus::TokenType::AddAssign:
                        return (Ret)(v.val += R.val);
                    case KUR::plus::TokenType::SubtractAssign:
                        return (Ret)(v.val -= R.val);
                    case KUR::plus::TokenType::MultiplyAssign:
                        return (Ret)(v.val *= R.val);
                    case KUR::plus::TokenType::DivideAssign:
                        return (Ret)(v.val /= R.val);
                    case KUR::plus::TokenType::ModuloAssign:
                        return (Ret)(v.val = (__ntype_dec)((__ntype_int)v.val % (__ntype_int)R.val));
                };
                return (Ret)0;
            };
        };
        std::string evaluate(std::vector<Token>& tokens,Storage<std::string>& store,int precision = 0x10,bool mode = false){
            std::vector<plus::NumberType>vec;
            std::string ret;
            int oprc = 0;
            __ntype_dec retd = 0;
            __ntype_int retl = 0;
            auto _null = NumberType(true);
            for (size_t i = 0;i < tokens.size();++i){
                auto& token = tokens[i];
                if (isdig(token.type)){
                    if (Calculate::is_integer(token.val))vec.push_back(NumberType((__ntype_dec)std::stoll(token.val),true,token.type));
                    else vec.push_back(NumberType(std::stod(token.val),false,token.type));
                } else if (isopr(token.type)){
                    oprc = Calculate::get_opr_c(token.type);
                    if (oprc == 1){
                        auto& _R = vec.back();
                        if (_R.is_int)retl = Calculate::calculate<__ntype_int>(token.type,_R,0,store);
                        else retd = Calculate::calculate<__ntype_dec>(token.type,_R,0,store);
                        bool _is = _R.is_int;
                        auto type = _R.type;
                        vec.pop_back();
                        if (_is)vec.push_back(NumberType((__ntype_dec)retl,_is,type));
                        else vec.push_back(NumberType(retd,_is,type));
                    } else if (oprc == 2){
                        auto& _R = vec.back();
                        auto& _L = (vec.size() >= 2) ? vec[vec.size() - 2] : _null;
                        if (_L.no_flag){
                            _L.is_int = _R.is_int;
                            _L.type = _R.type;
                        };
                        bool div = true;
                        if (token.type == TokenType::Divide || token.type == TokenType::DivideAssign)div = mode;
                        bool _is = _L.is_int && _R.is_int && div;
                        if (_is)retl = Calculate::calculate<__ntype_int>(token.type,_L,_R,store);
                        else retd = Calculate::calculate<__ntype_dec>(token.type,_L,_R,store);
                        auto type = _L.type;
                        vec.pop_back();
                        if (!vec.empty())vec.pop_back();
                        if (_is)vec.push_back(NumberType((__ntype_dec)retl,_is,type));
                        else vec.push_back(NumberType((__ntype_dec)retd,_is,type));
                    } else{
                        ret = "Unsupported operator";
                        return ret;
                    }
                } else if (isvar(token.type)){
                    vec.push_back(NumberType(store.get<NumberType>(token.val).get(),token.is_int,token.type,token.val));
                } else{
                    ret = "Invalid expression";
                    return ret;
                }
            };
            auto& v0 = vec[0];
            if (isvar(v0.type)){
                if (v0.is_int)ret = std::to_string((__ntype_int)store.get<NumberType>(v0.valname).val);
                else ret = std::to_string(store.get<NumberType>(v0.valname).val);
                return ret;
            };
            if (v0.is_int)ret = std::to_string((__ntype_int)v0.get());
            else ret = (precision == 0x6) ? std::to_string(v0.get()) : plus::to_string(v0.get(),precision);
            if (!v0.is_int){
                while (ret.back() == '0')ret.pop_back();
                if (ret.back() == '.')ret.pop_back();
            };
            return ret;
        };
        inline std::string eval(const std::string& expr,Storage<std::string>& store,bool& mode,int precision){
            auto tokens = convert_token_rpn(build_token_msg(expr));
            return evaluate(tokens,store,precision,mode);
        };
        class Interpreter{
        public:
            bool mode = false;//true:integer;false:decimal-fp64;
            int precision = 0x6;
            std::string retstr;
            Storage<std::string>store;
            Interpreter(bool _mode = false):mode(_mode){};
            std::string& eval(const std::string& expr){
                this->retstr = plus::eval(expr,store,mode,precision);
                return this->retstr;
            };
            void InterActive(){
                std::string str;
                while (true){
                    std::cout << ">>";
                    std::getline(std::cin,str);
                    if (str == "exit")break;
                    if (str == "clear"){
                    #ifdef __linux__
                        system("clear");
                    #elif _WIN32
                        system("cls");
                    #else
                        std::cout << "Not Support Clear Command.\n";
                    #endif
                        continue;
                    };
                    if (str.empty())continue;
                    std::cout << this->eval(str) << '\n';
                };
            };
            bool InterActiveSafe(){
                try{
                    this->InterActive();
                } catch (const std::exception& e){
                    std::cout << e.what();
                    return false;
                };
                return true;
            };
        };
    };
};