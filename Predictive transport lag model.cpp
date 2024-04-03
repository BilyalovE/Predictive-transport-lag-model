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

double call_sulfar(std::string path, int lineNumber) {
    Sulfar sulfar(path, lineNumber); // Имя файла и номер строки, которую нужно считать
    return sulfar.getSulfar();
}

/// @brief Главная функция, в которой происходит инициализация структур, краевых и начальных условий, а также вызов функции солвера и функции вывода в файл
int main(int argc, char** argv)
{
    setlocale(LC_ALL, "rus");
    //MassFlow  massFlow("D:/source/diplop AT-20-01/data txt/mass flow.txt", 3); // Имя файла и номер строки, которую нужно считать
    //std::cout << "Считанное значение massFlow: " << massFlow.getMassFlow() << std::endl;
    //
    //Density  density("D:/source/diplop AT-20-01/data txt/density.txt", 3); // Имя файла и номер строки, которую нужно считать
    //std::cout << "Считанное значение density: " << density.getDensity() << std::endl;

    //VolumeFlow volumeFlow("D:/source/diplop AT-20-01/data txt/mass flow.txt", "D:/source/diplop AT-20-01/data txt/density.txt", 3);  // Имя файла и номер строки, которую нужно считать
    //std::cout << "Считанное значение volumeFlow: " << volumeFlow.getVolumeFlow() << std::endl;

    //Sulfar sulfar("D:/source/diplop AT-20-01/data txt/sulfar.txt", 3); // Имя файла и номер строки, которую нужно считать
    //std::cout << "Считанное значение sulfar: " << sulfar.getSulfar() << std::endl;
    //
    //DiscreteDataTime discreteDataTime("D:/source/diplop AT-20-01/data txt/discrete analysis data time.txt", 10); // Имя файла и номер строки, которую нужно считать
    //std::cout << "Считанное значение даты и времени как строки: " << discreteDataTime.getStrDataTime() << std::endl;
    //std::cout << "Считанное значение даты как строки: " << discreteDataTime.getStrData() << std::endl;
    //std::cout << "Считанное значение времени как строки: " << discreteDataTime.getStrTime() << std::endl;
    //std::cout << "Считанное значение времени как числа секунд формата int: " <<discreteDataTime.getIntSecTime() << std::endl;
    
    /// @param Структура параметров трубопровода
    Pipeline_parameters  pipeline_characteristics;
    /// @param Начальное значение содержания серы в бензине каткрекинга в трубе
    double initial_condition_sulfar = call_sulfar("D:/source/diplop AT-20-01/data txt/sulfar.txt", 2);

    /// Предполагаем, что в начальный момент времени всю трубу заполняют бензин каткрекинга с начальными параметрами initial_condition_sulfar
    /// @param Начальный слой по плотности
    vector <double> initial_sulfar_layer(pipeline_characteristics.n, initial_condition_sulfar);

    // Создаем  буфер для решаемой задачи
    /// @param number_layers_buffer - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя - предыдущий и текущий слои)
    int number_layers_buffer = 2;
    ring_buffer_t <vector<double>> buffer(number_layers_buffer, initial_sulfar_layer);
   
    
    /// @param Измеренное значение содержания серы в бензине каткрекинга в трубе
    double input_condition_sulfar = call_sulfar("D:/source/diplop AT-20-01/data txt/sulfar.txt", 3); ;
    
    /// Расчёт произвольного числа слоев (solver_parameters.number_layers) через вызов функции solver в цикле
    /// @param sum_dt -  сумма времени моделирования 
    double sum_dt = 0;
    double speed = 0;
    /// @param - счетчик слоя серы на момент  моделирования.
    int i = 0; 
    do {
        VolumeFlow volumeFlow("D:/source/diplop AT-20-01/data txt/mass flow.txt", "D:/source/diplop AT-20-01/data txt/density.txt", 3);  // Имя файла и номер строки, которую нужно считать 
        TransportEquation transportEquation(pipeline_characteristics, volumeFlow.getVolumeFlow());
        if (i==0){ transportEquation.methodCharacteristic(buffer.current(), buffer.previous(), input_condition_sulfar);
        }
        else {
            transportEquation.methodCharacteristic(buffer.current(), buffer.previous(), 0);
        }
        sum_dt += transportEquation.get_dt();
        speed = transportEquation.get_speed();
        buffer.advance(1);
        i++;
    } while (sum_dt * speed <= pipeline_characteristics.L + pipeline_characteristics.get_dx());
    std::cout << sum_dt << std::endl;

    std::cout << speed << std::endl;
    return 0;
}