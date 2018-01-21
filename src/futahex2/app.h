#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_app.h"

class app : public QMainWindow
{
    Q_OBJECT

public:
    app(QWidget *parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::appClass ui;
};
