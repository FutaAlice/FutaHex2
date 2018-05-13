#pragma once
#include <list>
#include <string>
#include <QtWidgets/QMainWindow>
#include <fhutils/board.h>
#include <fhutils/iengine.h>
#include <fhutils/record.h>
#include "ui_app.h"

class app : public QMainWindow {
    Q_OBJECT
public:
    enum class AIColorSetting { Red, Blue, None };
    enum class PlayerColorSetting { Red, Blue, Auto };
public:
    app(QWidget *parent = Q_NULLPTR);
    static const int w{ 720 };
    fhutils::iengine::IEngine *pEngine{ nullptr };

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
    void onAction4();
    void onAction5();
    void onAction6();
    void onAction7();
    void onAction8();
    void onAction9();
    void onAction10();
    void onAction11();
    void onAction12();
    void onAction13();
    void onAction14();
    void onAction15();
    void onAction16();
    void onAction17();
    void onAction18();
    void onAction19();
    void onActionAIRed();
    void onActionAIBlue();
    void onActionAINone();
    void onActionPlayerRed();
    void onActionPlayerBlue();
    void onActionPlayerAuto();
    void onEnableCostomizePlayerColor(bool checked);
    void onEnableBeep(bool checked);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void PlayerSetPiece(int row, int col);
    void AISetPiece(int row, int col);
    void RecSetPiece(int row, int col);

    void AIMove();
    void AIStop();

    fhutils::board::Color getAIColor();
    fhutils::board::Color getPlayerColor();
    void setAIColor(AIColorSetting);
    void setPlayerColor(PlayerColorSetting);
    void changeBoardsize(int boardsize = 0);
    void updateBoard();
    void appendText(const char *text, QColor color = Qt::black);
    void appendText(const char *text, fhutils::board::Color color);
    void appendText(std::string &str, QColor color = Qt::black);
    void appendText(std::string &str, fhutils::board::Color color);
    void setPiece(int row, int col, fhutils::board::Color color);
    void resetPiece(int row, int col);

private:
    fhutils::board::IBoard *_pBoard{ nullptr };
    fhutils::record::Record _rec;

    AIColorSetting _acs{ AIColorSetting::None };
    PlayerColorSetting _pcs{ PlayerColorSetting::Auto };

    bool _bEnableBeep{ false };
    Ui::appClass ui;
};
