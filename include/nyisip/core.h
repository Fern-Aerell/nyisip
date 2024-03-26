#pragma once

#include <string>
#include <vector>

#define PROGRAM_NAME "nyisip"
#define PROGRAM_VERSION "1.0.0"

struct Binary {
    Binary() = default;
    explicit Binary(const std::vector<char>& data);
    explicit Binary(const std::vector<unsigned char>& data);
    std::vector<unsigned char> data;
};

struct Hexadecimal {
    Hexadecimal() = default;
    explicit Hexadecimal(std::string data);
    std::string data;
};

struct DecodedText {
    DecodedText() = default;
    explicit DecodedText(std::string data);
    std::string data;
};
