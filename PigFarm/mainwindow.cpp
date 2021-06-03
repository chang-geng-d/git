#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QFile>
#include<QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置样式表
    QFile styleSheetFile("://sources/MacOS.qss");
    styleSheetFile.open(QIODevice::ReadOnly);
    QString styleSheet=styleSheetFile.readAll();
    styleSheetFile.close();
    this->setStyleSheet(styleSheet);

    //设置页面初始样式
    this->setWindowIcon(QIcon("://sources/pig.png"));
    ui->stackedWidget->setCurrentWidget(ui->page_manage);
    ui->toolBox->setCurrentWidget(ui->toolPage_1);
    //ui->label_pixmap_pig_1->setScaledContents(true);
    //ui->pushButton_pause->setEnabled(false);
    //ui->pushButton_restart->setEnabled(false);
    //ui->pushButton_pauseInfect->setEnabled(false);
    //ui->pushButton_infectRestart->setEnabled(false);
    //ui->pushButton_isolateAllInfectedPens->setEnabled(false);
    //ui->pushButton_isolatePenInNum->setEnabled(false);
    //ui->lineEdit_isolateNum->setEnabled(false);
    //
    //设置初始ChartView
    QChart *m_chart = new QChart();
    m_chart->setTheme(QChart::ChartThemeLight);//设置白色主题
    m_chart->setDropShadowEnabled(true);//背景阴影
    m_chart->setTitleBrush(QBrush(QColor(0, 0, 255))); //设置标题Brush
    m_chart->setTitleFont(QFont("微软雅黑"));//设置标题字体
    //修改说明样式
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignRight);//底部对齐
    m_chart->legend()->setBackgroundVisible(true);//设置背景是否可视
    m_chart->legend()->setAutoFillBackground(true);//设置背景自动填充
    m_chart->legend()->setColor(QColor(222, 233, 251)); //设置颜色
    m_chart->legend()->setLabelColor(QColor(0, 100, 255)); //设置标签颜色
    m_chart->legend()->setMaximumHeight(150);

    chartView=new QChartView(m_chart,ui->ChartView);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(730,580);
    chartView->hide();
    //
    QFile file("./restartInfor.txt");
    if(file.exists()){
        //qDebug()<<"mainwindow_ok";
        farm=new PigFarm(file);
    }
    else{
        farm=new PigFarm;
    }
}

MainWindow::~MainWindow()
{
    QFile file("./restartInfor.txt");
    farm->writeFile(file);
    delete ui;
}

void MainWindow::on_actionto_page_manage_triggered()
{
    ui->label_HeadLine->setText("养猪场管理系统");
    ui->stackedWidget->setCurrentWidget(ui->page_manage);
    ui->statusbar->showMessage("管理系统与猪瘟模拟系统");
}

void MainWindow::on_actionto_page_chart_triggered()
{
    ui->label_HeadLine->setText("各品种猪属性分布统计");
    ui->stackedWidget->setCurrentWidget(ui->page_chart);
    ui->statusbar->showMessage("猪属性分布统计表");
}

void MainWindow::on_Button_aYear_clicked()
{
    timerID_1=this->startTimer(100);//每隔0.1秒启动一次事件
    ++farm->Year;
    ui->Button_aYear->setEnabled(false);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_startInfect->setEnabled(false);
}

void MainWindow::timerEvent(QTimerEvent *event){
    if(event->timerId()==timerID_1){
        static int sum=0;
        if(sum%90==0&&sum!=0){
            //qDebug()<<"OutPen";
            farm->OutPen();
            farm->AddWeight();
        }
        else{
            farm->AddWeight();
        }
        ui->label_year->setNum(sum);
        ++sum;
        if(sum==361){
            this->killTimer(timerID_1);
            sum=0;
            ui->label_year->setNum(sum);
            ui->Button_aYear->setEnabled(true);
            ui->pushButton_restart->setEnabled(false);
            ui->pushButton_pause->setEnabled(false);
            ui->pushButton_startInfect->setEnabled(true);
            return;
        }
    }
    else if(event->timerId()==timerID_2){
        static int sum_2=0;
        //qDebug()<<"sum_2="<<sum_2;
        int numAll=farm->showNumAll(),InfectedNum=farm->showInfectedNumAll();
        bool flag=farm->isInfectAllCanBeInfected();
        if((numAll==InfectedNum)||(flag==true)||(sum_2>1080)){
            if(numAll==InfectedNum){
                ui->display_textEdit->setText(QString("经过%1天之后猪场中猪全部被感染").arg(sum_2));
                //ui->display_textEdit->append(QString(sum_2));
                //ui->display_textEdit->append("天之后猪场中猪全部被感染");
            }
            else if(flag==true){
                ui->display_textEdit->setText("无其余任何未被感染的或未被隔离的猪栏可感染");
            }
            else if(sum_2>1080){
                ui->display_textEdit->setText("3年后，猪瘟消失");
            }
            killTimer(timerID_2);
            sum_2=0;
            ui->pushButton_pauseInfect->setEnabled(false);
            ui->pushButton_infectRestart->setEnabled(false);
            ui->pushButton_startInfect->setEnabled(true);
            ui->Button_aYear->setEnabled(true);
            ui->pushButton_isolateAllInfectedPens->setEnabled(false);
            ui->pushButton_isolatePenInNum->setEnabled(false);
            ui->lineEdit_isolateNum->setEnabled(false);
            return;
        }
        farm->infect_betweenPen();
        ++sum_2;
    }
}

void MainWindow::on_Button_soldRecord_clicked()
{
    int year=farm->Year;
    year-=ui->comboBox_soldRecord->currentIndex();
    QString fileName(QString("./outPenLog_Year_%1.txt").arg(year));
    //fileName+=QString::number(year);
    //fileName+=".txt";
    QFile file(fileName);
    if(file.exists()){
        file.open(QIODevice::ReadOnly);
        if(file.isOpen()){
            QString fileStr(file.readAll());
            //fileStr=file.readAll();
            //while(file.atEnd()==0){
            //    fileStr+=file.readLine();
            //}
            file.close();
            ui->display_textEdit->setText(fileStr);
        }
    }
    else{
        ui->display_textEdit->setText("对应的卖出记录不存在");
    }
}

void MainWindow::on_Button_buyPig_clicked()
{
    int year=farm->Year;
    year-=ui->comboBox_buyPig->currentIndex();
    QString fileName(QString("./buyInLog_Year_%1.txt").arg(year));
    //fileName+=QString::number(year);
    //fileName+=".txt";
    QFile file(fileName);
    if(file.exists()){
        file.open(QIODevice::ReadOnly);
        if(file.isOpen()){
            QString fileStr(file.readAll());
            //fileStr=file.readAll();
            file.close();
            ui->display_textEdit->setText(fileStr);
        }
    }
    else{
        ui->display_textEdit->setText("对应的买入记录不存在");
    }
}

void MainWindow::on_Button_penStatus_clicked()
{
    int num=ui->lineEdit_penStatus->text().toInt();
    if(num>=0&&num<100){
        ui->display_textEdit->setText(farm->refer_pen(num));
    }
    else{
        ui->display_textEdit->setText("无法查询:对应的猪栏编号不存在");
    }
}

void MainWindow::on_Button_pigStatus_clicked()
{
    int pen_num=ui->lineEdit_pigStatus->text().toInt();
    int pig_num=ui->comboBox_pigStatus->currentIndex();
    if(pen_num>=0&&pen_num<100){
        ui->display_textEdit->setText(farm->refer_pig(pen_num,pig_num));
    }
    else{
        ui->display_textEdit->setText("无法查询:对应的猪栏编号不存在");
    }
}

void MainWindow::on_pushButton_restart_clicked()
{
    timerID_1=this->startTimer(100);
    ui->pushButton_restart->setEnabled(false);
    ui->pushButton_pause->setEnabled(true);
}

void MainWindow::on_pushButton_pause_clicked()
{
    this->killTimer(timerID_1);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_restart->setEnabled(true);
}

void MainWindow::on_pushButton_weightChart_clicked()
{
    QString str("体重分布图");
    int sum=0,weights[4]={0};
    farm->showDifferWeights(weights);
    sum=weights[0]+weights[1]+weights[2]+weights[3];
    if(chartView->chart()->title()==str){
        QPieSeries *series=(QPieSeries *)chartView->chart()->series()[0];
        series->slices().at(0)->setValue(weights[0]/float(sum));
        series->slices().at(1)->setValue(weights[1]/float(sum));
        series->slices().at(2)->setValue(weights[2]/float(sum));
        series->slices().at(3)->setValue(weights[3]/float(sum));
        //qDebug()<<"update";
        chartView->update();
    }
    else{
        if(!chartView->chart()->series().isEmpty()){
            chartView->chart()->removeSeries(chartView->chart()->series()[0]);
        }
        QPieSeries *series = new QPieSeries();    //连续的餅图数据
        series->append("0~50斤之间",weights[0]/float(sum));
        series->append("50~100斤之间",weights[1]/float(sum));
        series->append("100~150斤之间",weights[2]/float(sum));
        series->append("150斤之上",weights[3]/float(sum));
        series->setLabelsVisible(true);
        series->setUseOpenGL(true);
        series->slices().at(0)->setColor(QColor(13, 128, 217)); //设置颜色
        series->slices().at(0)->setLabelColor(QColor(13, 128, 217));
        series->slices().at(1)->setColor(QColor(69, 13, 217));
        series->slices().at(1)->setLabelColor(QColor(69, 13, 217));
        series->slices().at(2)->setColor(QColor(13, 217, 152));
        series->slices().at(2)->setLabelColor(QColor(13, 217, 152));
        series->slices().at(3)->setColor(QColor(13, 217, 110));
        series->slices().at(3)->setLabelColor(QColor(13, 217, 110));

        chartView->chart()->addSeries(series);//添加系列到QChart上
        chartView->chart()->setTitle(str);
        chartView->show();
    }
}

void MainWindow::on_pushButton_numChart_clicked()
{
    QString str("各品种数量分布图");
    int sum=0,num[3]={0};
    farm->showDifferBreedNums(num);
    sum=num[0]+num[1]+num[2];
    if(chartView->chart()->title()==str){
        QPieSeries *series=(QPieSeries *)chartView->chart()->series()[0];
        series->slices().at(0)->setValue(num[0]/float(sum));
        series->slices().at(1)->setValue(num[1]/float(sum));
        series->slices().at(2)->setValue(num[2]/float(sum));
        //qDebug()<<"update";
        chartView->update();
    }
    else{
        if(!chartView->chart()->series().isEmpty()){
            chartView->chart()->removeSeries(chartView->chart()->series()[0]);
        }
        QPieSeries *series = new QPieSeries();    //连续的餅图数据
        series->append("黑猪", num[0]/float(sum));
        series->append("小花猪", num[1]/float(sum));
        series->append("大花白猪", num[2]/float(sum));
        series->setLabelsVisible(true);
        series->setUseOpenGL(true);
        series->slices().at(0)->setColor(QColor(13, 128, 217)); //设置颜色
        series->slices().at(0)->setLabelColor(QColor(13, 128, 217));
        series->slices().at(1)->setColor(QColor(69, 13, 217));
        series->slices().at(1)->setLabelColor(QColor(69, 13, 217));
        series->slices().at(2)->setColor(QColor(13, 217, 152));
        series->slices().at(2)->setLabelColor(QColor(13, 217, 152));

        chartView->chart()->addSeries(series);//添加系列到QChart上
        chartView->chart()->setTitle(str);
        chartView->show();
    }
}

void MainWindow::on_pushButton_feedTimeChart_clicked()
{
    QString str("喂养时间分布图");
    int sum=0,times[5]={0};
    farm->showDifferFeedTimes(times);
    sum=times[0]+times[1]+times[2]+times[3]+times[4];
    if(chartView->chart()->title()==str){
            QPieSeries *series=(QPieSeries *)chartView->chart()->series()[0];
            series->slices().at(0)->setValue(times[0]/float(sum));
            series->slices().at(1)->setValue(times[1]/float(sum));
            series->slices().at(2)->setValue(times[2]/float(sum));
            series->slices().at(3)->setValue(times[3]/float(sum));
            series->slices().at(4)->setValue(times[4]/float(sum));
            //qDebug()<<"update";
            chartView->update();
        }
        else{
            if(!chartView->chart()->series().isEmpty()){
                chartView->chart()->removeSeries(chartView->chart()->series()[0]);
            }
            QPieSeries *series = new QPieSeries();    //连续的餅图数据
            series->append("0~90天之间",times[0]/float(sum));
            series->append("90~180天之间",times[1]/float(sum));
            series->append("180~270天之间",times[2]/float(sum));
            series->append("270~360天之间",times[3]/float(sum));
            series->append("360天以上",times[4]/float(sum));

            series->setLabelsVisible(true);
            series->setUseOpenGL(true);
            series->slices().at(0)->setColor(QColor(13, 128, 217)); //设置颜色
            series->slices().at(0)->setLabelColor(QColor(13, 128, 217));
            series->slices().at(1)->setColor(QColor(69, 13, 217));
            series->slices().at(1)->setLabelColor(QColor(69, 13, 217));
            series->slices().at(2)->setColor(QColor(13, 217, 152));
            series->slices().at(2)->setLabelColor(QColor(13, 217, 152));
            series->slices().at(3)->setColor(QColor(13, 217, 110));
            series->slices().at(3)->setLabelColor(QColor(13, 217, 110));
            series->slices().at(4)->setColor(QColor(0,255,255));
            series->slices().at(4)->setLabelColor(QColor(0,255,255));

            chartView->chart()->addSeries(series);//添加系列到QChart上
            chartView->chart()->setTitle(str);
            chartView->show();
        }
}

void MainWindow::on_pushButton_showPigFarmStatus_clicked()
{
    QString str;
    farm->showPigFarmStatus_whenInfecting(str);
    ui->display_textEdit->setText(str);
}

void MainWindow::on_pushButton_isolateAllInfectedPens_clicked()
{
    ui->display_textEdit->setText("已自动隔离所有被感染的猪栏\n");
    ui->display_textEdit->append(farm->isolateAllInfectedPen());
}

void MainWindow::on_pushButton_isolatePenInNum_clicked()
{
    int num=ui->lineEdit_isolateNum->text().toInt();
    farm->isolateInfectedPenByNum(num);
    if(num<0||num>99){
        ui->display_textEdit->setText("无法隔离：对应编号的猪栏不存在");
    }
    else{
        ui->display_textEdit->setText(QString("已隔离编号为%1的猪栏").arg(num));
    }
}

void MainWindow::on_pushButton_startInfect_clicked()
{
    timerID_2=this->startTimer(500);
    while(timerID_1==timerID_2){
        killTimer(timerID_2);
        timerID_2=this->startTimer(500);
    }
    farm->startInfect();
    ui->pushButton_startInfect->setEnabled(false);
    ui->pushButton_pauseInfect->setEnabled(true);
    ui->Button_aYear->setEnabled(false);
    ui->pushButton_isolateAllInfectedPens->setEnabled(true);
    ui->pushButton_isolatePenInNum->setEnabled(true);
    ui->lineEdit_isolateNum->setEnabled(true);
}

void MainWindow::on_pushButton_pauseInfect_clicked()
{
    killTimer(timerID_2);
    ui->pushButton_pauseInfect->setEnabled(false);
    ui->pushButton_infectRestart->setEnabled(true);
}

void MainWindow::on_pushButton_infectRestart_clicked()
{
    timerID_2=this->startTimer(500);
    while(timerID_1==timerID_2){
        killTimer(timerID_2);
        timerID_2=this->startTimer(500);
    }
    ui->pushButton_pauseInfect->setEnabled(true);
    ui->pushButton_infectRestart->setEnabled(false);
}

void MainWindow::on_actiondisplay_triggered()
{
    penwidget=new penWidget(this,farm);
    connect(penwidget,SIGNAL(visual_pig_clicked(int,int)),this,SLOT(pig_in_penWidget_clicked(int,int)));
    //必须放到已初始化的对象之后
    penwidget->show();
    //ui->actiondisplay->setEnabled(false);
    //connect(penwidget,SIGNAL(close(void)),this,SLOT(penWidgetClose(void)));
}

void MainWindow::pig_in_penWidget_clicked(int penID,int pigID){
    //qDebug()<<"receive the visual_pig_clicked signal";
    ui->display_textEdit->setText(farm->refer_pig(penID,pigID));
}

void MainWindow::on_actionabout_triggered()
{
    QMessageBox *about=new QMessageBox(QMessageBox::NoIcon,"关于作者","“我打出来的”\n\t——晖晖如是说",QMessageBox::Ok,this);
    about->setIconPixmap(QPixmap("://sources/author.png"));
    about->setAttribute(Qt::WA_DeleteOnClose);
    about->show();
}
