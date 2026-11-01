#include "aboutdialog.h"
#include "appicons.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowIcon(AppIcons::applicationIcon());

    ui->brandMarkLabel->setPixmap(
        AppIcons::applicationIcon().pixmap(150, 150));
    ui->brandMarkLabel->setAlignment(Qt::AlignCenter);

    ui->productTitleLabel->setAccessibleName(tr("小牛文本编辑器"));
    ui->authorValueLabel->setAccessibleName(tr("开发者 陈彤彤"));
    ui->registrationValueLabel->setAccessibleName(tr("学号 2023426040105"));
    ui->contactValueLabel->setAccessibleName(
        tr("邮箱 272978345@qq.com"));

    ui->authorValueLabel->setTextInteractionFlags(
        Qt::TextSelectableByMouse);
    ui->registrationValueLabel->setTextInteractionFlags(
        Qt::TextSelectableByMouse);
    ui->contactValueLabel->setTextInteractionFlags(
        Qt::TextSelectableByMouse);

    ui->contactValueLabel->setToolTip(
        ui->contactValueLabel->text());
    ui->acceptButton->setAutoDefault(true);

    connect(ui->acceptButton, &QPushButton::clicked,
            this, &QDialog::accept);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
