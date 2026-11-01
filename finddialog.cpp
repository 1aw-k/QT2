#include "finddialog.h"
#include "ui_finddialog.h"

#include <QMessageBox>

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    setModal(false);

    connect(ui->acceptSearchButton, &QPushButton::clicked,
            this, &FindDialog::requestNext);
    connect(ui->dismissButton, &QPushButton::clicked,
            this, &QDialog::hide);
    connect(ui->queryInput, &QLineEdit::returnPressed,
            this, &FindDialog::requestNext);
}

FindDialog::~FindDialog()
{
    delete ui;
}

QString FindDialog::searchText() const
{
    return ui->queryInput->text();
}

void FindDialog::setSearchText(const QString &text)
{
    ui->queryInput->setText(text);
    ui->queryInput->selectAll();
}

void FindDialog::focusSearchField()
{
    ui->queryInput->setFocus();
    ui->queryInput->selectAll();
}

void FindDialog::requestNext()
{
    if (ui->queryInput->text().isEmpty()) {
        QMessageBox::information(this, tr("查找"), tr("请输入要查找的内容。"));
        ui->queryInput->setFocus();
        return;
    }

    emit findRequested(ui->queryInput->text(),
                       ui->matchCaseToggle->isChecked(),
                       ui->reverseDirectionOption->isChecked());
}
