#include "menu.h"
#include <QDebug>

Menu::Menu(QWidget *parent)
{
    widget = new QGroupBox(parent);
    l = new QVBoxLayout;
    widget->setLayout(l);
}

void Menu::addButton(QPushButton *pb)
{
    l->addWidget(pb);
}

QGroupBox *Menu::getWidget()
{
    return widget;
}

QVBoxLayout *Menu::getLayout()
{
    return l;
}

Menu::~Menu()
{
    delete widget;
}
