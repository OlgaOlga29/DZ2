#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        std::cerr << "Error: Use three parameters\n";
        return 5;
    }
    setlocale(LC_ALL, "Russian");

    const std::string mode(argv[1]); // Режим работы
    const std::string file_name(argv[2]); // Имя файла

    if(mode == "encryption") {
        // Режимшифрование
        std::string str;
        int pass, result;
        std::cout << "Enter your text:";
        getline(std::cin, str);
        std::cout << "Enter your Password:";
        std::cin >> pass;

        const char* byte_array = str.data();
        const unsigned int size = str.size();

        srand(pass);
        std::ofstream file;
        file.open("file", std::ios::binary);
        for (int i = 0; i < size; i+=2) {
            char symbol1 = byte_array[i];
            char symbol2 = (i+1 < size) ? byte_array[i+1] : static_cast<char>(0);
            int g = rand();
            result = g ^ (static_cast<int>(symbol1) << 8) | g ^ static_cast<int>(symbol2);
            result = (result << 3) | (result & 0xFFFF >> (16-3));

            char result1 = result >> 8;
            char result2 = result;
            file.write(&result1, sizeof(result1));
            file.write(&result2, sizeof(result2));
        }

        file.close();
    }

    else if (mode == "decryption") {
        int pass,result;
        std::cout<< "Enter your Password:" ;
        std::cin>> pass;
        srand(pass);
        int n;
        std::vector<char>vec;
        std::ifstream file("file", std::ios::binary);
        while (!file.eof())
        {
            file >> n;
            vec.push_back(n);
        }
        file.close();
        const char* byte_array = vec.data();
        for (int i = 0; i < vec.size(); i+=2) {
            char symbol1 = byte_array[i];
            char symbol2 = (i + 1 <vec.size()) ? byte_array[i + 1] : static_cast<char>(0);
            int g = rand();
            result = (static_cast<int>(symbol1) << 8) | static_cast<int>(symbol2);
            result = ((result & 0xFFFF) >> 3) | (result << (16 - 3));
            result ^= g;
            std::cout << result;
        }
    }



    return 0;
}