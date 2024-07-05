#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

char *SZN_ReadStr(char end, size_t length)
{
    if (length <= 0)
        return NULL;
    char *ret = (char *)malloc((length + 1) * sizeof(char));
    if (ret == NULL)
        return NULL;
    size_t i;
    for (i = 0; i < length; i++)
    {
        char c = getchar();
        if (c == end)
        {
            ret[i] = '\0';
            break;
        }
        ret[i] = c;
    }
    if (i == length)
        ret[i] = '\0';
    return ret;
}
// deafult initcapacity = 0
size_t SZN_ReadLineUnlimited(char **cptr, size_t initcapacity)
{
    size_t capacity = 0x10;
    if (initcapacity)
    {
        capacity = initcapacity;
    };
    char *line = (char *)malloc(capacity * sizeof(char));
    if (!line)
        return NULL;
    size_t length = 0;
    int c;

    while ((c = fgetc(stdin)) != EOF && c != '\n')
    {
        if (length + 1 == capacity)
        {
            capacity = capacity + (capacity >> 1);
            char *temp = (char *)realloc(line, capacity * sizeof(char));
            if (!temp)
            {
                free(line);
                return NULL;
            }
            line = temp;
        }
        line[length++] = c;
    }

    if (length == 0 && c == EOF)
    {
        free(line);
        return NULL;
    }

    line[length] = '\0';
    *cptr = line;
    return length;
}
// 有BUG,别用
wchar_t *SZN_ReadWstr(wchar_t end, size_t length)
{
    if (length <= 0)
        return NULL;
    wchar_t *ret = (wchar_t *)malloc((length + 1) * sizeof(wchar_t));
    if (ret == NULL)
        return NULL;
    size_t i;
    for (i = 0; i < length; i++)
    {
        wchar_t c = getwchar();
        if (c == end)
        {
            ret[i] = '\0';
            break;
        }
        ret[i] = c;
    }
    if (i == length)
        ret[i] = '\0';
    return ret;
}
void SZN_cout(char *str)
{
    setlocale(LC_ALL, "");
    printf("%s", str);
}
void SZN_wcout(wchar_t *wstr)
{
    setlocale(LC_ALL, "");
    wprintf(L"%ls", wstr);
}
void SZN_pause()
{
    getchar();
}
void sort_bubble(int *_Begin, int *_End, int (*_CmpPfn)(int *, int *)) // 垃圾性能,最简实现版
{
    size_t n = _End - _Begin;
    for (size_t i = 0; i < n; ++i)
    {
        int _Flag = 1;
        for (size_t j = 0; j < n - i - 1; ++j)
        {
            if (_CmpPfn(_Begin + j + 1, _Begin + j))
            {
                int _Tmp = _Begin[j];
                _Begin[j] = _Begin[j + 1];
                _Begin[j + 1] = _Tmp;
                _Flag = 0;
            };
        };
        if (_Flag)
            break;
    };
};
void sort_select(int *_Begin, int *_End, int (*_CmpPfn)(int *, int *))
{
    int *_Tmp0 = _Begin;
    while (_Tmp0 < _End)
    {
        int *_Tmp1 = _Tmp0;
        int *_Min_ = _Tmp1;
        while (_Tmp1 < _End)
        {
            if (_CmpPfn(_Tmp1, _Min_))
                _Min_ = _Tmp1;
            ++_Tmp1;
        };
        int _Tmp = *_Min_;
        *_Min_ = *_Tmp0;
        *_Tmp0 = _Tmp;
        ++_Tmp0;
    };
};
void sort_insert(int *_Begin, int *_End, int (*_CmpPfn)(int *, int *))
{
    for (int *_Sorted = _Begin + 1; _Sorted < _End; ++_Sorted)
    {
        int _Key = *_Sorted;
        int *_Pos = _Sorted - 1;
        while (_Pos >= _Begin && _CmpPfn(&_Key, _Pos))
        {
            *(_Pos + 1) = *_Pos;
            --_Pos;
        };
        *(_Pos + 1) = _Key;
    };
};
void _merge(int *_Begin, int *_Middle, int *_End, int (*_CmpPfn)(int *, int *), int *_Tmp)
{
    int *_Beg = _Begin;
    int *_Pos = _Middle;
    int *_Tp_ = _Tmp;
    while (_Beg < _Middle && _Pos < _End)
    {
        if (_CmpPfn(_Beg, _Pos))
        {
            *_Tp_++ = *_Beg++;
        }
        else
        {
            *_Tp_++ = *_Pos++;
        };
    };
    while (_Beg < _Middle)
        *_Tp_++ = *_Beg++;
    while (_Pos < _End)
        *_Tp_++ = *_Pos++;
    while (_Begin < _End)
        *_Begin++ = *_Tmp++;
};
void sort_merge(int *_Begin, int *_End, int (*_CmpPfn)(int *, int *), int *_Tmp)
{
    if (_End - _Begin > 1)
    {
        int *_Mid = _Begin + ((_End - _Begin) >> 1);
        sort_merge(_Begin, _Mid, _CmpPfn, _Tmp);
        sort_merge(_Mid, _End, _CmpPfn, _Tmp);
        _merge(_Begin, _Mid, _End, _CmpPfn, _Tmp);
    };
};
void swap(int *l, int *r)
{
    int t = *l;
    *l = *r;
    *r = t;
};
int *_partition(int *_Begin, int *_End, int (*_CmpPfn)(int *, int *))
{
    int *_Mid = _Begin + ((_End - _Begin) >> 1);
    int *_Ed = _End - 1;
    if (_CmpPfn(_Mid, _Begin))
        swap(_Mid, _Begin);
    if (_CmpPfn(_Ed, _Begin))
        swap(_Ed, _Begin);
    if (_CmpPfn(_Ed, _Mid))
        swap(_Ed, _Mid);
    int *_P0 = _Begin;
    int *_Pos = _Mid;
    for (int *_P1 = _Begin; _P1 < _End; ++_P1)
    {
        if (_CmpPfn(_P1, _Pos))
        {
            swap(_P0, _P1);
            ++_P0;
        };
    };
    swap(_P0, _Pos);
    return _P0;
};
void sort_quick(int *_Begin, int *_End, int (*_CmpPfn)(int *, int *))
{ //[_Begin,_End)    _Cmpfn(T*,T*)
    if (_End > _Begin)
    {
        int *_Pos = _partition(_Begin, _End, _CmpPfn);
        sort_quick(_Begin, _Pos, _CmpPfn);
        sort_quick(_Pos + 1, _End, _CmpPfn);
    };
};
void for_each(int *elm, size_t size, void (*pfn)(int *))
{
    while (size--)
    {
        pfn(elm++);
    };
};
void quick_sort(int q[], int l, int r)
{
    if (l >= r)
        return;
    int i = l - 1, j = r + 1, x = q[(l + r) >> 1];
    while (i < j)
    {
        while (q[++i] < x)
            ;
        while (q[--j] > x)
            ;
        if (i < j)
            (q[i] ^= q[j], q[j] ^= q[i], q[i] ^= q[j]);
    }
    quick_sort(q, l, j), quick_sort(q, j + 1, r);
}