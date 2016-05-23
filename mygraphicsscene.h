#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

#include "menu.h"

class MyGraphicsScene : public QGraphicsScene
{
public:
    MyGraphicsScene();
    MyGraphicsScene(const QRectF &sceneRect, QObject *parent = 0);
    void setMenu(Menu *m, int hMargin, int vMargin);
    void removeMenu();

private:
    Menu *menu;
    QGraphicsProxyWidget *pmenu;
};

#endif // MYGRAPHICSSCENE_H
