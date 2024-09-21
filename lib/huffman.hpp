#pragma once
#include "utility.hpp"
namespace kur{
    template<typename T>class huffman_data{
        static_assert(base::is_character<T>::value,"T require character type.");
    public:
        base::ull count = 0;
        T base_char = T(0);
        huffman_data* left = nullptr;
        huffman_data* right = nullptr;
        huffman_data(){};
        huffman_data(const huffman_data& tmp){
            this->count = tmp.count;
            this->base_char = tmp.base_char;
        }
        huffman_data(T _base_char,base::ull _count){
            this->count = _count;
            this->base_char = _base_char;
        };
        ~huffman_data(){
            if (left)delete left;
            if (right)delete right;
        }
    };
    template<typename T>class huffman{
        static_assert(base::is_character<T>::value,"T require character type.");
    public:
        using base_string = base::String<T>;
        using rcv_t = base::remove_cv_t<T>;
        using node = huffman_data<rcv_t>;
        using node_ptr = node*;
        using code_type = base::pair<rcv_t,base_string>;
        node* root = nullptr;
        base_string data;
        kur::unordered_map<rcv_t,ull>char_map;
        std::vector<code_type>codes;
        huffman(){};
        huffman(const  rcv_t* str){
            this->init(str);
        }
        huffman(const base_string& str){
            this->init(str);
        };
        void init(const  rcv_t* str){
            this->data = base_string(str);
        }
        void init(const base_string& str){
            this->data = str;
        }
        void set_data(const base_string& str){
            this->data = str;
            char_map.clear();
        }
        ull get_depth(node* root){
            return root ? (1 + base::maximum(get_depth(root->left),get_depth(root->right))) : 0;
        }
        void traverse(node* root,base::vector<rcv_t>& code){
            if (!root)return;
            if (!root->left && !root->right){
                base_string data;
                data.data.resize(code.size());
                for (ull idx = 0;idx < code.size();++idx)data.write(code[idx]);
                codes.push_back({root->base_char,data});
                return;
            }
            if (root->left){
                code.push_back('0');
                traverse(root->left,code);
                code.pop_back();
            }
            if (root->right){
                code.push_back('1');
                traverse(root->right,code);
                code.pop_back();
            }
        }
        std::vector<code_type>& get_codes(){
            this->establish();
            codes.clear();
            ull depth = this->get_depth(this->root);
            base::vector<rcv_t>vec;
            this->traverse(this->root,vec);
            return this->codes;
        };
        void establish(){
            base::heap<node_ptr>hp([](const node_ptr& up,const node_ptr& down)->bool{
                return up->count < down->count;
                });
            for (auto& itr : this->data)char_map[itr]++;
            for (auto& itr : char_map)hp.insert(new node(itr.first,itr.second));
            while (hp.size() > 1){
                node_ptr left = hp.get();
                node_ptr right = hp.get(false);
                node_ptr parent_node = new node((T)0,left->count + (right ? right->count : 0));
                parent_node->left = left;
                parent_node->right = right;
                hp.insert(parent_node);
            }
            root = hp.get();
            if (!root)throw std::runtime_error("Failed to build Huffman tree");
        }
        ~huffman(){
            if (this->root)delete this->root;
        }
    };

    template<typename T>class huffman_code{
    public:
        using base_string = base::String<T>;
        using rcv_t = base::remove_cv_t<T>;
        using _code_t = std::vector<base::pair<rcv_t,base_string>>;
        using code_t = base::pair<std::vector<bool>,_code_t>;
        using node = huffman_data<rcv_t>;
        huffman_code(){};
        code_t encode(const base_string& src){
            huffman<T> data;
            data.set_data(src);
            code_t pr;
            pr.second = data.get_codes();
            kur::unordered_map<rcv_t,base_string>codes;
            for (auto& itr : pr.second)codes[itr.first] = itr.second;
            for (ull idx = 0;idx < src.data._pos;++idx)for (auto& itr : codes[src[idx]])pr.first.push_back(itr - '0');
            return pr;
        }
        void _construct_node(node* root,rcv_t ch,const base_string& code){
            for (ull idx = 0;idx < code.data._pos;++idx){
                bool bit = code[idx] - '0';
                if (bit){
                    if (!root->right)root->right = new node();
                    root = root->right;
                } else{
                    if (!root->left)root->left = new node();
                    root = root->left;
                }
            }
            root->base_char = ch;
        }
        base_string decode(const code_t& codes){
            node* root = new node();
            base_string str;
            node* tmp = root;
            for (ull idx = 0;idx < codes.second.size();++idx)this->_construct_node(root,codes.second[idx].first,codes.second[idx].second);
            for (ull idx = 0;idx < codes.first.size();++idx){
                if (codes.first[idx])tmp = tmp->right;
                else tmp = tmp->left;
                if (!tmp->left && !tmp->right){
                    str.push_back(tmp->base_char);
                    tmp = root;
                };
            };
            delete root;
            return str;
        }
    };
    static class huffman_store{
    public:
        template<typename T>using base_string = base::String<T>;
        template<typename T>using rcv_t = base::remove_cv_t<T>;
        template<typename T>using pair_t = base::pair<rcv_t<T>,base_string<T>>;
        template<typename T>using _code_t = std::vector<pair_t<T>>;
        template<typename T>using code_t = base::pair<std::vector<bool>,_code_t<T>>;
        /*struct data_span{
        ull total_bytes;
        ull map_bits;
        ull data_bits;
        bit* map;
        bit* data;
        };*/
        static inline ull get_size(const byte* bytes){
            return *((ull*)bytes);
        }
        static base_string<char>to_string(const byte* bytes){
            return base_string<char>((const char*)bytes);
        };
        template<typename T>static base_string<T> decode(const byte* bytes){
            ull* ptr = (ull*)bytes;
            ull total_size = *ptr++;
            ull map_bits = *ptr++;
            ull data_bits = *ptr++;
            byte* tmp = (byte*)ptr;
            code_t<T>code;
            ull map_ofx = ((map_bits + 7) >> 3);
            for (ull idx = 0;idx < map_ofx;){
                pair_t<T> pr;
                pr.first = *(T*)tmp;
                tmp += sizeof(T);
                ull bits = *(ull*)tmp;
                tmp += sizeof(ull);
                for (ull j = 0;j < bits;++j)pr.second.push_back((tmp[j >> 3] & (1 << (j % 8))) ? '1' : '0');
                tmp += ((bits + 7) >> 3);
                idx += sizeof(T) + sizeof(ull) + ((bits + 7) >> 3);
                code.second.push_back(pr);
            }
            for (ull idx = 0;idx < data_bits;++idx)code.first.push_back(!!(tmp[idx >> 3] & (1 << (idx % 8))));
            return huffman_code<T>().decode(code);
        }
        template<typename T>static byte* encode(const base_string<T>& data){
            code_t<T> code = huffman_code<T>().encode(data);
            ull bitcount = 0;
            for (ull idx = 0;idx < code.second.size();++idx){
                ull bits = code.second[idx].second.size() + ((sizeof(ull) + sizeof(T)) << 3);//char(T),code_bits(ull),code(bit)
                bitcount += (((bits + 7) >> 3) << 3);
            }
            ull map_bits = bitcount;
            ull data_bits = code.first.size();
            bitcount += data_bits;
            ull bytes = ((bitcount + 7) >> 3);
            ull total_bytes = (sizeof(ull) * 3) + bytes;
            byte* tmp = new byte[total_bytes];
            memset(tmp,0,total_bytes);
            ull* ptr = (ull*)tmp;
            ptr[0] = total_bytes;
            ptr[1] = map_bits;
            ptr[2] = data_bits;
            byte* dptr = (byte*)(ptr + 3);
            for (ull idx = 0;idx < code.second.size();++idx){
                auto& ref = code.second[idx];
                *(T*)dptr = ref.first;
                dptr += sizeof(T);
                *(ull*)dptr = ref.second.size();
                dptr += sizeof(ull);
                for (ull j = 0;j < ref.second.size();++j)if (ref.second[j] - '0')dptr[j >> 3] |= (1 << (j % 8));
                dptr += ((ref.second.size() + 7) >> 3);
            }
            for (ull idx = 0;idx < data_bits;++idx)if (code.first[idx])dptr[idx >> 3] |= (1 << (idx % 8));
            return tmp;
        };
        static inline byte* encode(const byte* bytes){
            return  huffman_store::encode<char>(huffman_store::to_string(bytes));
        };
    };
}