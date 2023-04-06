package first;

abstract public class Zeidel {
    int mN;
    double[] exec(int N, double[][] A, double[] B){
        mN = N;
        double[][] mA = A.clone();
        for(int i = 0;i<A.length;++i){
            mA[i] = A[i].clone();
        }
        double[] mB = B.clone();
        double[] res = new double[mN];
        double[] prev = new double[mN];
        for(int i = 0;i<mN;++i){
            double t = mA[i][i];
            mA[i][i] = 0;
            for(int j = 0;j<mN;++j){
                mA[i][j] /= (-1*t);
            }
            mB[i] /= t;
        }
        for(int i = 0;i<mN;++i){
            prev[i] = mB[i];
        }
        double normaFirst = 0;
        for(int i = 0;i<mN;++i){
            double sum = 0;
            for(int j = 0;j<mN;++j){
                sum += Math.abs(mA[i][j]);
            }
            normaFirst = Math.max(normaFirst, sum);
        }
        double normaSecond = 0;
        for(int i = 0;i<mN;++i){
            double sum = 0;
            for(int j = 0;j<mN;++j){
                sum += Math.abs(mA[j][i]);
            }
            normaSecond = Math.max(normaSecond, sum);
        }
        double normaB = Math.min(normaFirst, normaSecond);
        double normaBetta = 0;
        for(int i = 0;i<mN;++i){
            normaBetta = Math.max(normaBetta, Math.abs(mB[i]));
        }
        printRes(0, prev, -1, -1);
        int iteration = 1;
            while(endCondition()){
            for(int i = 0;i<mN;++i){
                double sum = 0;
                for(int j = 0;j<i;++j){
                    sum += res[j] * mA[i][j];
                }
                for(int j = i;j<mN;++j){
                    sum += prev[j] * mA[i][j];
                }
                sum += mB[i];
                res[i] = sum;
            }
            double eps = Math.pow(normaB, iteration)*normaBetta/(1.0 - normaB);
            double omega = calcOmega(res, prev);
            printRes(iteration, res, eps, omega);
            prev = res.clone();
            ++iteration;
        }
        return res;
}
    private double calcOmega(double[] current, double[] prev){
        double res = 0;
        for(int i = 0;i<mN;++i){
            res = Math.max(res, Math.abs(current[i] - prev[i]));
        }
        return res;
    }
    private void printRes(int iteration, double[] res, double eps, double omega){
        System.out.print(iteration + ":");
        for (double re : res) {
            System.out.print("\t" + String.format("%.10f", re));
        }
        if(eps >= 0 || omega >=0){
            System.out.println("\t" + String.format("%.10f",eps) + "\t" + String.format("%.10f",omega));
        } else {
            System.out.println();
        }
    }
    abstract boolean endCondition();
}
