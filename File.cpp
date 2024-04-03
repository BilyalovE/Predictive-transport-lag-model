#include "File.h"



File::File(std::string path, int lineNumber) {
        this->path = path;
        this->lineNumber = lineNumber;
       
    };

void File::readValue() {
        // Устанавливаем русскую локаль
        std::locale::global(std::locale("ru_RU.UTF-8"));
        fin.open(path);
        if (!fin.is_open()) {
            std::cout << "Ошибка открытия файла " << path << std::endl;
            flag = false;
        }
        else {
            // Перемещаем указатель на нужную строку
            for (int i = 1; i < lineNumber; ++i) {
                std::string line;
                if (!std::getline(fin, line)) {
                    std::cerr << "Недостаточно строк в файле!" << std::endl;
                    flag = false;
                    return;
                }
            }
            // Считываем значение из нужной строки
            std::string line;
            if (!std::getline(fin, line)) {
                std::cerr << "Ошибка при чтении строки " << lineNumber << std::endl;
                flag = false;
                return;
            }
            // Присваиваем считанную строку currentValue
            currentValue = line;
        }
    }

std::string File::getValue() {
        readValue();
        return currentValue;
    }

File::~File() {
        if (fin.is_open())
            fin.close();
    }

