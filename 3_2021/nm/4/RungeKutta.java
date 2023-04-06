package fourth;

abstract public class RungeKutta {
    abstract double getA();
    abstract double getB();
    abstract double getStep();
    abstract double getX0();
    abstract double getY0();
    private double[][] table;
    private double[][] kTable;
    double getX(int i){
        return table[i][0];
    }
    double getY(int i){
        return table[i][1];
    }
    double getK(int i, int n){
        return kTable[i][n];
    }
    abstract double f(double x, double y);
    void exec(){
        int intervalCount = (int)((getB() - getA())/getStep());
        ++intervalCount;
        table = new double[intervalCount][2];
        kTable = new double[intervalCount-1][4];
        table[0][0] = getX0();
        table[0][1] = getY0();
        printStep();
        for(int i = 1;i<intervalCount;++i){
            kTable[i-1][0] = f(getX(i-1), getY(i-1));
            kTable[i-1][1] = f(getX(i-1) + getStep()/2, getY(i-1) + 0.5*getStep()*getK(i-1, 0));
            kTable[i-1][2] = f(getX(i-1) + getStep()/2, getY(i-1) + 0.5*getStep()*getK(i-1, 1));
            kTable[i-1][3] = f(getX(i-1) + getStep(),getY(i-1) + getStep()*getK(i-1, 2) );
            table[i][0] = getX(i-1)+getStep();
            table[i][1] = getY(i-1) + (getStep()/6)*(getK(i-1, 0)+2*getK(i-1,1) + 2*getK(i-1,2) + getK(i-1,3));
        }
        printTable();
        System.out.println();
    }
    void printStep(){
        System.out.println("Шаг: " + getStep());
    }
    void printTable(){
        for(int i = 0;i<table.length-1;++i){
            System.out.printf("i: %d, x: %.8f, y: %.8f, k1: %.9f, k2: %.9f, k3: %.9f, k4: %.9f\n", i, getX(i), getY(i), getK(i,0), getK(i,1), getK(i,2), getK(i,3));
        }
        System.out.printf("i: %d, x: %.8f, y: %.8f\n",table.length-1, getX(table.length-1), getY(table.length-1));
    }
}
