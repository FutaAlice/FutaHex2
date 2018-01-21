#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_app.h"

class app : public QMainWindow
{
    Q_OBJECT

public:
    app(QWidget *parent = Q_NULLPTR);

private:
    Ui::appClass ui;
};
