package fourth;

import first.Progonka;

abstract public class KonechnieRaznosti {
    abstract double K(double x);
    abstract double L(double x);
    abstract double M(double x);
    abstract double F(double x);
    abstract double R();
    abstract double S();
    abstract double W();
    abstract double V();
    abstract double T();
    abstract double Z();
    abstract double a();
    abstract double b();
    abstract double h();
    void exec(){
        int iCount = (int)((b() - a())/h()) + 1;
        double[] X = new double[iCount];
        for(int i = 0;i<iCount;++i){
            X[i] = a() + (double)i * h();
        }
        double[][] matrix = new double[iCount][3];
        double[] d = new double[iCount];
        matrix[0][0] = 0;
        matrix[0][1] = (-R()/h()) + S();
        matrix[0][2] = R()/h();
        d[0] = T();
        for(int i = 1;i<iCount-1;++i){
            double x = X[i];
            matrix[i][0] = (K(x)/(h()*h())) - L(x)/2.0/h();
            matrix[i][1] = (-2.0*K(x)/(h()*h())) + M(x);
            matrix[i][2] = (K(x)/(h()*h())) + L(x)/2.0/h();
            d[i] = F(x);
        }
        matrix[matrix.length-1][0] = V()/h();
        matrix[matrix.length-1][1] = (-V()/h()) - W();
        matrix[matrix.length-1][2] = 0;
        d[d.length-1] = -Z();
        System.out.println("Матрица: ");
        for(int i = 0;i< matrix.length;++i){
            double[] values = matrix[i];
            System.out.printf("a: %.9f, b: %.9f, c: %.9f, d: %.9f\n",
                    values[0],values[1],values[2], d[i]);
        }
        d[matrix.length-1] = -Z();
        double[] res = Progonka.exec(matrix.length, matrix, d);

        System.out.println("Результаты:");
        for(int i = 0;i<res.length;++i){
            System.out.printf("x: %.9f, y: %.9f\n", X[i], res[i]);
        }
    }
}
