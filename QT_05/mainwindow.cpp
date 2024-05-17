#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    displayed_resolution = 100; // ms
    
    stopwatch = new Stopwatch(displayed_resolution, this);
    
    QPixmap start_pm("../icons/start.png");
    QPixmap stop_pm("../icons/stop.png");
    QPixmap reset_pm("../icons/reset.png");
    QPixmap round_pm("../icons/round.png");
    start_icon.addPixmap(start_pm);
    stop_icon.addPixmap(stop_pm);
    reset_icon.addPixmap(reset_pm);
    round_icon.addPixmap(round_pm);
    
    ui->clb_start_stop->setIcon(start_icon);
    ui->clb_start_stop->setText("Старт");
    ui->clb_start_stop->setCheckable(true);
    ui->clb_reset->setIcon(reset_icon);
    ui->clb_reset->setText("Очистить");
    ui->clb_round->setIcon(round_icon);
    ui->clb_round->setText("Круг");
    ui->clb_round->setEnabled(false);
    
    ui->tb_rounds->setReadOnly(true);
    
    QObject::connect(stopwatch, &Stopwatch::ding, this, &MainWindow::stopwatch_ding);
    
    stopwatch->primaryHandshake();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::stopwatch_ding(QString new_value)
{
    ui->tl_time_display->setText(new_value);
}


void MainWindow::on_clb_start_stop_clicked(bool checked)
{
    if (checked)
    {
        stopwatch->start();
        ui->clb_start_stop->setIcon(stop_icon);
        ui->clb_start_stop->setText("Стоп");
    }
    else
    {
        stopwatch->stop();
        ui->clb_start_stop->setIcon(start_icon);
        ui->clb_start_stop->setText("Старт");
    }
    ui->clb_round->setEnabled(checked);
}


void MainWindow::on_clb_reset_clicked()
{
    ui->tl_time_display->setText(stopwatch->reset());
}


void MainWindow::on_clb_round_clicked()
{
    ui->tb_rounds->append(stopwatch->round());
}

