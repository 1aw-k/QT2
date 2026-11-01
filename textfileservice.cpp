#include "textfileservice.h"

#include <QFile>
#include <QSaveFile>
#include <QTextCodec>

TextFileLoadResult TextFileService::load(const QString &filePath)
{
    TextFileLoadResult result;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        result.status = TextFileLoadResult::Status::OpenFailed;
        result.error = file.errorString();
        return result;
    }

    const QByteArray bytes = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        result.status = TextFileLoadResult::Status::ReadFailed;
        result.error = file.errorString();
        return result;
    }

    if (bytes.contains('\0')) {
        result.status = TextFileLoadResult::Status::BinaryContent;
        return result;
    }

    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec *codec = QTextCodec::codecForUtfText(bytes, utf8);
    result.text = codec ? codec->toUnicode(bytes)
                        : QString::fromUtf8(bytes);
    return result;
}

bool TextFileService::store(const QString &filePath,
                            const QString &text,
                            QString *errorMessage)
{
    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        if (errorMessage)
            *errorMessage = file.errorString();
        return false;
    }

    const QByteArray bytes = text.toUtf8();
    if (file.write(bytes) != bytes.size() || !file.commit()) {
        if (errorMessage)
            *errorMessage = file.errorString();
        return false;
    }

    return true;
}
