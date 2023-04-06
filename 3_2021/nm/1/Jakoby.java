package first;

public class Jakoby {
    private static double[][] multiplication(double[][] a, double[][] b){
        int n = a.length;
        double[][] res = new double[n][n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    res[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return res;
    }

    private static double[][] unitMatrix(){
        double[][] matrix = new double[3][3];
        for(int i = 0;i<3;++i) {
            for (int j = 0; j < 3; ++j)
                matrix[i][j] = 0;
            matrix[i][i] = 1;
        }
        return matrix;
    }
    private static void normalize(double[][]A){
         A[1][0]/=A[0][0];
         A[2][0]/=A[0][0];
         A[0][0]=1;
         A[0][1]/=A[1][1];
         A[2][1]/=A[1][1];
         A[1][1]=1;
         A[0][2]/=A[2][2];
         A[1][2]/=A[2][2];
         A[2][2]=1;
    }
    private static  double[][] getRotationMatrix(double phi, Element e){
        double[][]matrix = unitMatrix();
        int k = e.k, m = e.m;
        matrix[k][m] = -Math.sin(phi);
        matrix[m][k] = -matrix[k][m];
        matrix[k][k] = matrix[m][m] = Math.cos(phi);
        return matrix;
    }
    static class Element{
        double value;
        int k,m;
    }
    static double[][] copyMatrix(double[][] A){
        double[][] matrix = new double[3][3];
        for(int i = 0;i<3;++i){
            System.arraycopy(A[i], 0, matrix[i], 0, 3);
        }
        return matrix;
    }
    static void transpose(double[][] m){
        for(int i = 0;i<3;++i){
            for(int j = i;j<3;++j){
                double x = m[i][j];
                m[i][j] = m[j][i];
                m[j][i] = x;
            }
        }
    }
    static Element findMaxElementUpperMainDiagonal(double[][] A){
        int n  = A.length;
        Element max = new Element();
        max.value = A[0][1];
        max.k = 0;
        max.m = 1;
        for(int i = 0 ; i < n;++i){
            for(int j = i+1;j<n;++j){
                if(Math.abs(max.value) < Math.abs(A[i][j])){
                    max.value = A[i][j];
                    max.k = i;
                    max.m = j;
                }
            }
        }
        return max;
    }
    public static  void exec(double[][] A){
        double[][] V = unitMatrix();
        for(int i = 0;i<7;++i){
            Element maxEl = findMaxElementUpperMainDiagonal(A);
            double phi;
            int k = maxEl.k, m = maxEl.m;
            if(A[k][k]-A[m][m] == 0){
                phi = A[k][m] > 0 ? Math.PI/4 : -Math.PI/4;
            } else{
                phi = 0.5 * Math.atan((2.0f*A[k][m])/(A[k][k]-A[m][m]));
            }
            System.out.println("Index: " + i);
            System.out.println("Angle (phi): " + phi);
            System.out.println();
            double[][] H = getRotationMatrix(phi, maxEl);
            System.out.println("H matrix: ");
            printMatrix(H);
            double[][] transposeH = copyMatrix(H);
            System.out.println("H^(-1) matrix: ");
            printMatrix(transposeH);
            transpose(transposeH);
            A = multiplication(multiplication(transposeH,A),H);
            System.out.printf("A%d matrix:\n", i+1);
            printMatrix(A);
            V = multiplication(V, H);
            System.out.println('\n');
        }
        System.out.println("V matrix:");
        printMatrix(V);
        printEigenValues(A[0][0], A[1][1], A[2][2]);
        normalize(V);
        System.out.println();
        printEigenVectors(V);

    }
    static void printEigenValues(double a, double b, double c){
        System.out.printf("lb1: %f\t lb2: %f\tlb3: %f\n", a,b,c);
    }
    static void printEigenVectors(double[][] A){
        System.out.printf("lb1: %f %f %f\n", A[0][0],A[1][0],A[2][0]);
        System.out.printf("lb2: %f %f %f\n", A[0][1],A[1][1],A[2][1]);
        System.out.printf("lb3: %f %f %f\n", A[0][2],A[1][2],A[2][2]);
    }
    static void printMatrix(double[][] matrix){
        for(int i = 0;i<3;++i){
            for(int j = 0;j<3;++j){
                System.out.printf("%.9f ", matrix[i][j]);
            }
            System.out.println();
        }
        System.out.println();
    }
}
