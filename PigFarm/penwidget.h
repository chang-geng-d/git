#ifndef PENWIDGET_H
#define PENWIDGET_H

#include <QMainWindow>
#include<QLabel>
#include"pigfarm.h"
namespace Ui {
class penWidget;
}

class penWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit penWidget(QWidget *parent = nullptr,PigFarm *pigFarm=NULL);
    ~penWidget();
public slots:
    void pig_clicked(int penID,int pigID);
signals:
    void visual_pig_clicked(int penID,int pigID);
    void close(void);
private:
    Ui::penWidget *ui;
protected:
    void timerEvent(QTimerEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    int timerID;
    int penID=0;
    PigFarm *farm=NULL;
    QLabel **pigs=NULL;
    void showPigsInPen(void);
    void deletePigsInPen(void);
};

#endif // PENWIDGET_H
