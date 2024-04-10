import numpy as np
import pandas as pd
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, TextBox

rawData = pd.read_csv('C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Результат моделирования.csv')
parametersNames = rawData.columns.tolist()[2:]
[timeLabel, coordLabel] = rawData.columns.tolist()[:2]
plotsCount = len(parametersNames) 



fig = plt.figure(figsize=(11, 7))  # Установка размера фигуры на 11x7 дюймов


axes = [plt.subplot(plotsCount, 1, _ + 1) for _ in range(plotsCount)]

data_skip = len(set(rawData[coordLabel]))
time_moments = list(set(rawData[timeLabel]))
time_moments.sort()

def init_func():
    for i in range(len(axes)):
        axes[i].clear()
        axes[i].grid(visible=True)
        axes[i].set_xlabel(coordLabel)
        axes[i].set_ylabel(parametersNames[i])
        coordData = rawData[coordLabel]
        paramData = rawData[parametersNames[i]]
        
        # Проверяем, есть ли в данных значения NaN или Inf
        if not np.isnan(paramData).any() and not np.isinf(paramData).any():
            xLim = [0, max(coordData) + 100]  # Пределы оси x с 50 единицами после максимального значения
            yLim = [0, max(paramData) + 30]    # Пределы оси y с 5 единицами после максимального значения
        else:
            # Если в данных есть значения NaN или Inf, устанавливаем пределы по умолчанию
            xLim = [0, 1]
            yLim = [0, 1]
        
        # Установка пределов для осей x и y
        axes[i].set_xlim(xLim)
        axes[i].set_ylim(yLim)
        
        # Установка шага для оси x
        x_ticks = np.arange(0, xLim[1], 100)
        axes[i].set_xticks(x_ticks)
        
        # Установка шага для оси y
        y_ticks = np.arange(0, yLim[1], 10)
        axes[i].set_yticks(y_ticks)

        # Установка толщины линий графика
        for line in axes[i].lines:
            line.set_linewidth(100)  # Установка толщины линий на 2 точки
        
    ini_draw()

        
plots = list()
def ini_draw(step=0):
    for i in range(len(parametersNames)):
        coordData = rawData[coordLabel]
        paramData = rawData[parametersNames[i]]
        plots.append(axes[i].plot(coordData[step * data_skip: (step + 1) * data_skip], paramData[step * data_skip: (step + 1) * data_skip], color='b'))

def change(step):
    for i in range(len(axes)):
        paramData = rawData[parametersNames[i]]
        plots[i][0].set_ydata(paramData[step * data_skip: (step + 1) * data_skip])
        text_box.set_val(f"{time_moments[step]} c")

init_func()

if len(time_moments) != 1:
        ax_time = plt.axes([0.15, 0.0001, 0.5, 0.04])
        time_slider = Slider(ax_time, 'Steps', 0, len(time_moments)-1, valstep=1)
        ax_textbox = fig.add_axes([0.75, 0.001, 0.2, 0.04])
        text_box = TextBox(ax_textbox, "t =", textalignment="center")
        text_box.set_val("0 c")

        time_slider.on_changed(change)

# Перед отображением графика установите размер окна
fig.set_size_inches(11, 7)  # Установка размера окна на 11x7 дюймов


#plt.tight_layout()
# Настройка расположения графика в окне
plt.subplots_adjust(top=0.95, bottom=0.2, right=0.95, left=0.05, hspace=1, wspace=1)
plt.tight_layout()
plt.show()


