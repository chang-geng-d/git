#ifndef PIGFARM_H
#define PIGFARM_H

#include"pigpen.h"

#include<QFile>
#include<QDataStream>
#include<QString>
class PigFarm
{
public:
    PigFarm();//无文件时初始化
    PigFarm(QFile &file);//重启时用文件初始化(使用哪一个构造函数由外部判断)
    void writeFile(QFile &file);//保存至文件
    ~PigFarm();
    float SoldOut_All(void);//返回一次卖出的猪的价钱
    void showDifferBreedNums(int num[3]);//返回猪场中各种猪的数量，0为黑猪，1为小花猪，2为大花白猪
    void showDifferWeights(int *weights);//返回猪场中各体重区间猪的数量，0为0~50斤之间，1为50~100斤之间，2为100~150斤之间,3为150斤之上
    void showDifferFeedTimes(int *times);//返回猪栏中各喂养时间区间猪的数量，0为0~90天之间，1为90~180天之间，2为180~270天之间，3为270`360天之间
    void showPigFarmStatus_whenInfecting(QString &str);
    void AddPig(void);//向猪场内添加一批猪仔(数量，种类随机生成)
    void AddWeight(void);//配合计时器使用

    QString refer_pen(int pen_num){return pen[pen_num]->pig_NumAndBreed();}//用户查询猪栏中猪的种类和数量
    QString refer_pig(int pen_num,int pig_num){return pen[pen_num]->pig_status(pig_num);}//返回对应编号猪的状态(假如对应编号的猪存在)
    int showPigNumInPen(int penID){return pen[penID]->showNumAll();}
    int showPigBreedInPen(int penID,int pigID);
    void OutPen(void);//实现猪场猪的售卖，猪崽的添加，以及调用相关的文件存储函数
    void writeFile(QFile &file_out,QFile &file_in);//实现存储一年中售卖猪和购入猪的情况
    bool isInfectAllCanBeInfected(void);
    int showNumAll(void);
    int showInfectedNumAll(void);
    void startInfect(void);
    void infect_betweenPen(void);//猪栏之间15%机率感染
    QString isolateAllInfectedPen(void);//隔离所有受感染的猪栏
    void isolateInfectedPenByNum(int num);//隔离对应编号的猪栏
    int Year;
private:
    PigPen *pen[100];
};

#endif // PIGFARM_H
