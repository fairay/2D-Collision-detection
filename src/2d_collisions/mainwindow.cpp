#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _qscene(new QGraphicsScene(-10, -10, 10, 10)),
    _scene(1000, 3.0)
{
    ui->setupUi(this);

    QSize _scene_size = ui->graphicsView->size();

    ui->graphicsView->setSceneRect(0, 0, _scene_size.width(), _scene_size.height());
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(&(*_qscene));

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
    time_t fps_time = clock();
    size_t fps_count = 0;
    double fps_update = 400.0;

    bool is_visual = this->_is_visual();
    bool is_thread = this->_is_threading();
    upd_t alorithm = this->_get_algorithm();

    while(clock() - time < 1000*60)
    {
        time_t new_time = clock();
        _scene.update((double)(new_time - pre_time)/1000, alorithm, is_thread);
        if (is_visual)
            _scene.show(_qscene);
        fps_count++;

        if (new_time - fps_time > fps_update)
        {
            ui->fps_count->setNum(static_cast<int>(fps_count / ((new_time - fps_time) / 1000.0)));

            fps_time = new_time;
            fps_count = 0;

            is_visual = this->_is_visual();
            is_thread = this->_is_threading();
            alorithm = this->_get_algorithm();
        }

        pre_time = new_time;
        QCoreApplication::processEvents();
    }
}

bool MainWindow::_is_threading()
{
    return ui->is_multithread->isChecked();
}
bool MainWindow::_is_visual()
{
    return ui->is_visual->isChecked();
}
upd_t MainWindow::_get_algorithm()
{
    return static_cast<upd_t>(ui->algorithm->currentIndex());
}

void MainWindow::on_startButton_clicked()
{
    _main_cycle();
}
