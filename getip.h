#ifndef GETIP_H
#define GETIP_H

#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>

class GetIp : public QObject
{
    Q_OBJECT
public:
    GetIp();
    QGroupBox *getWidget();
    ~GetIp();

private:
    QGroupBox *grp;
    QLineEdit *txt;

signals:
    void clickButton(QString);

private slots:
    void slotOnClick();
    void enterPress();
};

#endif // GETIP_H
