import tkinter as tk
from tkinter import ttk
import pandas as pd

import numpy as np
import pandas as pd
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
# Чтение данных из файла CSV
file_path = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Прогнозируемое транспортное запаздывание.csv"  # Укажите путь к вашему файлу CSV
data = pd.read_csv(file_path)

# Получение данных из столбцов
sulfur_content = data['Содержание серы'].values
time_transport_delay = data['Время транспортного запаздывания'].values

# Создание графического интерфейса Tkinter
root = tk.Tk()
root.title("Данные о транспортном запаздывании и содержании серы")
root.geometry("800x600")

# Функция для обновления меток с данными
def update_labels():
    label_sulfur.config(text=f"Содержание серы: {sulfur_content}")
    label_transport_delay.config(text=f"Время транспортного запаздывания: {time_transport_delay}")
    root.after(1000, update_labels)  # Повторно вызываем функцию через 1 секунду

# Создание и размещение меток для отображения данных
label_sulfur = ttk.Label(root, text=f"Содержание серы: {sulfur_content}")
label_sulfur.grid(row=0, column=0, padx=10, pady=10, sticky="nw")

label_transport_delay = ttk.Label(root, text=f"Время транспортного запаздывания: {time_transport_delay}")
label_transport_delay.grid(row=1, column=0, padx=10, pady=10, sticky="nw")

# Запуск функции обновления меток
update_labels()

# Чтение данных из файла CSV для графика
file_path_graph = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Результат моделирования.csv"  # Укажите путь к файлу CSV для графика
data_graph = pd.read_csv(file_path_graph, sep=',')

# Чтение данных из файла CSV для вывода результата расчета транспортного запаздывания
file_path_delay = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Прогнозируемое транспортное запаздывание.csv"  # Укажите путь к файлу CSV с прогнозируемым транспортным запаздыванием
data_delay = pd.read_csv(file_path_delay, sep=',')

# Инициализация словарей для хранения массивов данных для графика
column_arrays_0 = {}
column_arrays_1500 = {}

# Фильтрация данных по координате равной 0 и заполнение массивов данными для графика
for column in data_graph.columns:
    column_data_0 = data_graph.loc[data_graph['Координата'] == 0, column].values
    column_arrays_0[column] = column_data_0

# Фильтрация данных по координате равной 1500 и заполнение массивов данными для графика
for column in data_graph.columns:
    column_data_1500 = data_graph.loc[data_graph['Координата'] == 1500, column].values
    column_arrays_1500[column] = column_data_1500

# Получение массивов данных для времени и содержания серы для координаты 0
time_data_0 = column_arrays_0['Время'][:]
sulfur_data_0 = column_arrays_0['Сера'][:]

# Получение массивов данных для времени и плотности для координаты 1500
time_data_1500 = column_arrays_1500['Время'][:]
density_data_1500 = column_arrays_1500['Сера'][:]

# Precompute values для графика
rawData = pd.read_csv(file_path_graph)
parametersNames = rawData.columns[2:].tolist()
timeLabel, coordLabel = rawData.columns[:2]
plotsCount = len(parametersNames)
data_skip = len(set(rawData[coordLabel]))
unique_times = sorted(set(rawData[timeLabel]))
x_ticks_count = 16
y_ticks_count = 26
x_min = 0
x_max = 1500
y_min = rawData[parametersNames].min().min() - 10
y_max = rawData[parametersNames].max().max() + 10
prev_time = None

# Создание функции обновления графика
def update_plot(i):
    global prev_time
    for ax, parameter in zip(axes, parametersNames):
        coordData = rawData[coordLabel]
        paramData = rawData[parameter]
        ax.clear()
        ax.grid(True)  
        ax.plot(coordData[i * data_skip: (i + 1) * data_skip], paramData[i * data_skip: (i + 1) * data_skip], color='blue', linewidth=2)
        ax.set_xlim(x_min, x_max)
        ax.set_ylim(y_min, y_max)
        x_ticks = np.linspace(x_min, x_max, num=x_ticks_count)
        ax.set_xticks(x_ticks)
        y_ticks = np.linspace(y_min, y_max, num=y_ticks_count)
        ax.set_yticks(y_ticks)
        ax.set_xlabel(coordLabel)  
        ax.set_ylabel(parameter)  
        if prev_time != unique_times[i]:
            ax.text(1.1, 1.1, f"Время метода характеристик: {time_data_0[i]}", transform=ax.transAxes, ha='right', va='top', fontsize=10, bbox=dict(facecolor='white', alpha=0.5))
            prev_time = unique_times[i]

# Создание графического интерфейса для графика
fig, axes = plt.subplots(plotsCount, 1, figsize=(8, 6), sharex=True)
if plotsCount == 1:
    axes = [axes]
for ax, parameter in zip(axes, parametersNames):
    ax.grid(visible=True)
    ax.set_xlabel(coordLabel)
    ax.set_ylabel(parameter)

# Создание анимации для графика
anim = FuncAnimation(fig, update_plot, frames=len(unique_times), interval=0.001, repeat=False)

# Запуск главного цикла
plt.get_current_fig_manager().window.state('zoomed')
plt.show()
root.mainloop()
