#include "penwidget.h"
#include "ui_penwidget.h"

#include<QMouseEvent>
#include<QMessageBox>
#include<QTime>
#include<QDebug>
#include<QPainter>

#include"visual_pig.h"

void penWidget::showPigsInPen(){
    unsigned int seed=0;
    //qDebug()<<"penID"<<penID;
    int num=farm->showPigNumInPen(penID);
    //qDebug()<<"out_showPigNumInPen(int penID)";
    pigs=new QLabel*[num];
    //qDebug()<<"pigs="<<pigs;
    int pigID=0;
    int breed=0;
    for(int i=0;i<num;++i){
        //qDebug()<<"i="<<i<<"\tpigID="<<pigID;
        breed=farm->showPigBreedInPen(penID,pigID);
        //qDebug()<<"breed="<<breed;
        if(breed==-1){
            ++pigID;
            --i;
            continue;
        }
        else{
            pigs[i]=new visual_pig(this,penID,pigID,breed);
            //pigs[i]->resize(60,60);
            connect(pigs[i],SIGNAL(clicked(int,int)),this,SLOT(pig_clicked(int,int)));
            //if(breed==0){
            //    pigs[i]->setPixmap(QPixmap("://sources/BlackPig.png"));
            //    qsrand(seed*seed*752);
            //}
            //else if(breed==1){
            //    pigs[i]->setPixmap(QPixmap("://sources/littlePig.png"));
            //    qsrand(seed*seed*183);
            //}
            //else if(breed==2){
            //    pigs[i]->setPixmap(QPixmap("://sources/bigWritePig.png"));
            //    qsrand(seed*seed*745);
            //}
            qsrand(seed*seed*QTime(0,0,0).secsTo(QTime::currentTime()));
            pigs[i]->move(qrand()%580,qrand()%580);
            pigs[i]->show();
            ++seed;
            ++pigID;
        }
    }
}

void penWidget::paintEvent(QPaintEvent *event){
    //绘制背景图
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height()-26,QPixmap("://sources/glass.png"));
    QPen pen;
    pen.setWidth(5);
    pen.setStyle(Qt::DashDotDotLine);
    painter.setPen(pen);
    painter.drawRect(0,0,658,682);
}

void penWidget::deletePigsInPen(){
    int num=farm->showPigNumInPen(penID);
    for(int i=0;i<num;++i){
        disconnect(pigs[i],SIGNAL(clicked(int,int)),this,SLOT(pig_clicked(int,int)));
        delete pigs[i];
    }
    delete[] pigs;
}

penWidget::penWidget(QWidget *parent,PigFarm *pigFarm) :
    QMainWindow(parent),
    ui(new Ui::penWidget)
{
    ui->setupUi(this);
    //qDebug()<<"into_dynamic_display_widget(QWidget *parent)";
    this->farm=pigFarm;
    showPigsInPen();
    timerID=this->startTimer(100);
    //qDebug()<<"enter";
    QMessageBox message(QMessageBox::Information,"提示","使用左键单击查看上一猪栏，使用右键单击查看下一猪栏",QMessageBox::Ok,this);
    message.exec();
}

penWidget::~penWidget()
{
    deletePigsInPen();
    delete ui;
    emit close();
}

void penWidget::timerEvent(QTimerEvent *){
    unsigned int seed=QTime(0,0,0).secsTo(QTime::currentTime());
    int num=farm->showPigNumInPen(penID);
    for(int i=0;i<num;++i){
        qsrand(seed*seed*156);
        int x=pigs[i]->x();
        int y=pigs[i]->y();
        switch(qrand()%4){
        case 0:{
            if(x<=577){
                pigs[i]->move(x+1,y);
            }
            break;
        }
        case 1:{
            if(y<=577){
                pigs[i]->move(x,y+1);
            }
            break;
        }
        case 2:{
            if(x>=3){
                pigs[i]->move(x-1,y);
            }
            break;
        }
        case 3:{
            if(y>=3){
                pigs[i]->move(x,y-1);
            }
            break;
        }
        }
        ++seed;
    }
}

void penWidget::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::LeftButton){
        if(penID>0){
            deletePigsInPen();
            //qDebug()<<"pigs="<<pigs;
            --penID;
            showPigsInPen();
            QString str=QString("猪栏编号：%1").arg(penID);
            //str+=QString::number(penID);
            ui->statusbar->showMessage(str);
        }
        else{
            QMessageBox waring(QMessageBox::Warning,"警告","已到达第一个猪栏，无法向前继续查看！",QMessageBox::Ok,this);
            waring.exec();
        }
    }
    else if(event->button()==Qt::RightButton){
        if(penID<99){
            deletePigsInPen();
            //qDebug()<<"pigs="<<pigs;
            ++penID;
            //qDebug()<<"penID="<<penID;
            showPigsInPen();
            QString str=QString("猪栏编号：%1").arg(penID);
            //str+=QString::number(penID);
            ui->statusbar->showMessage(str);
        }
        else{
            QMessageBox waring(QMessageBox::Warning,"警告","已到达最后一个猪栏，无法向后继续查看！",QMessageBox::Ok,this);
            waring.exec();
        }
    }
}

void penWidget::pig_clicked(int penID,int pigID){
    //qDebug()<<"receive the clicked signal";
    emit this->visual_pig_clicked(penID,pigID);
    //qDebug()<<"emit the visual_pig_clicked signal";
    //qDebug()<<"penID="<<penID<<"\tpigID="<<pigID;
}
