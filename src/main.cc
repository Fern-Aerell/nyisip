#include "nyisip/core.h"
#include "nyisip/nyisip.h"

#include <cstring>
#include <format>
#include <iostream>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

bool isSizeT(const std::string& str) {
  std::istringstream iss(str);
  size_t value = 0;
  iss >> value;
  return !iss.fail() && iss.eof();
}

size_t convertToSizeT(const char* str) {
  // Mengonversi string menjadi size_t menggunakan std::strtoul
  char* endPtr = nullptr;
  const int radix(10);
  unsigned long longValue = std::strtoul(str, &endPtr, radix);

  // Periksa apakah konversi gagal atau ada karakter tambahan setelah konversi
  if(*endPtr != '\0' ||
     longValue >
         static_cast<unsigned long>(std::numeric_limits<size_t>::max())) {
    throw std::invalid_argument("Index tidak valid.");
  }

  return static_cast<size_t>(longValue);
}

int main(int argc, const char* argv[]) {
  try {
    if(argc <= 1) {
      std::cout << "Silahkan 'nyisip -i | --info' untuk informasi lebih lanjut."
                << "\n";
      return 0;
    }
    const auto args = std::span(argv, sizeof(argc));
    if(argc == 4) {
      if(strcmp(args[1], "-k") == 0 && isSizeT(args[2])) {
        Nyisip nyisip(args[3]);
        nyisip.keluarKanIndex(convertToSizeT(args[2]));
        std::cout << std::format(
                         "Mengeluarkan file index ke-{} sisipan selesai.",
                         args[2]
                     )
                  << "\n";
        return 0;
      }
      if(strcmp(args[1], "-h") == 0 && isSizeT(args[2])) {
        Nyisip nyisip(args[3]);
        nyisip.HapusKanIndex(convertToSizeT(args[2]));
        std::cout << std::format(
                         "Menghapuskan file index ke-{} sisipan selesai.",
                         args[2]
                     )
                  << "\n";
        return 0;
      }
      if(strcmp(args[1], "-kh") == 0 && isSizeT(args[2])) {
        Nyisip nyisip(args[3]);
        nyisip.keluarKanDanHapusIndex(convertToSizeT(args[2]));
        std::cout << std::format(
                         "Mengeluarkan dan menghapus file index ke-{} sisipan "
                         "selesai.",
                         args[2]
                     )
                  << "\n";
        return 0;
      }
    }
    if(argc == 3) {
      if(strcmp(args[1], "-k") == 0) {
        Nyisip nyisip(args[2]);
        nyisip.keluarKanSemua();
        std::cout << "Mengeluarkan semua file sisipan selesai."
                  << "\n";
        return 0;
      }
      if(strcmp(args[1], "-h") == 0) {
        Nyisip nyisip(args[2]);
        nyisip.HapusKanSemua();
        std::cout << "Menghapuskan semua file sisipan selesai."
                  << "\n";
        return 0;
      }
      if(strcmp(args[1], "-kh") == 0) {
        Nyisip nyisip(args[2]);
        nyisip.keluarKanDanHapusSemua();
        std::cout << "Mengeluarkan dan menghapus semua file sisipan selesai."
                  << "\n";
        return 0;
      }
      if(strcmp(args[1], "-d") == 0) {
        Nyisip nyisip(args[2]);
        std::vector<std::string> daftar(nyisip.daftar());
        size_t index = 0;
        std::cout << std::format("File '{}' telah disisipkan :", args[2])
                  << "\n\n";
        for(const auto& element : daftar) {
          std::cout << std::format("[{}] => {}", index, element) << "\n";
          ++index;
        }
        std::cout << "\nMembuatkan daftar selesai."
                  << "\n";
        return 0;
      }
    }
    if(argc == 2) {
      if(strcmp(args[1], "-v") == 0 || strcmp(args[1], "--versi") == 0) {
        std::cout << PROGRAM_NAME << " " << PROGRAM_VERSION << "\n";
        return 0;
      }
      if(strcmp(args[1], "-i") == 0 || strcmp(args[1], "--info") == 0) {
        std::cout << "Program untuk menyisipkan suatu file ke file lain"
                  << "\n\n";
        std::cout << "Penggunaan : "
                  << "\n";
        std::cout << "  nyisip -i | --info                               "
                     "Menampilkan informasi."
                  << "\n";
        std::cout << "  nyisip -v | --versi                              "
                     "Menampilkan versi."
                  << "\n";
        std::cout << "  nyisip <file-yg-mau-disisipkan> <file-sisipan>*  "
                     "Menyisipkan file."
                  << "\n";
        std::cout << "  nyisip -k <index> <file-yg-disisipkan>           "
                     "Mengeluarkan file sisipan dengan index tertentu."
                  << "\n";
        std::cout << "  nyisip -k <file-yg-disisipkan>                   "
                     "Mengeluarkan semua file sisipan."
                  << "\n";
        std::cout
            << "  nyisip -h <index> <file-yg-disisipkan>           Menghapus "
               "file sisipan dengan index tertentu dari file yang disisipkan."
            << "\n";
        std::cout << "  nyisip -h <file-yg-disisipkan>                   "
                     "Menghapus semua file sisipan dari file yang disisipkan."
                  << "\n";
        std::cout << "  nyisip -kh <index> <file-yg-disisipkan>          "
                     "Mengeluarkan dan menghapus file sisipan dengan index "
                     "tertentu dari file yang disisipkan."
                  << "\n";
        std::cout << "  nyisip -kh <file-yg-disisipkan>                  "
                     "Mengeluarkan dan menghapus semua file sisipan dari file "
                     "yang disisipkan."
                  << "\n";
        std::cout << "  nyisip -d <file-yg-disisipkan>                   "
                     "Melihat daftar file sisipan."
                  << "\n\n";
        std::cout << "Dibuat oleh AerellDev"
                  << "\n";
        return 0;
      }
    }
    if(argc > 2) {
      Nyisip nyisip(args[1]);
      for(size_t index = 2; index < static_cast<size_t>(argc); ++index) {
        nyisip.sisipKan(args[index]);
      }
      std::cout << "Penyisipan selesai."
                << "\n";
      return 0;
    }
    if(argc == 2) {
      throw std::invalid_argument(
          std::format("Perintah '{}' tidak ada.", args[1])
      );
    }
  } catch(const std::exception& error) { std::cerr << error.what() << "\n"; }
  return 0;
}
