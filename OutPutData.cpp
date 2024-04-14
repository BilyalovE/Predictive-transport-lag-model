#include "OutPutData.h"

OutPutData::OutPutData(const std::string output_name, const std::vector <double> &previous_layer, double sum_dt)
{
    this->previous_layer = previous_layer;
    this->sum_dt = sum_dt;
    this->output_name = output_name;
}

std::string OutPutData::setNormalTimeFormat() {
    int hours = sum_dt / 3600;
    int minutes = (sum_dt - hours *3600) / 60;
    int seconds = sum_dt - hours * 3600 - minutes * 60;

    std::string strHours = std::to_string(hours);
    std::string strMinutes = std::to_string(minutes);
    std::string strSeconds = std::to_string(seconds);
    return strHours + ":" + strMinutes + ":" + strSeconds;
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
        outFile << "Время,Координата,Сера" << endl;
        // Записать значения текущего слоя в файл
        for (size_t i = 0; i < previous_layer.size(); i++) {
            outFile << setNormalTimeFormat() << "," << i * pipe.get_dx() << "," << previous_layer[i]  << endl;


        }
        outFile.close();
    }
    //последующие слои
    else {
        ofstream outFile(output_name + ".csv", ios::app);
        // Записать значения текущего слоя в файл
        for (size_t i = 0; i < previous_layer.size(); i++) {
            outFile << setNormalTimeFormat() << "," << i * pipe.get_dx() << "," << previous_layer[i]  << endl;

        }
        outFile.close();
    }
}