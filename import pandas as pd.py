import pandas as pd
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



# Создание графиков
plt.figure(figsize=(17, 10))

# График для координаты 0
plt.subplot(2, 1, 1)
plt.plot(time_data_0, sulfur_data_0, linestyle='-')
plt.title('Изменение серы на входе в трубу', fontsize=12)
plt.xlabel('Время', fontsize=12)
plt.ylabel('Содержание серы', fontsize=12)
plt.grid(True)
plt.xlim(0, max(time_data_0))  # Устанавливаем максимальное значение по оси времени
plt.xticks(rotation=45)  # Поворот подписей по оси x
plt.gca().set_xticks(time_data_0)  # Установка меток оси x в соответствии с данными

# Применение пользовательского числа разбиений для сетки на верхнем графике
plt.gca().locator_params(axis='y', nbins=30)
# Применение пользовательского числа разбиений для сетки на нижнем графике
plt.gca().locator_params(axis='x', nbins=50)
plt.tick_params(axis='both', which='major', labelsize=8)  # Уменьшение размера цифр осей данных

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
plt.subplot(2, 1, 2)
plt.plot(time_data_1500, density_data_1500, linestyle='-')
plt.title('Изменение серы на выходе из трубы', fontsize=12)
plt.xlabel('Время', fontsize=12)
plt.ylabel('Содержание серы', fontsize=12)
plt.grid(True)
plt.xlim(0, max(time_data_1500))  # Устанавливаем максимальное значение по оси времени
plt.xticks(rotation=45)  # Поворот подписей по оси x
plt.gca().set_xticks(time_data_1500)  # Установка меток оси x в соответствии с данными

# Применение пользовательского числа разбиений для сетки на нижнем графике
plt.gca().locator_params(axis='y', nbins=30)
# Применение пользовательского числа разбиений для сетки на нижнем графике
plt.gca().locator_params(axis='x', nbins=50)
plt.tick_params(axis='both', which='major', labelsize=8)  # Уменьшение размера цифр осей данных

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

# Отображение графиков
plt.tight_layout()
plt.show()