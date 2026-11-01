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

    ui->titleLabel->setAccessibleName(tr("小牛文本编辑器"));
    ui->developerValueLabel->setAccessibleName(tr("开发者 陈彤彤"));
    ui->studentIdValueLabel->setAccessibleName(tr("学号 2023426040105"));
    ui->emailValueLabel->setAccessibleName(
        tr("邮箱 272978345@qq.com"));

    ui->developerValueLabel->setTextInteractionFlags(
        Qt::TextSelectableByMouse);
    ui->studentIdValueLabel->setTextInteractionFlags(
        Qt::TextSelectableByMouse);
    ui->emailValueLabel->setTextInteractionFlags(
        Qt::TextSelectableByMouse);

    ui->emailValueLabel->setToolTip(
        ui->emailValueLabel->text());
    ui->okButton->setAutoDefault(true);

    connect(ui->okButton, &QPushButton::clicked,
            this, &QDialog::accept);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
