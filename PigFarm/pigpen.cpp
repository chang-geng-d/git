#include "pigpen.h"
#include "pig.h"

#include<QTime>
#include<QtGlobal>
unsigned int PigPen::randseed=0;//由于直接放于类头文件中会出现重定义错误，故静态成员放于实现部分定义
PigPen::PigPen()//构造函数，在猪栏里随机生成猪
{
    //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    qsrand(randseed*randseed*QTime(0,0,0).secsTo(QTime::currentTime()));
    //qDebug()<<randseed;
    infected_pigNum=0;
    isInfected=false;
    isIsolated=false;
    is_blackPigPen=qrand()%2;
    pigs=new Pig(is_blackPigPen);
    ++breed_num[pigs->showBreed()];
    pigs->num=9;
    pig_exist[9]=true;
    num=1;
    Pig *temp=pigs;
    for(int i=qrand()%9; i>0; --i){
        int t=0;
        while(pig_exist[t]==true){
            ++t;
        }
        pig_exist[t]=true;
        temp->nextPig=new Pig(is_blackPigPen);
        temp=temp->nextPig;
        temp->num=t;
        ++breed_num[temp->showBreed()];
        ++num;
    }
    ++randseed;
}

PigPen::PigPen(QDataStream *stream){
    //qDebug()<<"PigPen_in";
    //初始化数据成员
    *stream>>num;
    //qDebug()<<num;
    for(int i=0;i<3;++i){
        *stream>>breed_num[i];
        //qDebug()<<breed_num[i];
    }
    *stream>>is_blackPigPen;
    //qDebug()<<is_blackPigPen;
    for(int i=0;i<10;++i){
        *stream>>pig_exist[i];
        //qDebug()<<pig_exist[i];
    }
    *stream>>isInfected>>infected_pigNum>>isIsolated;
    //qDebug()<<isInfected<<infected_pigNum;
    //重建链表
    if(num!=0){
        pigs=new Pig(stream);
        Pig *temp=pigs;
        for(int i=0;i<num-1;++i){
            temp->nextPig=new Pig(stream);
            temp=temp->nextPig;
        }
    }
}

void PigPen::writeFile(QDataStream *stream){
    *stream<<num;
    //qDebug()<<num;
    for(int i=0;i<3;++i){
        *stream<<breed_num[i];
        //qDebug()<<breed_num[i];
    }
    *stream<<is_blackPigPen;
    //qDebug()<<is_blackPigPen;
    for(int i=0;i<10;++i){
        *stream<<pig_exist[i];
        //qDebug()<<pig_exist[i];
    }
    *stream<<isInfected<<infected_pigNum<<isIsolated;
    //qDebug()<<isInfected<<infected_pigNum;
    Pig *temp=pigs;
    while(temp!=NULL){
        //qDebug()<<"a";
        temp->writeFile(stream);
        temp=temp->nextPig;
    }
}

PigPen::~PigPen(){
    Pig *temp1=pigs;
    Pig *temp2=pigs->nextPig;
    while(temp2!=NULL){
        delete temp1;
        temp1=temp2;
        temp2=temp2->nextPig;
    }
    delete temp1;
}

void PigPen::add_pig(Pig *pig){//猪种类和数目的判断在外部做
    int i=0;
    while(pig_exist[i]==true){
        ++i;
    }
    pig_exist[i]=true;
    pig->num=i;
    pig->nextPig=pigs;
    pigs=pig;
    ++breed_num[pig->showBreed()];
    ++num;
}

float PigPen::sold_out(){//返回猪栏中可以卖出的猪的总售价，并删除这些猪
    Pig *temp1=pigs;
    float sum=0;
    while(pigs!=NULL&&pigs->can_be_sold()){
        //qDebug()<<"pigs_num"<<pigs->num<<pigs->can_be_sold()<<pigs->status();
        sum+=pigs->price();
        pigs=pigs->nextPig;
        pig_exist[temp1->num]=false;
        --breed_num[temp1->showBreed()];
        delete temp1;
        --num;
        temp1=pigs;
    }
    temp1=pigs;
    if(pigs!=NULL){
        Pig *temp2=pigs->nextPig;
        while(temp2!=NULL){
            if(temp2->can_be_sold()){
                //qDebug()<<"temp2_num"<<temp2->num<<temp2->can_be_sold()<<temp2->status();
                sum+=temp2->price();
                temp1->nextPig=temp2->nextPig;
                pig_exist[temp2->num]=false;
                --breed_num[temp2->showBreed()];
                delete temp2;
                --num;
                temp2=temp1->nextPig;
            }
            else{
                temp1=temp1->nextPig;
                if(temp2!=NULL){
                    temp2=temp2->nextPig;
                }
            }
        }
    }
    return sum;
}

void PigPen::weight_add(){
    Pig *temp=pigs;
    while(temp!=NULL){
        temp->weight_add();
        temp=temp->nextPig;
    }
}

QString PigPen::pig_status(int num){//返回对应编号猪的状态(假如对应编号的猪存在)
    QString status;
    if(pig_exist[num]==false){
        status="None";
        return status;
    }
    else{
        Pig *temp=pigs;
        while(temp!=NULL){
            if(temp->num==num){
                break;
            }
            temp=temp->nextPig;
        }
        status="编号：";
        status+=QString::number(temp->num);
        status+=temp->status();
        return status;
    }
}

QString PigPen::pig_NumAndBreed(){//用户查询猪栏中猪的种类和数量
    QString str;
    if(is_blackPigPen){
        str=QString("种类：黑猪\n数量：%1").arg(num);
        //str+=QString::number(num);
        return str;
    }
    else{
        str=QString("种类：小花猪\n数量：%1").arg(breed_num[1]);
        //str+=QString::number(breed_num[1]);
        str+=QString("\n种类：大花白猪\n数量：%1").arg(breed_num[2]);
        //str+=QString::number(breed_num[2]);
        return str;
    }
}

void PigPen::showDifferWeights(int *weights){
    Pig *temp=pigs;
    int t=0;
    while(temp!=NULL){
        t=temp->showWeight();
        if(t<0){
            return;
        }
        if(t<50){
            ++weights[0];
        }
        else if(t<100){
            ++weights[1];
        }
        else if(t<150){
            ++weights[2];
        }
        else{
            ++weights[3];
        }
        temp=temp->nextPig;
    }
}

void PigPen::showDifferFeedTime(int *times){
    Pig *temp=pigs;
    int t=0;
    while(temp!=NULL){
        t=temp->showFeedTime();
        if(t<0){
            return;
        }
        if(t<90){
            ++times[0];
        }
        else if(t<180){
            ++times[1];
        }
        else if(t<270){
            ++times[2];
        }
        else if(t<360){
            ++times[3];
        }
        else{
            ++times[4];
        }
        temp=temp->nextPig;
    }
}

void PigPen::infect_inPen(){
    if(isInfected==false||infected_pigNum==num){
        return;
    }
    else{
        Pig *temp=pigs;
        unsigned int seed=QTime(0,0,0).secsTo(QTime::currentTime());
        while(temp!=NULL){
            if(temp->is_infected==false){
                qsrand(seed*seed*1006);
                //qDebug()<<"seed="<<seed;
                if((qrand()%101)<=50){
                    temp->is_infected=true;
                    ++infected_pigNum;
                    //qDebug()<<"infected_pigNum="<<infected_pigNum;
                }
                ++seed;
            }
            temp=temp->nextPig;
        }
    }
}

int PigPen::showPigBreed(int pigID){
    if(pig_exist[pigID]==false||(pigID<0||pigID>9)){
        return -1;
    }
    else{
        Pig *temp=pigs;
        while(temp->num!=pigID){
            temp=temp->nextPig;
        }
        return temp->showBreed();
    }
}
