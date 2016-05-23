#include "getip.h"
#include <QLayout>

GetIp::GetIp()
{
    grp = new QGroupBox;
    QGridLayout *gl = new QGridLayout;
    QHBoxLayout *hl = new QHBoxLayout;

    QFont f("times", 20);

    QLabel *lbl = new QLabel("enter ip");
    lbl->setFont(f);
    txt = new QLineEdit;
    hl->addWidget(lbl, 1);
    hl->addWidget(txt, 4);

    QPushButton *btn = new QPushButton("OK");
    btn->setFont(f);
    gl->addLayout(hl, 1, 0);
    gl->addWidget(btn, 2, 0);
    gl->setRowStretch(0, 10);
    gl->setRowStretch(1, 1);
    gl->setRowStretch(2, 1);

    grp->setLayout(gl);

    connect(btn, SIGNAL(clicked(bool)), this, SLOT(slotOnClick()));
}

void GetIp::slotOnClick()
{
    QString str = txt->text();
    txt->clear();
    emit clickButton(str);
}

void GetIp::enterPress()
{
    if (txt->hasFocus())
    {
        slotOnClick();
    }
}

QGroupBox *GetIp::getWidget()
{
    return grp;
}

GetIp::~GetIp()
{
    delete grp;
}
