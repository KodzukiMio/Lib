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
char *SZN_ReadLineUnlimited(size_t initcapacity)
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
    return line;
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