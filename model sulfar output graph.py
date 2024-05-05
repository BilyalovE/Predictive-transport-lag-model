import pandas as pd
import matplotlib.pyplot as plt

# Чтение данных из файла CSV

file_path = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Результат моделирования.csv"
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



# Создание графиков
plt.figure(figsize=(16.5, 10))

# График для координаты 0
plt.subplot(3, 1, 1)
plt.plot(time_data_0, sulfur_data_0, linestyle='-')
plt.title('Изменение серы на входе в трубу', fontsize=12)
plt.xlabel('Время', fontsize=10)
plt.ylabel('Содержание серы', fontsize=10)
plt.grid(True)
plt.ylim(170, max(sulfur_data_0)+10) 
plt.xlim(0, max(time_data_0))  # Устанавливаем максимальное значение по оси времени
plt.xticks(rotation=45)  # Поворот подписей по оси x
plt.gca().set_xticks(time_data_0)  # Установка меток оси x в соответствии с данными

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

# Добавление стрелок на концах осей
plt.annotate('', xy=(0, 0), xytext=(0, 1), arrowprops=dict(facecolor='black', arrowstyle='<|-', linewidth=2))
plt.annotate('', xy=(0, 0), xytext=(1, 0), arrowprops=dict(facecolor='black', arrowstyle='<|-', linewidth=2))

# График для координаты 1500
plt.subplot(3, 1, 2)
plt.plot(time_data_1500, density_data_1500, linestyle='-')
plt.title('Изменение серы на выходе из трубы', fontsize=12)
plt.xlabel('Время', fontsize=10)
plt.ylabel('Содержание серы', fontsize=10)
plt.grid(True)
plt.ylim(170, max(density_data_1500)+10) 
plt.xlim(0, max(time_data_1500))  # Устанавливаем максимальное значение по оси времени
plt.xticks(rotation=45)  # Поворот подписей по оси x
plt.gca().set_xticks(time_data_1500)  # Установка меток оси x в соответствии с данными

# Применение пользовательского числа разбиений для сетки на нижнем графике
plt.gca().locator_params(axis='y', nbins=20)
# Применение пользовательского числа разбиений для сетки на нижнем графике
plt.gca().locator_params(axis='x', nbins=50)
plt.tick_params(axis='both', which='major', labelsize=7)  # Уменьшение размера цифр осей данных

# Выделение осей цветом и толщиной
for spine in plt.gca().spines.values():
    if spine.spine_type == 'left' or spine.spine_type == 'bottom':
        spine.set_linewidth(1)  # Установка толщины осей
        spine.set_color('black')  # Установка цвета осей
        spine.set_capstyle('butt')  # Скругление концов осей
    else:
        spine.set_visible(False)  # Скрытие остальных линий границ

# Добавление стрелок на концах осей
plt.annotate('', xy=(0, 0), xytext=(0, 1), arrowprops=dict(facecolor='black', arrowstyle='<|-', linewidth=2))
plt.annotate('', xy=(0, 0), xytext=(1, 0), arrowprops=dict(facecolor='black', arrowstyle='<|-', linewidth=2))





# Чтение данных из файла CSV
file_path = "C:/Users/bilyalov/source/repos/Predictive-transport-lag-model/Прогнозирование качества сырья на выходе при заданном времени прогноза.csv"
data = pd.read_csv(file_path, sep=',')

# Получение массивов данных для времени и содержания серы
time_data = data['Время']
sulfur_data_prog = data['Сера']


# График для прогнозной серы
plt.subplot(3, 1, 3)
plt.plot(time_data, sulfur_data_prog, linestyle='-')
plt.title('Спрогнозированное содержание серы на выходе', fontsize=12)
plt.xlabel('Время', fontsize=10)
plt.ylabel('Содержание серы', fontsize=10)
plt.grid(True)
plt.ylim(170, max(sulfur_data_prog)+10) 
plt.xlim(0, max(time_data))  # Устанавливаем максимальное значение по оси времени
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




# Отображение графика
plt.tight_layout()
plt.show()
