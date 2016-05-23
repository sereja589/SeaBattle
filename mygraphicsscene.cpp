#include <QWidget>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QLayout>

#include "mygraphicsscene.h"

MyGraphicsScene::MyGraphicsScene()
{
    menu = 0;
}

MyGraphicsScene::MyGraphicsScene(const QRectF &sceneRect, QObject *parent) : QGraphicsScene(sceneRect, parent)
{
    menu = 0;
}

void MyGraphicsScene::setMenu(Menu *m, int hMargin, int vMargin)
{
    if (!menu)
    {
        menu = m;
        QGroupBox *w = m->getWidget();
        pmenu = addWidget(w);
        pmenu->setGeometry(sceneRect());
    }
}

void MyGraphicsScene::removeMenu()
{
    if (menu)
    {
        removeItem(pmenu);
        menu = 0;
    }
}
