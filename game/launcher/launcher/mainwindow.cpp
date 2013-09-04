#include "mainwindow.h"
#include <QtGui>    
  
IrregularWidget::IrregularWidget(QWidget *parent)    
: QWidget(parent, Qt::FramelessWindowHint)    
{    
    setWindowTitle("Irregular widget");    
    //加载一幅有部分区域是透明的图片作为程序的界面    
    m_Pixmap.load("E:\\WORKSPACE\\zeus-cocos2dx\\game\\ggstudio_game\\bin\\data\\map\\002-Woods01.png");    
    resize( m_Pixmap.size() );    
    //不规则窗口的关键，将图片透明的地方设为穿透    
    setMask( m_Pixmap.mask() );    
}    
  
void IrregularWidget::mousePressEvent(QMouseEvent *event)    
{    
    //按住左键可以托动窗口，按下右键关闭程序    
    if(event->button() == Qt::LeftButton)    
    {    
        m_CurrentPos = event->globalPos() - frameGeometry().topLeft();    
        event->accept();    
    }    
    else if(event->button() == Qt::RightButton)    
        close();    
}    
  
void IrregularWidget::mouseMoveEvent(QMouseEvent *event)    
{    
    if (event->buttons() && Qt::LeftButton)    
    {    
        move(event->globalPos() - m_CurrentPos);    
        event->accept();    
    }    
}    
  
void IrregularWidget::paintEvent(QPaintEvent *event)    
{    
    QPainter painter(this);    
    painter.drawPixmap(0, 0, m_Pixmap);    
}    
  
void IrregularWidget::leaveEvent(QEvent *event)    
{    
    //鼠标离开窗口时是普通的指针    
    setCursor(Qt::ArrowCursor);    
}    
  
void IrregularWidget::enterEvent(QEvent *event)    
{    
    //鼠标留在窗口上时是一个手指    
    setCursor(Qt::PointingHandCursor);    
}    
