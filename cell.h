#ifndef CELL_H
#define CELL_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>

class Cell : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    Cell(qreal _size, int row, int column);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void setIsPoint();
    void setIsCross();

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    qreal _size;
    bool isCross;
    bool isPoint;
    int row;
    int column;

signals:
    //void clicked(int, int);
};

#endif // CELL_H
