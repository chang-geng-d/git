#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>//计时器

#include"pigfarm.h"
#include"penwidget.h"

#include<QtCharts>
#include<QPieSeries>
#include<QChartView>
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
    void pig_in_penWidget_clicked(int penID,int pigID);
private slots:
    void on_actionto_page_manage_triggered();

    void on_actionto_page_chart_triggered();

    void on_Button_aYear_clicked();

    void on_Button_soldRecord_clicked();

    void on_Button_buyPig_clicked();

    void on_Button_penStatus_clicked();

    void on_Button_pigStatus_clicked();

    void on_pushButton_restart_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_numChart_clicked();

    void on_pushButton_weightChart_clicked();

    void on_pushButton_feedTimeChart_clicked();

    void on_pushButton_showPigFarmStatus_clicked();

    void on_pushButton_isolateAllInfectedPens_clicked();

    void on_pushButton_isolatePenInNum_clicked();

    void on_pushButton_startInfect_clicked();

    void on_pushButton_pauseInfect_clicked();

    void on_pushButton_infectRestart_clicked();

    void on_actiondisplay_triggered();

    void on_actionabout_triggered();

    void on_action_references_triggered();

protected:
    void timerEvent(QTimerEvent *) override;
private:
    Ui::MainWindow *ui;
    int timerID_1;
    int timerID_2;
    PigFarm *farm;
    penWidget *penwidget=NULL;
    QChartView *chartView=NULL;
};
#endif // MAINWINDOW_H
