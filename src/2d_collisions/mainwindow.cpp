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


/// Главный цикл обновления
void MainWindow::_main_cycle()
{
    time_t time = clock();
    time_t pre_time = time - 1;
    while(clock() - time < 1000*30)
    {
        time_t new_time = clock();
        _scene.update((double)(new_time - pre_time)/1000, BRUTE_FORCE);
        _scene.show(_qscene);
        // ui->fps_count->setNum(static_cast<int>(1000.0 / (new_time - pre_time)));
        pre_time = new_time;
        QCoreApplication::processEvents();
    }
}

void MainWindow::on_startButton_clicked()
{
    _main_cycle();
}
