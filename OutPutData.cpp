#include "OutPutData.h"

OutPutData::OutPutData(const std::string output_name, const std::vector <double> &previous_layer, double sum_dt)
{
    this->previous_layer = previous_layer;
    this->sum_dt = sum_dt;
    this->output_name = output_name;
}

OutPutData::OutPutData(const std::string output_name, const double sulfur, const double timeDelayPredict)
{
    this->sulfur = sulfur;
    this->output_name = output_name;
    this->timeDelayPredict = timeDelayPredict;
}



std::string OutPutData::setNormalTimeFormat(const double time) {
    int hours = time / 3600;
    int minutes = (time - hours *3600) / 60;
    int seconds = time - hours * 3600 - minutes * 60;

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
void OutPutData::outputModelingFlowRawMaterials()
{
    // Используем пространство имен std
    using namespace std;
    setlocale(LC_ALL, "rus");
    ofstream outFile;
    outFile.open(output_name + ".csv", ofstream::app);
    //0 слой с записью заголовка
    if (sum_dt == 0) {
        outFile << "Время,Координата,Сера" << endl;
        // Записать значения текущего слоя в файл
        for (size_t i = 0; i < previous_layer.size(); i++) {
            outFile << setNormalTimeFormat(sum_dt) << "," << i * pipe.get_dx() << "," << previous_layer[i]  << endl;


        }
        outFile.close();
    }
    //последующие слои
    else {
        // Записать значения текущего слоя в файл
        for (size_t i = 0; i < previous_layer.size(); i++) {
            outFile << setNormalTimeFormat(sum_dt) << "," << i * pipe.get_dx() << "," << previous_layer[i]  << endl;

        }
        outFile.close();
    }
}

void OutPutData::outputTransportDelay()
{
    // Используем пространство имен std
    using namespace std;
    setlocale(LC_ALL, "rus");
    ofstream outFile;
    outFile.open(output_name + ".csv", ofstream::app);
    //0 время транспортного запаздывания - начало прогнозирования - точка находится в начале трубы
    if (timeDelayPredict == 0) {
        outFile << "Сера,Транспортное запаздывание" << endl;
        outFile.close();
    }
    else {
        
        outFile << sulfur << "," << setNormalTimeFormat(timeDelayPredict) << endl;
        outFile.close();
    }
}