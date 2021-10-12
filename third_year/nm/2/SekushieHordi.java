package second;
abstract public class SekushieHordi {
    abstract double getA();
    abstract double getB();
    abstract double f(double x);
    void exec() throws RuntimeException{
        double a,b;
        a = getA();
        b = getB();
        int iteration =1;
        System.out.println("\t\t a \t\t\t\t c \t\t\t\t b \t\t\t\t f(a) \t\t\t\t f(c) \t\t\t\t f(b)");
        while(endCondition()){
            double fa = f(a);
            double fb = f(b);
            double c = (a*fb - b * fa)/(fb-fa);
            double fc = f(c);

            printRes(iteration, a,c,b,fa,fc,fb);
            if(fa * fc < 0) {
                b = c;
            } else if(fc*fb < 0){
                a = c;
            } else{
                throw new RuntimeException();
            }
            ++iteration;
        }
    }
    void printRes(int iteration, double a, double c, double b, double fa, double fc, double fb){
        System.out.println("[" + iteration+"] : \t" + String.format("%.10f\t%.10f\t%.10f\t%.10f\t%.10f\t%.10f\t", a,c,b,fa,fc,fb));
    }
    abstract boolean endCondition();
}
