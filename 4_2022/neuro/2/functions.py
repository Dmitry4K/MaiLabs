import numpy as np
import matplotlib.pyplot as plt


def mse(predicted, real):
    return np.sqrt(np.sum((real - predicted) ** 2) / predicted.shape[0])


def convert_to_X_Y_for_train(y, input_size):
    X = []
    Y = []
    for i in range(len(y) - input_size):
        X.append(y[i:i + input_size].copy())
        Y.append(y[i + input_size].copy())
    return np.array(X), np.array(Y)


def predict(model, point_count, X, start_x, step, d, showing=True):
    predicted_y = []  # сохраняем предсказанные y для графика
    predicted_x = []  # сохраняем соответствующие x
    Xf = np.array(X)  # список с набором точек по которым будет предсказываться следующая
    for _ in range(point_count):
        start_x = start_x + step  # считаем x для нашей точки
        predicted_y.append(model.forward(Xf.reshape(1, d)))  # предсказываем и запоминаем новую точку
        predicted_x.append(start_x)

        Xf = np.append(Xf[1:], predicted_y[-1])  # делаем сдвиг влево и добавляем новую предсказанную точку
    if showing:
        plt.plot(predicted_x, predicted_y, "green")
    return predicted_x, predicted_x