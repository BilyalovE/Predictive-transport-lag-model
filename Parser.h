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

class DiscreteDataTime {
    std::string strDataTime;
    std::string strData;
    std::string strTime;
    int totalSeconds;
    std::pair<std::string, std::string> splitDataTime;
    File file;

    std::pair<std::string, std::string> splitString(const std::string& strDataTime) {
        std::string strData;
        std::string strTime;
        bool foundSpace = false;

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

    int timeStringToSeconds(const std::string& strTime) {
        int hours, minutes, seconds;
        char delimiter;

        std::istringstream iss(strTime);
        iss >> hours >> delimiter >> minutes >> delimiter >> seconds;

        return hours * 3600 + minutes * 60 + seconds;
    }

public:
    DiscreteDataTime(std::string path, int lineNumber) : file(path, lineNumber) {}

    std::string getStrDataTime() {
        strDataTime = file.;
        return strDataTime;
    }

    std::string getStrData() {
        splitDataTime = splitString(getStrDataTime());
        strData = splitDataTime.first;
        return strData;
    }

    std::string getStrTime() {
        splitDataTime = splitString(getStrDataTime());
        strTime = splitDataTime.second;
        return strTime;
    }

    int getIntSecTime() {
        totalSeconds = timeStringToSeconds(getStrTime());
        return totalSeconds;
    }
};