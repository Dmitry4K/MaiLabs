package fourth;

abstract public class KoshiFirst {
    abstract double getA();
    abstract double getB();
    abstract double getStep();
    abstract double getX0();
    abstract double getY0();
    private double[][] table;
    double getX(int i){
        return table[i][0];
    }
    double getY(int i){
        return table[i][1];
    }
    abstract double f(double x, double y);
    void exec(){
        int intervalCount = (int)((getB() - getA())/getStep());
        ++intervalCount;
        table = new double[intervalCount][2];
        table[0][0] = getX0();
        table[0][1] = getY0();
        printStep();
        for(int i = 1;i<intervalCount;++i){
            table[i][0] = getX(i-1) + getStep();
            table[i][1] = getY(i-1)+ getStep()*f(getX(i-1), getY(i-1));
        }
        printTable();
        System.out.println();
    }
    void printStep(){
        System.out.println("Шаг: " + getStep());
    }
    void printTable(){
        for(int i = 0;i<table.length;++i){
            System.out.printf("i: %d, x: %.8f, y: %.8f\n", i, getX(i), getY(i));
        }
    }
}
