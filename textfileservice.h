#ifndef TEXTFILESERVICE_H
#define TEXTFILESERVICE_H

#include <QString>

struct TextFileLoadResult
{
    enum class Status {
        Loaded,
        OpenFailed,
        ReadFailed,
        BinaryContent
    };

    Status status = Status::Loaded;
    QString text;
    QString error;

    bool succeeded() const
    {
        return status == Status::Loaded;
    }
};

class TextFileService
{
public:
    static TextFileLoadResult load(const QString &filePath);
    static bool store(const QString &filePath,
                      const QString &text,
                      QString *errorMessage = nullptr);
};

#endif // TEXTFILESERVICE_H
