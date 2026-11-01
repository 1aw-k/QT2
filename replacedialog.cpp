#include "replacedialog.h"
#include "ui_replacedialog.h"

#include <QMessageBox>

ReplaceDialog::ReplaceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    setModal(false);

    connect(ui->seekButton, &QPushButton::clicked,
            this, &ReplaceDialog::requestFind);
    connect(ui->substituteButton, &QPushButton::clicked,
            this, &ReplaceDialog::requestReplace);
    connect(ui->substituteAllButton, &QPushButton::clicked,
            this, &ReplaceDialog::requestReplaceAll);
    connect(ui->dismissButton, &QPushButton::clicked,
            this, &QDialog::hide);
    connect(ui->lookupInput, &QLineEdit::returnPressed,
            this, &ReplaceDialog::requestFind);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

QString ReplaceDialog::searchText() const
{
    return ui->lookupInput->text();
}

void ReplaceDialog::setSearchText(const QString &text)
{
    ui->lookupInput->setText(text);
    ui->lookupInput->selectAll();
}

void ReplaceDialog::focusSearchField()
{
    ui->lookupInput->setFocus();
    ui->lookupInput->selectAll();
}

void ReplaceDialog::requestFind()
{
    if (!validateSearchText())
        return;

    emit findRequested(ui->lookupInput->text(),
                       ui->matchCaseToggle->isChecked(),
                       ui->reverseDirectionOption->isChecked());
}

void ReplaceDialog::requestReplace()
{
    if (!validateSearchText())
        return;

    emit replaceRequested(ui->lookupInput->text(),
                          ui->substitutionInput->text(),
                          ui->matchCaseToggle->isChecked(),
                          ui->reverseDirectionOption->isChecked());
}

void ReplaceDialog::requestReplaceAll()
{
    if (!validateSearchText())
        return;

    emit replaceAllRequested(ui->lookupInput->text(),
                             ui->substitutionInput->text(),
                             ui->matchCaseToggle->isChecked());
}

bool ReplaceDialog::validateSearchText()
{
    if (!ui->lookupInput->text().isEmpty())
        return true;

    QMessageBox::information(this, tr("替换"), tr("请输入要查找的内容。"));
    ui->lookupInput->setFocus();
    return false;
}
