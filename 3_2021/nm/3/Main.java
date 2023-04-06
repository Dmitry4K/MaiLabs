package third;

import second.SimpleNuton;

public class Main {
    public static void main(String[] args){

        Trapezii task1 = new Trapezii() {
            @Override
            double f(double x) { return (5*x + 2)/(x*x + 1); }
            @Override
            double getA() { return -2; }
            @Override
            double getB() { return 2; }
            @Override
            double getStep() { return 1.0f; }
        };
        task1.exec();

        Trapezii task2 = new Trapezii() {
            @Override
            double f(double x) { return (5*x + 2)/(x*x + 1); }
            @Override
            double getA() { return -2; }
            @Override
            double getB() { return 2; }
            @Override
            double getStep() { return 0.5f; }
        };
        task2.exec();

        Trapezii task3 = new Trapezii() {
            @Override
            double f(double x) { return (5*x + 2)/(x*x + 1); }
            @Override
            double getA() { return -2; }
            @Override
            double getB() { return 2; }
            @Override
            double getStep() { return 0.25f; }
        };
        task3.exec();

        Simpson task4 = new Simpson() {
            @Override
            double f(double x) { return (5*x + 2)/(x*x + 1); }
            @Override
            double getA() { return -2; }
            @Override
            double getB() { return 2; }
            @Override
            double getStep() { return 1.0f; }
        };
        task4.exec();

        Simpson task5 = new Simpson() {
            @Override
            double f(double x) { return (5*x + 2)/(x*x + 1); }
            @Override
            double getA() { return -2; }
            @Override
            double getB() { return 2;}
            @Override
            double getStep() { return 0.5f; }
        };
        task5.exec();

    }
}
