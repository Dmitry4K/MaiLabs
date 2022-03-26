# Выполнил : Коростелев Д.В.
# Группа : М8О-408Б-18
# Номер по списку : 10

# install.packages('C:/gurobi951/win64/R/gurobi_9.5-1.zip', repos=NULL)
# install.packages('slam')
# install.packages('ramify')

library(Matrix)
library(gurobi)
library(ramify)

k = 10
l = 8
n = 10 + k %/% 4
m = 30 - k %/% 4

print(paste('k = ', k))
print(paste('l = ', l))
print(paste('n = ', n))
print(paste('m = ', m))

print('TASK 2')
s = 0
A = matrix(s, nrow=m, ncol=n, byrow=T)
for (i in 1:m){
  for(j in 1:n){
    A[i,j]=1+(((j+k)*i+(j*j)+(i*i*i)+ 3*(i+l))%%(30 + k %/% 5))
  }
}       

print(A)

obj = matrix(s, nrow=1, ncol=ncol(A), byrow=T)
obj = ((50 + l + k - 1) : (50 + l + k - n))
print(obj)

rhs = matrix(s, nrow=1, ncol=nrow(A), byrow=T)
rhs = ((70 + l + k + 1) : (70 +l + k + m))
print(rhs)

model = list()
model$A          = A     # задаём матрицу системы
model$obj        = obj   # задаём целевую функцию
model$modelsense = 'max' # или 'min'
model$rhs        = rhs   # вектор правой части ограничений
model$vtype      = 'C'   # действительные переменные (по умолчанию), 'I' - целочисленные, 'B' - бинарные


print('TASK 3')
result = gurobi(model)
print(paste('obj=', result$objval)) # оптимальное значение целевой функции
print(paste('x=', result$objval))   # решение задачи
print('y = ')
print(result$pi)                    # решение двойственной задачи


print('TASK 4')
resource =matrix(result$pi, nrow=1, ncol=m, byrow=T)
mx_id = argmax(resource, rows = TRUE)
print('resource = ')
print(resource)                                             # ресурс
print(paste('max increasing resource = ', resource[mx_id])) # ресурс, измнение которого позволяет максимально увеличить значение целевой функции

rhs[mx_id] = rhs[mx_id]+1

model$A          = A
model$obj        = obj
model$modelsense = 'max'
model$rhs        = rhs 
model$vtype      = 'C' 

result = gurobi(model)
print(paste('obj=', result$objval))
print(paste('x=', result$objval))
print('y = ')
print(result$pi)


print('TASK 5')

resource =matrix(result$pi, nrow=1, ncol=m, byrow=T)
mx_id = argmax(resource, rows = TRUE)

rhs[mx_id] = rhs[mx_id]-1
rhs[1] = rhs[1]+1

model$A          = A
model$obj        = obj
model$modelsense = 'max'
model$rhs        = rhs 
model$vtype = 'C'

result = gurobi(model)
print(paste('obj=', result$objval))
print(paste('x=', result$objval))
print('y = ')
print(result$pi)


print('TASK 6')

mat_y =matrix(result$pi, nrow=1, ncol=m, byrow=T)

rhs[1] = rhs[1]-1

model$A          = A 
model$obj        = obj
model$modelsense = 'max' 
model$rhs        = rhs 
model$vtype = 'I'

result = gurobi(model)
print(paste('obj=', result$objval))
print(paste('x=', result$objval))


print('TASK 7')

vtypes = c()
for (i in 1 : (n %/% 2)) {
  vtypes = c(vtypes, 'I')
}
for (i in 1 : (n %/% 2)) {
  vtypes = c(vtypes, 'C')
}
print(vtypes)

model$A          = A 
model$obj        = obj 
model$modelsense = 'max'
model$rhs        = rhs
model$vtype = vtypes 

result = gurobi(model)
print(paste('obj=', result$objval))
print(paste('x=', result$objval))

rm(list=ls())
