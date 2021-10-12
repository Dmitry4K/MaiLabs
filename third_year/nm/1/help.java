package first;

public class help {
    public static double p1(double x){
        return (-3.0f/4.0f)*x*x + (-49.0f/52.0f)*x -(11.0f/52.0f);
    }
    public static double nev(double[] x, double[] y){
        double res = 0;
        for(int i = 0;i<x.length;++i){
            res += Math.pow(p1(x[i])-y[i],2);
        }
        return res;
    }
    public static void main(String[] args){
        double[] x = {-1, 1, 2, 4};
        double[] y = {0, -2, -5, -16};
        System.out.println(nev(x,y));
    }
}
