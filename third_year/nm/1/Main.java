package first;

import java.util.Arrays;

public class Main {
    public static void main(String[] args) {

        //Метод прогонки
        int n1 = 9;
        double[][] aMatrix1 = {{0, -10,-3},
                {4, -7, 1},
                {-2, 9, 4},
                {3, 8, 1},
                {-5, 11, -4},
                {5, -9, 1},
                {-5, 11, -2},
                {5, 7, 3},
                {5, -11, 0}};

        double[] bMatrix1 = {2, 35, 43, 20, -53, 0, -5, -29, 39};
        System.out.println("Метод прогонки");
        Progonka.exec(n1, aMatrix1, bMatrix1);
        System.out.println();




        //Метод простых итераций
        int n2 = 3;
        double[][] aMatrix2 = {
                {37, 1, 10},
                {3, 36, -8},
                {-1, 6, -39}
        };
        double [] bMatrix2 = {-357, -237, 310};

        SimpleIterations taskSimpleIterations = new SimpleIterations(){
            int iterations = 10;
            @Override
            boolean endCondition() {
                if(iterations > 0){
                    --iterations;
                    return true;
                } else {
                    return false;
                }
            }
        };
        System.out.println("Метод простых итераций");
        System.out.println(Arrays.toString(taskSimpleIterations.exec(n2, aMatrix2, bMatrix2)));
        System.out.println();




        //Метод Зейделя
        Zeidel taskZeidel = new Zeidel() {
            int iterations = 5;
            @Override
            boolean endCondition() {
                if(iterations > 0){
                    --iterations;
                    return true;
                } else {
                    return false;
                }
            }
        };
        System.out.println("Метод Зейделя");
        System.out.println(Arrays.toString(taskZeidel.exec(n2, aMatrix2, bMatrix2)));
        System.out.println();




        //Степенной метод
        int n3 = 3;
        double[][] aMatrix3 = {
                {-17, 4, -4},
                {4, -14, -8},
                {-4, -8, -4}
        };
        Stepennoy taskStepennoy = new Stepennoy() {
            int iterations = 10;
            @Override
            boolean endCondition() {
                if(iterations > 0){
                    --iterations;
                    return true;
                }else {
                    return false;
                }
            }
        };
        System.out.println("Степенной метод");
        taskStepennoy.exec(n3, aMatrix3);

        double[][] Matrix4 = {
                {-17, 4, -4},
                {4, -14, -8},
                {-4, -8, -4}
        };

        //Метод Якоби
        System.out.println("Метод Якоби");
        Jakoby.exec(Matrix4);
    }
}