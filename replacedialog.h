#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr);
    ~ReplaceDialog();

    QString searchText() const;
    void setSearchText(const QString &text);
    void focusSearchField();

signals:
    void findRequested(const QString &text, bool caseSensitive, bool backward);
    void replaceRequested(const QString &text, const QString &replacement,
                          bool caseSensitive, bool backward);
    void replaceAllRequested(const QString &text, const QString &replacement,
                             bool caseSensitive);

private slots:
    void requestFind();
    void requestReplace();
    void requestReplaceAll();

private:
    bool validateSearchText();

    Ui::ReplaceDialog *ui;
};

#endif // REPLACEDIALOG_H
