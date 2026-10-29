#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QApplication>
#include <QStyle>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QPixmap icon = QApplication::style()->standardIcon(QStyle::SP_FileIcon)
                       .pixmap(96, 96);
    ui->iconLabel->setPixmap(icon);
    ui->iconLabel->setAlignment(Qt::AlignCenter);

    connect(ui->okButton, &QPushButton::clicked,
            this, &QDialog::accept);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
