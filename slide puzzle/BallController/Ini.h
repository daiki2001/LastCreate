#pragma once
#include <map>
#include <string>

class Ini {
public:
    // iniファイルの読み込み
    Ini(const char* filePath);
    std::string getValue(const char* section, const char* key);
private:
    // エラーメッセージの出力
    void raise(const std::string& string);
    std::map<std::string, std::map<std::string, std::string>> content;
};
