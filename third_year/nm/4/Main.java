package fourth;

public class Main {
    public static void main(String[] args){
        KoshiFirst Task0 = new KoshiFirst() {
            @Override
            double getA() { return 1; }
            @Override
            double getB() { return 5; }
            @Override
            double getStep() { return 1; }
            @Override
            double getX0() { return 1; }
            @Override
            double getY0() { return -1; }
            @Override
            double f(double x, double y) { return 2*y - 3*x*x + 4; }
        };
        Task0.exec();

        KoshiFirst Task1 = new KoshiFirst() {
            @Override
            double getA() { return 1; }
            @Override
            double getB() { return 5; }
            @Override
            double getStep() { return 0.5; }
            @Override
            double getX0() { return 1; }
            @Override
            double getY0() { return -1; }
            @Override
            double f(double x, double y) { return 2*y - 3*x*x + 4; }
        };
        Task1.exec();

        KoshiFirst Task2 = new KoshiFirst() {
            @Override
            double getA() { return 1; }
            @Override
            double getB() { return 5; }
            @Override
            double getStep() {return 0.4; }
            @Override
            double getX0() { return 1; }
            @Override
            double getY0() { return -1; }
            @Override
            double f(double x, double y) { return 2*y - 3*x*x + 4; }
        };
        Task2.exec();

        RungeKutta Task3 = new RungeKutta() {
            @Override
            double getA() { return 1; }
            @Override
            double getB() { return 5; }
            @Override
            double getStep() { return 1; }
            @Override
            double getX0() { return 1; }
            @Override
            double getY0() { return -1; }
            @Override
            double f(double x, double y) { return 2*y - 3*x*x + 4; }
        };
        Task3.exec();
        RungeKutta Task4 = new RungeKutta() {
            @Override
            double getA() { return 1; }
            @Override
            double getB() { return 5; }
            @Override
            double getStep() { return 0.5; }
            @Override
            double getX0() { return 1; }
            @Override
            double getY0() {return -1;}
            @Override
            double f(double x, double y) { return 2*y - 3*x*x + 4; }
        };
        Task4.exec();
        KonechnieRaznosti task0 = new KonechnieRaznosti() {
            @Override
            double K(double x) { return 1; }
            @Override
            double L(double x) { return -1; }
            @Override
            double M(double x) { return -5; }
            @Override
            double F(double x) { return x*x + 2*x-3; }
            @Override
            double R() { return -3; }
            @Override
            double S() { return -5; }
            @Override
            double W() { return -2; }
            @Override
            double V() { return -4; }
            @Override
            double T() { return 2; }
            @Override
            double Z() { return 3; }
            @Override
            double a() { return -1; }
            @Override
            double b() { return 0; }
            @Override
            double h() { return 0.2; }
        };

        task0.exec();

        EulerMethodSecond task1 = new EulerMethodSecond() {
            @Override
            double f(double x, double y, double z) { return -2*z - 3 * y + 3*x + 3;}
            @Override
            double a() { return -1;}
            @Override
            double b() { return 1;}
            @Override
            double ya() { return 2;}
            @Override
            double yda() { return -7;}
            @Override
            double h() { return 0.5;}
        };
        task1.exec();

        EulerMethodSecond task2 = new EulerMethodSecond() {
            @Override
            double f(double x, double y, double z) {return -2*z - 3 * y + 3*x + 3; }
            @Override
            double a() { return -1;}
            @Override
            double b() { return 1;}
            @Override
            double ya() { return 2; }
            @Override
            double yda() { return -7;}
            @Override
            double h() { return 0.25;}
        };
        task2.exec();

        EulerMethodSecond task3 = new EulerMethodSecond() {
            @Override
            double f(double x, double y, double z) {return -2*z - 3 * y + 3*x + 3; }
            @Override
            double a() {return -1;}
            @Override
            double b() {return 1; }
            @Override
            double ya() { return 2;}
            @Override
            double yda() {return -7;}
            @Override
            double h() {return 0.2;}
        };
        task3.exec();

        RungeKuttaMethodSecond task4 = new RungeKuttaMethodSecond() {
            @Override
            double f(double x, double y, double z) {return -2*z - 3 * y + 3*x + 3; }
            @Override
            double a() {return -1;}
            @Override
            double b() {return 1; }
            @Override
            double ya() { return 2;}
            @Override
            double yda() {return -7;}
            @Override
            double h() {return 0.5;}
        };
        task4.exec();

        RungeKuttaMethodSecond task5 = new RungeKuttaMethodSecond() {
            @Override
            double f(double x, double y, double z) {return -2*z - 3 * y + 3*x + 3; }
            @Override
            double a() {return -1;}
            @Override
            double b() {return 1; }
            @Override
            double ya() { return 2;}
            @Override
            double yda() {return -7;}
            @Override
            double h() {return 0.25;}
        };
        task5.exec();
    }
}
