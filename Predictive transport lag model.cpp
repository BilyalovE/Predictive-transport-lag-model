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
#include "Block_1_transport_equation.h"
#include "Struct_pipe.h"
#include <string>
#include <fstream>
#include <locale>
#include "File.h"
#include <ctime>
#include <sstream>
#include <Windows.h>

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

class VolumeFlow : protected MassFlow, protected Density{
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




/// @brief Главная функция, в которой происходит инициализация структур, краевых и начальных условий, а также вызов функции солвера и функции вывода в файл
int main(int argc, char** argv)
{
    
    // Используем пространство имен std
    using namespace std;
    setlocale(LC_ALL, "rus");


    MassFlow  massFlow("D:/source/diplop AT-20-01/data txt/mass flow.txt", 3); // Имя файла и номер строки, которую нужно считать
    std::cout << "Считанное значение massFlow: " << massFlow.getMassFlow() << std::endl;
    
    Density  density("D:/source/diplop AT-20-01/data txt/density.txt", 3); // Имя файла и номер строки, которую нужно считать
    std::cout << "Считанное значение density: " << density.getDensity() << std::endl;

    VolumeFlow volumeFlow("D:/source/diplop AT-20-01/data txt/mass flow.txt", "D:/source/diplop AT-20-01/data txt/density.txt", 3);  // Имя файла и номер строки, которую нужно считать
    std::cout << "Считанное значение volumeFlow: " << volumeFlow.getVolumeFlow() << std::endl;

    Sulfar sulfar("D:/source/diplop AT-20-01/data txt/sulfar.txt", 3); // Имя файла и номер строки, которую нужно считать
    std::cout << "Считанное значение sulfar: " << sulfar.getSulfar() << std::endl;
    
    DiscreteDataTime discreteDataTime("D:/source/diplop AT-20-01/data txt/discrete analysis data time.txt", 10); // Имя файла и номер строки, которую нужно считать
    std::cout << "Считанное значение даты и времени как строки: " << discreteDataTime.getStrDataTime() << std::endl;
    std::cout << "Считанное значение даты как строки: " << discreteDataTime.getStrData() << std::endl;
    std::cout << "Считанное значение времени как строки: " << discreteDataTime.getStrTime() << std::endl;
    std::cout << "Считанное значение времени как числа секунд формата int: " <<discreteDataTime.getIntSecTime() << std::endl;
    
    /// @param Объявление структуры с именем Pipeline_parameters для переменной pipeline_characteristics 
    Pipeline_parameters  pipeline_characteristics;
    /// @param - количество точек расчетной сетки;
    int n = 10;
    /// @param Начальное значение плотности нефти в трубе
    double initial_condition_density = 805;
    /// @param Начальное содержание серы в нефти в трубе
    double initial_condition_sulfar = 0.15;

    /// Предполагаем, что в начальный момент времени всю трубу заполняют нефть с начальными параметрами initial_condition_density
    /// @param Начальный слой по плотности
    vector <double> initial_density_layer(n, initial_condition_density);
    /// @param Вектор, содержащий значения плотностей нефти входных партий 
    vector <double> input_conditions_density = { 880, 870, 870, 870, 880, 880, 880, 880, 0 };
    /// @param Начальный слой по сере
    vector <double> initial_sulfar_layer(n, initial_condition_sulfar);
    /// @param Вектор содержания серы в нефти входных партий 
    vector <double> input_conditions_sulfar = { 0.2, 0.1, 0.1, 0.1, 0.25, 0.25, 0.25, 0.25, 0 };
    vector <double> time_input_parties = { 10, 20, 40, 50, 80, 100, 200, 300, 400 };
    /// @param Число рассчитываемых параметров (2 параметра - сера и плотность)
    int num_parameters = 2;
    /// @param Вектор векторов параметров нефти входных партий
    vector <vector<double>> input_conditions(num_parameters, vector<double>(input_conditions_sulfar.size()));
    input_conditions = { input_conditions_density, input_conditions_sulfar };
    // Создаем  буфер для решаемой задачи
    /// @param number_layers_buffer - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя - предыдущий и текущий слои)
    int number_layers_buffer = 2;
    ring_buffer_t <vector<vector<double>>> buffer(number_layers_buffer, { initial_density_layer, initial_sulfar_layer });

    // Расчёт произвольного числа слоев (solver_parameters.number_layers) через вызов функции solver в цикле
    /// @param sum_dt -  сумма времени моделирования 
    double sum_dt = 0;
    /// @param j - счетчик слоев
    int j = 0;
    do {
        /// @param transport_equation (солвер - метод характеристик) - экземпляр класса Block_1
        Block_1_transport_equation transport_equation(pipeline_characteristics, n, j);
        // Проверка выхода за границы массива входных параметров нефти
        if (j < input_conditions_density.size()) {
            for (size_t i{ 0 }; i < num_parameters; i++) {
                transport_equation.method_characteristic(buffer.current()[i], buffer.previous()[i], input_conditions[i][j]);
            }
        }
        else {
            for (size_t i{ 0 }; i < num_parameters; i++) {
                double empty_pipe{ 0 };
                transport_equation.method_characteristic(buffer.current()[i], buffer.previous()[i], empty_pipe);
            }
        }
        transport_equation.output_data(buffer, sum_dt);
        buffer.advance(1);
        sum_dt += transport_equation.get_dt();
        j++;
    } while (sum_dt <= pipeline_characteristics.T);
    return 0;
}