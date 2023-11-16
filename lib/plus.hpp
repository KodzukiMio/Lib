#pragma once
#ifndef _kurzerbase_
#define _kurzerbase_
#endif
#include<type_traits>
#include <memory>
#include <string>
#include <unordered_map>
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
        /*
        Storage:运行时可动态储存操作任意类型的数据;
        Ty:索引类型
        */
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
        /*
        ArrayTuple:运行时可动态储存操作任意类型的数据;
        */
        class ArrayTuple{
        private:
            size_t _size = 0;
        public:
            Storage<size_t>store;
            template<typename T>inline void push(const T& obj){
                store.push(_size,obj);
                ++_size;
            };
            inline void push(const char* obj){//此类型会以std::string储存
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
    };
};