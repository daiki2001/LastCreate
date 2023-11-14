#pragma once
#include <map>
#include <string>

class Ini {
public:
    // ini�t�@�C���̓ǂݍ���
    Ini(const char* filePath);
    std::string getValue(const char* section, const char* key);
private:
    // �G���[���b�Z�[�W�̏o��
    void raise(const std::string& string);
    std::map<std::string, std::map<std::string, std::string>> content;
};
