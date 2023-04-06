package fourth;

abstract public class RungeKuttaMethodSecond {
    abstract double a();
    abstract double b();
    abstract double h();
    abstract double f(double x, double y, double z);
    abstract double ya();
    abstract double yda();
    void exec(){
        int ic = (int)((b()-a())/h())+1;
        double x[] = new double[ic];
        double y[] = new double[ic];
        double z[] = new double[ic];
        x[0] = a();
        y[0] = ya();
        z[0] = yda();
        System.out.println("Метод Рунге-Кутта для ОДУ второго порядка");
        System.out.println("Шаг: " + h());
        for(int i = 1;i<ic;++i){
           double X = x[i-1];
           double Y = y[i-1];
           double Z = z[i-1];
           double K1y = Z;
           double K1z = f(X,Y,Z);
           double K2y = Z + (h()/2)*K1z;
           double K2z = f(X + h()/2, Y+h()*K1y/2, Z + h()*K1z/2);
           double K3y = Z + h()*K2z/2;
           double K3z = f(X + h()/2, Y + h()*K2y/2, Z + h()*K2z/2);
           double K4y = Z + h() * K3z;
           double K4z = f(X + h(), Y + h() * K3y, Z + h() * K3z);

            y[i] = Y + h() / 6 * (K1y + 2*K2y + 2*K3y + K4y);
            z[i] = Z + h() / 6 * (K1z + 2*K2z + 2*K3z + K4z);
            x[i] = X + h();
            printRes(i, x[i],y[i],z[i], K1y,K1z,K2y,K2z,K3y,K3z,K4y,K4z);
        }
    }
    void printRes(int i, double x,
                  double y, double z,
                  double K1y, double K1z,
                  double K2y, double K2z,
                  double K3y, double K3z,
                  double K4y, double K4z){
        System.out.printf("i: %d, x: %.9f, y: %.9f, z: %.9f, K1y: %.9f, K1z: %.9f, K2y: %.9f, K2z: %.9f, K3y: %.9f, K3z: %.9f, K4y: %.9f, K4z: %.9f\n",
                i,x,y,z, K1y,K1z,K2y,K2z,K3y,K3z,K4y,K4z);
    }
}
