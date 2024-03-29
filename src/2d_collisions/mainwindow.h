#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include <memory>

#include "scene/scene.h"
#include "config.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update_scene(double dt, upd_t update_type, bool is_threading);

private slots:
    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<QGraphicsScene> _qscene;

    Scene _scene;

    void _main_cycle();
    bool _is_threading();
    bool _is_visual();
    upd_t _get_algorithm();
};
#endif // MAINWINDOW_H
