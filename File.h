#pragma once
#include <string>
#include <fstream>
#include <iostream>
class File {
private:
    /// @param ���� �����
    std::string path;
    /// @param ����� ������, ������ ����� ������� ��������
    int lineNumber;
    /// @param ��������� �� ����� ��������
    std::string currentValue;

    std::ifstream fin;
public:

    File(std::string path, int lineNumber);

    void readValue();

    std::string getValue();

    ~File();
};


