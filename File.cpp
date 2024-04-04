#include "File.h"



File::File(std::string path, int lineNumber) {
        this->path = path;
        this->lineNumber = lineNumber;
       
    }

File::File() {
    path(0);
    this->lineNumber = lineNumber;

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
        std::cout << "������ �������� ����� " << path << std::endl;
        flag = false;
    }
    else {
        // ���������� ��������� �� ������ ������
        bool insufficientLinesErrorDisplayed = false; // ���������� ��� ������������ ������ ��������� �� ������
        for (int i = 1; i < lineNumber; ++i) {
            std::string line;
            if (!std::getline(fin, line)) {
                if (!insufficientLinesErrorDisplayed) {
                    std::cerr << "������������ ����� � �����!" << std::endl;
                    flag = false;
                    insufficientLinesErrorDisplayed = true; // ������������� ����, ����� ��������� ���������� ������ ���� ���
                }
                return; // ���������� ���������� �������, ��� ��� ����� �� �������
            }
        }
        // ��������� �������� �� ������ ������
        std::string line;
        if (!std::getline(fin, line)) {
            std::cerr << "������ ��� ������ ������ " << lineNumber << std::endl;
            flag = false;
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

