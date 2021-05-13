#ifndef VISUAL_PIG_H
#define VISUAL_PIG_H

#include <QLabel>

class visual_pig : public QLabel
{
    Q_OBJECT
public:
    explicit visual_pig(QWidget *parent = nullptr,int penID=0,int pigID=0);

signals:
    void clicked(int penID,int pigID);
protected:
    void mousePressEvent(QMouseEvent *ev) override;
private:
    int penID=0;
    int pigID=0;
};

#endif // VISUAL_PIG_H
