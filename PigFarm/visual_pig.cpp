#include "visual_pig.h"
#include<QMouseEvent>
visual_pig::visual_pig(QWidget *parent,int penID,int pigID) : QLabel(parent)
{
    this->penID=penID;
    this->pigID=pigID;
}

void visual_pig::mousePressEvent(QMouseEvent *ev){
    emit this->clicked(penID,pigID);
}
