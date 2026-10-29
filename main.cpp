#include "mainwindow.h"

#include <QApplication>
#include <QStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(QStringLiteral("简单文本编辑器"));
    a.setApplicationVersion(QStringLiteral("1.0"));
    a.setWindowIcon(a.style()->standardIcon(QStyle::SP_FileIcon));

    MainWindow w;
    w.show();
    return a.exec();
}
