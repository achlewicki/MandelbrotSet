#include "drawingArea.h"
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <omp.h>
#include <complex>
#include <QElapsedTimer>
#include <QWheelEvent>

using namespace std;

DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent){
    im = new QImage(WIDTH, HEIGHT, QImage::Format_RGB888);
    im->fill(Qt::white);
}

void DrawingArea::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.drawImage(0, 0, *im);
    update();
}

void DrawingArea::setZoomVar(int value){
    zoomScale = value;
    mandelbrot();
}

void DrawingArea::setMoveVar(int value){
    moveScale = value;
}

void DrawingArea:: moveUpEvent(){
    moveVertically += moveScale * 2/3;
    mandelbrot();
}

void DrawingArea:: moveDownEvent(){
    moveVertically -= moveScale * 2/3;
    mandelbrot();
}

void DrawingArea:: moveLeftEvent(){
    moveHorizontally -= moveScale;
    mandelbrot();
}

void DrawingArea:: moveRightEvent(){
    moveHorizontally += moveScale;
    mandelbrot();
}

void DrawingArea:: zoomInEvent(){
    zoomValue *= zoomScale;
    moveHorizontally *= zoomScale;
    moveVertically *= zoomScale;
    mandelbrot();
}

void DrawingArea:: zoomOutEvent(){
    zoomValue /= zoomScale;
    moveHorizontally /= zoomScale;
    moveVertically /= zoomScale;
    mandelbrot();
}

void DrawingArea::reset(){
    zoomScale = 2;
    moveScale = 1;
    zoomValue = 1.0;
    moveVertically = 0.0;
    moveHorizontally = 540.0;
    counter = 0;
    avarageTime = 0;
    zoomArea = false;
    firstMandelbrotDraw = true;
    mandelbrot();
}

void DrawingArea::putPixel(int x, int y, int r, int g, int b){
    unsigned char *p = im->scanLine(y);

    if(x < 0 || y <0 || y >= HEIGHT || x >= WIDTH) return;

    p[3*x] = static_cast<unsigned char>(r);
    p[3*x + 1] = static_cast<unsigned char>(g);
    p[3*x + 2] = static_cast<unsigned char>(b);
}


void DrawingArea::mandelbrot(){
    QElapsedTimer timer;
    timer.start();
    firstMandelbrotDraw = false;
//    omp_set_num_threads(2);
    omp_set_num_threads(omp_get_max_threads());


#pragma omp parallel for schedule(dynamic, 1)
    for(int x = 0; x < WIDTH; x++){
            for(int y = 0; y < HEIGHT; y++){
                complex<double> p;

///             os X w przedziale <-2.4; 1.2> - bez przyblizen i przesuniec
                p.real(((3.6/(double(WIDTH)))*x - 1.8 - moveHorizontally/(double(WIDTH)))/zoomValue);

///             os Y w przedziale <-1.2; 1.2> - bez przyblizen i przesuniec
                p.imag(((2.4/(double(HEIGHT)))*y - 1.2 + moveVertically/(double(HEIGHT)))/zoomValue);

                complex<double> z;
                z.real(0);
                z.imag(0);

                int colorValue = 0;

                for(int n = 0; n < N; n++){
                    colorValue++;
                    complex<double> z1;
                    z1 = z * z + p;
                    if(abs(z1) >= 2.0){
                            break;
                    }
                    z = z1;
                }

///             BLACK
                if(colorValue == N)
                   putPixel(x, y, 0, 0, 0);
///             GREEN
                else if (colorValue > 38) {
                   putPixel(x, y, 50 - colorValue/4, 255 - colorValue,100  - colorValue/2);
                }
///             YELLOW
                else if (colorValue > 28) {
                   putPixel(x, y, 255, 255, 250 - colorValue*2);
                }
///             ORANGE
                else if (colorValue > 10) {
                   putPixel(x, y, 255, 240 - colorValue*3, 210 - colorValue*4);
                }
///             RED
                else if (colorValue > 0){
                   putPixel(x, y, 255, 130 + colorValue*7, 130 + colorValue*2);
                }
            }

    }

    if(zoomArea) drawZoomArea();
    updateEsteminateTime(timer.elapsed());
    avarageTime += timer.elapsed();
    counter++;
    updateAvarageEsteminateTime(avarageTime/counter);
    update();
}

void DrawingArea::drawZoomArea(){
    int x_start = WIDTH/2 - WIDTH/2/zoomScale;
    int x_end = WIDTH/2 + WIDTH/2/zoomScale;
    int y_start = HEIGHT/2 - HEIGHT/2/zoomScale;
    int y_end = HEIGHT/2 + HEIGHT/2/zoomScale;
/// Left vertical
    for(int x =  x_start - 2; x <= x_start; x++){
        for(int y = y_start; y < y_end; y++){
            putPixel(x, y, 255, 0, 0);
        }
    }
/// Upper horizontal
    for(int x = x_start - 2; x <= x_end; x++){
        for(int y = y_start - 2; y <= y_start; y++){
            putPixel(x, y, 255, 0, 0);
        }
    }
/// Right vertical
    for(int x = x_end - 2; x <= x_end; x++){
        for(int y = y_start; y < y_end; y++){
            putPixel(x, y, 255, 0, 0);
        }
    }
/// Down horizontal
    for(int x = x_start - 2; x <= x_end; x++){
        for(int y = y_end - 2; y <= y_end; y++){
            putPixel(x, y, 255, 0, 0);
        }
    }
}

void DrawingArea::changeZoomAreaState(){
    zoomArea = !zoomArea;
    if(!firstMandelbrotDraw) mandelbrot();
}

void DrawingArea::mousePressEvent(QMouseEvent *event){
    mousePosition.x = event->x();
    mousePosition.y = event->y();
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event){
    moveVertically += (mousePosition.y - event->y());
    moveHorizontally -= (mousePosition.x - event->x());
    mousePosition.x = event->x();
    mousePosition.y = event->y();
    mandelbrot();
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event){
    moveVertically += (mousePosition.y - event->y());
    moveHorizontally -= (mousePosition.x - event->x());
    mandelbrot();
}
