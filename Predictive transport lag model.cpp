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

   
    void outPut() 
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



/// @brief Главная функция, в которой происходит инициализация структур, краевых и начальных условий, а также вызов функции солвера и функции вывода в файл
int main(int argc, char** argv)
{
    std::vector <double> sulfar;
    std::vector <double> volumeFlow;
    std::vector <double> discreteTime;
    bool flag = true;
    int j = 2; 
    

    while (flag) {
        appendSulfar(sulfar, "sulfar.txt", j);
        appendVolumeFlow(volumeFlow, "mass flow.txt", "density.txt", j);
        appendDiscreteDataTime(discreteTime, "discrete analysis data time.txt", j);
        j++;
        File file("discrete analysis data time.txt", j);
        flag = file.fileStatus();
    }
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
    // @param empty_pipe - подставляем на случай, если труба пустая
    double empty_pipe{ 0 };
    double interpolationSulfar{ 0 };
    TransportEquation transport_equation(pipe, volumeFlow, discreteTime);
    double timeDelayPredict{ 0 };
    double timeDelayReal{ 0 };
    double firstCondSulfar = sulfar[0];
   // double realDifTime{ 0 };
    /// при таком отклонении следует делать переход устанвоки с режима на режим
    double relativeDeviationSulfur { 10 };
    do {
        //realDifTime = sum_dt;
        LineInterpolation sulfarInt(sulfar, discreteTime, sum_dt);
        interpolationSulfar = sulfarInt.line_interpolation();

        transport_equation.methodCharacteristic(buffer.current(), buffer.previous(), firstCondSulfar);
        OutPutData modeling("Результат моделирования", buffer.previous(), sum_dt);
        modeling.outputModelingFlowRawMaterials();
        buffer.advance(1);


        if (timeDelayPredict >= 0) {
            setlocale(LC_ALL, "rus");
            OutPutData transportDelay("Прогнозируемое транспортное запаздывание", firstCondSulfar, timeDelayPredict);
            transportDelay.outputTransportDelay();
            timeDelayPredict = transport_equation.transportDelay();
        }

        sum_dt = transport_equation.get_dt();


       // realDifTime = sum_dt - realDifTime;
        /*if (timeDelayPredict >= 0) {
            setlocale(LC_ALL, "rus");
            std::cout << "Пройденное время = " << realDifTime << " [c]" << std::endl;
        }*/
        
        
        timeDelayReal = sum_dt;

    } while (sum_dt <= pipe.T);
   
    return 0;
}