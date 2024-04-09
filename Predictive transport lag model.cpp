/*
    \ brief Метод характеристик (Разработка алгоритма прогнозирования качества поступающего сырья для компенсации возмущений установки гидроочистки бензина каталитического крекинга)
    \ author Bilyalov Eldar
    \ version 1.0 (Модель с переменным шагом по времени)
    \ date 02.04.24

*/

// Подключаем необходимые библиотеки
#include <iostream>
#include <vector>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "TransportEquation.h"
#include "Pipe.h"
#include <locale>
#include "File.h"
#include "Parser.h"
#include <chrono>
#include <thread>
#include <windows.h>


double callSulfar(std::string path, int lineNumber) {
    Sulfar sulfar(path, lineNumber); // Имя файла и номер строки, которую нужно считать
    return sulfar.getSulfar();
}


double callVolumeFlow(std::string path1, std::string path2, int lineNumber) {
    VolumeFlow volumeFlow(path1, path2, lineNumber); // Имя файла и номер строки, которую нужно считать
    return volumeFlow.getVolumeFlow();
}

double callDiscreteDataTime(std::string path, int lineNumber) {
    DiscreteDataTime discreteDataTime(path, lineNumber); // Имя файла и номер строки, которую нужно считать
    return discreteDataTime.getIntSecTime();

class Time {
    int timeDelay;
    int seconds;
    int minutes;
    int hours;
public:
    
   Time() {} // Конструктор по умолчанию

    Time(int timeDelay) {
        this->timeDelay = timeDelay;
    };

    void timeConvertation() {
        hours = timeDelay / 3600;
        int remainingSeconds = timeDelay % 3600;
        minutes = remainingSeconds / 60;
        seconds = remainingSeconds % 60;
    }
    friend std::ostream& operator<<(std::ostream& os, const Time& time);
};

std::ostream& operator<<(std::ostream& os, const Time& time) 
{
    os << time.hours << ":" << time.minutes << ":" << time.seconds;
    return os;
}


class OutFile {
private:
    /// @param Путь файла
    std::string path;
    std::ofstream fout;
    Time timeDelay;
    double sulfar;

public:
    OutFile(std::string path, const Time& timeDelay, double sulfar) {
        this->path = path;
        this->timeDelay = timeDelay;
        this->sulfar =sulfar;
    };

    OutFile() {
        
    };

   
    virtual void outPut() 
    {
        fout.open(path, std::ofstream::app);
        if (!fout.is_open())  
        {
            std::cout << "Ошибка открытия файла " << path << std::endl;
        }
        else 
        {
            timeDelay.timeConvertation();
            fout << timeDelay  << "," << sulfar << std::endl;
        }
    }

    ~OutFile() {
        if (fout.is_open())
            fout.close();
    };
};

void push_back(double*& arr, int& size, const double value) {
    double* newArray = new double[size + 1];
    for (int i = 0; i < size; i++) {
        newArray[i] = arr[i];
    }
    newArray[size] = value;
    size++;
    delete arr;
    arr = newArray;
}

/// @brief Главная функция, в которой происходит инициализация структур, краевых и начальных условий, а также вызов функции солвера и функции вывода в файл
int main(int argc, char** argv)
{
    setlocale(LC_ALL, "rus");
    int sizeVecSulfar = 0;
    int sizeVecFlow = 0;
    int sizeVecDisTime = 0;
    

    double* sulfar = new double[sizeVecSulfar];
    double* flow = new double[sizeVecFlow];
    double* discreteTime = new double[sizeVecDisTime];
    bool flag = true;
    int j = 2;
    while (flag) {
        push_back(sulfar, sizeVecSulfar, callSulfar("C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/data txt/sulfar.txt", j));
        push_back(flow, sizeVecFlow, callVolumeFlow("C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/data txt/mass flow.txt",
            "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/data txt/density.txt", j));
        push_back(discreteTime, sizeVecFlow, callDiscreteDataTime("C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/data txt/discrete analysis data time.txt", j));
        j++;
        File file("C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/data txt/discrete analysis data time.txt", j);
        flag = file.fileStatus();
    }
    


    std::cout << sizeVecDisTime << std::endl;
    std::cout << sulfar[1] << std::endl;
    std::cout << sulfar[499] << std::endl;
    std::cout << flow[499] << std::endl;



    delete[] sulfar;
    delete[] flow;

   
    return 0;
}