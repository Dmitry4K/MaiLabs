package second;

abstract public class Iterations {
    abstract double getA();
    abstract double getB();
    abstract double f(double x);
    abstract double g(double x);
    abstract double dg(double x);

    void exec() {
        double prev, current;
        prev = getA();
        int iteration = 1;
        System.out.println("\t\t x \t\t\t\tf(x) \t\t\t g(x) \t\t\t dg(x)");
        while(endCondition()){
            current = g(prev);
            printRes(iteration, current, f(current), g(current), dg(current));
            ++iteration;
            prev = current;
        }
    }
    void printRes(int iteration, double x, double fx, double gx, double dgx){
        System.out.println("[" + iteration+"] : \t" + String.format("%.10f\t%.10f\t%.10f\t%.10f", x, fx, gx, dgx));
    }
    abstract boolean endCondition();
}
