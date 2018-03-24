#pragma once

#include <QtWidgets/QMainWindow>
#include <list>
#include <string>
#include <position.h>
#include <color.h>
#include <record.h>
#include "ui_app.h"

namespace board { class IBoard; }
namespace engine { class IEngine; }

class app : public QMainWindow
{
    Q_OBJECT
public:
    enum class AIColorSetting { Red, Blue, None };
    enum class PlayerColorSetting { Red, Blue, Auto };
public:
    app(QWidget *parent = Q_NULLPTR);
    static const int w { 720 };
    engine::IEngine *pEngine { nullptr };


public slots:
    void onCanvasValidClick(int row, int col);
    void onNew();
    void onOpen();
    void onSave();
    void onRestart();
    void onAIMove();
    void onAIStop();
    void onTakeBack();
    void onView();
    void onAction5();
    void onAction7();
    void onAction9();
    void onAction11();
    void onAction13();
    void onAction15();
    void onAction17();
    void onAction19();
    void onActionAIRed();
    void onActionAIBlue();
    void onActionAINone();
    void onActionPlayerRed();
    void onActionPlayerBlue();
    void onActionPlayerAuto();
    
protected:
    void paintEvent(QPaintEvent *event);

private:
    void PlayerSetPiece(int row, int col);
    void AISetPiece(int row, int col);
    void RecSetPiece(int row, int col);

    void AIMove();
    void AIStop();

    color::Color getAIColor();
    color::Color getPlayerColor();
    void setAIColor(AIColorSetting);
    void setPlayerColor(PlayerColorSetting);
    void changeBoardsize(int boardsize = 0);
    void updateBoard();
    void appendText(const char *text, QColor color = Qt::black);
    void appendText(const char *text, color::Color color);
    void appendText(std::string &str, QColor color = Qt::black);
    void appendText(std::string &str, color::Color color);
    void setPiece(int row, int col, color::Color color);
    void resetPiece(int row, int col);

private:
    board::IBoard *_pBoard { nullptr };
    record::Record _rec;

    AIColorSetting _acs { AIColorSetting::None };
    PlayerColorSetting _pcs { PlayerColorSetting::Auto };

    Ui::appClass ui;
};
