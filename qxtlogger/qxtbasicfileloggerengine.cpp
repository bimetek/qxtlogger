#include "qxtbasicfileloggerengine.h"
#include <QDateTime>
#include <QTextStream>

namespace Qxt
{

static const QIODevice::OpenMode DefaultOpenMode =
        QIODevice::ReadWrite | QIODevice::Append | QIODevice::Unbuffered
        | QIODevice::Text;

class BasicFileLoggerEnginePrivate
{
    Q_DECLARE_PUBLIC(BasicFileLoggerEngine)
    BasicFileLoggerEngine * const q_ptr;

public:
    BasicFileLoggerEnginePrivate(BasicFileLoggerEngine *q) :
        q_ptr(q), dateFormat("hh:mm:ss.zzz") {}

    QString dateFormat;
};

BasicFileLoggerEngine::BasicFileLoggerEngine(const QString &fileName) :
    AbstractFileLoggerEngine(fileName, DefaultOpenMode),
    d_ptr(new BasicFileLoggerEnginePrivate(this))
{

}

BasicFileLoggerEngine::~BasicFileLoggerEngine()
{
    delete d_ptr;
}

QString BasicFileLoggerEngine::dateFormat() const
{
    return d_ptr->dateFormat;
}

void BasicFileLoggerEngine::setDateFormat(const QString &format)
{
    Q_D(BasicFileLoggerEngine);
    d->dateFormat = format;
}

void BasicFileLoggerEngine::writeToFile(
        const QString &level, const QVariantList &messages)
{
    Q_D(BasicFileLoggerEngine);

    if (messages.isEmpty())
        return;

    QIODevice *file = device();
    QTextStream stream(file);

    QString header = QString("[%1] [%2]").arg(
                QDateTime::currentDateTime().toString(d->dateFormat), level);
    QString padding = QString("").leftJustified(header.size(), ' ');

    stream << header;

    bool first = true;
    Q_FOREACH(const QVariant &out, messages)
    {
        if (!out.isNull())
        {
            if (!first)
                stream << padding;
            else
                first = false;
            stream << out.toString() << endl;
        }
    }
}


}   // namespace Qxt
