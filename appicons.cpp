#include "appicons.h"

#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPolygonF>

namespace {

QIcon renderIcon(void (*drawFunction)(QPainter &))
{
    QIcon icon;
    const int sizes[] = {16, 20, 24, 32, 48, 64, 128};

    for (int size : sizes) {
        QPixmap pixmap(size, size);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.scale(size / 64.0, size / 64.0);
        drawFunction(painter);
        painter.end();

        icon.addPixmap(pixmap);
    }

    return icon;
}

void drawNotebook(QPainter &painter)
{
    painter.setPen(QPen(QColor("#6f7c86"), 1.8, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(QColor("#d3dbe2"));
    painter.drawRoundedRect(QRectF(11, 12, 38, 46), 4, 4);

    painter.setPen(QPen(QColor("#89959f"), 2.2, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(Qt::NoBrush);
    for (int x = 17; x <= 45; x += 7)
        painter.drawRoundedRect(QRectF(x, 5, 4, 13), 2, 2);

    painter.setPen(QPen(QColor("#9aa8b2"), 1.3));
    painter.drawLine(19, 27, 42, 27);
    painter.drawLine(19, 33, 42, 33);
    painter.drawLine(19, 39, 38, 39);
    painter.drawLine(19, 45, 34, 45);

    painter.setPen(QPen(QColor("#f08a2d"), 1.2));
    painter.setBrush(QColor("#f19a39"));
    painter.drawPolygon(QPolygonF()
                        << QPointF(31, 42)
                        << QPointF(47, 26)
                        << QPointF(52, 31)
                        << QPointF(36, 47));

    painter.setPen(QPen(QColor("#a7555d"), 1.2));
    painter.setBrush(QColor("#e7a2aa"));
    painter.drawPolygon(QPolygonF()
                        << QPointF(47, 26)
                        << QPointF(50, 22)
                        << QPointF(55, 27)
                        << QPointF(52, 31));

    painter.setPen(QPen(QColor("#8c6d50"), 1));
    painter.setBrush(QColor("#f0c78a"));
    painter.drawPolygon(QPolygonF()
                        << QPointF(31, 42)
                        << QPointF(36, 47)
                        << QPointF(28, 50));

    painter.setPen(QPen(QColor("#3e4650"), 1));
    painter.setBrush(QColor("#3e4650"));
    painter.drawPolygon(QPolygonF()
                        << QPointF(28, 50)
                        << QPointF(27, 53)
                        << QPointF(32, 51));
}

void drawNewDocument(QPainter &painter)
{
    painter.setPen(QPen(QColor("#64717d"), 1.8, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(QColor("#f5f7f9"));
    painter.drawRoundedRect(QRectF(15, 7, 34, 50), 3, 3);

    painter.setBrush(QColor("#d4dce5"));
    painter.drawPolygon(QPolygonF()
                        << QPointF(39, 7)
                        << QPointF(39, 18)
                        << QPointF(49, 18));

    painter.setPen(QPen(QColor("#91a0ad"), 1.4));
    painter.drawLine(21, 25, 42, 25);
    painter.drawLine(21, 31, 42, 31);
    painter.drawLine(21, 37, 37, 37);
}

void drawOpenDocument(QPainter &painter)
{
    painter.setPen(QPen(QColor("#9b7216"), 1.6, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(QColor("#d9a62c"));
    painter.drawRoundedRect(QRectF(7, 19, 50, 34), 4, 4);

    painter.setBrush(QColor("#efc44e"));
    painter.drawPolygon(QPolygonF()
                        << QPointF(8, 24)
                        << QPointF(25, 24)
                        << QPointF(31, 30)
                        << QPointF(55, 30)
                        << QPointF(49, 51)
                        << QPointF(15, 51));

    painter.setPen(QPen(QColor("#fff1b7"), 1.4));
    painter.drawLine(18, 35, 44, 35);
    painter.drawLine(17, 41, 40, 41);
}

void drawSave(QPainter &painter)
{
    painter.setPen(QPen(QColor("#344bb2"), 1.8, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(QColor("#5b73d6"));
    painter.drawRoundedRect(QRectF(8, 7, 48, 50), 5, 5);

    painter.setPen(QPen(QColor("#dce3ff"), 1.2));
    painter.setBrush(QColor("#f5f7ff"));
    painter.drawRoundedRect(QRectF(20, 7, 24, 13), 2, 2);

    painter.setPen(QPen(QColor("#afbaf0"), 1));
    painter.drawLine(25, 17, 39, 17);

    painter.setPen(QPen(QColor("#d7defa"), 1.4));
    painter.setBrush(QColor("#f7f8ff"));
    painter.drawRoundedRect(QRectF(18, 31, 28, 18), 2, 2);

    painter.setPen(QPen(QColor("#7f91dd"), 1.2));
    painter.drawLine(23, 37, 41, 37);
    painter.drawLine(23, 42, 36, 42);
}

void drawSaveAs(QPainter &painter)
{
    painter.setPen(QPen(QColor("#8b95a5"), 1.5));
    painter.setBrush(QColor("#e4e8ef"));
    painter.drawRoundedRect(QRectF(7, 12, 30, 42), 3, 3);

    painter.setPen(QPen(QColor("#c3c9d4"), 1.2));
    painter.setBrush(QColor("#ffffff"));
    painter.drawRoundedRect(QRectF(12, 18, 20, 29), 2, 2);

    QPainterPath frontPage;
    frontPage.moveTo(24, 8);
    frontPage.lineTo(43, 8);
    frontPage.lineTo(53, 19);
    frontPage.lineTo(53, 52);
    frontPage.lineTo(24, 52);
    frontPage.closeSubpath();

    painter.setPen(QPen(QColor("#4255b5"), 1.7));
    painter.setBrush(QColor("#6b79dd"));
    painter.drawPath(frontPage);

    painter.setPen(QPen(QColor("#aeb8f4"), 1.2));
    painter.setBrush(QColor("#dfe4ff"));
    painter.drawPolygon(QPolygonF()
                        << QPointF(43, 8)
                        << QPointF(43, 19)
                        << QPointF(53, 19));

    painter.setPen(QPen(QColor("#f0f2ff"), 1.5, Qt::SolidLine,
                        Qt::RoundCap));
    painter.drawLine(30, 28, 46, 28);
    painter.drawLine(30, 34, 46, 34);
    painter.drawLine(30, 40, 42, 40);
}

void drawToolBarToggle(QPainter &painter)
{
    painter.setPen(QPen(QColor("#75849a"), 5.5, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(QPointF(22, 43), QPointF(44, 21));

    painter.setPen(QPen(QColor("#75849a"), 4.5, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawArc(QRectF(8, 7, 27, 27), 35 * 16, 245 * 16);

    painter.setPen(QPen(QColor("#aab5c5"), 2.2, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(QPointF(42, 18), QPointF(50, 10));
    painter.drawLine(QPointF(46, 22), QPointF(54, 14));
}

void drawStatusBarToggle(QPainter &painter)
{
    painter.setPen(QPen(QColor("#6e7f96"), 1.6, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(QColor("#e2e9f0"));
    painter.drawRoundedRect(QRectF(5, 20, 54, 25), 4, 4);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#5e8fc2"));
    painter.drawRoundedRect(QRectF(9, 25, 46, 7), 2, 2);

    painter.setPen(QPen(QColor("#97a6b8"), 1.2));
    painter.drawLine(10, 37, 25, 37);
    painter.drawLine(30, 37, 45, 37);
}

} // namespace

namespace AppIcons {

QIcon applicationIcon()
{
    return renderIcon(drawNotebook);
}

QIcon newDocumentIcon()
{
    return renderIcon(drawNewDocument);
}

QIcon openDocumentIcon()
{
    return renderIcon(drawOpenDocument);
}

QIcon saveIcon()
{
    return renderIcon(drawSave);
}

QIcon saveAsIcon()
{
    return renderIcon(drawSaveAs);
}

QIcon toolBarVisibilityIcon()
{
    return renderIcon(drawToolBarToggle);
}

QIcon statusBarVisibilityIcon()
{
    return renderIcon(drawStatusBarToggle);
}

} // namespace AppIcons
