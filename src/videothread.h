#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>

#include <QObject>
#include <QString>
#include <QImage>
#include <QDebug>
#include <atomic>


class VideoThread : public QObject
{
    Q_OBJECT
public:
    VideoThread(QString &path, int delay = 0);
    VideoThread(int index, int delay = 0);
    virtual ~VideoThread();

signals:
    void error_open();
    void send_image(QImage);
    void finished();

public slots:
    void play();
    void stop();
private:
    void show_video();

    int _frame_rate;
    int _delay; //ms

    QString _path;

    cv::VideoCapture _capture;
    cv::Mat _frame;
    cv::Mat _rgb_frame;
    QImage _img;

    std::atomic_bool _is_run;
};

#endif // VIDEOTHREAD_H
