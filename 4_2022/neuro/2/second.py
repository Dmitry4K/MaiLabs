import numpy as np
import matplotlib.pyplot as plt
import math
from functions import convert_to_X_Y_for_train, mse, predict
from model import Linear


if __name__ == "__main__":
    f2, l2, r2, s2 = lambda t: np.cos(2.5 * t * t - 5.0 * t), 0.0, 2.2, 0.01
    x, d = np.arange(l2, r2, s2), 4
    y = f2(x)
    X, Y = convert_to_X_Y_for_train(y, d)
    plt.plot(x[d:], y[d:], "blue")
    linear = Linear(step=0.003, epochs=3000).train(X, Y)
    print("Веса:", linear.w)
    predicted_x, predicted_y = predict(linear, 100, Y[-d:], x[-1], s2, d)
    print(f"Величина ошибки sqrt(mse): {math.sqrt(mse(f2(np.array(predicted_x)), predicted_y))}")
    plt.show()
