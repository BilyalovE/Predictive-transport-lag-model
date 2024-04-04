#pragma once
#include <string>
#include <fstream>
#include <locale>
#include "File.h"
#include <ctime>
#include <sstream>

class MassFlow : protected File {
    /// @param Массовый расход, [кг/с]
    double massFlow;
public:
    MassFlow(std::string path, int lineNumber) : File(path, lineNumber) {};
    double getMassFlow() {
        /// Перевожу из [т/час] в [кг/с]
        massFlow = (std::stod(getValue())) * 1000 / 3600;
        return massFlow;
    }
};

class Density : protected File {
    double density;
public:
    Density(std::string path, int lineNumber) : File(path, lineNumber) {};
    double getDensity() {
        density = std::stod(getValue());
        return density;
    }
};

class VolumeFlow : protected MassFlow, protected Density {
    double volumeFlow;
public:
    VolumeFlow(std::string pathMassFlow, std::string pathDensity, int lineNumber) : MassFlow(pathMassFlow, lineNumber), Density(pathDensity, lineNumber) {};
    double getVolumeFlow() {
        volumeFlow = getMassFlow() / getDensity();
        return volumeFlow;
    }
};

class Sulfar : protected File {
    double sulfar;
public:
    Sulfar(std::string path, int lineNumber) : File(path, lineNumber) {};
    double getSulfar() {
        sulfar = std::stod(getValue());
        return sulfar;
    };
};

class DiscreteDataTime : protected File {
    /// @param Дата и время как строка
    std::string strDataTime;
    /// @param Время как строка
    std::string strTime;
    /// @param Дата как строка
    std::string strData;
    /// @param Время как число секунд int
    int totalSeconds;
    /// @param структура данных для отдельного хранения даты и времени
    std::pair<std::string, std::string> splitDataTime;

    std::pair<std::string, std::string> splitString(const std::string& strDataTime) {
        std::string strData;
        std::string strTime;
        bool foundSpace = false; // флаг, указывающий, найден ли пробел

        for (char ch : strDataTime) {
            if (ch == ' ') {
                foundSpace = true;
            }
            else if (!foundSpace) {
                strData += ch;
            }
            else {
                strTime += ch;
            }
        }
        return std::make_pair(strData, strTime);
    }

    /// @brief Метод для преобразования времени формата "чч:мм:сс" в количество секунд
    int timeStringToSeconds(const std::string& strTime) {
        int hours, minutes, seconds;
        char delimiter;

        std::istringstream iss(strTime);
        iss >> hours >> delimiter >> minutes >> delimiter >> seconds;

        // Вычисляем общее количество секунд
        return hours * 3600 + minutes * 60 + seconds;
    }

public:
    DiscreteDataTime() { 
        path(0); lineNumber(0) };

    DiscreteDataTime(std::string path, int lineNumber) : File(path, lineNumber) {};
    /// @brief Метод возврата даты и времени как строки
    std::string getStrDataTime() {
        strDataTime = getValue();
        return strDataTime;
    }

    /// @brief Метод возврата даты как строки
    std::string getStrData() {
        splitDataTime = splitString(strDataTime);
        strData = splitDataTime.first;
        return strData;
    }

    /// @brief Метод возврата времени как строки
    std::string getStrTime() {
        splitDataTime = splitString(strDataTime);
        strTime = splitDataTime.second;
        return strTime;
    }

    /// @brief Метод возврата времени как числа секунд формата int
    int getIntSecTime() {
        strTime = getStrTime();
        totalSeconds = timeStringToSeconds(strTime);
        return totalSeconds;
    }
};