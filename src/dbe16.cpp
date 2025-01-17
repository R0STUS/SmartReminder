#include "dbe16.hpp"

std::vector<std::vector<Sym>> DBE16::as = {
    {
        {"ff", "�"}, {"0", " "}, {"1", "\n"}, {"2", "0"}, {"3", "1"},
        {"4", "2"}, {"5", "3"}, {"6", "4"}, {"7", "5"}, {"8", "6"},
        {"9", "7"}, {"a", "8"}, {"b", "9"}, {"c", "!"}, {"d", "@"},
        {"e", "#"}, {"f", "$"}, {"10", "%"}, {"11", "^"}, {"12", "&"},
        {"13", "*"}, {"14", "("}, {"15", ")"}, {"16", "-"}, {"17", "_"},
        {"18", ","}, {"19", "."}, {"20", "?"}, {"21", "/"}, {"22", "\\"},
        {"23", "|"}, {"24", "~"}, {"25", "`"}, {"26", ":"}, {"27", ";"},
        {"28", "<"}, {"29", ">"}, {"2a", "{"}, {"2b", "}"}, {"2c", "["},
        {"2d", "]"}, {"2e", "'"}, {"2f", "\""}, {"30", "="}, {"31", "+"},
        {"32", "№"}, {"33", "\xc2\xa9"}, {"34", "\xc2\xae"},
        {"35", "\xe2\x84\xa2"}, {"36", "\xc2\xa0"}, {"37", "\xc2\xa1"},
        {"38", "\xe2\x89\xa0"}, {"39", "\xc2\xb1"}, {"3a", "\xc2\xa7"},
        {"3b", "\xc2\xb6"}, {"3c", "\xe2\x88\x9e"}, {"3d", "\xe2\x88\x91"},
        {"3e", "\xe2\x88\x9a"}, {"3f", "\xe2\x88\xb0"}, {"40", "\xe2\x88\x9c"},
        {"41", "\xe2\x89\xa8"}, {"42", "\xe2\x88\xb2"}, {"43", "\xe2\x88\x9e"},
        {"44", "\xe2\x89\xa8"}, {"45", "\xe2\x88\x82"}, {"46", "\xe2\x88\x87"},
        {"47", "\xe2\x88\x88"}, {"48", "\xe2\x88\x89"}, {"49", "\xe2\x88\xa9"},
        {"4a", "\xe2\x88\xaa"}, {"4b", "\xe2\x88\xa8"}, {"4c", "\xe2\x88\xa9"},
        {"4d", "\xe2\x88\x9a"}, {"4e", "\xe2\x88\x88"}, {"4f", "\xe2\x88\xa3"},
        {"50", "\xe2\x88\xba"}
    }, { {"ff", "�"},
        {"0", "a"}, {"1", "b"}, {"2", "c"}, {"3", "d"},
        {"4", "e"}, {"5", "f"}, {"6", "g"}, {"7", "h"},
        {"8", "i"}, {"9", "j"}, {"a", "k"}, {"b", "l"},
        {"c", "m"}, {"d", "n"}, {"e", "o"}, {"f", "p"},
        {"10", "q"}, {"11", "r"}, {"12", "s"}, {"13", "t"},
        {"14", "u"}, {"15", "v"}, {"16", "w"}, {"17", "x"},
        {"18", "y"}, {"19", "z"}, {"1a", "A"}, {"1b", "B"},
        {"1c", "C"}, {"1d", "D"}, {"1e", "E"}, {"1f", "F"},
        {"20", "G"}, {"21", "H"}, {"22", "I"}, {"23", "J"},
        {"24", "K"}, {"25", "L"}, {"26", "M"}, {"27", "N"},
        {"28", "O"}, {"29", "P"}, {"2a", "Q"}, {"2b", "R"},
        {"2c", "S"}, {"2d", "T"}, {"2e", "U"}, {"2f", "V"},
        {"30", "W"}, {"31", "X"}, {"32", "Y"}, {"33", "Z"}
    }, { {"ff", "�"},
        {"0", "а"}, {"1", "б"}, {"2", "в"}, {"3", "г"},
        {"4", "д"}, {"5", "е"}, {"6", "ё"}, {"7", "ж"},
        {"8", "з"}, {"9", "и"}, {"a", "й"}, {"b", "к"},
        {"c", "л"}, {"d", "м"}, {"e", "н"}, {"f", "о"},
        {"10", "п"}, {"11", "р"}, {"12", "с"}, {"13", "т"},
        {"14", "у"}, {"15", "ф"}, {"16", "х"}, {"17", "ц"},
        {"18", "ч"}, {"19", "ш"}, {"1a", "щ"}, {"1b", "ъ"},
        {"1c", "ы"}, {"1d", "ь"}, {"1e", "э"}, {"1f", "ю"},
        {"20", "я"}, {"21", "А"}, {"22", "Б"}, {"23", "В"},
        {"24", "Г"}, {"25", "Д"}, {"26", "Е"}, {"27", "Ё"},
        {"28", "Ж"}, {"29", "З"}, {"2a", "И"}, {"2b", "Й"},
        {"2c", "К"}, {"2d", "Л"}, {"2e", "М"}, {"2f", "Н"},
        {"30", "О"}, {"31", "П"}, {"32", "Р"}, {"33", "С"},
        {"34", "Т"}, {"35", "У"}, {"36", "Ф"}, {"37", "Х"},
        {"38", "Ц"}, {"39", "Ч"}, {"3a", "Ш"}, {"3b", "Щ"},
        {"3c", "Ъ"}, {"3d", "Ы"}, {"3e", "Ь"}, {"3f", "Э"},
        {"40", "Ю"}, {"41", "Я"}
    }
};

std::string DBE16::convert(std::vector<Sym>& s, const std::string& input, bool isSensable, bool isHex) {
    std::string output;
    bool isCount = false;
    for (auto& i : s) {
        if (i.hex == input && isHex) {
            output = i.symbol;
            isCount = true;
        }
        else if (i.symbol == input && !isHex) {
            output = i.hex;
            isCount = true;
        }
    }
    if (!isCount) {
        if (!isSensable) {
            output = "�";
        }
        else {
            output = "ff";
        }
    }
    return output;
}

std::string DBE16::decode(const std::string& input) {
    std::string output = "";
    int whatnow = 0;
    std::string tmpc;
    std::string tmp = "";
    std::string words[2]{"", ""};
    for (char c : input) {
        if (c == ' ' && tmp != "") {
            if (whatnow == 0) {
                words[0] = tmp;
                whatnow = 1;
            }
            else if (whatnow == 1) {
                words[1] = tmp;
                whatnow = 0;
                tmpc = DBE16::convert(as[std::stoi(words[0], nullptr, 16)], words[1], false, true);
                output += tmpc;
            }
            tmp = "";
        }
        else
            tmp += c;
    }
    return output;
}

std::string DBE16::converttohex(const std::string& input) {
    std::string output = "";
    std::string converted;
    int numCount;
    std::stringstream ss;
    std::string hexNum;
    bool isCount = false;
    for (size_t i = 0; i < input.size();) {
        unsigned char c = static_cast<unsigned char>(input[i]);
        if (c < 0x80) {
            ss.str("");
            ss.clear();
            for (int j = 0; j < as.size(); j++) {
                for (const Sym& s : as[j]) {
                    if (s.symbol == std::string(1, c)) {
                        numCount = j;
                        ss << std::hex << numCount;
                        hexNum = ss.str();
                        isCount = true;
                        break;
                    }
                }
            }
            if (isCount)
                converted = hexNum + " " + DBE16::convert(as[numCount], std::string(1, c), true, false);
            else
                converted = "00 ff";
            output += converted + " ";
            i++;
        } else if ((c & 0xE0) == 0xC0) {
            std::string utf8Char = input.substr(i, 2);
            ss.str("");
            ss.clear();

            for (int j = 0; j < as.size(); j++) {
                for (const Sym& s : as[j]) {
                    if (s.symbol == utf8Char) {
                        numCount = j;
                        ss << std::hex << numCount;
                        hexNum = ss.str();
                        isCount = true;
                        break;
                    }
                }
            }
            if (isCount)
                converted = hexNum + " " + DBE16::convert(as[numCount], utf8Char, true, false);
            else
                converted = "00 ff";
            output += converted + " ";
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            std::string utf8Char = input.substr(i, 3);
            ss.str("");
            ss.clear();
            for (int j = 0; j < as.size(); j++) {
                for (const Sym& s : as[j]) {
                    if (s.symbol == utf8Char) {
                        numCount = j;
                        ss << std::hex << numCount;
                        hexNum = ss.str();
                        isCount = true;
                        break;
                    }
                }
            }
            if (isCount)
                converted = hexNum + " " + DBE16::convert(as[numCount], utf8Char, true, false);
            else
                converted = "00 ff";
            output += converted + " ";
            i += 3;
        } else {
            std::string utf8Char = input.substr(i, 4);
            ss.str("");
            ss.clear();
            for (int j = 0; j < as.size(); j++) {
                for (const Sym& s : as[j]) {
                    if (s.symbol == utf8Char) {
                        numCount = j;
                        ss << std::hex << numCount;
                        hexNum = ss.str();
                        isCount = true;
                        break;
                    }
                }
            }
            if (isCount)
                converted = hexNum + " " + DBE16::convert(as[numCount], utf8Char, true, false);
            else
                converted = "00 ff";
            output += converted + " ";
            i += 4;
        }
    }
    return output;
}

void DBE16::writefile(const std::string& filePath, const std::string& hexValues) {
    std::cout << "[" << filePath << "] Saving file..." << std::endl;
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile) {
        std::cerr << "Couldn't open file!" << std::endl;
        return;
    }
    std::istringstream iss(hexValues);
    std::string hexValue;
    while (iss >> hexValue) {
        unsigned char byteValue = static_cast<unsigned char>(std::stoi(hexValue, nullptr, 16));
        outFile.write(reinterpret_cast<const char*>(&byteValue), sizeof(byteValue));
    }
    outFile.close();
    if (!outFile) {
        std::cerr << "Couldn't close file!" << std::endl;
    } else {
        std::cout << "[" << filePath << "] File saved successfully." << std::endl;
    }
}

std::string DBE16::readfile(const std::string& filePath) {
    std::cout << "[" << filePath << "] Reading file..." << std::endl;
    std::ifstream inFile(filePath, std::ios::binary);
    if (!inFile) {
        std::cerr << "Failed to open read file!" << std::endl;
        return "";
    }
    std::ostringstream oss;
    unsigned char byteValue;
    while (inFile.read(reinterpret_cast<char*>(&byteValue), sizeof(byteValue))) {
        oss << std::hex << static_cast<int>(byteValue) << " ";
    }
    inFile.close();
    std::string hexString = oss.str();
    std::cout << "[" << filePath << "] 16x-code: " << hexString << std::endl;
    return decode(hexString);
}
