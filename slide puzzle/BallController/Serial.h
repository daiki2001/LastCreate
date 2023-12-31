#pragma once
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

class Serial {
public:
    // Serialクラスの生成
    static Serial* create(const char* dev);
    ~Serial();
    int read(char* buf, unsigned int bufSize);
private:
#if defined(_WIN32) || defined(_WIN64)
    Serial(HANDLE fd);
    HANDLE fd;
#else
    Serial(int fd);
    int fd;
#endif
};
