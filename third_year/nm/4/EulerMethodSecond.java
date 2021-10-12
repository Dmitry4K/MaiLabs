package fourth;

abstract class EulerMethodSecond {
    abstract double f(double x, double y, double z);
    abstract double a();
    abstract double b();
    abstract double ya();
    abstract double yda();
    abstract double h();
    void exec(){
        int ic = (int)((b() - a())/h());
        ++ic;
        double[] x = new double[ic];
        double[] y = new double[ic];
        double[] z = new double[ic];

        x[0] = a();
        y[0] = ya();
        z[0] = yda();

        for(int i = 1;i<ic;++i){
            x[i] = x[i-1] + h();
            y[i] = y[i-1] + h()*z[i-1];
            z[i] = z[i-1] + h()*f(x[i-1],y[i-1], z[i-1]);
        }
        printRes(ic, x,y,z);
    }
    void printRes(int ic, double[] x, double[]y, double[] z){
        System.out.println("Метод Эейлера для решения ОДУ второго порядка");
        System.out.println("Шаг: "+h());
        for(int i =0 ;i<ic;++i){
            System.out.printf("i: %d, x: %.9f, y: %.9f, z: %.9f\n", i, x[i],y[i],z[i]);
        }
    }
}
