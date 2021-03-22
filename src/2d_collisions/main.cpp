#include "mainwindow.h"
#include "scene/timer_.h"

#include <QApplication>
#include <iostream>

void performance_test(int n, upd_t alorithm, bool is_thread = true)
{
    Scene _scene(n, BALL_R);

    start_counter();
    double init_time = get_counter();
    double pre_time = init_time - 1;
    double fps_time = init_time;
    size_t fps_count = 0;
    double fps_update = 500.0;

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
}

int main(int argc, char *argv[])
{
    // to manage values -> config.h
    #ifdef MEASURE
    performance_test(BALL_N, ALG, IS_THREADING);
    return 0;
    #else
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    #endif // MEASURE
}

