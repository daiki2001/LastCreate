#include <stdexcept>
#include "serial.h"

#if defined(_WIN32) || defined(_WIN64)
#include <string>

Serial* Serial::create(const char* device) {
    const std::string deviceStr = std::string("\\\\.\\") + device;
    const std::wstring deviceWstr = std::wstring(deviceStr.begin(), deviceStr.end());
    HANDLE fd = INVALID_HANDLE_VALUE;
    try {
        // ‰Šú‰»
        if ((fd = CreateFile(deviceWstr.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) throw std::runtime_error("Failed to open");
        if (!SetupComm(fd, 1024, 1024)) throw std::runtime_error("Failed to setup");
        if (!PurgeComm(fd, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR)) {
            throw std::runtime_error("Failed to purge");
        }
        DCB dcb;
        if (!GetCommState(fd, &dcb)) throw std::runtime_error("Failed to get state");
        dcb.DCBlength = sizeof(DCB);
        dcb.BaudRate = 115200;
        dcb.fBinary = TRUE;
        dcb.ByteSize = 8;
        dcb.fParity = NOPARITY;
        dcb.StopBits = ONESTOPBIT;
        if (!SetCommState(fd, &dcb)) throw std::runtime_error("Failed to set state");
    } catch (std::runtime_error e) {
        printf("[Serial] %s\n", e.what());
        if (fd != INVALID_HANDLE_VALUE) CloseHandle(fd);
        return nullptr;
    }
    return new Serial(fd);
}

Serial::Serial(HANDLE fd) : fd(fd) {}

Serial::~Serial() {
    CloseHandle(this->fd);
}

int Serial::read(char* buf, unsigned int bufSize) {
    DWORD errors;
    COMSTAT stat;
    if (!ClearCommError(this->fd, &errors, &stat)) return -1;
    
    DWORD byteCount;
    if (!ReadFile(this->fd, buf, min(stat.cbInQue, bufSize), &byteCount, NULL)) return -1;
    return byteCount;
}

#else

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

Serial* Serial::create(const char* device) {
    int fd = -1;
    try {
        if ((fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY)) == -1) throw std::runtime_error("Failed to open");
        struct termios options;
        if (tcgetattr(fd, &options) == -1) throw std::runtime_error("Failed to get attr");
        options.c_cflag &= ~(CSIZE | CSTOPB | PARENB);
        options.c_cflag |= CS8 | CREAD | CLOCAL;
        if (cfsetspeed(&options, B115200) == -1) throw std::runtime_error("Failed to set speed");
        if (tcsetattr(fd, TCSANOW, &options) == -1) throw std::runtime_error("Failed to set attr");
    } catch (std::runtime_error e) {
        printf("[Serial] %s\n", e.what());
        if (fd != -1) close(fd);
        return nullptr;
    }
    return new Serial(fd);
}

Serial::Serial(int fd) : fd(fd) {}

Serial::~Serial() {
    close(this->fd);
}

int Serial::read(char* buf, unsigned int bufSize) {
    return ::read(this->fd, buf, bufSize);
}
#endif
