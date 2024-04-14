import numpy as np
import pandas as pd
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt

# Чтение данных из файла CSV
file_path = "D:/source/Predictive transport lag model/Результат моделирования.csv"
data = pd.read_csv(file_path, sep=',')

# Инициализация словарей для хранения массивов данных
column_arrays_0 = {}
column_arrays_1500 = {}

# Фильтрация данных по координате равной 0 и заполнение массивов данными
for column in data.columns:
    column_data_0 = data.loc[data['Координата'] == 0, column].values
    column_arrays_0[column] = column_data_0

# Фильтрация данных по координате равной 1500 и заполнение массивов данными
for column in data.columns:
    column_data_1500 = data.loc[data['Координата'] == 1500, column].values
    column_arrays_1500[column] = column_data_1500

# Получение массивов данных для времени и содержания серы для координаты 0
time_data_0 = column_arrays_0['Время'][:]
sulfur_data_0 = column_arrays_0['Сера'][:]

# Получение массивов данных для времени и плотности для координаты 1500
time_data_1500 = column_arrays_1500['Время'][:]
density_data_1500 = column_arrays_1500['Сера'][:]


rawData = pd.read_csv("D:/source/Predictive transport lag model/Результат моделирования.csv")
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

# Установите начальное количество разбиений для осей x и y
x_ticks_count = 16
y_ticks_count = 26
# Определите границы для осей x и y
x_min = 0
x_max = 1500
y_min = rawData[parametersNames].min().min() - 10  # Минимальное значение среди всех параметров
y_max = rawData[parametersNames].max().max() + 10  # Максимальное значение среди всех параметров

# Переменная для хранения предыдущего времени
prev_time = None

def update_plot(i):
    global prev_time
    for ax, parameter in zip(axes, parametersNames):
        coordData = rawData[coordLabel]
        paramData = rawData[parameter]
        ax.clear()
        ax.grid(True)  # Add grid
        ax.plot(coordData[i * data_skip: (i + 1) * data_skip], paramData[i * data_skip: (i + 1) * data_skip], color='k')
        
        # Установите границы для оси x и y
        ax.set_xlim(x_min, x_max)
        ax.set_ylim(y_min, y_max)
        
        # Set x-axis ticks
        x_ticks = np.linspace(x_min, x_max, num=x_ticks_count)
        ax.set_xticks(x_ticks)
        
        # Set y-axis ticks
        y_ticks = np.linspace(y_min, y_max, num=y_ticks_count)
        ax.set_yticks(y_ticks)
        
        ax.set_xlabel(coordLabel)  # Установите подпись оси x
        ax.set_ylabel(parameter)  # Установите подпись оси y
        
        # Вывод времени, только если оно изменилось
        if prev_time != unique_times[i]:
            
            ax.text(1.1, 1.1, f"Время метода характеристик: {time_data_0[i]}", transform=ax.transAxes, ha='right', va='top', fontsize=10, bbox=dict(facecolor='white', alpha=0.5))
            prev_time = unique_times[i]


anim = FuncAnimation(fig,
                     update_plot,
                     frames=len(unique_times),
                     interval=1000, repeat=False)

plt.show()
