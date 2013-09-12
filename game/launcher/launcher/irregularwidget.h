#ifndef IRREGULARWINDOW_H
#define IRREGULARWINDOW_H

#include <QtGui>
#include <QtWidgets>

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
    QPixmap m_Pixmap_ButtonNormal;
    QPixmap m_Pixmap_ButtonHover;
    QPixmap m_Pixmap_ButtonDown;
};

#endif // IRREGULARWINDOW_H
