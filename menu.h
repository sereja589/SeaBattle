#ifndef MENU_H
#define MENU_H

#include <QPushButton>
#include <QGroupBox>
#include <QLayout>

class Menu
{
public:
    Menu(QWidget *parent = 0);
    ~Menu();
    void addButton(QPushButton *pb);
    QGroupBox *getWidget();
    QVBoxLayout *getLayout();

private:
    QGroupBox *widget;
    QVBoxLayout *l;

};

#endif // MENU_H
