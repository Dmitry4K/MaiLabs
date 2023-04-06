package second;

public class Main {
    public static void main(String[] args) {

        //Пз2
        ///*

        PolovinnoeDelenie pd = new PolovinnoeDelenie() {
            //задаем начальные условия и условия окончания
            @Override
            double f(double x) {
                return 2*x*x*x + 2*x - 8;
            }
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
            @Override
            double getA(){
                return 1;
            }
            @Override
            double getB(){
                return 2;
            }
        };
        System.out.println("Метод половинного деления");
        pd.exec();
        System.out.println();

        SekushieHordi sh = new SekushieHordi() {
            //задаем начальные условия и условия окончания
            @Override
            double getA() {
                return 1;
            }

            @Override
            double getB() {
                return 2;
            }

            @Override
            double f(double x) {
                return x*x*x + 3*x - 5;
            }

            int iterations  =10;
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
        System.out.println("Метод секущих");
        sh.exec();
        System.out.println();
        System.out.println("Метод касательных");
        Kasatelnie kas = new Kasatelnie() {
            //задаем начальные условия и условия окончания
            @Override
            double getA() {
                return 0;
            }

            @Override
            double getB() {
                return 1;
            }

            int iterations  =10;
            @Override
            boolean endCondition() {
                if(iterations > 0){
                    --iterations;
                    return true;
                }else {
                    return false;
                }
            }

            @Override
            double f(double x) {
                return 0.5*x*x*x + 11 * x - 6.5;
            }

            @Override
            double df(double x) {
                return 1.5*x*x + 11 ;
            }

            @Override
            double ddf(double x) {
                return 3*x;
            }
        };
        kas.exec();
        System.out.println("Метод итераций");
        Iterations iter = new Iterations() {
            //задаем начальные условия и условия окончания
            @Override
            double getA() {
                return -2;
            }

            @Override
            double getB() {
                return -1;
            }

            @Override
            double f(double x) {
                return 4.0*x*x*x + 3.0*x + 22.0;
            }

            @Override
            double g(double x) {
                return Math.cbrt((-3.0*x - 22.0)/4.0);
            }

            @Override
            double dg(double x) {
                return (1.0/3.0) * Math.pow((-3.0/4.0)*x + 22.0/4.0, -2.0/3.0) * (-3.0/4.0);
            }

            int iterations  =10;
            @Override
            boolean endCondition() {
                if (iterations > 0) {
                    --iterations;
                    return true;
                } else {
                    return false;
                }
            }
        };
        iter.exec();
        System.out.println();


        System.out.println("Метод линеризации");
        SimpleNuton nuton1 = new SimpleNuton() {
            int iterrations = 5;
            @Override
            boolean isEnd() {
                if(iterrations > 0){
                    --iterrations;
                    return false;
                }
                return true;
            }

            @Override
            double f1(double x, double y) {
                return 3*x*x*x-y*y*y + 9;
            }

            @Override
            double f2(double x, double y) {
                return x*y -2*y-5;
            }

            @Override
            double df1x(double x, double y) {
                return 9*x*x;
            }

            @Override
            double df1y(double x, double y) {
                return -3*y*y;
            }

            @Override
            double df2x(double x, double y) {
                return y;
            }

            @Override
            double df2y(double x, double y) {
                return x-2;
            }

            @Override
            Point getStartPoint() {
                return new Point(3,4);
            }
        };
        nuton1.exec();
        SimpleNuton nuton2 = new SimpleNuton() {
            int iterrations = 5;
            @Override
            boolean isEnd() {
                if(iterrations > 0){
                    --iterrations;
                    return false;
                }
                return true;
            }

            @Override
            double f1(double x, double y) {
                return 3*x*x*x-y*y*y + 9;
            }

            @Override
            double f2(double x, double y) {
                return x*y -2*y-5;
            }

            @Override
            double df1x(double x, double y) {
                return 9*x*x;
            }

            @Override
            double df1y(double x, double y) {
                return -3*y*y;
            }

            @Override
            double df2x(double x, double y) {
                return y;
            }

            @Override
            double df2y(double x, double y) {
                return x-2;
            }

            @Override
            Point getStartPoint() {
                return new Point(-1,-1);
            }
        };
        nuton2.exec();
    }
}