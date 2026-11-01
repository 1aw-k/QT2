#include "searchservice.h"

#include <QTextDocument>

namespace {

QTextDocument::FindFlags buildFlags(const SearchSpecification &specification)
{
    QTextDocument::FindFlags flags;

    if (specification.caseSensitive)
        flags |= QTextDocument::FindCaseSensitively;
    if (specification.reverse)
        flags |= QTextDocument::FindBackward;

    return flags;
}

} // namespace

QTextCursor SearchService::seekNext(
    QTextDocument *document,
    const QTextCursor &start,
    const SearchSpecification &specification)
{
    if (!document || specification.needle.isEmpty())
        return QTextCursor();

    const QTextDocument::FindFlags flags = buildFlags(specification);
    QTextCursor origin = start;

    if (origin.hasSelection()) {
        origin.setPosition(specification.reverse
                               ? origin.selectionStart()
                               : origin.selectionEnd());
    }

    QTextCursor match = document->find(
        specification.needle, origin, flags);
    if (!match.isNull())
        return match;

    QTextCursor wrapPosition(document);
    wrapPosition.movePosition(specification.reverse
                                  ? QTextCursor::End
                                  : QTextCursor::Start);

    return document->find(specification.needle, wrapPosition, flags);
}

int SearchService::substituteAll(QTextDocument *document,
                                 const QString &needle,
                                 const QString &replacement,
                                 bool caseSensitive)
{
    if (!document || needle.isEmpty())
        return 0;

    SearchSpecification specification;
    specification.needle = needle;
    specification.caseSensitive = caseSensitive;

    const QTextDocument::FindFlags flags = buildFlags(specification);
    QTextCursor scan(document);
    int replacements = 0;

    scan.beginEditBlock();
    for (;;) {
        QTextCursor match =
            document->find(needle, scan, flags);
        if (match.isNull())
            break;

        match.insertText(replacement);
        scan = match;
        ++replacements;
    }
    scan.endEditBlock();

    return replacements;
}
