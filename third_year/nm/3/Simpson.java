package third;

public abstract class Simpson {
    abstract double f(double x);
    abstract double getA();
    abstract double getB();
    abstract double getStep();
    void exec(){
        int intervalCount = -1 + (int)((getB() -getA())/getStep());
        //создаем таблицу с x и y
        double[][] table = new double[intervalCount][2];
        //считаем промежуточные значения функции на интервале с заданным шагом
        double currentX = getA()+getStep();
        for(int i = 0;i<intervalCount;++i){
            table[i][0] = currentX;
            table[i][1] = f(currentX);
            currentX += getStep();
        }
        //выводим шаг и таблицу
        printSettings();
        printTable(table);
        //находим ответ по формуле
        double sum1 = 0;
        for(int i = 0 ;i<table.length;i+=2){
            sum1 += table[i][1];
        }
        double sum2 = 0;
        for(int i = 1 ;i<table.length;i+=2){
            sum2 += table[i][1];
        }
        double res = (1.0f/3.0f)*getStep() * (f(getA()) + 4 * sum1 + 2 * sum2 + f(getB()));
        printRes(intervalCount+1, res);
        System.out.println();
    }
    void printSettings(){
        System.out.println("Шаг: " + getStep());
    }
    void printTable(double[][] table){
        System.out.printf("i: %d, x: %.9f, y: %.9f\n",0 , getA(), f(getA()));
        for(int i = 0;i<table.length;++i){
            System.out.printf("i: %d, x: %.9f, y: %.9f\n",i+1, table[i][0], table[i][1]);
        }
        System.out.printf("i: %d, x: %.9f, y: %.9f\n",table.length+1 , getB(), f(getB()));
    }
    void printRes(int n, double res){
        System.out.print("T_"+n + ": " + res);
        System.out.println();
    }
}
