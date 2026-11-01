#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>

class CodeEditor;

class TextGutter : public QWidget
{
public:
    explicit TextGutter(CodeEditor *editor);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *m_editor;
};

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);

    int gutterWidth() const;
    void paintGutter(QPaintEvent *event);
    void setLineNumbersVisible(bool visible);
    bool lineNumbersVisible() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateTextGutterWidth(int newBlockCount);
    void updateTextGutter(const QRect &rect, int dy);
    void applyActiveLineHighlight();

private:
    QWidget *m_gutter;
    bool m_gutterVisible;
};

#endif // CODEEDITOR_H
