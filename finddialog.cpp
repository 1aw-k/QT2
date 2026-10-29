#include "finddialog.h"
#include "ui_finddialog.h"

#include <QMessageBox>

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    setModal(false);

    connect(ui->nextButton, &QPushButton::clicked,
            this, &FindDialog::requestNext);
    connect(ui->cancelButton, &QPushButton::clicked,
            this, &QDialog::hide);
    connect(ui->searchLineEdit, &QLineEdit::returnPressed,
            this, &FindDialog::requestNext);
}

FindDialog::~FindDialog()
{
    delete ui;
}

QString FindDialog::searchText() const
{
    return ui->searchLineEdit->text();
}

void FindDialog::setSearchText(const QString &text)
{
    ui->searchLineEdit->setText(text);
    ui->searchLineEdit->selectAll();
}

void FindDialog::focusSearchField()
{
    ui->searchLineEdit->setFocus();
    ui->searchLineEdit->selectAll();
}

void FindDialog::requestNext()
{
    if (ui->searchLineEdit->text().isEmpty()) {
        QMessageBox::information(this, tr("查找"), tr("请输入要查找的内容。"));
        ui->searchLineEdit->setFocus();
        return;
    }

    emit findRequested(ui->searchLineEdit->text(),
                       ui->caseSensitiveCheckBox->isChecked(),
                       ui->upRadioButton->isChecked());
}
