#ifndef FIELD_H
#define FIELD_H

#include <QGraphicsItemGroup>
#include <QGraphicsSceneEvent>
#include <QVector>

class Cell;
class Ship;

class Field : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    enum Orientation {VERTICAL, HORIZONTAL};

    Field(qreal _size, int n = 8);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QRect setShip(Ship *ship);
    void unsetShip(int x0, int y0, int w, int h);
    bool setShip(int x0, int y0, int w, int h);
    void setShipsRandom(QVector<Ship*> *v);
    bool setShip(int x0, int y0, Ship *s, Orientation ornt);
    bool isSetShip(int x0, int y0, Ship *s, Orientation ornt);
    bool changeOrientation(int x0, int y0, int w, int h);
    int damage(int x, int y);
    QRect getShip(int x, int y);
    bool getIsDamage(int x, int y);
    void addShip(QRect r);
    void setIsPass(int x, int y);
    void setIsDamage(int x, int y);
    QRect getOpponentShip(int x, int y);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QVector<QVector<bool> > usd;
    QVector<QVector<bool> > isDamage;
    QVector<QVector<Cell*> > cells;
    qreal _size;
    int n;

signals:
    void clicked(int x, int y);
};

#endif // FIELD_H
