package coursework;

import java.util.Arrays;

abstract public class Evaluation {
    abstract double a();
    abstract double b();
    abstract double T();
    abstract double alf1();
    abstract double alf2();
    abstract double alf3();
    abstract double hx();
    abstract double ht();
    abstract double f1(double t);
    abstract double f2(double t);
    abstract double f3(double x);
    abstract double phi1();
    abstract double phi2();
    abstract double phi4();
    abstract double phi5();
    abstract double f(double t, double x);
    private double k1 = ( (alf1() * ht()/(hx()*hx())) - (alf2() * ht()/(2.0*hx())));
    private double k2 = (ht()*alf3() + 1.0 - (2.0*alf1()*ht()/(hx()*hx())));
    private double k3 = ((alf1()*ht()/(hx()*hx())) + (alf2()*ht()/(2.0*hx())));
    void eval(){
        printSettings();
        System.out.println();
        int N = (int)((b()-a())/hx());
        double[] newY = new double[N+1];
        double[] oldY = new double[N+1];

        System.out.println("X:");
        //вычисляем нулевой слой
        for(int i = 0;i<=N ;++i){
            double x = a() + i*hx();
            oldY[i] = f3(x);
            printX(i,x);
        }
        System.out.println();
        System.out.println("Y:");
        printY(0, oldY);
        int k = 1;

        //вычисляем остальные слои
        for(double tk = 0; tk <T(); tk+=ht()) {
            //вычисляем k-ый слой
            for(int i = 1;i<N;++i){
                double xi = a() + i*hx();
                newY[i] = k1 * oldY[i-1] + k2 * oldY[i] + k3 * oldY[i+1] + ht() * f(tk,xi);
            }
            //вычисляем первое и последнее значение слоя
            double t = tk + ht();
            newY[0] = (2.0*hx()*f1(t) - 4.0*phi1() * newY[1] + phi1() * newY[2])/
                    (2.0*hx()*phi2() - 3.0*phi1());
            newY[newY.length-1] = (2.0*hx()*f2(t) + 4.0*phi4()*newY[newY.length-2] - phi4() * newY[newY.length-3])/
                    (2.0*hx()*phi5() + 3.0*phi4());
            printY(k, newY);
            ++k;
            oldY = newY.clone();
        }
    }
    void printX(int i, double x){
        System.out.printf("x[%d]: %.9f; ", i,x);
        System.out.println();
    }
    void printY(int i, double[] Y){
        for(int j = 0;j<Y.length;++j){
            System.out.printf("y[%d][%d]: %.9f; ", i,j,Y[j]);
        }
        System.out.println();
    }
    void printSettings(){
        System.out.println("Решение:\n");
        System.out.printf("%.9f <= x <= %.9f\n0 <= t <= %.9f\n", a(),b(), T());
        System.out.printf("Шаг по x (hx): %.9f, шаг по t (ht): %.9f\n", hx(), ht());
        System.out.printf("y[k+1][i] = (%.9f) * y[k][i-1] + (%.9f) * y[k][i] + (%.9f) * y[k][i+1] + ht * f(x[i], t[k])\n",
                k1,k2,k3);
        System.out.printf("y[k][0] = (%.9f) * f1(t[k]) + (%.9f) * y[k][1] + (%.9f) * y[k][2]\n",
                2.0*hx() / (2.0*hx()*phi2() - 3.0*phi1()), - 4.0*phi1()/(2.0*hx()*phi2() - 3.0*phi1()),
                phi1() / (2.0*hx()*phi2() - 3.0*phi1()));
        System.out.printf("y[k][N] = (%.9f) * f2(t[k]) + (%.9f) * y[k][N-1] + (%.9f) * y[k][N-2]\n",
                2.0*hx() / (2.0*hx()*phi5() + 3.0*phi4()), 4.0*phi4()/ (2.0*hx()*phi5() + 3.0*phi4()),
                - phi4()/ (2.0*hx()*phi5() + 3.0*phi4()));
    }
}
