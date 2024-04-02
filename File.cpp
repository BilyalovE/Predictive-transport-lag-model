#include "File.h"



File::File(std::string path, int lineNumber) {
        this->path = path;
        this->lineNumber = lineNumber;
    };

void File::readValue() {
        fin.open(path);
        if (!fin.is_open()) {
            std::cout << "������ �������� ����� " << path << std::endl;
        }
        else {
            // ���������� ��������� �� ������ ������
            for (int i = 1; i < lineNumber; ++i) {
                std::string line;
                if (!std::getline(fin, line)) {
                    std::cerr << "������������ ����� � �����!" << std::endl;
                    return;
                }
            }
            // ��������� �������� �� ������ ������
            std::string line;
            if (!std::getline(fin, line)) {
                std::cerr << "������ ��� ������ ������ " << lineNumber << std::endl;
                return;
            }
            // ����������� ��������� ������ currentValue
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

