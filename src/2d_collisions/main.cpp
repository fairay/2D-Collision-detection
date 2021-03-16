#include "mainwindow.h"
#include "scene/timer_.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    Scene _scene(2000, 3.0);

    start_counter();
    double init_time = get_counter();
    double pre_time = init_time - 1;
    double fps_time = init_time;
    size_t fps_count = 0;
    double fps_update = 500.0;

    bool is_thread = true;
    upd_t alorithm = BIN_TREE;

    while (get_counter() - init_time < 1000*5.0)
    {
        double new_time = get_counter();
        _scene.update((new_time - pre_time)/1000, alorithm, is_thread);

//        auto as = async(std::launch::async, &Scene::update, &_scene,
//                        (new_time - pre_time)/1000, alorithm, is_thread);
//        as.get();

        fps_count++;

        if (new_time - fps_time > fps_update)
        {
            printf("%d\n", static_cast<int>(fps_count / ((new_time - fps_time) / 1000.0)));

            fps_time = new_time;
            fps_count = 0;
        }

        pre_time = new_time;
    }

//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
    return 0;
}

