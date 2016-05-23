#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>
#include <QKeyEvent>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

signals:
    void enterPress();
};

#endif // MYGRAPHICSVIEW_H
