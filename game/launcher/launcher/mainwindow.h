#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class IrregularWidget;
}

class IrregularWidget : public QWidget    
{    
    Q_OBJECT    
public:    
    IrregularWidget(QWidget *parent = 0);    
  
protected:    
    void mousePressEvent(QMouseEvent *event);    
    void mouseMoveEvent(QMouseEvent *event);    
    void paintEvent(QPaintEvent *event);    
    void enterEvent(QEvent *event);    
    void leaveEvent(QEvent *event);    
  
private:    
    QPoint m_CurrentPos;    
    QPixmap m_Pixmap;    
};

#endif // MAINWINDOW_H
