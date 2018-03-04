#pragma once

#include <QWidget>

namespace board { class IBoard; }
class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(QWidget *parent);
    ~Canvas();

    void resize(board::IBoard *);

signals:
    void clickEmptyPoint(int row, int col);

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
private:
    void renderEmptyBoard();
    void renderBorder();
    void renderPieces();
    void renderInfo();

private:
    board::IBoard *_pBoard{ nullptr };

    QPointF _ct[19][19];
    const double _ratio { 2.0 / 3.0 };
    double _hex_h;
    double _hex_w;
    int _h;
    int _w;
    const QColor _cr { QColor(234, 67, 53) };
    const QColor _cb { QColor(66, 133, 244) };
    const QColor _bk { QColor(191, 191, 191) };
};
