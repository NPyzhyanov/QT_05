#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QString>
#include <memory>

#include "stopwatch.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
public slots:
    void stopwatch_ding(const unsigned int value);
    
private slots:
    void on_clb_start_stop_clicked(bool checked);
    void on_clb_reset_clicked();
    void on_clb_round_clicked();
    
private:    
    QString toTimeStrFormat(const unsigned int milliseconds);
    
    Ui::MainWindow *ui;
    Signals* sig;
    std::unique_ptr<Stopwatch> stopwatch;
    
    unsigned int displayed_resolution; // ms
    QIcon start_icon;
    QIcon stop_icon;
    QIcon reset_icon;
    QIcon round_icon;
    
    class Increment
    {
    public:
        Increment() { value = 0; }
        int operator()() { return ++value; }
        void reset() { value = 0; }
    private:
        int value;
    };
    Increment incr;
    unsigned int last_round_value; // ms
};
#endif // MAINWINDOW_H
