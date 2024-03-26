#pragma once

#include "nyisip/core.h"

#include <array>
#include <filesystem>
#include <string>
#include <vector>

class Nyisip {
  public:
    explicit Nyisip(const std::string& filePath);
    void sisipKan(const std::string& filePath);
    void keluarKanSemua();
    void keluarKanDanHapusSemua();
    void keluarKanIndex(const size_t& index);
    void keluarKanDanHapusIndex(const size_t& index);
    void HapusKanSemua();
    void HapusKanIndex(const size_t& index);
    std::vector<std::string> daftar();

  private:
    std::filesystem::path filePath;
    Hexadecimal generateHexKeyword(
        const bool& isFileContent, const bool& isSuffix, const size_t& index
    );
    std::unique_ptr<std::array<Hexadecimal, 2>>
    getHexFileSisipanById(Hexadecimal& hex, const size_t& index);
    std::vector<std::array<Hexadecimal, 2>>
    getHexAllFileSisipan(Hexadecimal& hex);
    void sisipKanFileHex(
        Hexadecimal& hex, const std::array<Hexadecimal, 2>& fileSisipanHex,
        const size_t& index
    );

  protected:
};
