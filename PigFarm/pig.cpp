#include "pig.h"

#include<QTime>
#include<QtGlobal>//提供随机数生成函数
unsigned int Pig::randseed=0;//由于直接放于类头文件中会出现重定义错误，故静态成员放于实现部分定义
//Pig::Pig()
//{
//    qsrand(randseed*randseed*100);//初始化随机数种子
//    weight=(qrand()%31)+20;
//    feeding_time=0;
//    breed=qrand()%3;
//    num=0;
//    nextPig=NULL;
//    is_infected=false;
//    ++randseed;
//}

Pig::Pig(QDataStream *stream){
    *stream>>num>>weight>>breed>>feeding_time>>is_infected;
}

void Pig::writeFile(QDataStream *stream){
    *stream<<num<<weight<<breed<<feeding_time<<is_infected;
    //qDebug()<<num<<weight<<breed<<feeding_time<<is_infected;
}

Pig::Pig(bool is_black){
    qsrand(randseed*randseed*QTime(0,0,0).secsTo(QTime::currentTime()));//初始化随机数种子
    weight=(qrand()%31)+20;
    feeding_time=0;
    nextPig=NULL;
    if(is_black==true){
        breed=0;
    }
    else {
        breed=qrand()%2+1;
    }
    ++randseed;
}

void Pig::weight_add(){
    qsrand(randseed*randseed*100);
    int temp=qrand();
    weight+=temp%2+(temp%3)/10.0;
    ++feeding_time;
    ++randseed;
}

float Pig::price(){
    int unit_price[3]={15, 7, 6};
    return unit_price[breed]*weight;
}

bool Pig::can_be_sold(){
    if(weight>150){
        return true;
    }
    else if(feeding_time>360){
        return true;
    }
    else{
        return false;
    }
}

QString Pig::status(){
    QString status;
    QString unit_breed[3]={"黑猪","小花猪","大花白猪"};
    status=QString("\n品种：%1").arg(unit_breed[breed]);
    //status+=unit_breed[breed];
    status+=QString("\n重量：%1").arg(weight);
    //status+=QString::number(weight);
    status+=QString("\n饲养天数：%1").arg(feeding_time);
    //status+=QString::number(feeding_time);
    status+="\n是否感染：";
    if(is_infected){
        status+="是";
    }
    else{
        status+="否";
    }
    return status;
}
