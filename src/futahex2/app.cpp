#include "app.h"
#include <ostream>
#include <QPainterPath>
#include <QPainter>
#include <board.h>
using namespace std;
using namespace board;

app::app(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.centralWidget->setLayout(ui.mainLayout);
    ui.link_red->setDisplayMethod(Canvas::DisplayMethod::LinkR);
    ui.link_blue->setDisplayMethod(Canvas::DisplayMethod::LinkB);
    resize(w * 16 / 9, w);

    QObject::connect(ui.canvas, SIGNAL(clickEmptyPoint(int, int)), this, SLOT(setPiece(int, int)));
    QObject::connect(ui.action5, SIGNAL(triggered()), this, SLOT(onAction5()));
    QObject::connect(ui.action7, SIGNAL(triggered()), this, SLOT(onAction7()));
    QObject::connect(ui.action9, SIGNAL(triggered()), this, SLOT(onAction9()));
    QObject::connect(ui.action11, SIGNAL(triggered()), this, SLOT(onAction11()));
    QObject::connect(ui.action13, SIGNAL(triggered()), this, SLOT(onAction13()));
    QObject::connect(ui.action15, SIGNAL(triggered()), this, SLOT(onAction15()));
    QObject::connect(ui.action17, SIGNAL(triggered()), this, SLOT(onAction17()));
    QObject::connect(ui.action19, SIGNAL(triggered()), this, SLOT(onAction19()));

    QObject::connect(ui.actionAIRed, SIGNAL(triggered()), this, SLOT(onActionAIRed()));
    QObject::connect(ui.actionAIBlue, SIGNAL(triggered()), this, SLOT(onActionAIBlue()));
    QObject::connect(ui.actionAINone, SIGNAL(triggered()), this, SLOT(onActionAINone()));

    QObject::connect(ui.actionPlayerRed, SIGNAL(triggered()), this, SLOT(onActionPlayerRed()));
    QObject::connect(ui.actionPlayerBlue, SIGNAL(triggered()), this, SLOT(onActionPlayerBlue()));
    QObject::connect(ui.actionPlayerAuto, SIGNAL(triggered()), this, SLOT(onActionPlayerAuto()));

    auto add_default_icon = [&, this](auto style, auto text) {
        auto icon = QApplication::style()->standardIcon(style);
        auto action = new QAction(icon, text, this);
        ui.mainToolBar->addAction(action);
        return action;
    };

    QObject::connect(add_default_icon(QStyle::SP_DirIcon, "open"),
                     SIGNAL(triggered()), this, SLOT(onOpen()));
    QObject::connect(add_default_icon(QStyle::SP_DialogSaveButton, "save"),
                     SIGNAL(triggered()), this, SLOT(onSave()));
    QObject::connect(add_default_icon(QStyle::SP_BrowserReload, "restart"),
                     SIGNAL(triggered()), this, SLOT(onRestart()));
    ui.mainToolBar->addSeparator();

    QObject::connect(add_default_icon(QStyle::SP_MediaPlay, "AI move"),
                     SIGNAL(triggered()), this, SLOT(onAIMove()));
    QObject::connect(add_default_icon(QStyle::SP_MediaSeekBackward, "take back"),
                     SIGNAL(triggered()), this, SLOT(onTakeBack()));
    ui.mainToolBar->addSeparator();

    QObject::connect(add_default_icon(QStyle::SP_FileDialogInfoView, "view"),
                     SIGNAL(triggered()), this, SLOT(onView()));
}

void app::paintEvent(QPaintEvent * event)
{
}

void app::setPiece(int row, int col)
{
    if (!_pBoard)
        return;

    Color color = getPlayerColor();
    (*_pBoard)(row, col) = color;

    _rec.push_back(pos_t(row, col, _pBoard->boardsize()));

    ostringstream oss;
    oss << "move: " << "(row " << row << ", col " << col << ")";
    appendText(oss.str(), color);

    updateBoard();
}

void app::resetPiece(int row, int col)
{
    if (!_pBoard)
        return;

    (*_pBoard)(row, col) = Color::Empty;
    
    updateBoard();
}

Color app::getAIColor()
{
    switch (_acs)
    {
    case AIColorSetting::Red:
        return Color::Red;
    case AIColorSetting::Blue:
        return Color::Blue;
    default:
        return Color::Empty;
    }
}

Color app::getPlayerColor()
{
    switch (_pcs)
    {
    case PlayerColorSetting::Red:
        return Color::Red;
    case PlayerColorSetting::Blue:
        return Color::Blue;
    default:
        if (_pBoard)
            return _pBoard->color();
        else
            return Color::Empty;
    }
}

void app::setAIColor(AIColorSetting acs)
{
    auto & actions = ui.menuColorAI->actions();
    int index = 0;
    for (auto action : actions)
    {
        action->setChecked(index++ == static_cast<int>(acs));
    }
    _acs = acs;
}

void app::setPlayerColor(PlayerColorSetting pcs)
{
    auto & actions = ui.menuColorPlayer->actions();
    int index = 0;
    for (auto action : actions)
    {
        action->setChecked(index++ == static_cast<int>(pcs));
    }
    _pcs = pcs;
}

void app::changeBoardsize(int boardsize)
{
    if (boardsize <= 0)
    {
        if (!_pBoard)
            return;
        boardsize = _pBoard->boardsize();
    }

    auto & actions = ui.menuBoardsize->actions();
    for (auto action : actions)
    {
        auto text = action->text().toInt();
        action->setChecked(boardsize == text);
    }

    delete _pBoard;
    _pBoard = IBoard::create(boardsize);

    updateBoard();
}

void app::updateBoard()
{
    ui.canvas->updateBoard(_pBoard);
    ui.link_red->updateBoard();
    ui.link_blue->updateBoard();
}

void app::appendText(const char * text, QColor color)
{
    ui.textBrowser->setTextColor(color);
    ui.textBrowser->append(text);
}

void app::appendText(const char * text, Color color)
{
    static const map<Color, const QColor> color_map =
    {
        { Color::Red, QColor(Qt::red) },
        { Color::Blue, QColor(Qt::blue) },
        { Color::Empty, QColor(Qt::black) }
    };
    QColor qc = color_map.find(color)->second;
    appendText(text, qc);
}

void app::appendText(string & str, QColor color)
{
    appendText(str.c_str(), color);
}

void app::appendText(string & str, color::Color color)
{
    appendText(str.c_str(), color);
}

void app::onAction5() { changeBoardsize(5); }
void app::onAction7() { changeBoardsize(7); }
void app::onAction9() { changeBoardsize(9); }
void app::onAction11() { changeBoardsize(11); }
void app::onAction13() { changeBoardsize(13); }
void app::onAction15() { changeBoardsize(15); }
void app::onAction17() { changeBoardsize(17); }
void app::onAction19() { changeBoardsize(19); }

void app::onActionAIRed() { setAIColor(AIColorSetting::Red); }
void app::onActionAIBlue() { setAIColor(AIColorSetting::Blue); }
void app::onActionAINone() { setAIColor(AIColorSetting::None); }
void app::onActionPlayerRed() { setPlayerColor(PlayerColorSetting::Red); }
void app::onActionPlayerBlue() { setPlayerColor(PlayerColorSetting::Blue); }
void app::onActionPlayerAuto() { setPlayerColor(PlayerColorSetting::Auto); }

void app::onOpen()
{
}

void app::onSave()
{
}

void app::onSaveAs()
{
}

void app::onRestart()
{
    auto size = _pBoard ? _pBoard->boardsize() : 11;
    changeBoardsize(size);
}

void app::onAIMove()
{
}

void app::onTakeBack()
{
    if (_rec.empty())
        return;
    pos_t pos = _rec.back();
    resetPiece(pos.row, pos.col);
    _rec.pop_back();
}

void app::onView()
{
    bool arrow = ui.canvas->getLineWithArrow();
    ui.canvas->setLineWithArrow(!arrow);
    ui.link_red->setLineWithArrow(!arrow);
    ui.link_blue->setLineWithArrow(!arrow);
    updateBoard();
}
