/*
    \ brief Метод характеристик (Разработка алгоритма прогнозирования качества поступающего сырья для компенсации возмущений установки гидроочистки бензина каталитического крекинга)
    \ author Bilyalov Eldar
    \ version 1.0 (Модель с переменным шагом по времени)
    \ date 02.04.24

*/

// Подключаем необходимые библиотеки
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "TransportEquation.h"
#include "Pipe.h"
#include "File.h"
#include "Parser.h"
#include <chrono>
#include <thread>
#include <windows.h>
#include "OutPutData.h"


void appendSulfar(std::vector <double> &sulf, std::string path, int lineNumber) {
    Sulfar sulfar(path, lineNumber); // Имя файла и номер строки, которую нужно считать
    sulf.push_back(sulfar.getSulfar());
}


void appendVolumeFlow(std::vector <double>& flow, std::string path1, std::string path2, int lineNumber) {
    VolumeFlow volumeFlow(path1, path2, lineNumber); // Имя файла и номер строки, которую нужно считать
    flow.push_back(volumeFlow.getVolumeFlow());
}

void appendDiscreteDataTime(std::vector <double>& time, std::string path, int lineNumber) {
    DiscreteDataTime discreteDataTime(path, lineNumber); // Имя файла и номер строки, которую нужно считать
    time.push_back(discreteDataTime.getIntSecTime());
}

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

//void push_back(double*& arr, int& size, const double value) {
//    double* newArray = new double[size + 1];
//    for (int i = 0; i < size; i++) {
//        newArray[i] = arr[i];
//    }
//    newArray[size] = value;
//    size++;
//    delete arr;
//    arr = newArray;
//}

/// @brief Главная функция, в которой происходит инициализация структур, краевых и начальных условий, а также вызов функции солвера и функции вывода в файл
int main(int argc, char** argv)
{
   
    //int sizeVecSulfar = 0;
    //int sizeVecFlow = 0;
   //int sizeVecDisTime = 0;
    
    /*double* sulfar = new double[sizeVecSulfar];
    double* flow = new double[sizeVecFlow];
    double* discreteTime = new double[sizeVecDisTime];
    bool flag = true;
    int j = 2;*/

    std::vector <double> sulfar;
    std::vector <double> volumeFlow;
    std::vector <double> discreteTime;
    bool flag = true;
    int j = 2; 
    setlocale(LC_ALL, "rus");

    while (flag) {
        appendSulfar(sulfar, "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/data txt/sulfar.txt", j);
        appendVolumeFlow(volumeFlow, "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/data txt/mass flow.txt",
            "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/data txt/density.txt", j);
        appendDiscreteDataTime(discreteTime, "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/data txt/discrete analysis data time.txt", j);
        j++;
        File file("C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/data txt/discrete analysis data time.txt", j);
        flag = file.fileStatus();
    }
    /*std::cout << sulfar.size() << std::endl;
    std::cout << sulfar[0] << std::endl;
    std::cout << volumeFlow[0] << std::endl;
    std::cout << discreteTime[0] << std::endl;*/

    double initial_sulfar = 200;
    Pipeline_parameters pipe;
    /// @param Начальный слой по сере
    std::vector <double> initial_sulfar_layer(pipe.n, initial_sulfar);
   
    // Создаем  буфер для решаемой задачи
    /// @param number_layers_buffer - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя - предыдущий и текущий слои)
    int number_layers_buffer = 2;
    ring_buffer_t <std::vector<double>> buffer(2, initial_sulfar_layer);
    /// @param sum_dt -  сумма времени моделирования 
    double sum_dt = 0;
    /// @param k - счетчик слоев
    int k = 0;
    // @param empty_pipe - подставляем на случай, если труба пустая
    double empty_pipe{ 0 };
  
    TransportEquation transport_equation(pipe, volumeFlow, discreteTime);
   
    do {
        
        // Проверка выхода за границы массива серы
        if (k < sulfar.size()) {
            transport_equation.methodCharacteristic(buffer.current(), buffer.previous(), sulfar[k], k);
            
        }
        else {
            transport_equation.methodCharacteristic(buffer.current(), buffer.previous(), empty_pipe, k);
            
        }
        OutPutData time_dt("Результат моделирования", buffer.previous(), sum_dt);
        time_dt.output_data();
        buffer.advance(1);
        sum_dt += transport_equation.get_dt();
        k++;
    } while (sum_dt <= pipe.T);
   
    return 0;
}