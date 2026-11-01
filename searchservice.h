#ifndef SEARCHSERVICE_H
#define SEARCHSERVICE_H

#include <QString>
#include <QTextCursor>

class QTextDocument;

struct SearchSpecification
{
    QString needle;
    bool caseSensitive = false;
    bool reverse = false;
};

class SearchService
{
public:
    static QTextCursor seekNext(QTextDocument *document,
                                const QTextCursor &start,
                                const SearchSpecification &specification);

    static int substituteAll(QTextDocument *document,
                             const QString &needle,
                             const QString &replacement,
                             bool caseSensitive);
};

#endif // SEARCHSERVICE_H
