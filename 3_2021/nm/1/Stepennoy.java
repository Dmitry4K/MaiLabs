package first;

abstract public class Stepennoy {
    private double[] multMatrixOnVector(double[][] A, double[] v) {
        int n = A.length;
        double[] res = new double[n];
        for(int i = 0;i<n;++i){
            double sum = 0;

            for(int j = 0;j<n;++j){
                sum += A[i][j] * v[j];
            }
            res[i] = sum;
        }
        return res;
    }
    private double len(double[] v){
        double sum = 0;
        for (double value : v) {
            sum += value * value;
        }
        return Math.sqrt(sum);
    }
    private double[] normalize(double[] v){
        double[] res = v.clone();
        double l = len(v);
        for(int i = 0;i<res.length;++i){
            res[i] /= l;
        }
        return res;
    }
    private double scalar(double[] a, double[] b){
        int n = a.length;
        double res = 0;
        for(int i = 0;i<n;++i){
            res += (a[i] * b[i]);
        }
        return res;
    }
    public void exec(int n, double[][] A){
        double[] w = new double[n];
        for(int i = 0;i<n;++i){
            w[i] = 1/Math.sqrt(n);
        }
        double ro;
        int iteration = 1;
        while(endCondition()){
            double[] v = multMatrixOnVector(A, w);
            ro = scalar(v,w);
            w = normalize(v);
            printRes(iteration, w,v, ro);
            ++iteration;
        }
    }
    void printRes(int iteration, double[] w, double[] v, double ro){
        System.out.print("["+iteration+"]: \tro : "+ ro + '\n');
        System.out.print("v : ");
        for (double item : v) {
            System.out.printf("%.9f  ", item);
        }
        System.out.println();
        System.out.print("w : ");
        for (double value : w) {
            System.out.printf("%.9f  ", value);
        }
        System.out.println();
        System.out.println();
    }
    abstract boolean endCondition();
}
