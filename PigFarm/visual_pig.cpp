#include "visual_pig.h"
#include<QMouseEvent>
visual_pig::visual_pig(QWidget *parent,int penID,int pigID,int breed) : QLabel(parent)
{
    this->penID=penID;
    this->pigID=pigID;
    this->resize(60,60);
    if(breed==0){
        this->setPixmap(QPixmap("://sources/BlackPig.png"));
    }
    else if(breed==1){
        this->setPixmap(QPixmap("://sources/littlePig.png"));
    }
    else if(breed==2){
        this->setPixmap(QPixmap("://sources/bigWritePig.png"));
    }
}

void visual_pig::mousePressEvent(QMouseEvent *ev){
    emit this->clicked(penID,pigID);
}
