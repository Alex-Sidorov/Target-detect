#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <videothread.h>
#include <clickedlabel.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slot_error_open_video();
    void slot_get_image_video(QImage);
    void slot_change_flag_is_run();

private slots:
    void on_select_video_clicked();
    void on_video_button_clicked();
    void on_camera_button_clicked();
    void on_working_button_clicked();
    void on_get_first_image_button_clicked();
    void on_get_second_image_button_clicked();
    void slot_draw_image_clicked(QPoint pos);
    void on_cancel_button_clicked();

private:
    void run_video();
    void run_camera();
    void create_thread(VideoThread *video_thread);


    Ui::MainWindow *ui;

    ClickedLabel *_label;

    QImage _first;
    QImage _second;
    QImage _drawed;
    QVector<QImage> _back_draweds;

    bool _is_run;
};

#endif // MAINWINDOW_H
