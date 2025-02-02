﻿/*
    \ brief Метод характеристик (Разработка алгоритма прогнозирования качества поступающего сырья для компенсации возмущений установки гидроочистки бензина каталитического крекинга)
    \ author Bilyalov Eldar
    \ version 1.0 (Модель с переменным шагом по времени)
    \ date 02.04.24

*/

#include <iostream>
#include <string>
#include <vector>
#include <locale.h>
#include <fstream>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "Pipe.h"
#include "LineInterpolation.h"

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
#include "TransportDelay.h"


void appendSulfar(std::vector <double> &sulf, std::string path, int lineNumber) {
    Sulfar sulfur(path, lineNumber); // Имя файла и номер строки, которую нужно считать
    sulf.push_back(sulfur.getSulfar());
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
    double sulfur;

public:
    OutFile(std::string path, const Time& timeDelay, double sulfur) {
        this->path = path;
        this->timeDelay = timeDelay;
        this->sulfur =sulfur;
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
            fout << timeDelay  << "," << sulfur << std::endl;
        }
    }

    ~OutFile() {
        if (fout.is_open())
            fout.close();
    };
};


class СalcOperationPrediction {
    
    Pipeline_parameters pipe;

    /// @param Желаемое время прогнозирования, [c]
    double desiredTime;
    /// @param Текущий слой серы
    std::vector <double> currentSulfarLayer;
    /// @param Скорость течения сырья на время dt (на время метода характеристик), [м/с]
    double speed;
    /// @param Расстояние до выхода из трубы
    double distanceToExit;
    /// index - Индекс минимального элемента
    int index;
    
    /// @brief Минимальное абсолютное отклонение от заданного времени 
    double minAbsErrorTime(std::vector <double>& timeToOut) {
        double min;
        std::vector <double> AbsErrorTime = timeToOut;
        for (int i = 0; i < pipe.n - 1; i++) {
            AbsErrorTime[i] = abs(timeToOut[i] - desiredTime);
        }
        min = AbsErrorTime[0];
        for (int i = 0; i < pipe.n - 1; i++) {
            if (min > AbsErrorTime[i]) {
                min = AbsErrorTime[i];
                index = i;
            };
        }
        return index;
    }

public:
    СalcOperationPrediction(const double desiredTime, const  std::vector <double>& currentSulfarLayer,
        const double speed) {
        this->desiredTime = desiredTime;
        this->currentSulfarLayer = currentSulfarLayer;
        this->speed = speed;
    }

    

    double calcOutputSulfarForOperator() {

        std::vector <double> timeToOut(pipe.n, 0);
        for (int i = 0; i < pipe.n - 1; i++) {
            distanceToExit = pipe.L - pipe.get_dx() * i;
            /// Получаю вектор времени до выхода каждой точки из трубы для текущего слоя серы
            timeToOut[i] = distanceToExit / speed;
        } 
        index = minAbsErrorTime(timeToOut);
        return currentSulfarLayer[index];
    }
};

/// @brief Главная функция, в которой происходит инициализация структур, краевых и начальных условий, а также вызов функции солвера и функции вывода в файл
int main(int argc, char** argv)
{
    /// @param Желаемое время прогнозирования, [c]
    double desiredOperatorTime{15 * 60};
    std::vector <double> sulfur;
    std::vector <double> volumeFlow;
    std::vector <double> discreteTime;
    bool flag = true;
    int j = 2; 
    

    while (flag) {
        appendSulfar(sulfur, "sulfur.txt", j);
        appendVolumeFlow(volumeFlow, "mass flow.txt", "density.txt", j);
        appendDiscreteDataTime(discreteTime, "discrete analysis data time.txt", j);
        j++;
        File file("discrete analysis data time.txt", j);
        flag = file.fileStatus();
    }
    double initial_sulfur = 220;
    Pipeline_parameters pipe;
    /// @param Начальный слой по сере
    std::vector <double> initial_sulfur_layer(pipe.n, initial_sulfur);
    /// Для вывода объемного расхода на график
    std::vector <double> interpolationVolumeFlow(pipe.n, 0);
    // Создаем  буфер для решаемой задачи
    /// @param number_layers_buffer - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя - предыдущий и текущий слои)
    int number_layers_buffer = 2;
    ring_buffer_t <std::vector<double>> buffer(2, initial_sulfur_layer);
    /// @param sum_dt -  сумма времени моделирования 
    double sum_dt = 0;
    // @param empty_pipe - подставляем на случай, если труба пустая
    double empty_pipe{ 0 };
    double interpolationSulfar{ 0 };
    double timeDelayPredict{ 0 };
    double timeDelayReal{ 0 };
    double firstCondSulfar = sulfur[0];
    double predictSulfur{ 0 };
    /// при таком отклонении следует делать переход устанвоки с режима на режим
    double relativeDeviationSulfur { 10 };
    double count_current_sulfur{ 0 };
    double count_last_sulfur = initial_sulfur;
    double dt = 0;
    double speed{ 0 };
    bool flagStartCountDelay = false;

    /// Для расчета ошибки синхронизированной во времени с реальными значениями выхода серы
    /// @param - Время прогнозирования серы
    double timeForPredictSulfur;
    /// @param predictTimeLine - вектор временного ряда прогнозной серы
    std::vector <double> predictTimeLine;
    /// @param arrPredictSulfar - вектор прогнозной серы
    std::vector <double> arrPredictSulfur;
    /// @param arrRealOutSulfar - вектор реальной серы на выходе трубы 
    std::vector <double> arrRealOutSulfur;
    /// @param timeLine - вектор времени характеристик - дискретного времени моделирования
    std::vector <double> timeLine;
    /// @param arrErrorSulfar - вектор ошибки прогнозной серы
    std::vector <double> arrErrorSulfur;

    do {
        TransportEquation transport_equation(pipe, volumeFlow, discreteTime, sum_dt);
        //realDifTime = sum_dt;
        LineInterpolation sulfurInt(sulfur, discreteTime, sum_dt);
        interpolationSulfar = sulfurInt.line_interpolation();
        transport_equation.methodCharacteristic(buffer.current(), buffer.previous(), interpolationSulfar);
        /// Определение серы на выходе через заданное оператором время 
        speed = transport_equation.get_speed();

        interpolationVolumeFlow.push_back(speed * pipe.get_inner_square() * 3600);
        СalcOperationPrediction sulfurPrediction(desiredOperatorTime, buffer.current(), speed);
        predictSulfur = sulfurPrediction.calcOutputSulfarForOperator();
        /// Для прогнозирования ошибки
        arrPredictSulfur.push_back(predictSulfur);

        OutPutData predictSulfarForOperator("Прогнозирование качества сырья на выходе при заданном времени прогноза", predictSulfur, sum_dt);
        predictSulfarForOperator.outputPredictSulfar();

        ///по факту мы прогнозируем серу на время через 15 минут, то есть нужно ее сравнивать с выходом, через 15 минут
        timeForPredictSulfur = sum_dt + desiredOperatorTime;
        predictTimeLine.push_back(timeForPredictSulfur);

        //OutPutData absErrorPredictSulfar("Для расчета ошибки прогнозирования", predictSulfur, timeForPredictSulfar);
        //absErrorPredictSulfar.outputPredictSulfar();

        OutPutData modeling("Результат моделирования", buffer.previous(), sum_dt);
        modeling.outputModelingFlowRawMaterials();
        /// Для синхронизации ошибки прогнозной и реальной серы
        arrRealOutSulfur.push_back(buffer.previous()[pipe.n-1]);
        timeLine.push_back(sum_dt);

        // Для первой реализации задачи - расчет транспортного запаздывания
        count_current_sulfur = interpolationSulfar;
        if (abs(count_current_sulfur - count_last_sulfur) >= relativeDeviationSulfur || count_current_sulfur == sulfur[0]) {
            dt = 0;
            predictSulfur = count_current_sulfur;
            timeDelayPredict = 0;
            flagStartCountDelay = true;
            count_last_sulfur = predictSulfur;
        }
        if (flagStartCountDelay) {
            setlocale(LC_ALL, "rus");
            speed = transport_equation.get_speed();
            TransportDelay time(dt, speed);
            timeDelayPredict = time.transportDelay();
            if (timeDelayPredict >= 0) {
                OutPutData transportDelay("Прогнозируемое транспортное запаздывание", predictSulfur, timeDelayPredict);
                transportDelay.outputTransportDelay();
            }
            if (timeDelayPredict == 0) {
                flagStartCountDelay = false;
            }
        }
        // Для первой реализации задачи - расчет транспортного запаздывания


        /// Для определения прогнозируемого запаздывания
        dt = transport_equation.get_dt();
        /// Для мониторга конца моделирования
        sum_dt += dt;
        buffer.advance(1);
    } while (sum_dt <= pipe.T);

    /// Выводим расход в файл
    OutPutData outputVolumeFlow("Интерполированный объемный расход", interpolationVolumeFlow);
    outputVolumeFlow.outputInerpolationVolumeFlow();
    
    std::vector<double> arrPredictSulfurSinc;
    /// Работаем с расчетом ошибки при синхронизациии временных рядов прогнозной серы на выходе трубы и реальной
    for (int i = 0; i < timeLine.size(); i++) {
        LineInterpolation predictSulfur(arrPredictSulfur, predictTimeLine, timeLine[i]);
        arrErrorSulfur.push_back(abs(arrRealOutSulfur[i] - predictSulfur.line_interpolation()) / arrRealOutSulfur[i] * 100);
        arrPredictSulfurSinc.push_back(predictSulfur.line_interpolation());
    }
    /// Выводим прогнозируемое содержание серы на выходе трубы, синхронизируемое с реальными показаниями
    OutPutData outputPredictSulfurSinc("Синхронизированный по выходу прогноз серы", arrPredictSulfurSinc, timeLine);
    outputPredictSulfurSinc.outputParamWithTime();

    /// Выводим ошибку прогнозирования
    OutPutData outputErrorPredictSulfur("Ошибка прогнозирования", arrErrorSulfur, timeLine);
    outputErrorPredictSulfur.outputParamWithTime();

    return 0;
}