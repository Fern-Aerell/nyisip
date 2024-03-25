#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <format>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#define APP_NAME "nyisip.exe"
#define APP_VERSION "1.0.0"

// Fungsi untuk membuat kata kunci
std::string nyisipKataKunci(const bool& isiFile, const bool& suffix, const size_t& id) {
	return std::format("[{}{}{}][{}]", std::string(APP_NAME APP_VERSION), isiFile ? "isifile" : "namafile", suffix ? "akhir" : "awal", id);
}

// Fungsi untuk membaca file dalam bentuk biner
std::vector<char> readBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary); // Buka file dalam mode biner
    std::vector<char> buffer; // Buffer untuk menyimpan data dari file

    if (file) {
        // Tentukan ukuran file
        file.seekg(0, std::ios::end);
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Reservasi buffer dengan ukuran file
        buffer.resize(fileSize);

        // Baca file ke dalam buffer
        if (!file.read(buffer.data(), fileSize)) {
            std::cerr << std::format("Tidak dapat membaca file '{}'.", filename) << std::endl;
            buffer.clear(); // Hapus buffer jika gagal membaca
        }
    } else {
        std::cerr << std::format("Tidak dapat membuka file '{}'.", filename) << std::endl;
    }

    return buffer;
}

// Fungsi untuk mengonversi std::vector<char> menjadi string heksadesimal
std::string vectorToHex(const std::vector<char>& data) {
    std::stringstream stream;
    stream << std::hex << std::setfill('0'); // Set manipulator untuk output heksadesimal

    for (char byte : data) {
        // Konversi setiap byte menjadi heksadesimal dan tambahkan ke stream
        stream << std::setw(2) << static_cast<int>(static_cast<unsigned char>(byte));
    }

    // Kembalikan string hasil konversi
    return stream.str();
}

// Fungsi untuk mengonversi string heksadesimal menjadi teks terdekripsi
std::string hexToString(const std::string& hexString) {
    std::string result;

    // Iterasi setiap dua karakter untuk mengonversi ke byte
    for (size_t i = 0; i < hexString.length(); i += 2) {
        // Ambil dua karakter heksadesimal
        std::string byteString = hexString.substr(i, 2);

        // Konversi string heksadesimal ke nilai integer
        unsigned int byteValue;
        std::stringstream ss;
        ss << std::hex << byteString;
        ss >> byteValue;

        // Tambahkan byte ke string hasil
        result.push_back(static_cast<char>(byteValue));
    }

    return result;
}

// Fungsi untuk mengonversi teks terdekripsi menjadi string heksadesimal
std::string stringToHex(const std::string& decodedText) {
    std::stringstream stream;

    // Iterasi setiap karakter dalam teks terdekripsi
    for (char ch : decodedText) {
        // Konversi setiap karakter ke dalam format heksadesimal
        stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(ch));
    }

    // Kembalikan string hasil konversi
    return stream.str();
}

// Fungsi untuk mengonversi string heksadesimal menjadi std::vector<char>
std::vector<char> hexToVector(const std::string& hexString) {
    std::vector<char> result;

    // Iterasi setiap dua karakter heksadesimal untuk mengonversi ke byte
    for (size_t i = 0; i < hexString.length(); i += 2) {
        // Ambil dua karakter heksadesimal
        std::string byteString = hexString.substr(i, 2);

        // Konversi string heksadesimal ke nilai integer
        unsigned int byteValue;
        std::stringstream ss;
        ss << std::hex << byteString;
        ss >> byteValue;

        // Tambahkan byte ke vektor hasil
        result.push_back(static_cast<char>(byteValue));
    }

    return result;
}

// Fungsi untuk mengonversi teks terdekripsi menjadi string std::vector<char>
std::vector<char> stringToVector(const std::string& decodedText) {
    return hexToVector(stringToHex(decodedText));
}

// Fungsi untuk menulis data dari std::vector<char> ke dalam file
bool writeBinaryFile(const std::string& filename, const std::vector<char>& data) {
    std::ofstream file(filename, std::ios::binary); // Buka file dalam mode biner

    if (file.is_open()) {
        // Menulis data ke dalam file
        file.write(data.data(), data.size());
        file.close();
        return true;
    } else {
        std::cerr << std::format("Tidak dapat menulis file '{}'.", filename) << std::endl;
        return false;
    }
}

void sisipKanFileById(std::string& decode, const std::string& lokasiFileSisipanStr, const size_t& id) {
    std::filesystem::path lokasiFileSisipan(lokasiFileSisipanStr);
    std::vector<char> bin = readBinaryFile(lokasiFileSisipan.string());
    std::string hex = vectorToHex(bin);
    decode += nyisipKataKunci(false, false, id) + lokasiFileSisipan.filename().string() + nyisipKataKunci(false, true, id);
    decode += nyisipKataKunci(true, false, id) + hexToString(hex) + nyisipKataKunci(true, true, id);
}

// Fungsi untuk menghapus prefix dari string
std::string removePrefix(const std::string& str, const std::string& prefix) {
    if (str.substr(0, prefix.size()) == prefix) {
        return str.substr(prefix.size());
    }
    return str;
}

// Fungsi untuk menghapus suffix dari string
std::string removeSuffix(const std::string& str, const std::string& suffix) {
    if (str.size() >= suffix.size() && str.substr(str.size() - suffix.size()) == suffix) {
        return str.substr(0, str.size() - suffix.size());
    }
    return str;
}

struct FileSisipanHex {
    std::string namaFileHex;
    std::string isiFileHex;
};

std::unique_ptr<FileSisipanHex> getFileSisipanHexById(std::string& hex, const size_t& id) {
    FileSisipanHex fileSisipanHex;
    std::string hexTagNamaFileAwal = stringToHex(nyisipKataKunci(false, false, id));
    std::string hexTagNamaFileAkhir = stringToHex(nyisipKataKunci(false, true, id));
    std::string hexTagIsiFileAwal = stringToHex(nyisipKataKunci(true, false, id));
    std::string hexTagIsiFileAkhir = stringToHex(nyisipKataKunci(true, true, id));
    std::regex regexHexTagNamaFile(hexTagNamaFileAwal + ".*" + hexTagNamaFileAkhir);
    std::regex regexHexTagIsiFile(hexTagIsiFileAwal + ".*" + hexTagIsiFileAkhir);
    std::sregex_iterator iterator(hex.begin(), hex.end(), regexHexTagNamaFile);
    std::sregex_iterator endIterator;
    bool status = false;
    if(iterator != endIterator) {
        std::smatch match = *iterator;
        fileSisipanHex.namaFileHex = match.str();
        fileSisipanHex.namaFileHex = removePrefix(fileSisipanHex.namaFileHex, hexTagNamaFileAwal);
        fileSisipanHex.namaFileHex = removeSuffix(fileSisipanHex.namaFileHex, hexTagNamaFileAkhir);
        hex.erase(match.position(), match.str().size());
        status = true;
    }
    iterator = std::sregex_iterator(hex.begin(), hex.end(), regexHexTagIsiFile);
    if (iterator != endIterator) {
        std::smatch match = *iterator;
        fileSisipanHex.isiFileHex = match.str();
        fileSisipanHex.isiFileHex = removePrefix(fileSisipanHex.isiFileHex, hexTagIsiFileAwal);
        fileSisipanHex.isiFileHex = removeSuffix(fileSisipanHex.isiFileHex, hexTagIsiFileAkhir);
        hex.erase(match.position(), match.str().size());
    }
    return status ? std::make_unique<FileSisipanHex>(std::move(fileSisipanHex)) : nullptr;
}

std::vector<FileSisipanHex> getAllFileSisipanHex(std::string& hex) {
    std::vector<FileSisipanHex> kFileSisipanHex;
    size_t id = 0;
    while(const auto hasil = getFileSisipanHexById(hex, id)) {
        kFileSisipanHex.push_back(*hasil);
        ++id;
    }
    return kFileSisipanHex;
}

/*

Command line
nyisip <file-yg-mau-disisipkan> <file-sisipan>* // Untuk menyisipkan file
nyisip -e <file-yg-mau-disisipkan> // Untuk mengeluarkan file yang disisipkan

nyisip1.0.0awal nyisip1.0.0akhir

*/

int main(int argc, const char* argv[]) {
    if(argc <= 1) {return 0;}
    if(strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "-eh") == 0) {
        std::filesystem::path lokasiFileYangMauDiEkstrak(argv[2]);
        std::vector<char> bin = readBinaryFile(lokasiFileYangMauDiEkstrak.string());
        std::string hex = vectorToHex(bin);
        std::vector<FileSisipanHex> kFileSisipanHex(getAllFileSisipanHex(hex));
        for(const auto& fileSisipanHex : kFileSisipanHex) {
            std::vector<char> isiFileBin = hexToVector(fileSisipanHex.isiFileHex);
            std::string namaFile(hexToString(fileSisipanHex.namaFileHex));
            std::string lokasiFileYangMauDisimpan = lokasiFileYangMauDiEkstrak.parent_path().string();
            lokasiFileYangMauDisimpan = lokasiFileYangMauDisimpan.empty() ? namaFile : lokasiFileYangMauDisimpan + "/" + namaFile;
            writeBinaryFile(lokasiFileYangMauDisimpan, isiFileBin);
        }
        if(strcmp(argv[1], "-eh") == 0) {
            bin = hexToVector(hex);
            writeBinaryFile(lokasiFileYangMauDiEkstrak.string(), bin);
        }
        return 0;
    }
    if(argc > 2) {
        std::filesystem::path lokasiFileYangMauDiSisipkan(argv[1]);
        std::vector<char> bin = readBinaryFile(lokasiFileYangMauDiSisipkan.string());
        writeBinaryFile(lokasiFileYangMauDiSisipkan.string() + ".bak", bin);
        std::string hex = vectorToHex(bin);
        std::string decode = hexToString(hex);
        size_t id = 0;
        for(size_t index = 2; index < static_cast<size_t>(argc); ++index) {
            sisipKanFileById(decode, argv[index], id);
            ++id;
        }
        hex = stringToHex(decode);
        bin = hexToVector(hex);
        writeBinaryFile(lokasiFileYangMauDiSisipkan.string(), bin);
    }
    return 0;
}
