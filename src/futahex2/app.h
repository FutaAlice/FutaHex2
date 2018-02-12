#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_app.h"

class app : public QMainWindow
{
    Q_OBJECT
public:
    app(QWidget *parent = Q_NULLPTR);
    static const int w { 720 };
protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::appClass ui;
    int m_nSize { 11 };
};
