#ifndef PIGPEN_H
#define PIGPEN_H

#include"pig.h"

#include<QString>
class PigPen
{
public:
    PigPen();
    PigPen(QDataStream *stream);//从文件初始化
    void writeFile(QDataStream *stream);
    ~PigPen();
    float sold_out(void);//返回猪栏中可以卖出的猪的总售价，并删除这些猪
    void add_pig(Pig *pig);
    void weight_add(void);
    QString pig_status(int num);//返回对应编号猪的状态(假如对应编号的猪存在)
    QString pig_NumAndBreed(void);//用户查询猪栏中猪的种类和数量
    int showPigBreed(int pigID);
    void showDifferWeights(int *weights);//返回猪栏中各体重区间猪的数量，0为0~50斤之间，1为50~100斤之间，2为100~150斤之间，3为150斤之上
    void showDifferFeedTime(int *times);//返回猪栏中各喂养时间区间猪的数量，0为0~90天之间，1为90~180天之间，2为180~270天之间，3为270`360天之间

    int Breed_num(int unit_breed){return breed_num[unit_breed];}//返回某种猪在此猪栏中的数量
    bool is_BlackPigPen(){return is_blackPigPen;}//是否是黑猪栏
    int showNumAll(void){return num;}//返回猪栏中猪的总数
    int showInfected_pigNum(void){return infected_pigNum;}//返回此猪栏中被感染的猪的数量
    void setInfect(void){pigs->is_infected=true;}
    void infect_inPen(void);//猪栏内50%机率感染
    bool isIsolated=false;    //此猪栏是否被隔离
    bool isInfected=false;    //说明此猪圈是否有感染猪瘟的猪
private:
    int num;    //栏中猪的数量
    int breed_num[3]={0};
    bool is_blackPigPen;
    Pig *pigs=NULL;
    bool pig_exist[10]={0};//对应编号的猪是否存在
    int infected_pigNum;
private:
    static unsigned int randseed;
};

#endif // PIGPEN_H
