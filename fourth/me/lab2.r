# Выполнил : Коростелев Д.В.
# Группа : М8О-408Б-18
# Номер по списку : 10

library(Matrix)
library(gurobi)

l = 8
k = 10
n = 10 + k%/%4
m = 30 - k%/%4

print(paste('k = ', k))
print(paste('l = ', l))
print(paste('n = ', n))
print(paste('m = ', m))


A <- c()
for (i in 1:m)
  for ( j in 1:n){
    A = append(A, -k + ( 1451*i + 1571*j + 2081*k + 2543*l  ) %% (30 + k%/%5) )
  }

A = matrix(A, ncol=n, byrow=T)

find_winnig <- function(matrixA, n, m){
  print("A:")
  print(matrixA)
  print("")
  
  b = matrix(1, 1, m)
  
  c = matrix(1, 1, n)
  
  min_max = max(matrixA[,1])
  min_j = 1
  for (j in 2:n){
    if (max(matrixA[,j]) < min_max){
      min_j = j
      min_max = max(matrixA[,j])
    }
  }
  
  max_min = min(matrixA[1,])
  max_i = 1
  for (i in 2:m){
    if (max_min < min(matrixA[i,])){
      max_i = i
      max_min = min(matrixA[i,])
    }
  }
  
  
  print(paste("Winning for first = ", min_max))
  print(paste("Winning for second = ", -max_min))
  print(paste("Strategy for first = ", max_i))
  print(paste("Strategy for second = ",min_j))
  print(paste("Clear strategy (exists - true, else - false) = ", min_max == max_min))
  
  model=list()
  model$A = t(matrixA) - min(matrixA) + 1
  model$obj = b
  model$modelsense = 'min'
  model$rhs = c
  model$sense = matrix('>=', 1, n)
  model$vtype = 'C'
  
  params = list()
  params$outputflag = 0
  
  result = gurobi(model, params)
  print('x=')
  print(result$x)
  print('y=')
  print(result$pi)
  price = ((t(result$x)%*%matrixA)%*%result$pi)
  print(paste("Price", price))
  print('')
  print('')
}

find_winnig(A, n, m)
find_winnig(-A, n, m)
find_winnig(t(A)%*%A, n, n)

rm(list=ls())
