#ifndef QXTLOGGER_H
#define QXTLOGGER_H

#include "qxtlogger_global.h"
#include <QObject>
#include <QVariant>

namespace Qxt
{

class LoggerPrivate;
class LoggerEngine;
class LogStream;

class QXTLOGGERSHARED_EXPORT Logger : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Logger)
    LoggerPrivate * const d_ptr;

    // Hidden to be implemented as singleton.
    explicit Logger();

public:

    enum LogLevel
    {
        NoLevels       = 0,
        TraceLevel     = 1 << 0,
        DebugLevel     = 1 << 1,
        InfoLevel      = 1 << 2,
        WarningLevel   = 1 << 3,
        ErrorLevel     = 1 << 4,
        CriticalLevel  = 1 << 5,
        FatalLevel     = 1 << 6,
        WriteLevel     = 1 << 7,
        AllLevels      = (TraceLevel | DebugLevel | InfoLevel | WarningLevel
                        | ErrorLevel | CriticalLevel | FatalLevel | WriteLevel)
    };
    Q_DECLARE_FLAGS(LogLevels, LogLevel)

    static QString logLevelToString(LogLevel level);
    static LogLevel stringToLogLevel(const QString& level);
    static Logger *getInstance();

    void initLoggerEngine(const QString &engineName);
    void killLoggerEngine(const QString &engineName);

    void installAsMessageHandler();
    void removeAsMessageHandler();

    void addLoggerEngine(const QString &engineName, LoggerEngine *engine);
    void removeLoggerEngine(const QString &engineName);
    void removeLoggerEngine(LoggerEngine *engine);
    LoggerEngine *takeLoggerEngine(const QString &engineName);
    LoggerEngine *engine(const QString &engineName) const;

    // Functions for checking loggers.
    QStringList allLoggerEngines() const;
    QStringList allEnabledLoggerEngines() const;
    QStringList allEnabledLoggerEngines(LogLevel level) const;
    QStringList allDisabledLoggerEngines() const;

    bool isLogLevelEnabled(const QString &engineName, LogLevel level) const;
    bool isLoggerEngine(const QString &engineName) const;
    bool isLoggerEngineEnabled(const QString &engineName) const;

    LogStream stream(LogLevel level);

    // Unfortunately we can't inline these due to header file ordering.
    LogStream trace();
    LogStream debug();
    LogStream info();
    LogStream warning();
    LogStream error();
    LogStream critical();
    LogStream fatal();
    LogStream write();

    void enableLogLevels(LogLevels levels);
    void enableLogLevels(const QString &engineName, LogLevels levels);
    void enableAllLogLevels();
    void enableAllLogLevels(const QString &engineName);
    void enableLoggerEngine(const QString &engineName);

    void disableLogLevels(LogLevels levels);
    void disableLogLevels(const QString &engineName, LogLevels levels);
    void disableAllLogLevels();
    void disableAllLogLevels(const QString &engineName);
    void disableLoggerEngine(const QString &engineName);

    void setMinimumLevel(LogLevel level);
    void setMinimumLevel(const QString &engineName, LogLevel level);

public Q_SLOTS:
    void log(
            LogLevel level, const QVariant &message,
            const QVariant &m1 = QVariant(), const QVariant &m2 = QVariant(),
            const QVariant &m3 = QVariant(), const QVariant &m4 = QVariant(),
            const QVariant &m5 = QVariant(), const QVariant &m6 = QVariant(),
            const QVariant &m7 = QVariant(), const QVariant &m8 = QVariant(),
            const QVariant &m9 = QVariant());
    inline void info(
            const QVariant &message, const QVariant &m1 = QVariant(),
            const QVariant &m2 = QVariant(), const QVariant &m3 = QVariant(),
            const QVariant &m4 = QVariant(), const QVariant &m5 = QVariant(),
            const QVariant &m6 = QVariant(), const QVariant &m7 = QVariant(),
            const QVariant &m8 = QVariant(), const QVariant &m9 = QVariant())
    {
        log(InfoLevel, message, m1, m2, m3, m4, m5, m6, m7, m8, m9);
    }
    inline void trace(
            const QVariant &message, const QVariant &m1 = QVariant(),
            const QVariant &m2 = QVariant(), const QVariant &m3 = QVariant(),
            const QVariant &m4 = QVariant(), const QVariant &m5 = QVariant(),
            const QVariant &m6 = QVariant(), const QVariant &m7 = QVariant(),
            const QVariant &m8 = QVariant(), const QVariant &m9 = QVariant())
    {
        log(TraceLevel, message, m1, m2, m3, m4, m5, m6, m7, m8, m9);
    }
    inline void warning(
            const QVariant &message, const QVariant &m1 = QVariant(),
            const QVariant &m2 = QVariant(), const QVariant &m3 = QVariant(),
            const QVariant &m4 = QVariant(), const QVariant &m5 = QVariant(),
            const QVariant &m6 = QVariant(), const QVariant &m7 = QVariant(),
            const QVariant &m8 = QVariant(), const QVariant &m9 = QVariant())
    {
        log(WarningLevel, message, m1, m2, m3, m4, m5, m6, m7, m8, m9);
    }
    inline void error(
            const QVariant &message, const QVariant &m1 = QVariant(),
            const QVariant &m2 = QVariant(), const QVariant &m3 = QVariant(),
            const QVariant &m4 = QVariant(), const QVariant &m5 = QVariant(),
            const QVariant &m6 = QVariant(), const QVariant &m7 = QVariant(),
            const QVariant &m8 = QVariant(), const QVariant &m9 = QVariant())
    {
        log(ErrorLevel, message, m1, m2, m3, m4, m5, m6, m7, m8, m9);
    }
    inline void debug(
            const QVariant &message, const QVariant &m1 = QVariant(),
            const QVariant &m2 = QVariant(), const QVariant &m3 = QVariant(),
            const QVariant &m4 = QVariant(), const QVariant &m5 = QVariant(),
            const QVariant &m6 = QVariant(), const QVariant &m7 = QVariant(),
            const QVariant &m8 = QVariant(), const QVariant &m9 = QVariant())
    {
        log(DebugLevel, message, m1, m2, m3, m4, m5, m6, m7, m8, m9);
    }
    inline void critical(
            const QVariant &message, const QVariant &m1 = QVariant(),
            const QVariant &m2 = QVariant(), const QVariant &m3 = QVariant(),
            const QVariant &m4 = QVariant(), const QVariant &m5 = QVariant(),
            const QVariant &m6 = QVariant(), const QVariant &m7 = QVariant(),
            const QVariant &m8 = QVariant(), const QVariant &m9 = QVariant())
    {
        log(CriticalLevel, message, m1, m2, m3, m4, m5, m6, m7, m8, m9);
    }
    inline void fatal(
            const QVariant &message, const QVariant &m1 = QVariant(),
            const QVariant &m2 = QVariant(), const QVariant &m3 = QVariant(),
            const QVariant &m4 = QVariant(), const QVariant &m5 = QVariant(),
            const QVariant &m6 = QVariant(), const QVariant &m7 = QVariant(),
            const QVariant &m8 = QVariant(), const QVariant &m9 = QVariant())
    {
        log(FatalLevel, message, m1, m2, m3, m4, m5, m6, m7, m8, m9);
    }
    inline void write(
            const QVariant &message, const QVariant &m1 = QVariant(),
            const QVariant &m2 = QVariant(), const QVariant &m3 = QVariant(),
            const QVariant &m4 = QVariant(), const QVariant &m5 = QVariant(),
            const QVariant &m6 = QVariant(), const QVariant &m7 = QVariant(),
            const QVariant &m8 = QVariant(), const QVariant &m9 = QVariant())
    {
        log(WriteLevel, message, m1, m2, m3, m4, m5, m6, m7, m8, m9);
    }

    void log(LogLevel level, const QVariantList &args);
    inline void info(const QVariantList &args)     { log(InfoLevel, args);     }
    inline void trace(const QVariantList &args)    { log(TraceLevel, args);    }
    inline void warning(const QVariantList &args)  { log(WarningLevel, args);  }
    inline void error(const QVariantList &args)    { log(ErrorLevel, args);    }
    inline void debug(const QVariantList &args)    { log(DebugLevel, args);    }
    inline void critical(const QVariantList &args) { log(CriticalLevel, args); }
    inline void fatal(const QVariantList &args)    { log(FatalLevel, args);    }
    inline void write(const QVariantList &args)    { log(WriteLevel, args);    }

Q_SIGNALS:
    void loggerEngineAdded(const QString &engineName);
    void loggerEngineRemoved(const QString &engineName);
    void loggerEngineEnabled(const QString &engineName);
    void loggerEngineDisabled(const QString &engineName);
};

}   // namespace Qxt

Q_DECLARE_OPERATORS_FOR_FLAGS(Qxt::Logger::LogLevels)
Q_DECLARE_METATYPE(Qxt::Logger::LogLevel)
Q_DECLARE_METATYPE(Qxt::Logger::LogLevels)

#define qxtLog (Qxt::Logger::getInstance())

#ifdef QXTLOGGERGLOBALNAMESPACE
typedef Qxt::Logger QxtLogger;
#endif

#include "qxtlogstream.h"

#endif // QXTLOGGER_H
