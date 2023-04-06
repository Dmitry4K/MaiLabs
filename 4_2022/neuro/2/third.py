import numpy as np
import matplotlib.pyplot as plt
import math
from functions import convert_to_X_Y_for_train, mse
from model import Linear

if __name__ == "__main__":
    f2, l2, r2, s2 = lambda t: np.cos(2.5 * t * t - 5.0 * t), 0.0, 2.2, 0.01
    fy = lambda t: 0.25 * np.cos(2.5 * t * t - 5.0 * t + math.pi)
    x = np.arange(l2, r2, s2)
    y = f2(x)
    ys = fy(x)  # считаем y по третьей функции
    d = 4
    X, _ = convert_to_X_Y_for_train(y, d)
    _, Y = convert_to_X_Y_for_train(ys, d)
    # важный момент - берем входные значения из второй функции и выходные из третьей
    linear = Linear(step=0.0001, epochs=2000).train(X, Y)
    print(linear.w)

    result = linear.forward(X)  # прдесказываем функцию
    plt.plot(x[d:], result, color="green")
    plt.plot(x[d:], ys[d:], color="yellow")
    plt.show()
    # не смотря на то, что третья функция отличается амплитудой и сдвигом - наша нейронная сеть предстаказывает третью функцию на обучающей выборке
    print(f"Величина ошибки sqrt(mse): {math.sqrt(mse(result, Y))}")

