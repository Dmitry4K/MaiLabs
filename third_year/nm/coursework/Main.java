package coursework;

public class Main {
    public static void main(String[] args){
        Evaluation task = new Evaluation() {
            @Override
            double a() { return -5.0;}
            @Override
            double b() { return -1; }
            @Override
            double T() { return 0.1; }
            @Override
            double alf1() {return 5.7;}
            @Override
            double alf2() {return -3.3;}
            @Override
            double alf3() {return -1.8;}
            @Override
            double hx() {return 0.8;}
            @Override
            double ht() {return 0.025;}
            @Override
            double f1(double t) {return (26.0 + (t/2.0));}
            @Override
            double f2(double t) {return (13.0 + t*t/(-2.0) + t);}
            @Override
            double f3(double x) {return (-3.0 + 10.0*Math.sin(1.57079633 * x));}
            @Override
            double phi1() {return 0;}
            @Override
            double phi2() {return -2.0; }
            @Override
            double phi4() { return 0;}
            @Override
            double phi5() { return -1.0;}
            @Override
            double f(double t, double x) {return (x+1.0)/(t + 1.0);}
        };
        task.eval();

        Evaluation task1 = new Evaluation() {
            @Override
            double a() {
                return 0;
            }

            @Override
            double b() {
                return Math.PI;
            }

            @Override
            double T() {
                return 2;
            }

            @Override
            double alf1() {
                return 1.0;
            }

            @Override
            double alf2() {
                return 0;
            }

            @Override
            double alf3() {
                return 0;
            }

            @Override
            double hx() {
                return Math.PI/10;
            }

            @Override
            double ht() {
                return 0.1;
            }

            @Override
            double f1(double t) {
                return 0;
            }

            @Override
            double f2(double t) {
                return 0;
            }

            @Override
            double f3(double x) {
                return Math.sin(x);
            }

            @Override
            double phi1() {
                return 0;
            }

            @Override
            double phi2() {
                return 1;
            }

            @Override
            double phi4() {
                return 0;
            }

            @Override
            double phi5() {
                return 1;
            }

            @Override
            double f(double t, double x) {
                return 0;
            }
        };
        //task1.eval();
    }
}
