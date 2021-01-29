#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _qscene(new QGraphicsScene(-10, -10, 10, 10)),
    _scene(100, 7.0)
{
    ui->setupUi(this);

    QSize _scene_size = ui->graphicsView->size();

    ui->graphicsView->setSceneRect(0, 0, _scene_size.width(), _scene_size.height());
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(&(*_qscene));

    _qscene->addLine(0, 0, 100, 200);
    _scene.show(_qscene);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_startButton_clicked()
{

}
