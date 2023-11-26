#pragma once
#ifndef _kurzerplus_
#define _kurzerplus_
#endif
#include <iostream>
#include <type_traits>
#include <memory>
#include <string>
#include <unordered_map>
#include <bitset>
#include <algorithm>
namespace KUR{
    namespace plus{
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
        int UBIDB2(std::string& num){ // u dec / 2
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
        std::string UBIPBI(std::string p0,std::string p1){// u dec + dec
            std::string str;
            size_t s0 = p0.size();
            size_t s1 = p1.size();
            size_t smx = std::max(s0,s1);
            size_t idx = 0;
            int carry = 0;
            std::reverse(p0.begin(),p0.end());
            std::reverse(p1.begin(),p1.end());
            while (idx < smx){
                if (idx < s0)carry += p0[idx] - '0';
                if (idx < s1)carry += p1[idx] - '0';
                str.push_back('0' + carry % 10);
                carry /= 10;
                ++idx;
            };
            if (carry)str.push_back('1');
            std::reverse(str.begin(),str.end());
            return str;
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
    };
};