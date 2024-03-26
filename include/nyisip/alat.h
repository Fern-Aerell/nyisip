#pragma once

#include "nyisip/core.h"

#include <string>

Binary readFileToBin(const std::string& filePath);

Hexadecimal binToHex(const Binary& bin);

DecodedText hexToDecode(const Hexadecimal& hex);

Hexadecimal decodeToHex(const DecodedText& decode);

Binary hexToBin(const Hexadecimal& hex);

void writeBinToFile(const Binary& bin, const std::string& filePath);

// Fungsi untuk menghapus prefix dari string
void removeStrPrefix(std::string& str, const std::string& prefix);

// Fungsi untuk menghapus suffix dari string
void removeStrSuffix(std::string& str, const std::string& suffix);
