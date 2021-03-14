#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene/timer_.h"
#include <future>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _qscene(new QGraphicsScene(-10, -10, 10, 10)),
    _scene(2000, 3.0)
{
    ui->setupUi(this);
    start_counter();

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


void MainWindow::update_scene(double dt, upd_t update_type, bool is_threading)
{
    _scene.update(dt, update_type, is_threading);
}

/// Главный цикл обновления
void MainWindow::_main_cycle()
{
    double init_time = get_counter();
    double pre_time = init_time - 1;
    double fps_time = init_time;
    size_t fps_count = 0;
    double fps_update = 500.0;

    bool is_visual = this->_is_visual();
    bool is_thread = this->_is_threading();
    upd_t alorithm = this->_get_algorithm();

    while(get_counter() - init_time < 1000*60.0)
    {
        double new_time = get_counter();
        _scene.update((new_time - pre_time)/1000, alorithm, is_thread);

//        auto as = async(std::launch::async, &Scene::update, &_scene,
//                        (new_time - pre_time)/1000, alorithm, is_thread);
//        as.get();

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
