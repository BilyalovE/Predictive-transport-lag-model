import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных из файла CSV
file_path_volume_flow = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Интерполированный объемный расход.csv"
data_flow = pd.read_csv(file_path_volume_flow, sep=',')

# Инициализация словарей для хранения массивов данных
column_arrays_volume_flow = {}
for column in data_flow.columns:
    column_data_0 = data_flow.values
    column_arrays_volume_flow[column] = column_data_0
volume_flow = column_arrays_volume_flow['Интерполированный объемный расход'][100:]

# Чтение данных из файла CSV
file_path = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Результат моделирования.csv"
data = pd.read_csv(file_path, sep=',')

# Инициализация словарей для хранения массивов данных
column_arrays_0 = {}
column_arrays_2500 = {}
for column in data.columns:
    column_data_0 = data.loc[data['Координата'] == 0, column].values
    column_arrays_0[column] = column_data_0

for column in data.columns:
    column_data_2500 = data.loc[data['Координата'] == 2500, column].values
    column_arrays_2500[column] = column_data_2500


time_data = column_arrays_0['Время'][:]
print(time_data[1])
sulfur_data_0 = column_arrays_0['Сера'][:]
sulfur_data_2500 = column_arrays_2500['Сера'][:]

# Чтение данных из файла CSV
file_path = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Синхронизированный по выходу прогноз серы.csv"
data = pd.read_csv(file_path, sep=',')

# Получение массивов данных для времени и содержания серы
sulfur_data_prog = data['Ошибка прогнозирования'][:]

# Чтение данных из файла CSV
file_path = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Ошибка прогнозирования.csv"
data = pd.read_csv(file_path, sep=',')

# Получение массивов данных для времени и содержания серы
absError = data['Ошибка прогнозирования'][:]


def plot_graph(fig, x, y, title, xlabel, ylabel, ymin, deltaYmax, xMax, numSubplot, orderNumber, keySubplot):
    ax = fig.add_subplot(numSubplot, 1, orderNumber)
    if keySubplot == 1:
        ax.plot(x, y, color='blue', linestyle='-', linewidth=2, label = "Реальное содержание серы")
        ax.plot(x, sulfur_data_prog, color='red', linestyle='--', linewidth=2, label = "Прогнозное содержание серы")
        ax.legend()
    else:
        ax.plot(x, y, color='blue', linestyle='-', linewidth=2)
    #ax.set_title(title, fontsize=12)
    #ax.set_xlabel(xlabel, fontsize=10)
    ax.set_ylabel(ylabel, fontsize=10)
    ax.grid(True)
    ax.set_ylim(ymin, max(y) + deltaYmax)
    ax.set_xlim(min(x), xMax)
    ax.tick_params(axis='both', which='major', labelsize=10)
    ax.locator_params(axis='y', nbins=10)
    #ax.tick_params(axis='x', rotation=45) 
    ax.xaxis.set_major_locator(plt.MaxNLocator(50))

    for spine in ax.spines.values():
        if spine.spine_type == 'left' or spine.spine_type == 'bottom':
            spine.set_linewidth(1)
            spine.set_color('black')
            spine.set_capstyle('butt')
        else:
            spine.set_visible(False)

fig1 = plt.figure(figsize=(16.5, 10))
plot_graph(fig1, time_data, sulfur_data_0, 'График изменения серы на входе в трубу', 'Время', 'Содержание серы, ppm', 170, 10, max(time_data), 3, 1, 0)
plot_graph(fig1, time_data, sulfur_data_2500, 'График изменения серы на выходе из трубы', 'Время', 'Содержание серы, ppm', 170, 10, max(time_data), 3, 2, 0)
plot_graph(fig1, time_data, volume_flow, 'График изменения расхода', 'Время', 'Объемный расход, м3/час', 90, 10, max(time_data), 3, 3, 0)

fig2 = plt.figure(figsize=(16.5, 10))
plot_graph(fig2, time_data, sulfur_data_0, 'График изменения серы на входе в трубу', 'Время', 'Содержание серы, ppm', 170, 10, time_data[2500], 4, 1, 0) 
plot_graph(fig2, time_data, sulfur_data_2500, 'График изменения серы на выходе из трубы', 'Время', 'Содержание серы, ppm', 170, 10, time_data[2500], 4, 2, 1) 
plot_graph(fig2, time_data, absError, 'Относительная ошибка прогнозирования', 'Время', 'Относительная ошибка, %', 0, 4, time_data[2500], 4, 3, 0) 
plot_graph(fig2, time_data, volume_flow, 'График изменения расхода', 'Время', 'Объемный расход, м3/час', 90, 10, time_data[2500], 4, 4, 0) 
plt.tight_layout()
plt.show()
