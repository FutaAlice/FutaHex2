#include <ostream>
#include <sstream>
#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainterPath>
#include <QPainter>
#include <fhutils/save.h>
#include "app.h"
using namespace std;
using namespace fhutils::logger;
using namespace fhutils::board;
using namespace fhutils::record;
using namespace fhutils::iengine;
using namespace fhutils::save;

app::app(QWidget *parent)
    : QMainWindow(parent) {
    qRegisterMetaType<QTextCharFormat>("QTextCharFormat");
    qRegisterMetaType<QTextCursor>("QTextCursor");

    setWindowIcon(QIcon(":/resource/futahex2.ico"));

    ui.setupUi(this);
    ui.centralWidget->setLayout(ui.mainLayout);
    ui.link_red->setDisplayMethod(Canvas::DisplayMethod::LinkR);
    ui.link_blue->setDisplayMethod(Canvas::DisplayMethod::LinkB);
    resize(w * 16 / 9, w);

    QObject::connect(ui.canvas, SIGNAL(clickEmptyPoint(int, int)),
        this, SLOT(onCanvasValidClick(int, int)));
    QObject::connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(onNew()));
    QObject::connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
    QObject::connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(onSave()));
    QObject::connect(ui.actionenable_costomize_player_color, SIGNAL(toggled(bool)),
        this, SLOT(onEnableCostomizePlayerColor(bool)));
    QObject::connect(ui.actionenable_beep, SIGNAL(toggled(bool)),
        this, SLOT(onEnableBeep(bool)));
    QObject::connect(ui.action4, SIGNAL(triggered()), this, SLOT(onAction4()));
    QObject::connect(ui.action5, SIGNAL(triggered()), this, SLOT(onAction5()));
    QObject::connect(ui.action6, SIGNAL(triggered()), this, SLOT(onAction6()));
    QObject::connect(ui.action7, SIGNAL(triggered()), this, SLOT(onAction7()));
    QObject::connect(ui.action8, SIGNAL(triggered()), this, SLOT(onAction8()));
    QObject::connect(ui.action9, SIGNAL(triggered()), this, SLOT(onAction9()));
    QObject::connect(ui.action10, SIGNAL(triggered()), this, SLOT(onAction10()));
    QObject::connect(ui.action11, SIGNAL(triggered()), this, SLOT(onAction11()));
    QObject::connect(ui.action12, SIGNAL(triggered()), this, SLOT(onAction12()));
    QObject::connect(ui.action13, SIGNAL(triggered()), this, SLOT(onAction13()));
    QObject::connect(ui.action14, SIGNAL(triggered()), this, SLOT(onAction14()));
    QObject::connect(ui.action15, SIGNAL(triggered()), this, SLOT(onAction15()));
    QObject::connect(ui.action16, SIGNAL(triggered()), this, SLOT(onAction16()));
    QObject::connect(ui.action17, SIGNAL(triggered()), this, SLOT(onAction17()));
    QObject::connect(ui.action18, SIGNAL(triggered()), this, SLOT(onAction18()));
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
    QObject::connect(add_default_icon(QStyle::SP_MediaStop, "AI stop"),
        SIGNAL(triggered()), this, SLOT(onAIStop()));
    QObject::connect(add_default_icon(QStyle::SP_MediaSeekBackward, "take back"),
        SIGNAL(triggered()), this, SLOT(onTakeBack()));
    ui.mainToolBar->addSeparator();

    QObject::connect(add_default_icon(QStyle::SP_FileDialogInfoView, "view"),
        SIGNAL(triggered()), this, SLOT(onView()));
}

void app::PlayerSetPiece(int row, int col) {
    if (pEngine) {
        const char *err = "AI is thinking. This operation will be ignored";
        msg(Level::Warning) << err;
        QMessageBox message(QMessageBox::NoIcon, "Warning",
            err, QMessageBox::Ok);
        message.exec();
        return;
    }

    Color color = getPlayerColor();
    setPiece(row, col, color);
    if (_bEnableBeep)
        QApplication::beep();
    if (_acs != AIColorSetting::None &&
        getAIColor() == _pBoard->color())
        AIMove();
}

void app::AISetPiece(int row, int col) {
    Color color = getAIColor();
    setPiece(row, col, color);
    if (_bEnableBeep)
        QApplication::beep();
}

void app::RecSetPiece(int row, int col) {
    Color color = _pBoard->color();
    setPiece(row, col, color);
}

void app::AIMove() {
    if (!_pBoard)
        return;

    if (pEngine) {
        const char *err = "AI is busy. This operation will be ignored";
        msg(Level::Warning) << err;
        QMessageBox message(QMessageBox::NoIcon, "Warning",
            err, QMessageBox::Ok);
        message.exec();
        return;
    }

    Color color = _pBoard->color();

    EngineCfg cfg;
    cfg.colorAI = color;
    cfg.pBoard = _pBoard;

    pEngine->configure(cfg);
    pEngine->compute([](pos_t result, void *opaque)->void * {
        cout << result << endl;
        auto *pApp = static_cast<app *>(opaque);
        pApp->AISetPiece(result.row, result.col);
        return NULL;
    }, static_cast<void *>(this));
}

void app::AIStop() {
    if (!pEngine)
        return;
    pEngine->terminate();
}

void app::paintEvent(QPaintEvent * event) {
    if (!_pBoard) {
        QPainter painter(this);
        painter.drawText(200, 200, QStringLiteral("plz select boardsize!"));
    }
}

void app::setPiece(int row, int col, Color color) {
    if (!_pBoard)
        return;

    IBoard &refBoard = *_pBoard;
    ostringstream oss;
    if (Color::Empty != refBoard(row, col)) {
        oss << "warning, ignore this move." << endl
            << "location(" << row << ", " << col << ") "
            << "already been captured!";
        appendText(oss.str());
        return;
    }

    refBoard(row, col) = color;

    _rec.push_back(RecordData(row, col, _pBoard->boardsize(), color));

    oss << "move: " << xy2symbol(row, col, _pBoard->boardsize())
        << "(row " << row << ", col " << col << ")";
    appendText(oss.str(), color);

    updateBoard();
}

void app::resetPiece(int row, int col) {
    if (!_pBoard)
        return;

    (*_pBoard)(row, col) = Color::Empty;

    updateBoard();
}

void app::onCanvasValidClick(int row, int col) {
    PlayerSetPiece(row, col);
}

void app::onNew() {
    changeBoardsize(_pBoard ? 0 : 11);
}

Color app::getAIColor() {
    switch (_acs) {
    case AIColorSetting::Red:
        return Color::Red;
    case AIColorSetting::Blue:
        return Color::Blue;
    case AIColorSetting::None:
    default:
        assert(_pBoard);
        return _pBoard->color();
    }
}

Color app::getPlayerColor() {
    switch (_pcs) {
    case PlayerColorSetting::Red:
        return Color::Red;
    case PlayerColorSetting::Blue:
        return Color::Blue;
    case PlayerColorSetting::Auto:
    default:
        assert(_pBoard);
        return _pBoard->color();
    }
}

void app::setAIColor(AIColorSetting acs) {
    auto & actions = ui.menuColorAI->actions();
    int index = 0;
    for (auto action : actions) {
        action->setChecked(index++ == static_cast<int>(acs));
    }
    _acs = acs;
    if (_pBoard &&
        _acs != AIColorSetting::None &&
        getAIColor() == _pBoard->color())
        AIMove();
}

void app::setPlayerColor(PlayerColorSetting pcs) {
    auto & actions = ui.menuColorPlayer->actions();
    int index = 0;
    for (auto action : actions) {
        action->setChecked(index++ == static_cast<int>(pcs));
    }
    _pcs = pcs;
}

void app::changeBoardsize(int boardsize) {
    if (boardsize <= 0) {
        if (!_pBoard)
            return;
        boardsize = _pBoard->boardsize();
    }

    _rec.clear();
    _rec.boardsize(boardsize);
    auto & actions = ui.menuBoardsize->actions();
    for (auto action : actions) {
        auto text = action->text().toInt();
        action->setChecked(boardsize == text);
    }

    delete _pBoard;
    _pBoard = IBoard::create(boardsize);

    updateBoard();
}

void app::updateBoard() {
    ui.canvas->updateBoard(_pBoard);
    ui.link_red->updateBoard();
    ui.link_blue->updateBoard();
}

void app::appendText(const char * text, QColor color) {
    ui.textBrowser->setTextColor(color);
    ui.textBrowser->append(text);
}

void app::appendText(const char * text, Color color) {
    static const map<Color, const QColor> color_map =
    {
        { Color::Red, QColor(Qt::red) },
        { Color::Blue, QColor(Qt::blue) },
        { Color::Empty, QColor(Qt::black) }
    };
    QColor qc = color_map.find(color)->second;
    appendText(text, qc);
}

void app::appendText(string & str, QColor color) {
    appendText(str.c_str(), color);
}

void app::appendText(string & str, Color color) {
    appendText(str.c_str(), color);
}

void app::onAction4() { changeBoardsize(4); }
void app::onAction5() { changeBoardsize(5); }
void app::onAction6() { changeBoardsize(6); }
void app::onAction7() { changeBoardsize(7); }
void app::onAction8() { changeBoardsize(8); }
void app::onAction9() { changeBoardsize(9); }
void app::onAction10() { changeBoardsize(10); }
void app::onAction11() { changeBoardsize(11); }
void app::onAction12() { changeBoardsize(12); }
void app::onAction13() { changeBoardsize(13); }
void app::onAction14() { changeBoardsize(14); }
void app::onAction15() { changeBoardsize(15); }
void app::onAction16() { changeBoardsize(16); }
void app::onAction17() { changeBoardsize(17); }
void app::onAction18() { changeBoardsize(18); }
void app::onAction19() { changeBoardsize(19); }

void app::onActionAIRed() { setAIColor(AIColorSetting::Red); }
void app::onActionAIBlue() { setAIColor(AIColorSetting::Blue); }
void app::onActionAINone() { setAIColor(AIColorSetting::None); }
void app::onActionPlayerRed() { setPlayerColor(PlayerColorSetting::Red); }
void app::onActionPlayerBlue() { setPlayerColor(PlayerColorSetting::Blue); }
void app::onActionPlayerAuto() { setPlayerColor(PlayerColorSetting::Auto); }

void app::onEnableCostomizePlayerColor(bool checked) {
    ui.actionPlayerRed->setEnabled(checked);
    ui.actionPlayerBlue->setEnabled(checked);
    setPlayerColor(PlayerColorSetting::Auto);
}

void app::onEnableBeep(bool checked) {
    _bEnableBeep = checked;
}

void app::onOpen() {
    const char *filters = "Hexy Files (*.gam);; FutaHex Files (*.fh)";
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Open File"), ".", filters);
    if (!filename.isNull()) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            msg(Level::Error) << "Can't open the file!";
            return;
        }
        QByteArray bytes = file.readAll();
        bool valid = false;
        if (filename.endsWith(".gam", Qt::CaseInsensitive)) {
            valid = _rec.from_gam((unsigned char *)bytes.data(), bytes.length());
        } else if (filename.endsWith(".fh", Qt::CaseInsensitive)) {
        }
        if (!valid) {
            QMessageBox message(QMessageBox::Warning, "FutaHex 2.0",
                "It's not a game file", QMessageBox::Ok);
            message.exec();
        } else {
            Record rec(_rec);
            _rec.clear();
            changeBoardsize(rec.boardsize());
            setAIColor(AIColorSetting::None);
            setPlayerColor(PlayerColorSetting::Auto);
            for (auto record_data : rec) {
                auto pos = record_data.pos();
                RecSetPiece(pos.row, pos.col);
            }
        }
        file.close();
    }
}

void app::onSave() {
    if (!_pBoard)
        return;
    size_t boardsize = _pBoard->boardsize();
    const char *filters = (boardsize <= 11) ?
        "Hexy Files (*.gam);; FutaHex Files (*.fh)" :
        "FutaHex Files (*.fh)";
    QString filename = QFileDialog::getSaveFileName(
        this, tr("Save File"), "", filters);
    if (!filename.isNull()) {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            msg(Level::Error) << "Can't open the file!";
            return;
        }
        QDataStream out(&file);
        unsigned char *buffer = nullptr;
        size_t buffer_size = 0;
        if (filename.endsWith(".gam", Qt::CaseInsensitive)) {
            _rec.to_gam(&buffer, &buffer_size);
        } else if (filename.endsWith(".fh", Qt::CaseInsensitive)) {
        }
        out.writeRawData((const char *)buffer, buffer_size);
        delete buffer;
        file.close();
    }
}

void app::onRestart() { changeBoardsize(_pBoard ? 0 : 11); }
void app::onAIMove() { AIMove(); }
void app::onAIStop() { AIStop(); }

void app::onTakeBack() {
    if (_rec.empty())
        return;

    setAIColor(AIColorSetting::None);
    setPlayerColor(PlayerColorSetting::Auto);

    pos_t pos = _rec.back().pos();
    resetPiece(pos.row, pos.col);
    _rec.pop_back();
}

void app::onView() {
    bool arrow = ui.canvas->getLineWithArrow();
    ui.canvas->setLineWithArrow(!arrow);
    ui.link_red->setLineWithArrow(!arrow);
    ui.link_blue->setLineWithArrow(!arrow);
    updateBoard();
}
