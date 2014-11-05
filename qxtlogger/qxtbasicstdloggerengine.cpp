#include "qxtbasicstdloggerengine.h"
#include <QTextStream>
#include <QTime>

namespace Qxt
{

const Logger::LogLevels RequiredLevels =
        Logger::WarningLevel | Logger::ErrorLevel | Logger::CriticalLevel
        | Logger::FatalLevel;

class BasicSTDLoggerEnginePrivate
{
    Q_DECLARE_PUBLIC(BasicSTDLoggerEngine)
    BasicSTDLoggerEngine * const q_ptr;

public:
    BasicSTDLoggerEnginePrivate(BasicSTDLoggerEngine *q) :
        q_ptr(q)
    {
        out->flush();
        out = new QTextStream(stdout);

        err->flush();
        err = new QTextStream(stderr);
    }

    ~BasicSTDLoggerEnginePrivate()
    {
        delete out;
        delete err;
    }

    void writeToStream(
            QTextStream *stream, const QString &level, const QVariantList &msgs)
    {
        if (msgs.isEmpty())
            return;

        Q_ASSERT(stream);

        QString timestamp = QTime::currentTime().toString("hh:mm:ss.zzz");
        QString header = QString("[%1] [%2] ").arg(timestamp, level);
        QString padding = QString("").leftJustified(header.size(), ' ');

        *stream << header;

        bool first = true;
        Q_FOREACH(const QVariant &out, msgs)
        {
            if (!out.isNull())
            {
                if (!first)
                    *stream << padding;
                else
                    first = false;
                *stream << out.toString() << endl;
            }
        }
        *stream << endl;
    }

    QTextStream *out;
    QTextStream *err;
};

BasicSTDLoggerEngine::BasicSTDLoggerEngine() :
    LoggerEngine(), d_ptr(new BasicSTDLoggerEnginePrivate(this))
{
    setLogLevelsEnabled(RequiredLevels
#ifndef QT_NO_DEBUG
        | Logger::DebugLevel
#endif
    );
    enableLogging();
}

BasicSTDLoggerEngine::~BasicSTDLoggerEngine()
{
    delete d_ptr;
}

void BasicSTDLoggerEngine::initLoggerEngine()
{
}

void BasicSTDLoggerEngine::killLoggerEngine()
{
}

bool BasicSTDLoggerEngine::isInitialized() const
{
    return d_ptr->out && d_ptr->err;
}

void BasicSTDLoggerEngine::writeFormatted(
        Logger::LogLevel level, const QVariantList &msgs)
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
        name = "!!FATAL!!";
        break;
    case Logger::TraceLevel:
        name = "Trace";
        break;
    case Logger::DebugLevel:
        name = "DEBUG";
        break;
    case Logger::InfoLevel:
        name = "INFO";
        break;
    default:
        break;
    }
    writeToStdOut(name, msgs);
}

void BasicSTDLoggerEngine::setLogLevelEnabled(
        Logger::LogLevels level, bool enable)
{
    // Must not disable required levels.
    if (!enable && (RequiredLevels & level))
        return;
    LoggerEngine::setLogLevelsEnabled(level, enable);
}

QTextStream *BasicSTDLoggerEngine::stdOutStream() const
{
    return d_ptr->out;
}

QTextStream *BasicSTDLoggerEngine::stdErrStream() const
{
    return d_ptr->err;
}

void BasicSTDLoggerEngine::writeToStdOut(
        const QString &level, const QVariantList &msgs)
{
    d_ptr->writeToStream(d_ptr->out, level, msgs);
}

void BasicSTDLoggerEngine::writeToStdErr(
        const QString &level, const QVariantList &msgs)
{
    d_ptr->writeToStream(d_ptr->err, level, msgs);
}

}   // namespace Qxt
