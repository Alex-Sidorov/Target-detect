#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QThread>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QGraphicsPathItem>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _label(new ClickedLabel),
    _is_run(false)
{
    connect(_label,&ClickedLabel::clicked,this,&MainWindow::slot_draw_image_clicked);

    ui->setupUi(this);
    ui->draw_area_layout->addWidget(_label);
}

MainWindow::~MainWindow()
{
    delete _label;
    delete ui;
}


void MainWindow::slot_error_open_video()
{
     QMessageBox::warning(this,"Ошибка","Не удалось открыть файл");
}
void MainWindow::slot_get_image_video(QImage img)
{
    ui->scene->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_select_video_clicked()
{
    auto path = QFileDialog::getOpenFileName(this);
    if(!path.isEmpty())
    {
        ui->path_video_line->setText(path);
    }
}

void MainWindow::run_video()
{
    auto path = ui->path_video_line->text();
    VideoThread *video_thread = new VideoThread(path, ui->delay_box->value());
    create_thread(video_thread);
}

void MainWindow::run_camera()
{
    VideoThread *video_thread = new VideoThread(ui->index_camera->value(), ui->delay_box->value());
    create_thread(video_thread);
}

void MainWindow::create_thread(VideoThread *video_thread)
{
    QThread *thread = new QThread;

    video_thread->moveToThread(thread);

    connect(video_thread,SIGNAL(error_open()),this,SLOT(slot_error_open_video()));
    connect(video_thread,SIGNAL(send_image(QImage)),this,SLOT(slot_get_image_video(QImage)));
    connect(video_thread,SIGNAL(finished()),this,SLOT(slot_change_flag_is_run()));
    connect(ui->stop_button,SIGNAL(clicked()),video_thread,SLOT(stop()),Qt::DirectConnection);

    connect(thread,SIGNAL(started()),video_thread,SLOT(play()));
    connect(video_thread,SIGNAL(finished()),thread,SLOT(quit()));
    connect(video_thread,SIGNAL(finished()),video_thread,SLOT(deleteLater()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    thread->start();
}

void MainWindow::slot_change_flag_is_run()
{
    _is_run = false;
}


void MainWindow::on_video_button_clicked()
{
    if(_is_run)
    {
        QMessageBox::warning(this,"Ошибка","Невозможно запустить два потока видео одновременно");
        return;
    }
    _is_run = true;
    run_video();
}

void MainWindow::on_camera_button_clicked()
{
    if(_is_run)
    {
        QMessageBox::warning(this,"Ошибка","Невозможно запустить два потока видео одновременно");
        return;
    }
    _is_run = true;
    run_camera();
}


void MainWindow::on_working_button_clicked()
{
    if(_drawed.isNull() || _second.isNull())
    {
        QMessageBox::warning(this,"Ошибка","Недостаточно изображений");
        return;
    }

    auto temp_img = _drawed;
    cv::Mat first_mat(temp_img.height(),temp_img.width(),CV_8UC4,const_cast<uchar*>(temp_img.bits()),static_cast<size_t>(temp_img.bytesPerLine()));
    cv::Mat first_gray;
    cv::medianBlur(first_mat,first_mat,7);
    cv::cvtColor(first_mat, first_gray, CV_RGBA2GRAY);

    cv::Mat first_bin;
    cv::threshold(first_gray.clone(),first_bin,ui->treshhold->value(),255,cv::THRESH_OTSU);

    auto _img = QImage(static_cast<unsigned char*>(first_bin.data),first_bin.cols,first_bin.rows,QImage::Format_Grayscale8);
    ui->bin_first_image->setPixmap(QPixmap::fromImage(_img));

    temp_img = _second;
    cv::Mat second_mat(temp_img.height(),temp_img.width(),CV_8UC4,const_cast<uchar*>(temp_img.bits()),static_cast<size_t>(temp_img.bytesPerLine()));
    cv::Mat second_gray;
    cv::medianBlur(second_mat,second_mat,7);
    cv::cvtColor(second_mat, second_gray, CV_RGBA2GRAY);
    cv::Mat second_bin;
    cv::threshold(second_gray,second_bin,ui->treshhold->value(),255,cv::THRESH_OTSU);

    _img = QImage(static_cast<unsigned char*>(second_bin.data),second_bin.cols,second_bin.rows,QImage::Format_Grayscale8);
    ui->bin_second_image->setPixmap(QPixmap::fromImage(_img));

    cv::Mat res_sub;
    cv::subtract(second_bin,first_bin,res_sub);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(first_gray,circles,cv::HOUGH_GRADIENT,1,first_gray.rows/16, 100, 30,5,70 );
    qDebug() << "count circles:" << circles.size();
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);
        int radius = c[2];
        qDebug()<<center.x<<center.y<<radius;
        circle(res_sub, center, radius, cv::Scalar(255,255,255), 2, cv::LINE_AA);
    }

    temp_img = QImage(static_cast<unsigned char*>(res_sub.data),res_sub.cols,res_sub.rows,QImage::Format_Grayscale8);
    _img = temp_img;
    for(int j = 0; j <_img.height(); j++)
    {
        for(int i = 0; i <_img.width(); i++)
        {
            for(size_t k = 0; k < circles.size(); k++)
            {
                int radius = circles[k][2];
                int x = circles[k][0] - radius;
                int y = circles[k][1] - radius;

                x = x < 0 ? 0 : x;
                y = y < 0 ? 0 : y;
                if((i >= x && i <= x + radius*2 && j >= y && j <= y + radius*2) && qGray(temp_img.pixel(i,j)) == 255)
                {
                    _img.setPixel(i,j,temp_img.pixel(i,j));
                    break;
                }
                else
                {
                    _img.setPixel(i,j,_drawed.pixel(i,j));
                }

            }
        }
    }

    ui->res_image->setPixmap(QPixmap::fromImage(_img));

    first_bin.release();
    first_mat.release();
    first_gray.release();
    second_bin.release();
    second_mat.release();
    second_gray.release();
    res_sub.release();
}

void MainWindow::on_get_first_image_button_clicked()
{
    if(!ui->scene->pixmap())
    {
        QMessageBox::warning(this,"Ошибка","Нет изображения");
        return;
    }
    _first = ui->scene->pixmap()->toImage();
    _drawed = _first;
    ui->first_image->setPixmap(QPixmap::fromImage(_first));
    _label->setPixmap(QPixmap::fromImage(_first));
}

void MainWindow::on_get_second_image_button_clicked()
{
    if(!ui->scene->pixmap())
    {
        QMessageBox::warning(this,"Ошибка","Нет изображения");
        return;
    }
    _second = ui->scene->pixmap()->toImage();
    ui->second_image->setPixmap(QPixmap::fromImage(_second));
}

void MainWindow::slot_draw_image_clicked(QPoint pos)
{
    if(_label->pixmap())
    {
        _back_draweds.push_back(_drawed);
        QPainter paint;
        paint.begin(&_drawed);
        paint.setPen(QPen(Qt::white,5,Qt::SolidLine,Qt::FlatCap));
        paint.drawEllipse(pos,ui->radius_box->value(),ui->radius_box->value());
        paint.end();
        _label->setPixmap(QPixmap::fromImage(_drawed));
    }
}

void MainWindow::on_cancel_button_clicked()
{
    if(_back_draweds.size())
    {
        _drawed = _back_draweds.back();
        _back_draweds.pop_back();
        _label->setPixmap(QPixmap::fromImage(_drawed));
    }
}
