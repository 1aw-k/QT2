#include "codeeditor.h"

#include <QFontMetrics>
#include <QPainter>
#include <QTextBlock>

TextGutter::TextGutter(CodeEditor *editor)
    : QWidget(editor)
    , m_editor(editor)
{
}

QSize TextGutter::sizeHint() const
{
    return QSize(m_editor->gutterWidth(), 0);
}

void TextGutter::paintEvent(QPaintEvent *event)
{
    m_editor->paintGutter(event);
}

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
    , m_gutter(new TextGutter(this))
    , m_gutterVisible(true)
{
    connect(this, &CodeEditor::blockCountChanged,
            this, &CodeEditor::updateTextGutterWidth);
    connect(this, &CodeEditor::updateRequest,
            this, &CodeEditor::updateTextGutter);
    connect(this, &CodeEditor::cursorPositionChanged,
            this, &CodeEditor::applyActiveLineHighlight);

    applyActiveLineHighlight();
    updateTextGutterWidth(0);
}

int CodeEditor::gutterWidth() const
{
    if (!m_gutterVisible)
        return 0;

    int digits = 1;
    int maxValue = qMax(1, blockCount());
    while (maxValue >= 10) {
        maxValue /= 10;
        ++digits;
    }

    return 15 + fontMetrics().horizontalAdvance(QString(digits, QLatin1Char('9')));
}

void CodeEditor::paintGutter(QPaintEvent *event)
{
    QPainter painter(m_gutter);
    painter.fillRect(event->rect(), palette().color(QPalette::AlternateBase));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    painter.setPen(palette().color(QPalette::Text));
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            const QString number = QString::number(blockNumber + 1);
            painter.drawText(0, top, m_gutter->width() - 6,
                             fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeEditor::setLineNumbersVisible(bool visible)
{
    if (m_gutterVisible == visible)
        return;

    m_gutterVisible = visible;
    m_gutter->setVisible(visible);
    updateTextGutterWidth(0);
}

bool CodeEditor::lineNumbersVisible() const
{
    return m_gutterVisible;
}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    const QRect rect = contentsRect();
    m_gutter->setGeometry(
        QRect(rect.left(), rect.top(), gutterWidth(), rect.height()));
}

void CodeEditor::updateTextGutterWidth(int)
{
    setViewportMargins(gutterWidth(), 0, 0, 0);
}

void CodeEditor::updateTextGutter(const QRect &rect, int dy)
{
    if (dy != 0)
        m_gutter->scroll(0, dy);
    else
        m_gutter->update(0, rect.y(), m_gutter->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateTextGutterWidth(0);
}

void CodeEditor::applyActiveLineHighlight()
{
    QList<QTextEdit::ExtraSelection> selections;
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = palette().color(QPalette::Highlight);
        lineColor.setAlpha(30);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        selections.append(selection);
    }

    setExtraSelections(selections);
}
