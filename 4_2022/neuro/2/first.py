import numpy as np

n = int(input())

matrix = np.array([0 for _ in range(n * n)]).reshape((n,n))
#matrix = np.eye(n)

with open('test.txt', 'w') as the_file:
    the_file.write(str(n)+ ' ')
    for i in range(n):
        for j in range(n):
            print(matrix[i][j], end=' ')
        print()
print()
print(np.linalg.det(matrix))