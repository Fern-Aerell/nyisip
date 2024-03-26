#include "nyisip/alat.h"

#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>

// Fungsi untuk membaca file dalam bentuk biner
Binary readFileToBin(const std::string& filePath) {
  std::ifstream file(filePath, std::ios::binary); // Buka file dalam mode biner
  std::vector<char> buffer; // Buffer untuk menyimpan data dari file
  if(!file) {
    buffer.clear();         // Hapus buffer jika gagal membaca
    throw std::runtime_error(
        std::format("Tidak dapat membuka file '{}'.", filePath)
    );
  }

  // Tentukan ukuran file
  file.seekg(0, std::ios::end);
  std::streamsize fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  // Reservasi buffer dengan ukuran file
  buffer.resize(fileSize);

  // Variabel untuk melacak jumlah byte yang telah dibaca
  std::streamsize bytesRead = 0;
  const int updateInterval = 1; // Update progress setiap 10% (bisa disesuaikan)

  // Baca file ke dalam buffer
  while(bytesRead < fileSize) {
    // Hitung jumlah byte yang akan dibaca pada iterasi ini
    std::streamsize bytesToRead = std::min(
        fileSize - bytesRead, static_cast<std::streamsize>(buffer.size())
    );

    // Baca data ke buffer
    file.read(buffer.data() + bytesRead, bytesToRead);

    // Perbarui jumlah byte yang telah dibaca
    bytesRead += bytesToRead;

    // Hitung persentase pembacaan
    const unsigned short seratus(100);
    int progress = static_cast<int>(
        (static_cast<double>(bytesRead) / static_cast<double>(fileSize)) *
        seratus
    );

    // Cetak loading progress jika mencapai updateInterval atau selesai membaca
    if(progress % updateInterval == 0 || bytesRead >= fileSize) {
      std::cout << std::format("\rMembaca file '{}' {}%", filePath, progress)
                << std::flush;
    }
  }

  // Beri baris baru setelah pembacaan selesai
  std::cout << "\n";

  if(!file) {
    throw std::runtime_error(
        std::format("Tidak dapat membaca file '{}'.", filePath)
    );
  }
  file.close();
  return Binary(buffer);
}

// Fungsi untuk mengonversi std::vector<char> menjadi string heksadesimal
Hexadecimal binToHex(const Binary& bin) {
  std::ostringstream stream;
  stream << std::hex
         << std::setfill('0'); // Set manipulator untuk output heksadesimal

  for(const unsigned char& byte : bin.data) {
    // Konversi setiap byte menjadi heksadesimal dan tambahkan ke stream
    stream << std::setw(2) << static_cast<int>(byte);
  }

  // Kembalikan string hasil konversi
  return Hexadecimal(stream.str());
}

// Fungsi untuk mengonversi string heksadesimal menjadi teks terdekripsi
DecodedText hexToDecode(const Hexadecimal& hex) {
  std::string result;

  // Iterasi setiap dua karakter untuk mengonversi ke byte
  for(size_t i = 0; i < hex.data.length(); i += 2) {
    // Ambil dua karakter heksadesimal
    std::string byteString = hex.data.substr(i, 2);

    // Konversi string heksadesimal ke nilai integer
    unsigned int byteValue = 0;
    std::stringstream sstream;
    sstream << std::hex << byteString;
    sstream >> byteValue;

    // Tambahkan byte ke string hasil
    result.push_back(static_cast<char>(byteValue));
  }

  return DecodedText(result);
}

// Fungsi untuk mengonversi teks terdekripsi menjadi string heksadesimal
Hexadecimal decodeToHex(const DecodedText& decode) {
  std::ostringstream stream;
  // Iterasi setiap karakter dalam teks terdekripsi
  for(const char& karakter : decode.data) {
    // Konversi setiap karakter ke dalam format heksadesimal
    stream << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(static_cast<unsigned char>(karakter));
  }
  // Kembalikan string hasil konversi
  return Hexadecimal(stream.str());
}

// Fungsi untuk mengonversi string heksadesimal menjadi std::vector<char>
Binary hexToBin(const Hexadecimal& hex) {
  std::vector<unsigned char> result;

  // Iterasi setiap dua karakter heksadesimal untuk mengonversi ke byte
  for(size_t i = 0; i < hex.data.length(); i += 2) {
    // Ambil dua karakter heksadesimal
    std::string byteString = hex.data.substr(i, 2);

    // Konversi string heksadesimal ke nilai integer
    unsigned int byteValue = 0;
    std::stringstream sstream;
    sstream << std::hex << byteString;
    sstream >> byteValue;

    // Tambahkan byte ke vektor hasil
    result.push_back(static_cast<unsigned char>(byteValue));
  }

  return Binary(result);
}

// Fungsi untuk menulis data dari std::vector<char> ke dalam file
void writeBinToFile(const Binary& bin, const std::string& filePath) {
  std::vector<char> data(bin.data.begin(), bin.data.end());
  std::ofstream file(filePath, std::ios::binary); // Buka file dalam mode biner

  if(!file.is_open()) {
    throw std::runtime_error(
        std::format("Tidak dapat menulis file '{}'.", filePath)
    );
  }

  // Variabel untuk melacak jumlah byte yang telah ditulis
  std::streamsize bytesWritten = 0;
  const int updateInterval = 1; // Update progress setiap 10% (bisa disesuaikan)

  // Menulis data ke dalam file
  while(bytesWritten < static_cast<std::streamsize>(data.size())) {
    // Hitung jumlah byte yang akan ditulis pada iterasi ini
    std::streamsize bytesToWrite = std::min(
        static_cast<std::streamsize>(data.size()) - bytesWritten,
        static_cast<std::streamsize>(sizeof(int))
    );

    // Menulis data ke dalam file
    file.write(data.data() + bytesWritten, bytesToWrite);

    // Perbarui jumlah byte yang telah ditulis
    bytesWritten += bytesToWrite;

    // Hitung persentase penulisan
    const unsigned short seratus(100);
    int progress = static_cast<int>(
        (static_cast<double>(bytesWritten) / static_cast<double>(data.size())) *
        seratus
    );

    // Cetak loading progress jika mencapai updateInterval atau selesai menulis
    if(progress % updateInterval == 0 ||
       bytesWritten >= static_cast<std::streamsize>(data.size())) {
      std::cout << std::format("\rMenulis file '{}' {}%", filePath, progress)
                << std::flush;
    }
  }

  // Beri baris baru setelah penulisan selesai
  std::cout << "\n";

  if(file.is_open()) {
  } else {
    throw std::runtime_error(
        std::format("Tidak dapat menulis file '{}'.", filePath)
    );
  }
}

// Fungsi untuk menghapus prefix dari string
void removeStrPrefix(std::string& str, const std::string& prefix) {
  if(str.substr(0, prefix.size()) == prefix) {
    str = str.substr(prefix.size());
  }
}

// Fungsi untuk menghapus suffix dari string
void removeStrSuffix(std::string& str, const std::string& suffix) {
  if(str.size() >= suffix.size() &&
     str.substr(str.size() - suffix.size()) == suffix) {
    str = str.substr(0, str.size() - suffix.size());
  }
}
