#include<iostream>
#include<string>
#include<bitset>
#include<algorithm>
template<size_t N = 64> class MUIALU{
public:
    bool carry = false;
    std::bitset<N> reg;
    void SetReg(std::bitset<N> value){
        reg = value;
    };
    void Add(std::bitset<N>& rvalue,std::bitset<N>& value){
        carry = false;
        for (size_t i = 0; i < N; ++i){
            bool h = rvalue[i];
            bool l = value[i];
            rvalue[i] = h ^ l ^ carry;
            carry = (h & (l | carry)) | (l & carry);
        }
    };
    void Add(std::bitset<N>& value){
        this->Add(reg,value);
    };
    void Subtract(std::bitset<N>& rvalue,std::bitset<N> value){
        value.flip();
        for (std::size_t i = 0; i < value.size(); ++i){
            if (value[i]){
                value[i] = 0;
            } else{
                value[i] = 1;
                break;
            }
        }
        this->Add(rvalue,value);
    };
    void Subtract(std::bitset<N> value){
        this->Subtract(reg,value);
    };
    void Multiply(std::bitset<N> value){
        std::bitset<N> result;
        for (size_t i = 0; i < N; ++i){
            if (value[i]){
                std::bitset<N> temp = reg << i;
                this->Add(result, temp);
            }
        }
        reg = result;
    };
};

int UBIDB2(std::string& num){//u dec / 2
    int tmp = 0;
    std::string stmp;
    for (auto c : num){
        tmp = (c - '0') + tmp * 10;
        if (!stmp.empty() || (tmp >> 1)){
            stmp.push_back((tmp >> 1) + '0');
        }
        tmp = tmp % 2;
    }
    num = stmp.empty() ? "0" : stmp;
    return tmp;
}
std::string UBITB(std::string& dec){//u dec -> bin
    std::string stmp = dec;
    std::string bin = "";
    while (stmp.size() > 1 || (stmp[0] - '0') >= 1){
        bin.push_back(UBIDB2(stmp) + '0');
    }
    std::reverse(bin.begin(),bin.end());
    return bin;
};

std::string UBIPBI(std::string& p0,std::string& p1){//u dec + dec
    std::string tm;
    std::string tn;
    std::string ret;
    if (p0.size() > p1.size()){
        tm = p0;
        tn = p1;
    } else{
        tm = p1;
        tn = p0;
    }
    std::reverse(tm.begin(),tm.end());
    std::reverse(tn.begin(),tn.end());

    int carry = 0;
    for (size_t i = 0; i < tn.size(); ++i){
        int sum = tm[i] - '0' + tn[i] - '0' + carry;
        carry = sum / 10;
        ret.push_back(sum % 10 + '0');
    }
    for (size_t i = tn.size(); i < tm.size(); ++i){
        int sum = tm[i] - '0' + carry;
        carry = sum / 10;
        ret.push_back(sum % 10 + '0');
    }
    if (carry > 0){
        ret.push_back(carry + '0');
    }
    std::reverse(ret.begin(),ret.end());
    return ret;
}
std::string UBTBI(const std::string& bin){ //u bin -> dec
    std::string ret = "0";
    std::string r2 = "1";
    for (int i = bin.size() - 1; i >= 0; --i){
        if (bin[i] == '1'){
            ret = UBIPBI(ret,r2);
        }
        r2 = UBIPBI(r2,r2);
    }
    return ret;
}
/*
constexpr const int r2m = 666;
int main(){
    MUIALU<r2m>alu;//200->664.386
    std::string str0,str1;
    std::cin >> str0 >> str1;
    std::bitset<r2m>b0(UBITB(str0));
    std::bitset<r2m>b1(UBITB(str1));
    alu.SetReg(b0);
    alu.Subtract(b1);
    std::cout << UBTBI(alu.reg.to_string());
    return 0;
}
*/