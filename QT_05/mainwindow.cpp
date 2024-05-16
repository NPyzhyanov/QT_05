#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sig = new Signals(this);
    
    displayed_resolution = 100; // ms
    stopwatch = std::make_unique<Stopwatch>(displayed_resolution, sig);
    last_round_value = 0; // ms
    
    ui->tl_time_display->setText(toTimeStrFormat(0));
    
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
    
    QObject::connect(sig, &Signals::ding, this, &MainWindow::stopwatch_ding);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::stopwatch_ding(const unsigned int value)
{
    ui->tl_time_display->setText(toTimeStrFormat(value));
}


QString MainWindow::toTimeStrFormat(const unsigned int milliseconds)
{
    const unsigned int minutes = milliseconds / 60000;
    const unsigned int seconds = (milliseconds % 60000) / 1000;
    const unsigned int portions_of_second = (milliseconds % 1000) / displayed_resolution;
    
    return (minutes < 10 ? "0" + QString::number(minutes) : QString::number(minutes)) + ":" + 
            (seconds < 10 ? "0" + QString::number(seconds) : QString::number(seconds)) + "." + 
            QString::number(portions_of_second);
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
    ui->clb_reset->setEnabled(!checked);
}


void MainWindow::on_clb_reset_clicked()
{
    stopwatch->reset();
    incr.reset();
    last_round_value = 0;
}


void MainWindow::on_clb_round_clicked()
{
    unsigned int current_value = stopwatch->get_current();
    ui->tb_rounds->append("Круг №" + QString::number(incr()) + ", время: " + toTimeStrFormat(current_value - last_round_value) + " сек");
    last_round_value = current_value;
}

