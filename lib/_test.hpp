#pragma once
#include<Windows.h>
#include"lib/asm.h"
namespace KUR{
    class gdtime{ // 获取精确时间
    public:
        LARGE_INTEGER t1;
        LARGE_INTEGER tc;
        gdtime(){
            QueryPerformanceFrequency(&tc);
            QueryPerformanceCounter(&t1);
        };
        double get(){
            LARGE_INTEGER t2;
            QueryPerformanceCounter(&t2);
            return (double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
        };
    };
};