#ifndef DROPSHADOWWIDGET_H
#define DROPSHADOWWIDGET_H


#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QMainWindow>
#include <QMouseEvent>
#include <qmath.h>

/*
 * 对话框基类重写
*/
class DropShadowWidget : public QDialog  //
{
    Q_OBJECT

public:

    explicit DropShadowWidget(QWidget *parent = 0);
    ~DropShadowWidget();

protected:

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *);

private:

    QPoint move_point; //移动的距离
    bool mouse_press; //按下鼠标左键

};

#endif // DROPSHADOWWIDGET_H
