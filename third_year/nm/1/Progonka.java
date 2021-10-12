package first;

public class Progonka {
    public static int n;
    public static double[][] matrix;
    public static double[] d;
    public static double[] x;
    public static double[] p, q;
    public static double getA(int index){
        return matrix[index-1][0];
    }
    public static double getB(int index){
        return matrix[index-1][1];
    }
    public static double getC(int index){
        return matrix[index-1][2];
    }
    public static double getD(int index){
        return d[index-1];
    }
    public static double[] exec(int mN, double[][] mAMatrix, double[] mBMatrix){
        n = mN;
        matrix = mAMatrix;
        d = mBMatrix;
        p = new double[n+1];
        q = new double[n+1];
        x = new double[n];
        p[0] = q[0] = 0;
        System.out.println("P[i]\t\tQ[i]");
        printRes(p[0],q[0]);
        for(int i = 1;i<n+1;++i){
            p[i] = (-getC(i))/(getB(i) + getA(i)*p[i-1]);
            q[i] = (getD(i) - (getA(i)*q[i-1]))/(getB(i) + getA(i)*p[i-1]);
            printRes(p[i],q[i]);
        }
        x[n-1] = q[n];
        for(int i = n-2;i>=0;--i){
            x[i] = q[i+1] + p[i+1]*x[i+1];
        }
        for(int i = 0;i<n;++i){
            System.out.println("x[" + i+ "] : " + x[i]);
        }
        return x;
    }
    static void printRes(double p, double q){
        System.out.printf("%2.9f %2.9f\n", p,q);
    }
}
