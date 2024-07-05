#pragma once
#include "utility.hpp"
#include "plus.hpp"
namespace KUR{
    namespace script{
        enum vm_types{
            data_segment,
            code_segment,
            variable,
            vm_stack,
            vm_heap,
            address,
            pointer,
            exe_engine,
            lexer,
            vm_token
        };
        class object{
        public:
            void* v_adr = nullptr;
            vm_types type;
        };
        class segment:public object{
        public:
            using obj_p = std::shared_ptr<object>;
            std::vector<std::shared_ptr<object>>data;
            void init(vm_types _type){
                this->type = _type;
            };
            virtual void construct(){
                this->init(vm_types::address);
            };
        };
        class data_seg:public segment{
        public:
            void construct()override{
                this->init(vm_types::data_segment);
            };
        };
        class code_seg:public segment{
        public:
            void construct()override{
                this->init(vm_types::code_segment);
            };
        };
        class vheap:public segment{
        public:
            void construct()override{
                this->init(vm_types::vm_heap);
            };
        };
        class vstack:public segment{
        public:
            void construct()override{
                this->init(vm_types::vm_stack);
            };
        };
        class var:public object{
        public:
            var(){
                this->type = vm_types::variable;
            };
        };
        class execution_engine:public object{
        public:
            execution_engine(){
                this->type = vm_types::exe_engine;
            };
        };
        class token:public object{
        public:
            token(){
                this->type = vm_types::vm_token;
            };
        };
        class lexer:public object{
            lexer(){
                this->type = vm_types::lexer;
            };
            std::vector<token>tokenize(std::string& str){
                std::vector<token>tokens;

            };
        };
        class virtual_machine{
        public:
            data_seg datas;
            code_seg codes;
            vheap v_heap;
            vstack v_stack;
        };
    };
};