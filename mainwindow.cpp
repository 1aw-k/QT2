#include "mainwindow.h"
#include "aboutdialog.h"
#include "finddialog.h"
#include "replacedialog.h"
#include "ui_mainwindow.h"

#include <QAbstractButton>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCloseEvent>
#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDialog>
#include <QIcon>
#include <QKeySequence>
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSaveFile>
#include <QStatusBar>
#include <QStyle>
#include <QTextCharFormat>
#include <QTextCodec>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextStream>
#include <QTimer>
#include <QToolBar>

namespace {

QPixmap iconCanvas()
{
    QPixmap pixmap(24, 24);
    pixmap.fill(Qt::transparent);
    return pixmap;
}

QIcon cutIcon(const QColor &color)
{
    QPixmap pixmap = iconCanvas();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, 1.7, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(6, 5, 17, 16);
    painter.drawLine(18, 5, 7, 16);
    painter.setBrush(color);
    painter.drawEllipse(QRectF(4.5, 16, 4.5, 4.5));
    painter.drawEllipse(QRectF(16.5, 16, 4.5, 4.5));
    return QIcon(pixmap);
}

QIcon copyIcon(const QColor &color)
{
    QPixmap pixmap = iconCanvas();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, 1.6));
    painter.drawRect(QRectF(4, 5, 11, 13));
    painter.drawRect(QRectF(9, 9, 11, 11));
    return QIcon(pixmap);
}

QIcon pasteIcon(const QColor &color)
{
    QPixmap pixmap = iconCanvas();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, 1.6));
    painter.drawRoundedRect(QRectF(5, 4, 14, 17), 2, 2);
    painter.drawRect(QRectF(9, 2.5, 6, 4));
    painter.drawLine(8, 10, 16, 10);
    painter.drawLine(8, 14, 16, 14);
    painter.drawLine(8, 18, 14, 18);
    return QIcon(pixmap);
}

QIcon wrapIcon(const QColor &color)
{
    QPixmap pixmap = iconCanvas();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, 1.7, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(4, 6, 20, 6);
    painter.drawLine(4, 12, 15, 12);
    painter.drawLine(4, 18, 20, 18);
    painter.drawLine(15, 9, 18, 12);
    painter.drawLine(15, 15, 18, 12);
    return QIcon(pixmap);
}

QIcon lineNumbersIcon(const QColor &color)
{
    QPixmap pixmap = iconCanvas();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, 1.5));
    for (int row = 0; row < 4; ++row) {
        const int y = 4 + row * 5;
        painter.drawPoint(4, y);
        painter.drawLine(8, y, 20, y);
    }
    return QIcon(pixmap);
}

QIcon fontIcon(const QColor &color)
{
    QPixmap pixmap = iconCanvas();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QFont font = painter.font();
    font.setBold(true);
    font.setPixelSize(18);
    painter.setFont(font);
    painter.setPen(color);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, QStringLiteral("A"));
    return QIcon(pixmap);
}

QIcon colorLetterIcon(const QColor &textColor, const QColor &accent,
                      bool fillBackground)
{
    QPixmap pixmap = iconCanvas();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    if (fillBackground) {
        painter.fillRect(QRectF(2, 15, 20, 6), accent);
        painter.setPen(QPen(accent.darker(130), 1));
        painter.drawRect(QRectF(2, 15, 20, 6));
    } else {
        painter.setPen(QPen(accent, 2));
        painter.drawLine(4, 21, 20, 21);
    }

    QFont font = painter.font();
    font.setBold(true);
    font.setPixelSize(16);
    painter.setFont(font);
    painter.setPen(textColor);
    painter.drawText(QRect(0, 0, 24, 18), Qt::AlignCenter, QStringLiteral("A"));
    return QIcon(pixmap);
}

QIcon editorBackgroundIcon(const QColor &color)
{
    QPixmap pixmap = iconCanvas();
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(QRectF(3, 4, 18, 16), color);
    painter.setPen(QPen(color.darker(150), 1.5));
    painter.drawRect(QRectF(3, 4, 18, 16));
    return QIcon(pixmap);
}

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_findDialog(nullptr)
    , m_replaceDialog(nullptr)
    , m_statusInfoLabel(nullptr)
    , m_statusEncodingLabel(nullptr)
    , m_autoSaveTimer(new QTimer(this))
{
    ui->setupUi(this);

    setupConnections();
    setupIconsAndView();

    m_findDialog = new FindDialog(this);
    m_replaceDialog = new ReplaceDialog(this);
    connect(m_findDialog, &FindDialog::findRequested,
            this, &MainWindow::findText);
    connect(m_replaceDialog, &ReplaceDialog::findRequested,
            this, &MainWindow::findText);
    connect(m_replaceDialog, &ReplaceDialog::replaceRequested,
            this, &MainWindow::replaceText);
    connect(m_replaceDialog, &ReplaceDialog::replaceAllRequested,
            this, &MainWindow::replaceAllText);

    m_statusInfoLabel = new QLabel(this);
    m_statusEncodingLabel = new QLabel(tr("UTF-8"), this);
    statusBar()->addPermanentWidget(m_statusInfoLabel);
    statusBar()->addPermanentWidget(m_statusEncodingLabel);

    m_autoSaveTimer->setInterval(60 * 1000);
    connect(m_autoSaveTimer, &QTimer::timeout,
            this, &MainWindow::autoSave);
    m_autoSaveTimer->start();

    setCurrentFilePath(QString());
    updateWindowTitle();
    updateStatusBar();
    updateActionStates();
    ui->editor->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}

void MainWindow::newFile()
{
    if (!maybeSave())
        return;

    ui->editor->clear();
    ui->editor->document()->clearUndoRedoStacks();
    ui->editor->document()->setModified(false);
    setCurrentFilePath(QString());
    updateWindowTitle();
    updateStatusBar();
    updateActionStates();
    statusBar()->showMessage(tr("已新建文件"), 2000);
}

void MainWindow::openFile()
{
    if (!maybeSave())
        return;

    const QString filePath = QFileDialog::getOpenFileName(
        this, tr("打开文件"), QString(),
        tr("文本文件 (*.txt *.md *.cpp *.h *.json *.xml);;所有文件 (*.*)"));
    if (!filePath.isEmpty())
        loadFile(filePath);
}

bool MainWindow::saveFile()
{
    if (m_currentFilePath.isEmpty())
        return saveAs();

    if (!writeFile(m_currentFilePath, true))
        return false;

    ui->editor->document()->setModified(false);
    updateWindowTitle();
    updateActionStates();
    statusBar()->showMessage(tr("文件已保存"), 2500);
    return true;
}

bool MainWindow::saveAs()
{
    const QString filePath = QFileDialog::getSaveFileName(
        this, tr("另存为"), m_currentFilePath,
        tr("文本文件 (*.txt);;所有文件 (*.*)"));
    if (filePath.isEmpty())
        return false;

    if (!writeFile(filePath, true))
        return false;

    setCurrentFilePath(filePath);
    ui->editor->document()->setModified(false);
    updateWindowTitle();
    updateActionStates();
    statusBar()->showMessage(tr("文件已保存"), 2500);
    return true;
}

void MainWindow::showFindDialog()
{
    const QTextCursor cursor = ui->editor->textCursor();
    const QString selectedText = cursor.selectedText();
    if (!selectedText.isEmpty() && !selectedText.contains(QChar::ParagraphSeparator))
        m_findDialog->setSearchText(selectedText);

    m_findDialog->show();
    m_findDialog->raise();
    m_findDialog->activateWindow();
    m_findDialog->focusSearchField();
}

void MainWindow::showReplaceDialog()
{
    const QTextCursor cursor = ui->editor->textCursor();
    const QString selectedText = cursor.selectedText();
    if (!selectedText.isEmpty() && !selectedText.contains(QChar::ParagraphSeparator))
        m_replaceDialog->setSearchText(selectedText);

    m_replaceDialog->show();
    m_replaceDialog->raise();
    m_replaceDialog->activateWindow();
    m_replaceDialog->focusSearchField();
}

void MainWindow::showAboutDialog()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::findText(const QString &text, bool caseSensitive, bool backward)
{
    if (findNextInEditor(text, caseSensitive, backward))
        return;

    QMessageBox::information(this, tr("查找"),
                             tr("未找到“%1”。").arg(text));
}

void MainWindow::replaceText(const QString &text, const QString &replacement,
                             bool caseSensitive, bool backward)
{
    if (!findNextInEditor(text, caseSensitive, backward)) {
        QMessageBox::information(this, tr("替换"),
                                 tr("未找到“%1”。").arg(text));
        return;
    }

    QTextCursor cursor = ui->editor->textCursor();
    cursor.insertText(replacement);
    ui->editor->setTextCursor(cursor);
    ui->editor->setFocus();
    statusBar()->showMessage(tr("已替换 1 处"), 2000);
}

void MainWindow::replaceAllText(const QString &text, const QString &replacement,
                                bool caseSensitive)
{
    QTextDocument::FindFlags flags;
    if (caseSensitive)
        flags |= QTextDocument::FindCaseSensitively;

    QTextCursor cursor(ui->editor->document());
    cursor.beginEditBlock();
    int count = 0;
    while (true) {
        QTextCursor found = ui->editor->document()->find(text, cursor, flags);
        if (found.isNull())
            break;

        found.insertText(replacement);
        cursor = found;
        ++count;
    }
    cursor.endEditBlock();

    if (count == 0) {
        QMessageBox::information(this, tr("替换全部"),
                                 tr("未找到“%1”。").arg(text));
        return;
    }

    ui->editor->setTextCursor(cursor);
    ui->editor->setFocus();
    statusBar()->showMessage(tr("已替换 %1 处").arg(count), 3000);
}

void MainWindow::chooseFont()
{
    bool accepted = false;
    const QFont selectedFont = QFontDialog::getFont(
        &accepted, ui->editor->font(), this, tr("选择字体"));
    if (!accepted)
        return;

    QTextCharFormat format;
    format.setFont(selectedFont);
    applyTextFormat(format);
    statusBar()->showMessage(tr("字体已更新"), 2000);
}

void MainWindow::chooseTextColor()
{
    const QColor color = QColorDialog::getColor(
        ui->editor->palette().color(QPalette::Text),
        this, tr("选择字体颜色"));
    if (!color.isValid())
        return;

    QTextCharFormat format;
    format.setForeground(color);
    applyTextFormat(format);
    statusBar()->showMessage(tr("字体颜色已更新"), 2000);
}

void MainWindow::chooseTextBackground()
{
    const QBrush currentBackground =
        ui->editor->textCursor().charFormat().background();
    const QColor initialColor =
        currentBackground.style() == Qt::NoBrush
            ? QColor("#fff59d")
            : currentBackground.color();

    const QColor color = QColorDialog::getColor(
        initialColor, this, tr("选择字体背景色"));
    if (!color.isValid())
        return;

    const bool appliesToSelection = ui->editor->textCursor().hasSelection();
    QTextCharFormat format;
    format.setBackground(color);
    applyTextFormat(format);

    if (appliesToSelection) {
        QTextCursor cursor = ui->editor->textCursor();
        cursor.clearSelection();
        ui->editor->setTextCursor(cursor);
        ui->editor->mergeCurrentCharFormat(format);
        ui->editor->viewport()->update();
    }

    statusBar()->showMessage(
        appliesToSelection ? tr("字体背景色已应用到所选文字")
                           : tr("字体背景色已应用到全文"),
        2500);
}

void MainWindow::chooseEditorBackground()
{
    const QColor color = QColorDialog::getColor(
        ui->editor->palette().color(QPalette::Base), this, tr("选择编辑器背景色"));
    if (!color.isValid())
        return;

    QPalette editorPalette = ui->editor->palette();
    editorPalette.setColor(QPalette::Base, color);
    ui->editor->setPalette(editorPalette);
    ui->editor->viewport()->setPalette(editorPalette);
    ui->editor->viewport()->setAutoFillBackground(true);
    ui->editor->viewport()->update();
    ui->editor->update();
    statusBar()->showMessage(tr("编辑器背景色已更新"), 2000);
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(tr("%1[*] - 简单文本编辑器").arg(displayName()));
    setWindowModified(ui->editor->document()->isModified());
}

void MainWindow::updateStatusBar()
{
    if (!m_statusInfoLabel)
        return;

    const QTextCursor cursor = ui->editor->textCursor();
    const int length = ui->editor->toPlainText().size();
    const int lines = ui->editor->document()->blockCount();
    const int line = cursor.blockNumber() + 1;
    const int column = cursor.positionInBlock() + 1;

    m_statusInfoLabel->setText(
        tr("length: %1    lines: %2    Ln: %3    Col: %4")
            .arg(length)
            .arg(lines)
            .arg(line)
            .arg(column));
}

void MainWindow::updateActionStates()
{
    QTextDocument *document = ui->editor->document();
    const bool hasSelection = ui->editor->textCursor().hasSelection();
    const bool canPaste = QApplication::clipboard()->mimeData()
                          && QApplication::clipboard()->mimeData()->hasText();

    ui->actionUndo->setEnabled(document->isUndoAvailable());
    ui->actionRedo->setEnabled(document->isRedoAvailable());
    ui->actionCut->setEnabled(hasSelection);
    ui->actionCopy->setEnabled(hasSelection);
    ui->actionPaste->setEnabled(canPaste);
    ui->actionSelectAll->setEnabled(!document->isEmpty());
}

void MainWindow::autoSave()
{
    if (m_currentFilePath.isEmpty()
        || !ui->editor->document()->isModified()) {
        return;
    }

    if (writeFile(m_currentFilePath, false)) {
        ui->editor->document()->setModified(false);
        updateWindowTitle();
        updateActionStates();
        statusBar()->showMessage(tr("已自动保存"), 2000);
    }
}

bool MainWindow::maybeSave()
{
    if (!ui->editor->document()->isModified())
        return true;

    QMessageBox box(QMessageBox::Warning, tr("未保存的修改"),
                    tr("“%1”已修改。\n是否保存修改？").arg(displayName()),
                    QMessageBox::NoButton, this);
    QAbstractButton *saveButton = box.addButton(tr("保存"), QMessageBox::AcceptRole);
    QAbstractButton *discardButton = box.addButton(tr("不保存"),
                                                   QMessageBox::DestructiveRole);
    QAbstractButton *cancelButton = box.addButton(tr("取消"), QMessageBox::RejectRole);
    box.setDefaultButton(qobject_cast<QPushButton *>(saveButton));
    box.setEscapeButton(cancelButton);
    box.exec();

    if (box.clickedButton() == saveButton)
        return saveFile();
    if (box.clickedButton() == discardButton)
        return true;

    return false;
}

bool MainWindow::loadFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("打开失败"),
                             tr("无法打开文件：\n%1\n\n%2")
                                 .arg(filePath, file.errorString()));
        return false;
    }

    const QByteArray data = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        QMessageBox::warning(this, tr("打开失败"),
                             tr("读取文件时发生错误：\n%1")
                                 .arg(file.errorString()));
        return false;
    }

    if (data.contains('\0')) {
        QMessageBox::warning(this, tr("无法作为文本打开"),
                             tr("所选文件可能是二进制文件，无法安全地作为文本打开。"));
        return false;
    }

    QTextCodec *fallbackCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec *codec = QTextCodec::codecForUtfText(data, fallbackCodec);
    const QString text = codec ? codec->toUnicode(data) : QString::fromUtf8(data);

    ui->editor->setPlainText(text);
    ui->editor->document()->clearUndoRedoStacks();
    ui->editor->document()->setModified(false);
    QTextCursor cursor = ui->editor->textCursor();
    cursor.movePosition(QTextCursor::Start);
    ui->editor->setTextCursor(cursor);

    setCurrentFilePath(filePath);
    updateWindowTitle();
    updateStatusBar();
    updateActionStates();
    statusBar()->showMessage(tr("已打开 %1").arg(QFileInfo(filePath).fileName()), 3000);
    ui->editor->setFocus();
    return true;
}

bool MainWindow::writeFile(const QString &filePath, bool showError)
{
    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        if (showError) {
            QMessageBox::warning(this, tr("保存失败"),
                                 tr("无法写入文件：\n%1\n\n%2")
                                     .arg(filePath, file.errorString()));
        }
        return false;
    }

    const QByteArray data = ui->editor->toPlainText().toUtf8();
    if (file.write(data) != data.size() || !file.commit()) {
        if (showError) {
            QMessageBox::warning(this, tr("保存失败"),
                                 tr("保存文件时发生错误：\n%1")
                                     .arg(file.errorString()));
        }
        return false;
    }

    return true;
}

bool MainWindow::findNextInEditor(const QString &text, bool caseSensitive,
                                  bool backward)
{
    if (text.isEmpty())
        return false;

    QTextDocument::FindFlags flags;
    if (caseSensitive)
        flags |= QTextDocument::FindCaseSensitively;
    if (backward)
        flags |= QTextDocument::FindBackward;

    QTextCursor searchCursor = ui->editor->textCursor();
    if (searchCursor.hasSelection()) {
        searchCursor.setPosition(backward ? searchCursor.selectionStart()
                                          : searchCursor.selectionEnd());
    }

    QTextCursor found = ui->editor->document()->find(text, searchCursor, flags);

    if (found.isNull()) {
        QTextCursor wrapCursor(ui->editor->document());
        wrapCursor.movePosition(backward ? QTextCursor::End
                                         : QTextCursor::Start);
        found = ui->editor->document()->find(text, wrapCursor, flags);
    }

    if (found.isNull())
        return false;

    ui->editor->setTextCursor(found);
    ui->editor->centerCursor();
    ui->editor->setFocus();
    return true;
}

void MainWindow::applyTextFormat(const QTextCharFormat &format)
{
    QTextCursor cursor = ui->editor->textCursor();
    const int originalPosition = cursor.position();
    const bool hadSelection = cursor.hasSelection();

    if (!hadSelection)
        cursor.select(QTextCursor::Document);

    cursor.mergeCharFormat(format);

    if (!hadSelection) {
        cursor.setPosition(originalPosition);
        cursor.clearSelection();
    }

    ui->editor->setTextCursor(cursor);
    ui->editor->mergeCurrentCharFormat(format);
    ui->editor->viewport()->update();
    ui->editor->setFocus();
}

void MainWindow::setCurrentFilePath(const QString &filePath)
{
    m_currentFilePath = filePath;

    if (filePath.isEmpty())
        setWindowFilePath(QString());
    else
        setWindowFilePath(filePath);
}

void MainWindow::setupConnections()
{
    connect(ui->actionNew, &QAction::triggered,
            this, &MainWindow::newFile);
    connect(ui->actionOpen, &QAction::triggered,
            this, &MainWindow::openFile);
    connect(ui->actionSave, &QAction::triggered,
            this, &MainWindow::saveFile);
    connect(ui->actionSaveAs, &QAction::triggered,
            this, &MainWindow::saveAs);
    connect(ui->actionExit, &QAction::triggered,
            this, &QWidget::close);

    connect(ui->actionUndo, &QAction::triggered,
            ui->editor, &QPlainTextEdit::undo);
    connect(ui->actionRedo, &QAction::triggered,
            ui->editor, &QPlainTextEdit::redo);
    connect(ui->actionCut, &QAction::triggered,
            ui->editor, &QPlainTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered,
            ui->editor, &QPlainTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered,
            ui->editor, &QPlainTextEdit::paste);
    connect(ui->actionSelectAll, &QAction::triggered,
            ui->editor, &QPlainTextEdit::selectAll);
    connect(ui->actionFind, &QAction::triggered,
            this, &MainWindow::showFindDialog);
    connect(ui->actionReplace, &QAction::triggered,
            this, &MainWindow::showReplaceDialog);

    connect(ui->actionFont, &QAction::triggered,
            this, &MainWindow::chooseFont);
    connect(ui->actionTextColor, &QAction::triggered,
            this, &MainWindow::chooseTextColor);
    connect(ui->actionTextBackground, &QAction::triggered,
            this, &MainWindow::chooseTextBackground);
    connect(ui->actionEditorBackground, &QAction::triggered,
            this, &MainWindow::chooseEditorBackground);

    connect(ui->actionWordWrap, &QAction::toggled,
            this, [this](bool enabled) {
        ui->editor->setLineWrapMode(enabled
                                        ? QPlainTextEdit::WidgetWidth
                                        : QPlainTextEdit::NoWrap);
    });
    connect(ui->actionLineNumbers, &QAction::toggled,
            ui->editor, &CodeEditor::setLineNumbersVisible);
    connect(ui->actionToolBar, &QAction::toggled,
            ui->mainToolBar, &QToolBar::setVisible);
    connect(ui->actionStatusBar, &QAction::toggled,
            statusBar(), &QStatusBar::setVisible);
    connect(ui->actionAbout, &QAction::triggered,
            this, &MainWindow::showAboutDialog);

    connect(ui->editor->document(), &QTextDocument::modificationChanged,
            this, &MainWindow::updateWindowTitle);
    connect(ui->editor->document(), &QTextDocument::undoAvailable,
            this, &MainWindow::updateActionStates);
    connect(ui->editor->document(), &QTextDocument::redoAvailable,
            this, &MainWindow::updateActionStates);
    connect(ui->editor, &QPlainTextEdit::copyAvailable,
            this, &MainWindow::updateActionStates);
    connect(ui->editor, &QPlainTextEdit::textChanged,
            this, &MainWindow::updateActionStates);
    connect(ui->editor, &QPlainTextEdit::cursorPositionChanged,
            this, &MainWindow::updateStatusBar);
    connect(QApplication::clipboard(), &QClipboard::dataChanged,
            this, &MainWindow::updateActionStates);
}

void MainWindow::setupIconsAndView()
{
    QStyle *windowStyle = style();
    ui->actionNew->setIcon(windowStyle->standardIcon(QStyle::SP_FileIcon));
    ui->actionOpen->setIcon(windowStyle->standardIcon(QStyle::SP_DialogOpenButton));
    ui->actionSave->setIcon(windowStyle->standardIcon(QStyle::SP_DialogSaveButton));
    ui->actionSaveAs->setIcon(windowStyle->standardIcon(QStyle::SP_DialogSaveButton));
    ui->actionUndo->setIcon(windowStyle->standardIcon(QStyle::SP_ArrowBack));
    ui->actionRedo->setIcon(windowStyle->standardIcon(QStyle::SP_ArrowForward));
    ui->actionRedo->setShortcuts(QList<QKeySequence>()
                                 << QKeySequence(QStringLiteral("Ctrl+Shift+Z"))
                                 << QKeySequence(QStringLiteral("Ctrl+Y")));
    ui->actionCut->setIcon(cutIcon(QColor("#c0392b")));
    ui->actionCopy->setIcon(copyIcon(QColor("#2867b2")));
    ui->actionPaste->setIcon(pasteIcon(QColor("#2f8f4e")));
    ui->actionFind->setIcon(windowStyle->standardIcon(QStyle::SP_FileDialogContentsView));
    ui->actionReplace->setIcon(windowStyle->standardIcon(QStyle::SP_FileDialogDetailedView));
    ui->actionWordWrap->setIcon(wrapIcon(palette().color(QPalette::WindowText)));
    ui->actionLineNumbers->setIcon(lineNumbersIcon(palette().color(QPalette::WindowText)));
    ui->actionFont->setIcon(fontIcon(palette().color(QPalette::WindowText)));
    ui->actionTextColor->setIcon(colorLetterIcon(
        palette().color(QPalette::WindowText), QColor("#d35400"), false));
    ui->actionTextBackground->setIcon(colorLetterIcon(
        palette().color(QPalette::WindowText), QColor("#f6d55c"), true));
    ui->actionEditorBackground->setIcon(editorBackgroundIcon(QColor("#dff1f8")));
    ui->actionAbout->setIcon(windowStyle->standardIcon(QStyle::SP_MessageBoxInformation));

    ui->mainToolBar->setIconSize(QSize(20, 20));
    ui->editor->setLineWrapMode(ui->actionWordWrap->isChecked()
                                    ? QPlainTextEdit::WidgetWidth
                                    : QPlainTextEdit::NoWrap);
    ui->editor->setLineNumbersVisible(ui->actionLineNumbers->isChecked());
    ui->mainToolBar->setVisible(ui->actionToolBar->isChecked());
    statusBar()->setVisible(ui->actionStatusBar->isChecked());
}

QString MainWindow::displayName() const
{
    if (m_currentFilePath.isEmpty())
        return tr("新建文本文件");

    return QFileInfo(m_currentFilePath).fileName();
}
