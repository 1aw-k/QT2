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
    void substituteEverywhere(const QString &text, const QString &replacement,
                        bool caseSensitive);
    void pickEditorFont();
    void pickForegroundColor();
    void pickTextFillColor();
    void pickCanvasColor();
    void refreshWindowTitle();
    void refreshStatusMetrics();
    void refreshActionAvailability();
    void persistSnapshot();

private:
    bool maybeSave();
    bool readDocument(const QString &filePath);
    bool storeDocument(const QString &filePath, bool showError);
    bool seekNextMatch(const QString &text, bool caseSensitive, bool backward);
    void mergeTextFormat(const QTextCharFormat &format);
    void assignDocumentPath(const QString &filePath);
    void setupConnections();
    void configureActionsAndAppearance();
    QString currentDocumentLabel() const;

    Ui::MainWindow *ui;
    FindDialog *m_searchPanel;
    ReplaceDialog *m_substitutionPanel;
    QLabel *m_stateGlyph;
    QLabel *m_metricsLabel;
    QLabel *m_identityLabel;
    QTimer *m_snapshotTimer;
    QString m_documentPath;
};
#endif // MAINWINDOW_H
