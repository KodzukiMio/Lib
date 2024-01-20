#pragma once
#include<cstdint>
#include <stdexcept>
//请自行链接library.obj
#if defined(_WIN64) || defined(__x86_64__) || defined(__amd64__)
// Windows x64环境 或 Unix-like系统的x64环境 (Linux, macOS等)
#define ENVIRONMENT_IS_X64 1
#else
#define ENVIRONMENT_IS_X64 0
#endif
namespace KUR{
    namespace ASM{
    #if ENVIRONMENT_IS_X64
        extern "C" {
            inline int64_t _test_main(void);//绝对不要使用这个!
            inline void swap(void* L,void* R);
            inline int32_t _sumof32(size_t length,const int32_t* address,size_t offset);
            inline int64_t _sumof64(size_t length,const int64_t* address,size_t offset);
            inline void set_rax(int64_t val);
            inline void set_rbx(int64_t val);
            inline void set_rdx(int64_t val);
            inline void set_rsi(int64_t val);
            inline void set_rdi(int64_t val);
            inline void set_rsp(int64_t val);
            inline void set_rbp(int64_t val);
            inline int64_t get_rax(void);
            inline int64_t get_rbx(void);
            inline int64_t get_rsi(void);
            inline int64_t get_rdi(void);
            inline int64_t get_rsp(void);
            inline int64_t get_rbp(void);
        };
        inline void validate_arguments(void* address,size_t length,size_t offset){
            if (!address || !length || !offset)throw std::invalid_argument("Invalid arguments: Null pointer or zero length/offset.");
        };
        inline int64_t sumof(int64_t* address,size_t length,size_t offset = sizeof(int64_t)){
            validate_arguments(address,length,offset);
            return _sumof64(length,address,offset);
        };
        inline uint64_t sumof(uint64_t* address,size_t length,size_t offset = sizeof(uint64_t)){
            validate_arguments(address,length,offset);
            return static_cast<uint64_t>(_sumof64(length,reinterpret_cast<const int64_t*>(address),offset));
        };
        inline int32_t sumof(int32_t* address,size_t length,size_t offset = sizeof(int32_t)){
            validate_arguments(address,length,offset);
            return _sumof32(length,address,offset);
        };
        inline uint32_t sumof(uint32_t* address,size_t length,size_t offset = sizeof(uint32_t)){
            validate_arguments(address,length,offset);
            return static_cast<uint32_t>(_sumof32(length,reinterpret_cast<const int32_t*>(address),offset));
        };
    };
    namespace undefined{//这个名称空间下的代码不保证可以预期运行
    #if defined(_MSC_VER)
    #pragma optimize("", off)
        //msvc专用,设置rax的值.
        int64_t set(int64_t val){
            return val;
        };
        void _get(){
            return;
        };
        //msvc专用,获取rax的值.
        int64_t(*get)(void) = reinterpret_cast<int64_t(*)(void)>(_get);
    #pragma optimize("", on)
    #endif
    };
#endif //ENVIRONMENT_IS_X641
}; // namespace KUR
