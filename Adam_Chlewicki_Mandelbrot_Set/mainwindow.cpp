#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    connect(ui->upButton,SIGNAL(clicked()),ui->drawingArea,SLOT(moveDownEvent()));
    connect(ui->downButton,SIGNAL(clicked()),ui->drawingArea,SLOT(moveUpEvent()));
    connect(ui->leftButton,SIGNAL(clicked()),ui->drawingArea,SLOT(moveRightEvent()));
    connect(ui->rightButton,SIGNAL(clicked()),ui->drawingArea,SLOT(moveLeftEvent()));

    connect(ui->zoomInButton,SIGNAL(clicked()),ui->drawingArea,SLOT(zoomInEvent()));
    connect(ui->zoomOutButton,SIGNAL(clicked()),ui->drawingArea,SLOT(zoomOutEvent()));

//    connect(ui->generate,SIGNAL(clicked()),ui->okno,SLOT(reset()));

    connect(ui->moveScaleSpinBox,SIGNAL(valueChanged(int)),ui->drawingArea,SLOT(setMoveVar(int)));
    connect(ui->zoomScaleSpinBox,SIGNAL(valueChanged(int)),ui->drawingArea,SLOT(setZoomVar(int)));

    connect(ui->drawingArea, SIGNAL(updateEsteminateTime(double)), ui->lcdNumber, SLOT(display(double)));
    connect(ui->drawingArea, SIGNAL(updateAvarageEsteminateTime(double)), ui->lcdNumber_2, SLOT(display(double)));

    connect(ui->checkBox, SIGNAL(stateChanged(int)), ui->drawingArea, SLOT(changeZoomAreaState()));

}

void MainWindow::on_generate_clicked(){
    ui->moveScaleSpinBox->setValue(1);
    ui->zoomScaleSpinBox->setValue(2);
    ui->checkBox->setChecked(false);
    ui->drawingArea->reset();
}

MainWindow::~MainWindow(){
    delete ui;
}
