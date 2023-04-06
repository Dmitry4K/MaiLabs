package second;
abstract public class Kasatelnie {
    abstract double getA();
    abstract double getB();
    abstract boolean endCondition();
    abstract double f(double x);
    //первая производная
    abstract double df(double x);
    //вторая производная
    abstract double ddf(double x);
    void exec() throws RuntimeException{
        double prev, cur;
        double a = getA();
        double b = getB();
        double fa = f(a);
        double ddfa = ddf(a);
        if(fa * ddfa > 0){
            prev = a;
        } else {
            prev = b;
        }
        int iteration = 1;
        System.out.println("\t\t a[n-1] \t\tf(a[n-1]) \t\t df(a[n-1]) \t ddf(a[n-1]) \t a[n]");
        while(endCondition()){
            cur = prev - (f(prev)/df(prev));
            printRes(iteration, prev, f(prev), df(prev), ddf(prev), cur);
            ++iteration;
            prev = cur;
        }
    }
    void printRes(int iteration, double prev, double fprev, double dfprev, double ddfprev, double cur){
        System.out.println("[" + iteration+"] : \t" + String.format("%.10f\t%.10f\t%.10f\t%.10f\t%.10f\t", prev,fprev,dfprev, ddfprev,cur));
    }
}
