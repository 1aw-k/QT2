#include "appicons.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(QStringLiteral("小牛文本编辑器"));
    a.setApplicationVersion(QStringLiteral("1.0"));
    a.setWindowIcon(AppIcons::applicationIcon());

    MainWindow w;
    w.show();
    return a.exec();
}
