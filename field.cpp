#include <QtAlgorithms>
#include <QtMath>
#include <QDebug>
#include <QPair>

#include "field.h"
#include "cell.h"
#include "ship.h"

Field::Field(qreal _size, int n) : _size(_size), n(n)
{
    cells.resize(n);
    for (int i =0; i < n; ++i)
        cells[i].resize(n);
    usd.resize(n);
    for (int i = 0; i < n; ++i)
        usd[i].resize(n);
    isDamage.resize(n);
    for (int i = 0; i < n; ++i)
        isDamage[i].resize(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
        {
            usd[i][j] = false;
            isDamage[i][j] = false;
        }

    qreal x1 = -_size / 2;
    qreal y1 = -_size / 2;
    qreal d = _size / n;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
        {
            Cell *c = new Cell(d, j, i);
            cells[i][j] = c;
            addToGroup(c);
            c->setPos(x1 + i * d + d / 2, y1 + j * d + d / 2);
        }
}

QRectF Field::boundingRect() const
{
    return QRectF(-_size / 2 - 1, -_size / 2 - 1, _size + 2, _size + 2);
}

void Field::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(painter->Antialiasing);
    QPen p(Qt::red);
    p.setWidth(2);
    painter->setPen(p);
    qreal x1 = -_size / 2;
    qreal y1 = -_size / 2;
    qreal x2 = _size / 2;
    qreal y2 = _size / 2;
    qreal d = _size / n;

    for (int i = 0; i <= n; ++i)
    {
        qreal currY = y1 + i * d;
        painter->drawLine(x1, currY, x2, currY);
    }
    for (int i = 0; i <= n; ++i)
    {
        qreal currX = x1 + i * d;
        painter->drawLine(currX, y1, currX, y2);
    }
}

bool Field::setShip(int x0, int y0, int w, int h)
{
    if (x0 + w > n || y0 + h > n)
        return false;

    int x1 = qMax(x0 - 1, 0);
    int x2 = qMin(x0 + w, n - 1);
    int y1 = qMax(y0 - 1, 0);
    int y2 = qMin(y0 + h, n - 1);

    bool f = true;
    for (int i = x1; i <= x2 && f; ++i)
        for (int j = y1; j <= y2 && f; ++j)
        {
            if (usd[i][j])
            {
                f = false;
                break;
            }
        }

    if (f)
    {
        for (int i = x0; i < x0 + w; ++i)
            for (int j = y0; j < y0 + h; ++j)
                usd[i][j] = true;
    }
    return f;
}

QRect Field::setShip(Ship *ship)
{
    QRectF shipRect;
    qreal x = ship->getX();
    int wShip, hShip;
    if (ship->getOrientation() == ship->HORIZONTAL)
    {
        shipRect =  QRectF(ship->pos().x() - ship->getN() * x / 2, ship->pos().y() - x / 2,
                           ship->getN() * x, x);
        wShip = ship->getN();
        hShip = 1;
    }
    else
    {
        shipRect = QRectF(ship->pos().x() - x / 2, ship->pos().y() - ship->getN() * x / 2,
                          x, ship->getN() * x);
        wShip = 1;
        hShip = ship->getN();
    }

    static const qreal eps = 0.00001;

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
        {
            QPointF topleft = cells[i][j]->scenePos() - QPointF(x / 2, x / 2);
            QPointF d = topleft - shipRect.topLeft();
            if (qAbs(d.x()) < x / 2 - eps && qAbs(d.y()) < x / 2 - eps)
            {
                if (setShip(i, j, wShip, hShip))
                {
                    shipRect.setTopLeft(shipRect.topLeft() + d);
                    shipRect.setBottomRight(shipRect.bottomRight() + d);
                    ship->setPos(shipRect.center());
                    return QRect(i, j, wShip, hShip);
                }
                return QRect(0, 0, 0, 0);
            }
        }

    return QRect(0, 0, 0, 0);
}

void Field::unsetShip(int x0, int y0, int w, int h)
{
    for (int i = x0; i < x0 + w; ++i)
        for (int j = y0; j < y0 + h; ++j)
            usd[i][j] = false;
}

bool Field::changeOrientation(int x0, int y0, int w, int h)
{
    if (setShip(x0, y0, h, w))
    {
        return true;
    }
    else
    {
        setShip(x0, y0, w, h);
        return false;
    }
}

QRect Field::getShip(int x, int y)
{
    int x1 = x, x2 = x;
    int y1 = y, y2 = y;
    while (x1 - 1 >= 0 && usd[x1 - 1][y])
        --x1;
    while (x2 + 1 < n && usd[x2 + 1][y])
        ++x2;
    while (y1 - 1 >= 0 && usd[x][y1 - 1])
        --y1;
    while (y2 + 1 < n && usd[x][y2 + 1])
        ++y2;
    return QRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
}

QRect Field::getOpponentShip(int x, int y)
{
    int x1 = x, x2 = x;
    int y1 = y, y2 = y;
    while (x1 - 1 >= 0 && isDamage[x1 - 1][y])
        --x1;
    while (x2 + 1 < n && isDamage[x2 + 1][y])
        ++x2;
    while (y1 - 1 >= 0 && isDamage[x][y1 - 1])
        --y1;
    while (y2 + 1 < n && isDamage[x][y2 + 1])
        ++y2;
    return QRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
}

int Field::damage(int x, int y)
{
    if (usd[x][y])
        cells[x][y]->setIsCross();
    else
        cells[x][y]->setIsPoint();
    isDamage[x][y] = true;
    if (!usd[x][y])
        return 0;

    QRect rct = getShip(x, y);
    int ret = 2;
    for (int i = rct.left(); i <= rct.right(); ++i)
        for (int j = rct.top(); j <= rct.bottom(); ++j)
            if (usd[i][j] && !isDamage[i][j])
                ret = 1;
    return ret;
}

void Field::setIsPass(int x, int y)
{
    cells[x][y]->setIsPoint();
    isDamage[x][y] = 1;
}

void Field::setIsDamage(int x, int y)
{
    cells[x][y]->setIsCross();
    isDamage[x][y] = 2;
}

bool Field::getIsDamage(int x, int y)
{
    return isDamage[x][y];
}

void Field::addShip(QRect r)
{
    QGraphicsRectItem *item = new QGraphicsRectItem;
    addToGroup(item);
    qreal d = _size / n;
    qreal x0 = -_size / 2;
    item->setPos(x0 + r.x() * d, x0 + r.y() * d);
    item->setRect(0, 0, r.width() * d, r.height() * d);
    QPen p(Qt::blue);
    p.setWidth(3);
    QBrush br(QColor(170, 170, 255, 100));
    item->setPen(p);
    item->setBrush(br);
}

void Field::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF p = event->pos();
    qreal d = _size / n;
    qreal x0 = -_size / 2;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
        {
            QRectF r(-_size / 2 + i * d, -_size / 2 + j * d, d, d);
            if (r.contains(p))
            {
                emit clicked(i, j);
                break;
            }
        }
}

bool Field::isSetShip(int x0, int y0, Ship *s, Orientation ornt)
{
    int w, h;
    if (ornt == HORIZONTAL)
        w = s->getN(), h = 1;
    else
        w = 1, h = s->getN();
    if (x0 + w >= n || y0 + h >= n)
        return false;
    int x1 = qMax(x0 - 1, 0);
    int x2 = qMin(x0 + w, n);
    int y1 = qMax(y0 - 1, 0);
    int y2 = qMin(y0 + h, n);
    for (int i = x1; i <= x2; ++i)
        for (int j = y1; j <= y2; ++j)
            if (usd[i][j])
            {
                return false;
            }
    return true;
}

bool Field::setShip(int x0, int y0, Ship *s, Orientation ornt)
{
    if (!isSetShip(x0, y0, s, ornt))
        return false;
    int w, h;
    if (ornt == HORIZONTAL)
        w = s->getN(), h = 1;
    else
        w = 1, h = s->getN();
    setShip(x0, y0, w, h);
    qreal d = _size / n;
    qreal xl = -_size / 2;
    if ((s->getOrientation() == s->VERTICAL && ornt == HORIZONTAL) || (s->getOrientation() == s->HORIZONTAL && ornt == VERTICAL))
        s->changeOrientation();
    QPointF ps(xl + x0 * d + d * w / 2, xl + y0 * d + d * h / 2);
    s->setPos(mapToScene(ps));
    s->setRect(QRect(x0, y0, w, h));
    return true;
}

void Field::setShipsRandom(QVector<Ship*> *v)
{
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            usd[i][j] = false;
    for (int i = 0; i < (int)v->size(); ++i)
    {
        Ship *s = (*v)[i];
        QVector<QPair<QPoint, Orientation> > v1;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
            {
                if (isSetShip(i, j, s, HORIZONTAL))
                    v1.push_back(QPair<QPoint, Orientation>(QPoint(i, j), HORIZONTAL));
                if (isSetShip(i, j, s, VERTICAL))
                    v1.push_back(QPair<QPoint, Orientation>(QPoint(i, j), VERTICAL));
            }
        if (v1.empty())
        {
            setShipsRandom(v);
            return;
        }
        int r = qrand() % v1.size();
        setShip(v1[r].first.x(), v1[r].first.y(), s, v1[r].second);
    }
}
