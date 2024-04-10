#include "OutPutData.h"

OutPutData::OutPutData(std::string output_name, std::vector <double> &current_layer, double sum_dt)
{
    this->current_layer = current_layer;
    this->sum_dt = sum_dt;
    this->output_name = output_name;
}
 /// @brief output_data - метод вывода слоев в файл формата csv
 /// @param i - счётчик слоев;
 /// @param  solver_parameters - структура параметров, необходимых для реализации функции солвера ;
 /// @param buffer - буфер, рассчитанный после солвера;
 /// @param sum_dt - текущее время моделирования
void OutPutData::output_data()
{
    // Используем пространство имен std
    using namespace std;
    setlocale(LC_ALL, "rus");
    //0 слой с записью заголовка
    if (sum_dt == 0) {
        ofstream outFile(output_name + ".csv");
        outFile << "Время,Координата,Плотность,Сера,Давление" << endl;
        // Записать значения текущего слоя в файл
        for (size_t i = 0; i < current_layer.size(); i++) {
            outFile << sum_dt << "," << i * pipe.get_dx() << "," << current_layer[i]  << endl;


        }
        outFile.close();
    }
    //последующие слои
    else {
        ofstream outFile(output_name + ".csv", ios::app);
        // Записать значения текущего слоя в файл
        for (size_t i = 0; i < current_layer.size(); i++) {
            outFile << sum_dt << "," << i * pipe.get_dx() << "," << current_layer[i]  << endl;

        }
        outFile.close();
    }
}