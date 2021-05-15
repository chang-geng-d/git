#include "pigfarm.h"

#include<QtGlobal>
#include<QTime>
#include<QDebug>
PigFarm::PigFarm()//无文件的情况下自动生成
{
    Year=0;
    for(int i=0;i<100;++i){
        pen[i]=new PigPen;
    }
}

PigFarm::PigFarm(QFile &file){
    file.open(QIODevice::ReadOnly);
    //qDebug()<<"PigFarm_in";
    if(file.isOpen()){
        QDataStream stream(&file);
        stream>>Year;
        //重建指针数组
        for(int i=0;i<100;++i){
            //qDebug()<<"A";
            pen[i]=new PigPen(&stream);
        }
        file.close();
        //qDebug()<<"PigFarm_ok";
    }
    else{
        //qDebug()<<"文件打开错误";
    }

}

void PigFarm::writeFile(QFile &file){
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    stream<<Year;
    for(int i=0;i<100;++i){
        //qDebug()<<"PigFarm_i="<<i;
        pen[i]->writeFile(&stream);
    }
    file.close();
}

PigFarm::~PigFarm(){
    for(int i=0;i<100;++i){
        delete pen[i];
    }
}

float PigFarm::SoldOut_All(){
    float sum=0;
    for(int i=0;i<100;++i){
        //qDebug()<<"i="<<i;
        sum+=pen[i]->sold_out();
    }
    return sum;
}

void PigFarm::showDifferBreedNums(int *num){
    num[0]=0;num[1]=0;num[2]=0;
    for(int i=0;i<100;++i){
        if(pen[i]->is_BlackPigPen()){
            num[0]+=pen[i]->Breed_num(0);
        }
        else{
            num[1]+=pen[i]->Breed_num(1);
            num[2]+=pen[i]->Breed_num(2);
        }
    }
}

void PigFarm::AddPig(){
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int min=0;
    for(int num=(qrand()%50)+50;num>0;--num){
        for(int i=0;i<100;++i){
            if(pen[min]->showNumAll()>pen[i]->showNumAll()){
                min=i;
            }
        }
        //qDebug()<<"min="<<min;
        Pig *pig=new Pig(pen[min]->is_BlackPigPen());
        pen[min]->add_pig(pig);
        min=0;
    }
}

void PigFarm::AddWeight(){
    for(int i=0;i<100;++i){
        pen[i]->weight_add();
    }
}

void PigFarm::OutPen(){
    QString fileName_out=QString("./outPenLog_Year_%1.txt").arg(Year);
    QString fileName_in=QString("./buyInLog_Year_%1.txt").arg(Year);
    //fileName_in+=QString::number(Year);
    //fileName_in+=".txt";
    //qDebug()<<"OutPen_in";
    QFile file_out(fileName_out);
    QFile file_in(fileName_in);
    writeFile(file_out,file_in);
}

void PigFarm::writeFile(QFile &file_out,QFile &file_in){
    static int n=1;
    //qDebug()<<"n="<<n;

    file_out.open(QIODevice::WriteOnly|QIODevice::Append);
    if(file_out.isOpen()){
        QTextStream stream_out(&file_out);
        stream_out.setCodec("utf-8");
        int pig_breeds[3]={0};
        int pig_breeds_next[3]={0};
        this->showDifferBreedNums(pig_breeds);
        stream_out<<QString("\n第")<<n<<QString("次卖出总价：");
        //qDebug()<<"soldOut_1_1";
        stream_out<<this->SoldOut_All();
        //qDebug()<<"soldOut_1_2";
        this->showDifferBreedNums(pig_breeds_next);
        stream_out<<QString("\n卖出黑猪数量:")<<pig_breeds[0]-pig_breeds_next[0];
        stream_out<<QString("\n卖出小花猪数量:")<<pig_breeds[1]-pig_breeds_next[1];
        stream_out<<QString("\n卖出大花白猪数量:")<<pig_breeds[2]-pig_breeds_next[2];
        file_out.close();
    }

    file_in.open(QIODevice::WriteOnly|QIODevice::Append);
    if(file_in.isOpen()){
        QTextStream stream_in(&file_in);
        stream_in.setCodec("utf-8");
        int pig_breeds[3]={0};
        int pig_breeds_next[3]={0};
        this->showDifferBreedNums(pig_breeds);
        stream_in<<QString("\n第")<<n<<QString("次买入");
        //qDebug()<<"soldOut_2_1";
        this->AddPig();
        //qDebug()<<"soldOut_2_2";
        this->showDifferBreedNums(pig_breeds_next);
        stream_in<<QString("\n买入黑猪数量")<<pig_breeds_next[0]-pig_breeds[0];
        stream_in<<QString("\n买入小花猪数量")<<pig_breeds_next[1]-pig_breeds[1];
        stream_in<<QString("\n买入大花白猪数量")<<pig_breeds_next[2]-pig_breeds[2];
        file_in.close();
    }

    ++n;
    if(n==5){
        n=1;
    }
}

void PigFarm::showDifferWeights(int *weights){
    //qDebug()<<"showDifferWeights_in";
    for(int i=0;i<100;++i){
        pen[i]->showDifferWeights(weights);
    }
    //qDebug()<<"weights[0]="<<weights[0]<<"weights[1]="<<weights[1]<<"weights[2]="<<weights[2];
}

void PigFarm::showDifferFeedTimes(int *times){
    for(int i=0;i<100;++i){
        pen[i]->showDifferFeedTime(times);
    }
}

void PigFarm::startInfect(){
    unsigned int seed=QTime(0,0,0).secsTo(QTime::currentTime());
    qsrand(seed*seed*1309);
    int temp=qrand()%100;
    pen[temp]->isInfected=true;
    pen[temp]->setInfect();
}

void PigFarm::infect_betweenPen(){
    //qDebug()<<"enter_infect_betweenPen";
    unsigned int seed=QTime(0,0,0).secsTo(QTime::currentTime());
    for(int i=0;i<100;++i){
        if(pen[i]->isInfected){
            if(pen[i]->isIsolated==false){
                if(i-1>0&&pen[i-1]->isInfected==false){
                    qsrand(seed*seed*seed);
                    if(qrand()%101<=15){
                        pen[i-1]->isInfected=true;
                        //qDebug()<<"i-1="<<i-1;
                        pen[i-1]->setInfect();
                    }
                    ++seed;
                }
                if(i+1<100&&pen[i+1]->isInfected==false){
                    qsrand(seed*seed*seed);
                    if(qrand()%101<=15){
                        pen[i+1]->isInfected=true;
                        //qDebug()<<"i+1="<<i+1;
                        pen[i+1]->setInfect();
                    }
                    ++seed;
                }
            }
            //qDebug()<<"i="<<i;
            pen[i]->infect_inPen();
        }
    }
}

int PigFarm::showNumAll(){
    unsigned int sum=0;
    for(int i=0;i<100;++i){
        sum+=pen[i]->showNumAll();
    }
    return sum;
}

int PigFarm::showInfectedNumAll(){
    unsigned int infected_sum=0;
    for(int i=0;i<100;++i){
        infected_sum+=pen[i]->showInfected_pigNum();
    }
    return infected_sum;
}

void PigFarm::showPigFarmStatus_whenInfecting(QString &str){
    str+="猪场中猪总数：";
    str+=QString::number(showNumAll());
    str+="\n猪场中被感染猪总数：";
    str+=QString::number(showInfectedNumAll());
    str+="\n感染率：";
    str+=QString::number((float(showInfectedNumAll())/showNumAll())*100);
    str+="%\n<!-----分割线----->";
    for(int i=0;i<100;++i){
        str+="\n猪栏编号：";
        str+=QString::number(i);
        str+="\n是否有感染的猪：";
        if(pen[i]->isInfected){
            str+="是";
        }
        else{
            str+="否";
        }
        str+="\n是否被隔离：";
        if(pen[i]->isIsolated){
            str+="是";
        }
        else{
            str+="否";
        }
        str+="\n猪总数：";
        str+=QString::number(pen[i]->showNumAll());
        str+="\n被感染猪数量：";
        str+=QString::number(pen[i]->showInfected_pigNum());
        str+="\n--------------------";
    }
}

void PigFarm::isolateAllInfectedPen(){
    for(int i=0;i<100;++i){
        if(pen[i]->isInfected){
            pen[i]->isIsolated=true;
        }
    }
}

void PigFarm::isolateInfectedPenByNum(int num){
    if(num>=0&&num<100){
        pen[num]->isIsolated=true;
    }
}

bool PigFarm::isInfectAllCanBeInfected(){
    bool flag=true;
    for(int i=0;i<100;++i){
        if(pen[i]->isInfected==true){
            if(pen[i]->isIsolated==false){
                flag=false;
                break;
            }
            else{
                if(pen[i]->showInfected_pigNum()<pen[i]->showNumAll()){
                    flag=false;
                    break;
                }
            }
        }
    }
    return flag;
}

int PigFarm::showPigBreedInPen(int penID, int pigID){
    return pen[penID]->showPigBreed(pigID);
}
