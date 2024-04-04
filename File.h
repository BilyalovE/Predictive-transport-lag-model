#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <locale>

class DiscreteDataTime;

class File {
private:
    /// @param ���� �����
    std::string path;
    /// @param ����� ������, ������ ����� ������� ��������
    int lineNumber;
    /// @param ��������� �� ����� ��������
    std::string currentValue;
    std::ifstream fin;
    void readValue();
    bool flag = true;

    friend DiscreteDataTime;
protected:
    std::string getValue();

public:
    
    File(std::string path, int lineNumber);
    File();
    bool fileStatus();
    ~File();
};


