#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "../../include/zlog.h"
using namespace std;
string readFileDate(const char *filePath)
{
    string str, tmp;
    ifstream myfile(filePath);
    if (!myfile.is_open())
    {
        cout << filePath << " open failed" << endl;
    }
    while (getline(myfile, tmp))
    {
        str.append(tmp);
    }
    myfile.close();
    return str;
}
/*
in:
out: type:int;
*/
int readBinFileData(const char *filePath, char **buf)
{
    FILE *fp = fopen(filePath, "rb");
    if (!fp)
    {
        cout << filePath << " open failed" << endl;
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    //获取到文件大小 字节数
    int file_len = ftell(fp);
    *buf = (char *)malloc(file_len);
    if (NULL == *buf)
    {
        cout << "readBinFileData malloc failed" << endl;
        return -1;
    }
    memset(*buf, 0, file_len);
    //复位文件句柄
    rewind(fp);
    fread(*buf, sizeof(char), file_len, fp);
    fclose(fp);

    return file_len;
}

char *readBinFileData(const char *filePath)
{
    FILE *fp = fopen(filePath, "rb");
    if (!fp)
    {
        cout << filePath << " open failed" << endl;
    }
    fseek(fp, 0L, SEEK_END);
    //获取到文件大小 字节数
    int file_len = ftell(fp);
    char *buf = (char *)malloc(file_len);
    if (NULL == buf)
    {
        cout << "readBinFileData malloc failed" << endl;
    }
    memset(buf, 0, file_len);
    //复位文件句柄
    rewind(fp);
    fread(buf, sizeof(char), file_len, fp);
    fclose(fp);

    return buf;
}
