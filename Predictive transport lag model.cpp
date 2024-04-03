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
#include "Pipe.h"
#include <locale>
#include "File.h"
#include "Parser.h"






/// @brief Главная функция, в которой происходит инициализация структур, краевых и начальных условий, а также вызов функции солвера и функции вывода в файл
int main(int argc, char** argv)
{
    
    // Используем пространство имен std
    using namespace std;
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
    Sulfar sulfar("D:/source/diplop AT-20-01/data txt/sulfar.txt", 2); // Имя файла и номер строки, которую нужно считать
    /// @param Начальное значение содержания серы в бензине каткрекинга в трубе
    double initial_condition_sulfar = sulfar.getSulfar();
    /// Предполагаем, что в начальный момент времени всю трубу заполняют бензин каткрекинга с начальными параметрами initial_condition_sulfar
    /// @param Начальный слой по плотности
    vector <double> initial_sulfar_layer(n, initial_condition_sulfar);

    /*/// @param Вектор, содержащий значения плотностей нефти входных партий 
    vector <double> input_conditions_density = { 880, 870, 870, 870, 880, 880, 880, 880, 0 };
    /// @param Начальный слой по сере
    vector <double> initial_sulfar_layer(n, initial_condition_sulfar);
    /// @param Вектор содержания серы в нефти входных партий 
    vector <double> input_conditions_sulfar = { 0.2, 0.1, 0.1, 0.1, 0.25, 0.25, 0.25, 0.25, 0 };
    vector <double> time_input_parties = { 10, 20, 40, 50, 80, 100, 200, 300, 400 };*/
    /*/// @param Число рассчитываемых параметров (2 параметра - сера и плотность)
    int num_parameters = 2;
    /// @param Вектор векторов параметров нефти входных партий
    vector <vector<double>> input_conditions(num_parameters, vector<double>(input_conditions_sulfar.size()));
    input_conditions = { input_conditions_density, input_conditions_sulfar };*/

    // Создаем  буфер для решаемой задачи
    /// @param number_layers_buffer - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя - предыдущий и текущий слои)
    int number_layers_buffer = 2;
    ring_buffer_t <vector<double>> buffer(number_layers_buffer, initial_sulfar_layer);
   
    Sulfar sulfar("D:/source/diplop AT-20-01/data txt/sulfar.txt", 3); // Имя файла и номер строки, которую нужно считать
    /// @param Измеренное значение содержания серы в бензине каткрекинга в трубе
    double input_condition_sulfar = sulfar.getSulfar();
    
        // Расчёт произвольного числа слоев (solver_parameters.number_layers) через вызов функции solver в цикле
    /// @param sum_dt -  сумма времени моделирования 
    double sum_dt = 0;
    /// @param j - счетчик слоев
    int j = 0;
    do {
        Block_1_transport_equation transport_equation(pipeline_characteristics, n, j);
        transport_equation.method_characteristic(buffer, input_condition_sulfar);
            
        transport_equation.output_data(buffer, sum_dt);
        buffer.advance(1);
        sum_dt += transport_equation.get_dt();
        j++;
    } while (sum_dt <= pipeline_characteristics.T);
    return 0;
}