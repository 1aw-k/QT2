#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class FindDialog;
class QCloseEvent;
class QLabel;
class QTextCharFormat;
class QTimer;
class ReplaceDialog;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void openFile();
    bool saveFile();
    bool saveAs();
    void showFindDialog();
    void showReplaceDialog();
    void showAboutDialog();
    void findText(const QString &text, bool caseSensitive, bool backward);
    void replaceText(const QString &text, const QString &replacement,
                     bool caseSensitive, bool backward);
    void replaceAllText(const QString &text, const QString &replacement,
                        bool caseSensitive);
    void chooseFont();
    void chooseTextColor();
    void chooseTextBackground();
    void chooseEditorBackground();
    void updateWindowTitle();
    void updateStatusBar();
    void updateActionStates();
    void autoSave();

private:
    bool maybeSave();
    bool loadFile(const QString &filePath);
    bool writeFile(const QString &filePath, bool showError);
    bool findNextInEditor(const QString &text, bool caseSensitive, bool backward);
    void applyTextFormat(const QTextCharFormat &format);
    void setCurrentFilePath(const QString &filePath);
    void setupConnections();
    void setupIconsAndView();
    QString displayName() const;

    Ui::MainWindow *ui;
    FindDialog *m_findDialog;
    ReplaceDialog *m_replaceDialog;
    QLabel *m_statusBarIconLabel;
    QLabel *m_statusInfoLabel;
    QLabel *m_statusEncodingLabel;
    QTimer *m_autoSaveTimer;
    QString m_currentFilePath;
};
#endif // MAINWINDOW_H
