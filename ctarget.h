#ifndef CTARGET_H
#define CTARGET_H
#include <QWidget>
#include <QFrame>
#include <qevent.h>

class CTarget : public QFrame
{
    Q_OBJECT
public:
    explicit CTarget(QWidget *parent = 0);
    float m_lat,m_lon;
    QString id;
    bool selected ;
    void setCoordinates(float lat,float lon);
    void setPosistion(short x, short y);
protected:

    void highLight();
    void resetView();
    void hoverEnter(QHoverEvent *event);
    void hoverLeave(QHoverEvent *event);
    void hoverMove(QHoverEvent *event);
    void OnClick();

    bool event(QEvent *event);


signals:

public slots:


};

#endif // CTARGET_H
