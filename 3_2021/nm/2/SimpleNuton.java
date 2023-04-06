package second;


abstract public class SimpleNuton {
    abstract boolean isEnd();

    abstract double f1(double x, double y);

    abstract double f2(double x, double y);
    //производная f1 по x
    abstract double df1x(double x, double y);
    //производная f1 по y
    abstract double df1y(double x, double y);
    //производная f2 по x
    abstract double df2x(double x, double y);
    //производная f2 по y
    abstract double df2y(double x, double y);

    abstract Point getStartPoint();

    private Point function(Point p) {
        Point result = new Point();
        result.x = f1(p.x, p.y);
        result.y = f2(p.x, p.y);
        return result;
    }

    private double[][] getJakobyMatrix(Point p) {
        double[][] res = new double[2][2];
        res[0][0] = df1x(p.x, p.y);
        res[0][1] = df1y(p.x, p.y);
        res[1][0] = df2x(p.x, p.y);
        res[1][1] = df2y(p.x, p.y);
        return res;
    }

    private double[][] getReverse(double[][] matrix) {
        double det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
        double[][] reverse = new double[2][2];
        reverse[0][0] = matrix[1][1] / det;
        reverse[0][1] = -matrix[0][1] / det;
        reverse[1][0] = -matrix[1][0] / det;
        reverse[1][1] = matrix[0][0] / det;
        return reverse;
    }

    private Point mult(double[][] m, Point p) {
        Point res = new Point();
        res.x = m[0][0] * p.x + m[0][1] * p.y;
        res.y = m[1][0] * p.x + m[1][1] * p.y;
        return res;
    }

    public void exec() {
        Point currentPoint = getStartPoint();
        System.out.printf("(%.9f, %.9f)\n", currentPoint.x, currentPoint.y);
        printRes(0, currentPoint, f1(currentPoint.x, currentPoint.y), f2(currentPoint.x, currentPoint.y));
        int iter = 1;
        while (!isEnd()) {
            double[][] reverseJakoby = getReverse(getJakobyMatrix(currentPoint));
            Point nextPoint = new Point();
            Point funcPoint = function(currentPoint);
            Point multPoint = mult(reverseJakoby, funcPoint);
            nextPoint.x = currentPoint.x - multPoint.x;
            nextPoint.y = currentPoint.y - multPoint.y;
            currentPoint = nextPoint;
            printRes(iter, currentPoint, f1(currentPoint.x, currentPoint.y), f2(currentPoint.x, currentPoint.y));
            ++iter;
        }
    }

    private void printRes(int iter, Point cur, double f1p, double f2p) {
        System.out.printf("%d : x: %.9f\ty: %.9f\tf1: %.9f\tf2: %.9f\n",iter,  cur.x, cur.y, f1p, f2p);
    }
}

class Point{
    Point(){}
    Point(double X, double Y){
        x = X;
        y = Y;
    }
    double x, y;
}
