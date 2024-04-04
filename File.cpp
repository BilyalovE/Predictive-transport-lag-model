#include "File.h"


class DiscreteDataTime;

File::File(std::string path, int lineNumber) {
        this->path = path;
        this->lineNumber = lineNumber;
       
    }

File::File() {
}

bool File::fileStatus()
{
    readValue();
    return flag;
}
;

void File::readValue() {
    fin.open(path);
    setlocale(LC_ALL, "rus");
    if (!fin.is_open()) {
        std::cout << "Ошибка открытия файла " << path << std::endl;
        flag = false;
    }
    else {
        // Перемещаем указатель на нужную строку
        bool insufficientLinesErrorDisplayed = false; // Переменная для отслеживания вывода сообщения об ошибке
        for (int i = 1; i < lineNumber; ++i) {
            std::string line;
            if (!std::getline(fin, line)) {
                if (!insufficientLinesErrorDisplayed) {
                    std::cerr << "Недостаточно строк в файле!" << std::endl;
                    flag = false;
                    insufficientLinesErrorDisplayed = true; // Устанавливаем флаг, чтобы сообщение выводилось только один раз
                }
                return; // Прекращаем выполнение функции, так как строк не хватает
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
    friend DiscreteDataTime;
}


std::string File::getValue() {
        readValue();
        return currentValue;
    }

File::~File() {
        if (fin.is_open())
            fin.close();
    }

