#pragma once

#include <QWidget>

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(QWidget *parent);
    ~Canvas();

    void resizeBoard(int);
protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
private:
    void renderEmptyBoard();
    void renderBorder();
    void renderPieces();
    void renderInfo();

private:
    QPointF _ct[19][19];
    const double _ratio { 2.0 / 3.0 };
    void *_pBoard { nullptr };
    int _size { 11 };
    double _hex_h;
    double _hex_w;
    int _h;
    int _w;
    const QColor _cr { QColor(234, 67, 53) };
    const QColor _cb { QColor(66, 133, 244) };
    const QColor _bk { QColor(191, 191, 191) };
};
