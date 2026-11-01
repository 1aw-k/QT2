#include "aboutdialog.h"
#include "appicons.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowIcon(AppIcons::applicationIcon());

    ui->iconLabel->setPixmap(
        AppIcons::applicationIcon().pixmap(140, 140));
    ui->iconLabel->setAlignment(Qt::AlignCenter);

    connect(ui->okButton, &QPushButton::clicked,
            this, &QDialog::accept);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
