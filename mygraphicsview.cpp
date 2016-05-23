#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView()
{

}

void MyGraphicsView::resizeEvent(QResizeEvent *event)
{
    fitInView(sceneRect());
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
        emit enterPress();
    QGraphicsView::keyPressEvent(event);
}
