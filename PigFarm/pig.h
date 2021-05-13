#ifndef PIG_H
#define PIG_H

#include<QString>
#include<QDataStream>
#include<QDebug>
class Pig
{
public:
    Pig();
    Pig(bool is_black);
    Pig(QDataStream *stream);//从文件初始化
    void writeFile(QDataStream *stream);
    void weight_add(void);
    bool can_be_sold(void);
    float price(void);
    QString status(void);
    int showBreed(void){return breed;}
    int showWeight(void){return weight;}
    int showFeedTime(void){return feeding_time;}

    int num;//猪在猪圈中的编号
    bool is_infected=false;//此猪是否被感染
    Pig *nextPig=NULL;
private:
    float weight;//猪的体重
    int breed;//猪的品种，0代表黑猪，1代表小花猪，2代表大花白猪
    int feeding_time;//猪的饲养天数
private:
    static unsigned int randseed;
};

#endif // PIG_H
