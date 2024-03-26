#include "nyisip/core.h"

#include <vector>

Binary::Binary(const std::vector<char>& data): data{data.begin(), data.end()} {}

Binary::Binary(const std::vector<unsigned char>& data): data(data) {}

Hexadecimal::Hexadecimal(std::string data): data(std::move(data)) {}

DecodedText::DecodedText(std::string data): data(std::move(data)) {}
