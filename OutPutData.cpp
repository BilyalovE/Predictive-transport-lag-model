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

OutPutData::OutPutData(const std::string output_name, const double sulfur, const double sum_dt)
{
    this->sulfur = sulfur;
    this->output_name = output_name;
    this->sum_dt = sum_dt;
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

void OutPutData::outputPredictSulfar() {
    using namespace std;
    ofstream outFile;
    outFile.exceptions(ofstream::badbit | ofstream::failbit);
    try {
        // Проверяем, существует ли файл
        bool fileExists = ifstream(output_name + ".csv").good();

        outFile.open(output_name + ".csv", ofstream::app);

        // Если файл не существует, записываем заголовки
        if (!fileExists) {
            outFile << "Содержание серы,Текущее время моделирования" << endl;
        }

        // Записываем данные
        outFile << sulfur << "," << setNormalTimeFormat(sum_dt) << endl;

        outFile.close();
    }
    catch (const ofstream::failure& ex) {
        ex.what();
    }
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
    ofstream outFile;
    outFile.exceptions(ofstream::badbit | ofstream::failbit);
    
    try
    {
        setlocale(LC_ALL, "rus");
        outFile.open(output_name + ".csv", ofstream::app);
        //0 слой с записью заголовка
        if (sum_dt == 0) {
            outFile << "Время,Координата,Сера" << endl;
            // Записать значения текущего слоя в файл
            for (size_t i = 0; i < previous_layer.size(); i++) {
                outFile << setNormalTimeFormat(sum_dt) << "," << i * pipe.get_dx() << "," << previous_layer[i] << endl;

            }
            outFile.close();
        }
        //последующие слои
        else {
            // Записать значения текущего слоя в файл
            for (size_t i = 0; i < previous_layer.size(); i++) {
                outFile << setNormalTimeFormat(sum_dt) << "," << i * pipe.get_dx() << "," << previous_layer[i] << endl;

            }
            outFile.close();
        }
    }
    catch (const ofstream::failure& ex)
    {
        ex.what();
    }
    
}

void OutPutData::outputTransportDelay() {
    using namespace std;
    ofstream outFile;
    outFile.exceptions(ofstream::badbit | ofstream::failbit);
    try {
        // Проверяем, существует ли файл
        bool fileExists = ifstream(output_name + ".csv").good();

        outFile.open(output_name + ".csv", ofstream::app);

        // Если файл не существует, записываем заголовки
        if (!fileExists) {
            outFile << "Содержание серы,Время транспортного запаздывания" << endl;
        }

        // Записываем данные
        outFile << sulfur << "," << setNormalTimeFormat(timeDelayPredict) << endl;

        outFile.close();
    }
    catch (const ofstream::failure& ex) {
        ex.what();
    }
}