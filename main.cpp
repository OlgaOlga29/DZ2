#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

//режим encryption/decryption необходимо указать в Edit Configurations
int main(int argc, const char *argv[]) {
    int shift = 3;
    if (argc != 3) {
        std::cerr << "Error: Use three parameters\n";
        return 5;
    }
    setlocale(LC_ALL, "Russian");

    const std::string mode(argv[1]); // Режим работы
    const std::string file_name(argv[2]); // Имя файла

    if (mode == "encryption") {
        // Режимшифрование
        std::string str;
        int pass, result;
        std::cout << "Enter your text:";
        getline(std::cin, str);
        std::cout << "Enter your Password:";
        std::cin >> pass;

        const char *byte_array = str.data();
        const unsigned int size = str.size();

        srand(pass);
        std::ofstream file;
        file.open("file", std::ios::binary);
        for (int i = 0; i < size; i += 2) {
            char symbol1 = byte_array[i];
            char symbol2 = (i + 1 < size) ? byte_array[i + 1] : static_cast<char>(0);
            unsigned int g = rand();
            result = g ^ (static_cast<int>(symbol1) << 8 | static_cast<int>(symbol2));
            result = (result << shift) | ((result & 0xFFFF) >> (16 - shift));

            char result1 = result >> 8;
            char result2 = result;
            file.write(&result1, sizeof(result1));
            file.write(&result2, sizeof(result2));
        }

        file.close();
    } else if (mode == "decryption") {
        unsigned int pass, result;
        std::cout << "Enter your Password:";
        std::cin >> pass;
        srand(pass);
        std::ifstream file;
        file.open("file", std::ios::binary);
        std::vector<char> vec((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
        file.close();
        std::vector<char> inference(vec.size());
        for (int i = 0; i < vec.size(); i += 2) {
            char symbol1 = vec[i];
            char symbol2 = (i + 1 < vec.size()) ? vec[i + 1] : static_cast<char>(0);
            unsigned int g = rand();
            result = static_cast<int>(symbol1) << 8u & 0xFF00 | static_cast<int>(symbol2) & 0x00FF;
            result = ((result & 0xFFFF) >> shift) | (result << (16 - shift));
            result ^= g;
            unsigned char result1 = result >> 8;
            unsigned char result2 = result;
            inference[i] = result1;
            inference[i + 1] = result2;
        }
        for (int i = 0; i < inference.size(); i++) {
            std::cout << inference[i];
        }
    }
    return 0;
}
