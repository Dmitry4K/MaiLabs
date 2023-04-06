import numpy as np


class Linear:
    def __init__(self, epochs=50, step=0.0001):
        self.epochs = epochs
        self.step = step
        self.w = None
        self.X = None
        self.Y = None

    def config(self):
        self.w = np.random.randn(len(self.X[0]))  # конфигурируем слой рандомными нейронами

    def forward(self, X, add_one=True):
        copied_x = X.copy()
        # если надо - добавляем единичку справа, ко всем входным параметрам
        modified_x = np.c_[copied_x, np.ones(copied_x.shape[0])] if add_one else copied_x
        return np.dot(modified_x, self.w)  # скалярное производение

    def train(self, X: list, Y: list):
        copied_x = X.copy()
        copied_y = Y.copy()
        self.X = np.c_[copied_x, np.ones(copied_x.shape[0])]
        self.Y = copied_y
        self.config()
        for epoch in range(self.epochs):
            R = self.forward(self.X, add_one=False)
            E = np.array(self.Y) - R
            for e, x in zip(E, self.X):
                self.w += (self.step * e) * x
        return self
