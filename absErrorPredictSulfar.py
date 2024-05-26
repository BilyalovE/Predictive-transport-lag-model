import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных из файла CSV

file_path = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Результат моделирования.csv"
data = pd.read_csv(file_path, sep=',')


# Инициализация словарей для хранения массивов данных
column_arrays_1500 = {}

# Фильтрация данных по координате равной 1500 и заполнение массивов данными
for column in data.columns:
    column_data_1500 = data.loc[data['Координата'] == 1500, column].values
    column_arrays_1500[column] = column_data_1500

# Получение массивов данных для времени и плотности для координаты 1500
time_data_1500 = column_arrays_1500['Время'][:]
sulfar_data_1500 = column_arrays_1500['Сера'][:]


# Чтение данных из файла CSV
file_path = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Для расчета ошибки прогнозирования.csv"
data = pd.read_csv(file_path, sep=',')

index = 0 
predictTimeOperator = '0:15:0'
for i in range(0, len(time_data_1500)):
    if time_data_1500[i] == predictTimeOperator:
        index = i
        
print(index)
# Получение массивов данных для времени и содержания серы
time_data = data['Текущее время моделирования']
sulfur_data_prog = data['Содержание серы']



indexMines = index

absError = []

for i in range(0, len(sulfur_data_prog) - indexMines):
    absError.append(abs(sulfur_data_prog[i] - sulfar_data_1500[index]))
    index += 1

time_data1 = []
for i in range(83, len(time_data)):
    time_data1.append(time_data[i])



""" plt.plot(time_data1, absError, linestyle='-')
plt.title('Спрогнозированное содержание серы на выходе', fontsize=12)
plt.xlabel('Время', fontsize=10)
plt.ylabel('Содержание серы', fontsize=10)
plt.grid(True)
plt.xlim(0, max(time_data1))  # Устанавливаем максимальное значение по оси времени
plt.xticks(rotation=45)  # Поворот подписей по оси x
plt.gca().set_xticks(time_data)  # Установка меток оси x в соответствии с данными

# Применение пользовательского числа разбиений для сетки на верхнем графике
plt.gca().locator_params(axis='y', nbins=20)
# Применение пользовательского числа разбиений для сетки на нижнем графике
plt.gca().locator_params(axis='x', nbins=50)
plt.tick_params(axis='both', which='major', labelsize=7) # Уменьшение размера цифр осей данных

# Выделение осей цветом и толщиной
for spine in plt.gca().spines.values():
    if spine.spine_type == 'left' or spine.spine_type == 'bottom':
        spine.set_linewidth(1)  # Установка толщины осей
        spine.set_color('black')  # Установка цвета осей
        spine.set_capstyle('butt')  # Скругление концов осей
    else:
        spine.set_visible(False)  # Скрытие остальных линий границ


print(time_data[1003])
print(time_data_1500[1086])


# Отображение графика
plt.tight_layout()
plt.show()
 """


