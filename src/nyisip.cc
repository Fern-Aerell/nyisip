#include "nyisip/nyisip.h"

#include "nyisip/alat.h"
#include "nyisip/core.h"

#include <filesystem>
#include <format>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

Nyisip::Nyisip(const std::string& filePath): filePath(filePath) {}

void Nyisip::sisipKan(const std::string& filePath) {
  Binary bin(readFileToBin(this->filePath.string()));
  Hexadecimal hex(binToHex(bin));
  std::vector<std::array<Hexadecimal, 2>> allFileSisipanHex(
      this->getHexAllFileSisipan(hex)
  );
  std::array<Hexadecimal, 2> fileSisipanHexArray;
  std::filesystem::path fileSisipanPath(filePath);
  fileSisipanHexArray[0] =
      decodeToHex(DecodedText(fileSisipanPath.filename().string()));
  Binary fileSisipanBin(readFileToBin(filePath));
  Hexadecimal fileSisipanHex(binToHex(fileSisipanBin));
  fileSisipanHexArray[1] = std::move(fileSisipanHex);
  allFileSisipanHex.push_back(std::move(fileSisipanHexArray));
  size_t index = 0;
  std::cout << std::format(
                   "Menyisipkan file '{}' ke '{}'.", filePath,
                   this->filePath.string()
               )
            << "\n";
  for(const auto& element : allFileSisipanHex) {
    this->sisipKanFileHex(hex, element, index);
    ++index;
  }
  bin = hexToBin(hex);
  return writeBinToFile(bin, this->filePath.string());
}

void Nyisip::keluarKanSemua() {
  Binary bin(readFileToBin(this->filePath.string()));
  Hexadecimal hex(binToHex(bin));
  std::vector<std::array<Hexadecimal, 2>> allFileSisipanHex(
      this->getHexAllFileSisipan(hex)
  );
  if(!allFileSisipanHex.empty()) {
    for(const auto& element : allFileSisipanHex) {
      Binary isiFileBin = hexToBin(element[1]);
      std::string namaFile(hexToDecode(element[0]).data);
      std::string lokasiFileYangMauDisimpan =
          this->filePath.parent_path().string();
      if(lokasiFileYangMauDisimpan.empty()) {
        lokasiFileYangMauDisimpan += namaFile;
      } else {
        lokasiFileYangMauDisimpan += "/" + namaFile;
      }
      std::cout << std::format(
                       "Mengeluarkan file '{}' dari '{}'.", namaFile,
                       this->filePath.string()
                   )
                << "\n";
      writeBinToFile(isiFileBin, lokasiFileYangMauDisimpan);
    }
  } else {
    throw std::runtime_error("Tidak ada file yang disisipkan.");
  }
}

void Nyisip::keluarKanDanHapusSemua() {
  Binary bin(readFileToBin(this->filePath.string()));
  Hexadecimal hex(binToHex(bin));
  std::vector<std::array<Hexadecimal, 2>> allFileSisipanHex(
      this->getHexAllFileSisipan(hex)
  );
  if(!allFileSisipanHex.empty()) {
    for(const auto& element : allFileSisipanHex) {
      Binary isiFileBin = hexToBin(element[1]);
      std::string namaFile(hexToDecode(element[0]).data);
      std::string lokasiFileYangMauDisimpan =
          this->filePath.parent_path().string();
      if(lokasiFileYangMauDisimpan.empty()) {
        lokasiFileYangMauDisimpan += namaFile;
      } else {
        lokasiFileYangMauDisimpan += "/" + namaFile;
      }
      std::cout << std::format(
                       "Mengeluarkan dan menghapus file '{}' dari '{}'.",
                       namaFile, this->filePath.string()
                   )
                << "\n";
      writeBinToFile(isiFileBin, lokasiFileYangMauDisimpan);
    }
    bin = hexToBin(hex);
    writeBinToFile(bin, this->filePath.string());
  } else {
    throw std::runtime_error("Tidak ada file yang disisipkan.");
  }
}

void Nyisip::keluarKanIndex(const size_t& index) {
  Binary bin(readFileToBin(this->filePath.string()));
  Hexadecimal hex(binToHex(bin));
  std::vector<std::array<Hexadecimal, 2>> allFileSisipanHex(
      this->getHexAllFileSisipan(hex)
  );
  if(!(index < allFileSisipanHex.size())) {
    throw std::runtime_error(
        std::format("Tidak ada file sisipan index ke-{}.", index)
    );
  }
  if(!allFileSisipanHex.empty()) {
    Binary isiFileBin = hexToBin(allFileSisipanHex[index][1]);
    std::string namaFile(hexToDecode(allFileSisipanHex[index][0]).data);
    std::string lokasiFileYangMauDisimpan =
        this->filePath.parent_path().string();
    if(lokasiFileYangMauDisimpan.empty()) {
      lokasiFileYangMauDisimpan += namaFile;
    } else {
      lokasiFileYangMauDisimpan += "/" + namaFile;
    }
    std::cout << std::format(
                     "Mengeluarkan file '{}' dari '{}'.", namaFile,
                     this->filePath.string()
                 )
              << "\n";
    writeBinToFile(isiFileBin, lokasiFileYangMauDisimpan);
  } else {
    throw std::runtime_error("Tidak ada file yang disisipkan.");
  }
}

void Nyisip::keluarKanDanHapusIndex(const size_t& index) {
  Binary bin(readFileToBin(this->filePath.string()));
  Hexadecimal hex(binToHex(bin));
  std::vector<std::array<Hexadecimal, 2>> allFileSisipanHex(
      this->getHexAllFileSisipan(hex)
  );
  if(!(index < allFileSisipanHex.size())) {
    throw std::runtime_error(
        std::format("Tidak ada file sisipan index ke-{}.", index)
    );
  }
  if(!allFileSisipanHex.empty()) {
    Binary isiFileBin = hexToBin(allFileSisipanHex[index][1]);
    std::string namaFile(hexToDecode(allFileSisipanHex[index][0]).data);
    std::string lokasiFileYangMauDisimpan =
        this->filePath.parent_path().string();
    if(lokasiFileYangMauDisimpan.empty()) {
      lokasiFileYangMauDisimpan += namaFile;
    } else {
      lokasiFileYangMauDisimpan += "/" + namaFile;
    }
    std::cout << std::format(
                     "Mengeluarkan dan menghapus file '{}' dari '{}'.",
                     namaFile, this->filePath.string()
                 )
              << "\n";
    writeBinToFile(isiFileBin, lokasiFileYangMauDisimpan);
    allFileSisipanHex.erase(
        allFileSisipanHex.begin() + static_cast<long long>(index)
    );
    size_t index = 0;
    for(const auto& element : allFileSisipanHex) {
      this->sisipKanFileHex(hex, element, index);
      ++index;
    }
    bin = hexToBin(hex);
    writeBinToFile(bin, this->filePath.string());
  } else {
    throw std::runtime_error("Tidak ada file yang disisipkan.");
  }
}

void Nyisip::HapusKanSemua() {
  Binary bin(readFileToBin(this->filePath.string()));
  Hexadecimal hex(binToHex(bin));
  std::vector<std::array<Hexadecimal, 2>> allFileSisipanHex(
      this->getHexAllFileSisipan(hex)
  );
  if(!allFileSisipanHex.empty()) {
    std::cout << std::format(
                     "Menghapus semua file sisipan dari '{}'.",
                     this->filePath.string()
                 )
              << "\n";
    bin = hexToBin(hex);
    writeBinToFile(bin, this->filePath.string());
  } else {
    throw std::runtime_error("Tidak ada file yang disisipkan.");
  }
}

void Nyisip::HapusKanIndex(const size_t& index) {
  Binary bin(readFileToBin(this->filePath.string()));
  Hexadecimal hex(binToHex(bin));
  std::vector<std::array<Hexadecimal, 2>> allFileSisipanHex(
      this->getHexAllFileSisipan(hex)
  );
  if(!(index < allFileSisipanHex.size())) {
    throw std::runtime_error(
        std::format("Tidak ada file sisipan index ke-{}.", index)
    );
  }
  if(!allFileSisipanHex.empty()) {
    std::cout << std::format(
                     "Menghapus file sisipan index ke-{} dari '{}'.", index,
                     this->filePath.string()
                 )
              << "\n";
    allFileSisipanHex.erase(
        allFileSisipanHex.begin() + static_cast<long long>(index)
    );
    size_t index = 0;
    for(const auto& element : allFileSisipanHex) {
      this->sisipKanFileHex(hex, element, index);
      ++index;
    }
    bin = hexToBin(hex);
    writeBinToFile(bin, this->filePath.string());
  } else {
    throw std::runtime_error("Tidak ada file yang disisipkan.");
  }
}

std::vector<std::string> Nyisip::daftar() {
  Binary bin(readFileToBin(this->filePath.string()));
  Hexadecimal hex(binToHex(bin));
  std::vector<std::array<Hexadecimal, 2>> allFileSisipanHex(
      this->getHexAllFileSisipan(hex)
  );
  if(!allFileSisipanHex.empty()) {
    std::vector<std::string> hasil;
    hasil.reserve(allFileSisipanHex.size());
    for(const auto& element : allFileSisipanHex) {
      hasil.push_back(hexToDecode(element[0]).data);
    }
    return hasil;
  }
  throw std::runtime_error("Tidak ada file yang disisipkan.");
}

// Fungsi untuk membuat kata kunci
Hexadecimal Nyisip::generateHexKeyword(
    const bool& isFileContent, const bool& isSuffix, const size_t& index
) {
  return decodeToHex(DecodedText(std::format(
      "[{}{}{}][{}]", std::string(PROGRAM_NAME PROGRAM_VERSION),
      isFileContent ? "filecontent" : "filename", isSuffix ? "end" : "begin",
      index
  )));
}

std::unique_ptr<std::array<Hexadecimal, 2>>
Nyisip::getHexFileSisipanById(Hexadecimal& hex, const size_t& index) {
  std::array<Hexadecimal, 2> hasil;
  Hexadecimal tagNamaFileAwalHex = generateHexKeyword(false, false, index);
  Hexadecimal tagNamaFileAkhirHex = generateHexKeyword(false, true, index);
  Hexadecimal tagIsiFileAwalHex = generateHexKeyword(true, false, index);
  Hexadecimal tagIsiFileAkhirHex = generateHexKeyword(true, true, index);
  std::regex tagNamaFileHexRegex(
      tagNamaFileAwalHex.data + ".*" + tagNamaFileAkhirHex.data
  );
  std::regex tagIsiFileHexRegex(
      tagIsiFileAwalHex.data + ".*" + tagIsiFileAkhirHex.data
  );
  std::sregex_iterator iterator(
      hex.data.begin(), hex.data.end(), tagNamaFileHexRegex
  );
  std::sregex_iterator endIterator;
  bool status = false;
  if(iterator != endIterator) {
    const std::smatch& match = *iterator;
    hasil[0].data = match.str();
    removeStrPrefix(hasil[0].data, tagNamaFileAwalHex.data);
    removeStrSuffix(hasil[0].data, tagNamaFileAkhirHex.data);
    hex.data.erase(match.position(), match.str().size());
    status = true;
  }
  iterator = std::sregex_iterator(
      hex.data.begin(), hex.data.end(), tagIsiFileHexRegex
  );
  if(iterator != endIterator) {
    const std::smatch& match = *iterator;
    hasil[1].data = match.str();
    removeStrPrefix(hasil[1].data, tagIsiFileAwalHex.data);
    removeStrSuffix(hasil[1].data, tagIsiFileAkhirHex.data);
    hex.data.erase(match.position(), match.str().size());
  }
  return status ? std::make_unique<std::array<Hexadecimal, 2>>(std::move(hasil))
                : nullptr;
}

std::vector<std::array<Hexadecimal, 2>>
Nyisip::getHexAllFileSisipan(Hexadecimal& hex) {
  std::vector<std::array<Hexadecimal, 2>> kFileSisipanHex;
  size_t index = 0;
  while(const auto hasil = this->getHexFileSisipanById(hex, index)) {
    kFileSisipanHex.push_back(*hasil);
    ++index;
  }
  return kFileSisipanHex;
}

void Nyisip::sisipKanFileHex(
    Hexadecimal& hex, const std::array<Hexadecimal, 2>& fileSisipanHex,
    const size_t& index
) {
  // Nama file
  hex.data += generateHexKeyword(false, false, index).data;
  hex.data += fileSisipanHex[0].data;
  hex.data += generateHexKeyword(false, true, index).data;
  // Isi file
  hex.data += generateHexKeyword(true, false, index).data;
  hex.data += fileSisipanHex[1].data;
  hex.data += generateHexKeyword(true, true, index).data;
}
