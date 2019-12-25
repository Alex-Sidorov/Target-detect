#include "videothread.h"
#include <QThread>

VideoThread::VideoThread(QString &path, int delay):
    _frame_rate(0),
    _delay(delay),
    _path(path),
    _is_run(false)
{

}

VideoThread::VideoThread(int index, int delay):
    _frame_rate(0),
    _delay(delay),
    _capture(index),
    _is_run(false)
{

}

void VideoThread::stop()
{
    _is_run = false;
}

void VideoThread::play()
{
    if(!_path.isEmpty())
    {
        _capture.open(_path.toStdString());
    }

    if(_capture.isOpened())
    {

        _is_run = true;
        _frame_rate = static_cast<int>(_capture.get(cv::CAP_PROP_FPS));
        show_video();
    }
    else
    {
        emit error_open();
    }
}

void VideoThread::show_video()
{
    int delay_with_fps = 1000/_frame_rate;
    decltype (_frame) temp;
    while(_capture.read(temp) && _is_run)
    {
         cv::waitKey(delay_with_fps);
        _frame = temp;
        if (_frame.channels() == 3)
        {
            cv::cvtColor(_frame, _rgb_frame, CV_BGR2RGB);
            _img = QImage(static_cast<unsigned char*>(_rgb_frame.data),_rgb_frame.cols,_rgb_frame.rows,QImage::Format_RGB888);
        }
        else
        {
            _img = QImage(static_cast<unsigned char*>(_frame.data),_frame.cols,_frame.rows,QImage::Format_Indexed8);
        }
        emit send_image(_img);
        QThread::msleep(delay_with_fps + _delay);
    }
    _capture.release();
    temp.release();
    _frame.release();
    _rgb_frame.release();
    emit finished();
}

VideoThread::~VideoThread()
{

}
