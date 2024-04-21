import numpy as np
import pandas as pd
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt

# Чтение данных из файла CSV
file_path = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Прогнозируемое транспортное запаздывание.csv"
data = pd.read_csv(file_path, sep=',', encoding='utf-8', header=0)

# Получение массива данных времени транспортного запаздывания
time_delay_array = data['Врем'].values

# Получение массива данных содержания серы
sulfur_array = data['Содержание серы'].values

# Печать первых значений для проверки
print(time_delay_array[:5])
print(sulfur_array[:5])

file_path1 = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Результат моделирования.csv"
data_model = pd.read_csv(file_path1, sep=',')

# Инициализация словарей для хранения массивов данных
column_arrays_0 = {}
column_arrays_1500 = {}

# Фильтрация данных по координате равной 0 и заполнение массивов данными
for column in data_model.columns:
    column_data_0 = data_model.loc[data_model['Координата'] == 0, column].values
    column_arrays_0[column] = column_data_0

# Фильтрация данных по координате равной 1500 и заполнение массивов данными
for column in data_model.columns:
    column_data_1500 = data_model.loc[data_model['Координата'] == 1500, column].values
    column_arrays_1500[column] = column_data_1500

# Получение массивов данных для времени и содержания серы для координаты 0
time_data_0 = column_arrays_0['Время'][:]
sulfur_data_0 = column_arrays_0['Сера'][:]

# Получение массивов данных для времени и плотности для координаты 1500
time_data_1500 = column_arrays_1500['Время'][:]
density_data_1500 = column_arrays_1500['Сера'][:]

rawData = pd.read_csv(file_path1)
parametersNames = rawData.columns[2:].tolist()
timeLabel, coordLabel = rawData.columns[:2]

# Precompute values
plotsCount = len(parametersNames)
data_skip = len(set(rawData[coordLabel]))
unique_times = sorted(set(rawData[timeLabel]))

# Create figure and axes
if plotsCount == 1:
    fig, axes = plt.subplots(1, 1, figsize=(8, 6), sharex=True)
    axes = [axes]  # Wrap single axes object in a list
else:
    fig, axes = plt.subplots(plotsCount, 1, figsize=(8, 6), sharex=True)

# Initialize plots
for ax, parameter in zip(axes, parametersNames):
    ax.grid(visible=True)
    ax.set_xlabel(coordLabel)
    ax.set_ylabel(parameter)

# Установка начального количества разбиений для осей x и y
x_ticks_count = 16
y_ticks_count = 30
# Определение границ для осей x и y
x_min = 0
x_max = 1500
y_min = rawData[parametersNames].min().min() - 10  # Минимальное значение среди всех параметров
y_max = rawData[parametersNames].max().max() + 10  # Максимальное значение среди всех параметров

# Переменные для хранения предыдущего времени и индекса массива sulfur_array
prev_time = None
j = 0
flag = False
def update_plot(i):
    global prev_time, j, flag
      # Инициализация флага
    for ax, parameter in zip(axes, parametersNames):
        coordData = rawData[coordLabel]
        paramData = rawData[parameter]
        ax.clear()
        ax.grid(True)  # Добавление сетки
        ax.plot(coordData[i * data_skip: (i + 1) * data_skip], paramData[i * data_skip: (i + 1) * data_skip], color='blue', linewidth=2)
        
        # Установка границ для осей x и y
        ax.set_xlim(x_min, x_max)
        ax.set_ylim(y_min, y_max)
        
        # Установка делений на оси x
        x_ticks = np.linspace(x_min, x_max, num=x_ticks_count)
        ax.set_xticks(x_ticks)
        
        # Установка делений на оси y
        y_ticks = np.linspace(y_min, y_max, num=y_ticks_count)
        ax.set_yticks(y_ticks)
        
        ax.set_xlabel(coordLabel)  # Установка подписи оси x
        ax.set_ylabel(parameter)  # Установка подписи оси y
        
        # Вывод времени, только если оно изменилось
        if prev_time != unique_times[i]:
            ax.text(1.1, 1.1, f"Время метода характеристик: {time_data_0[i]}", transform=ax.transAxes, ha='right', va='top', fontsize=10, bbox=dict(facecolor='white', alpha=0.5))
            prev_time = unique_times[i]  # Обновление времени
        
       
        if sulfur_array[j] == sulfur_data_0[i]:
            flag = True
            ax.text(0.2, 1.055, f"Время транспортного запаздывания: {time_delay_array[j]}", transform=ax.transAxes, ha='right', va='top', fontsize=10, bbox=dict(facecolor='white', alpha=0.5))
            ax.text(0.1, 1.1, f"Сера: {sulfur_array[j]}", transform=ax.transAxes, ha='right', va='top', fontsize=10, bbox=dict(facecolor='white', alpha=0.5))
        if j > 0:
            if flag:
                if sulfur_array[j] == sulfur_array[j-1]:
                    ax.text(0.2, 1.055, f"Время транспортного запаздывания: {time_delay_array[j]}", transform=ax.transAxes, ha='right', va='top', fontsize=10, bbox=dict(facecolor='white', alpha=0.5))
                    ax.text(0.1, 1.1, f"Сера: {sulfur_array[j]}", transform=ax.transAxes, ha='right', va='top', fontsize=10, bbox=dict(facecolor='white', alpha=0.5))
                    
                else:
                    flag = False
        if not flag:
            ax.text(0.2, 1.055, f"Время транспортного запаздывания: {0}", transform=ax.transAxes, ha='right', va='top', fontsize=10, bbox=dict(facecolor='white', alpha=0.5))
            ax.text(0.1, 1.1, f"Сера: {sulfur_data_0[i]}", transform=ax.transAxes, ha='right', va='top', fontsize=10, bbox=dict(facecolor='white', alpha=0.5))
            j-=1
        j += 1  
        
anim = FuncAnimation(fig,
                     update_plot,
                     frames=len(unique_times),
                     interval=0.001, repeat=False)

plt.show()
