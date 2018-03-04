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
    void onAction5();
    void onAction7();
    void onAction9();
    void onAction11();
    void onAction13();
    void onAction15();
    void onAction17();
    void onAction19();
    
protected:
    void paintEvent(QPaintEvent *event);

private:
    void onAction(int boardsize = 0);

private:
    board::IBoard *_pBoard { nullptr };

    Ui::appClass ui;
};
