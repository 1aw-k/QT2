#include "replacedialog.h"
#include "ui_replacedialog.h"

#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    setModal(false);

    connect(ui->findNextButton, &QPushButton::clicked,
            this, &ReplaceDialog::requestFind);
    connect(ui->replaceButton, &QPushButton::clicked,
            this, &ReplaceDialog::requestReplace);
    connect(ui->replaceAllButton, &QPushButton::clicked,
            this, &ReplaceDialog::requestReplaceAll);
    connect(ui->cancelButton, &QPushButton::clicked,
            this, &QDialog::hide);
    connect(ui->searchLineEdit, &QLineEdit::returnPressed,
            this, &ReplaceDialog::requestFind);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

QString ReplaceDialog::searchText() const
{
    return ui->searchLineEdit->text();
}

void ReplaceDialog::setSearchText(const QString &text)
{
    ui->searchLineEdit->setText(text);
    ui->searchLineEdit->selectAll();
}

void ReplaceDialog::focusSearchField()
{
    ui->searchLineEdit->setFocus();
    ui->searchLineEdit->selectAll();
}

void ReplaceDialog::requestFind()
{
    if (!validateSearchText())
        return;

    emit findRequested(ui->searchLineEdit->text(),
                       ui->caseSensitiveCheckBox->isChecked(),
                       ui->upRadioButton->isChecked());
}

void ReplaceDialog::requestReplace()
{
    if (!validateSearchText())
        return;

    emit replaceRequested(ui->searchLineEdit->text(),
                          ui->replaceLineEdit->text(),
                          ui->caseSensitiveCheckBox->isChecked(),
                          ui->upRadioButton->isChecked());
}

void ReplaceDialog::requestReplaceAll()
{
    if (!validateSearchText())
        return;

    emit replaceAllRequested(ui->searchLineEdit->text(),
                             ui->replaceLineEdit->text(),
                             ui->caseSensitiveCheckBox->isChecked());
}

bool ReplaceDialog::validateSearchText()
{
    if (!ui->searchLineEdit->text().isEmpty())
        return true;

    QMessageBox::information(this, tr("替换"), tr("请输入要查找的内容。"));
    ui->searchLineEdit->setFocus();
    return false;
}
