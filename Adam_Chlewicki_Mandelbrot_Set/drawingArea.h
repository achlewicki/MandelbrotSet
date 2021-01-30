#ifndef OKNO_H
#define OKNO_H
#include <QWidget>

class DrawingArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = nullptr);

protected:
    QImage *im;

    struct point{
      int x, y;
    };

    point mousePosition;

    int WIDTH = 900;
    int HEIGHT = 600;
    int N = 200;

    int zoomScale;
    int moveScale;
    double zoomValue;
    double moveVertically;
    double moveHorizontally;

    double avarageTime;
    int counter;

    bool zoomArea;
    bool firstMandelbrotDraw;

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void putPixel(int x, int y, int r, int g, int b);
    void mandelbrot();
    void drawZoomArea();

signals:
    double updateEsteminateTime(double time);
    double updateAvarageEsteminateTime(double time);

public slots:
    void setZoomVar(int value);
    void setMoveVar(int value);

    void moveUpEvent();
    void moveDownEvent();
    void moveLeftEvent();
    void moveRightEvent(); 

    void zoomInEvent();
    void zoomOutEvent();

    void reset();

    void changeZoomAreaState();
};

#endif // OKNO_H
