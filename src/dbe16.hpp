#ifndef DBE16_HPP
#define DBE16_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <locale>
#include <codecvt>

struct Sym {
    std::string hex;
    std::string symbol;
};

class DBE16 {
public:
    static std::string convert(std::vector<Sym>& s, const std::string& input, bool isSensable, bool isHex);
    static std::string decode(const std::string& input);
    static std::string converttohex(const std::string& input);
    static std::string readfile(const std::string& filePath);
    static void writefile(const std::string& filePath, const std::string& content);

    static std::vector<std::vector<Sym>> as;
};

#endif // DBE16_HPP
