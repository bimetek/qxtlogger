#include "qxtabstractfileloggerengine.h"
#include <QFile>

namespace Qxt
{

class AbstractFileLoggerEnginePrivate
{
    Q_DECLARE_PUBLIC(AbstractFileLoggerEngine)
    AbstractFileLoggerEngine * const q_ptr;

public:
    AbstractFileLoggerEnginePrivate(
            AbstractFileLoggerEngine *q, QIODevice::OpenMode m) :
        q_ptr(q), mode(m) {}

    QString fileName;
    QIODevice::OpenMode mode;
};

AbstractFileLoggerEngine::AbstractFileLoggerEngine(
        const QString &fileName, QIODevice::OpenMode mode) :
    AbstractIOLoggerEngine(),
    d_ptr(new AbstractFileLoggerEnginePrivate(this, mode))
{
    setLogFileName(fileName);
}

AbstractFileLoggerEngine::~AbstractFileLoggerEngine()
{
    delete d_ptr;
}

void AbstractFileLoggerEngine::initLoggerEngine()
{
    Q_D(AbstractFileLoggerEngine);

    killLoggerEngine();     // Cleanup possible previous init-ed values.

    QString fileName = d->fileName;
    if (fileName.isEmpty())
        return;

    QFile *file = new QFile(fileName);
    setDevice(file);
    if (!file->open(d->mode) || !file->isWritable())
        killLoggerEngine();
    else
        enableLogging();
}

void AbstractFileLoggerEngine::killLoggerEngine()
{
    QIODevice *dev = device();
    if (!dev)
        return;
    if (dev->isOpen())
        dev->close();
    delete dev;
    setDevice(0);
}

bool AbstractFileLoggerEngine::isInitialized() const
{
    return device();
}

void AbstractFileLoggerEngine::writeFormatted(
        Logger::LogLevel level, const QVariantList &messages)
{
    QString name;
    switch (level)
    {
    case Logger::ErrorLevel:
        name = "Error";
        break;
    case Logger::WarningLevel:
        name = "Warning";
        break;
    case Logger::CriticalLevel:
        name = "Critical";
        break;
    case Logger::FatalLevel:
        name = "Fatal";
        break;
    case Logger::TraceLevel:
        name = "Trace";
        break;
    case Logger::DebugLevel:
        name = "Debug";
        break;
    case Logger::InfoLevel:
        name = "Info";
        break;
    default:
        break;
    }
    writeToFile(name, messages);
}

void AbstractFileLoggerEngine::setLogFileName(const QString &fileName)
{
    Q_D(AbstractFileLoggerEngine);
    d->fileName = fileName;
    initLoggerEngine();     // Re-init with new file name.
}

QString AbstractFileLoggerEngine::logFileName() const
{
    return d_ptr->fileName;
}

}   // namespace Qxt
