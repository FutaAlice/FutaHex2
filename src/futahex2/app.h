#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_app.h"

namespace board { class IBoard; }

class app : public QMainWindow
{
    Q_OBJECT
public:
    app(QWidget *parent = Q_NULLPTR);
    static const int w { 720 };

public slots:
    void changeBoardsize(int boardsize = 0);
    void changeBoardsize5();
    void changeBoardsize7();
    void changeBoardsize9();
    void changeBoardsize11();
    void changeBoardsize13();
    void changeBoardsize15();
    void changeBoardsize17();
    void changeBoardsize19();
    
protected:
    void paintEvent(QPaintEvent *event);

private:
    board::IBoard *_pBoard { nullptr };

    Ui::appClass ui;
};
