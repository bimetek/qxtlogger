#include "qxtlogger.h"
#include <QMetaEnum>
#include <QMutexLocker>
#include <QStringList>
#include "qxtbasicstdloggerengine.h"
#include "qxtlogger_p.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#   define qxtInstallMessageHandler qInstallMessageHandler
#else
#   define qxtInstallMessageHandler qInstallMsgHandler
#endif

namespace Qxt
{

static const QString DefaultLoggerName = "DEFAULT";

void LoggerMessageHandler(
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(context);
#else
        QtMsgType type, const char *msg) {
#endif
    switch (type)
    {
    case QtDebugMsg:
        Logger::getInstance()->debug(msg, "qdebug");
        break;
    case QtWarningMsg:
        Logger::getInstance()->warning(msg, "qwarning");
        break;
    case QtCriticalMsg:
        Logger::getInstance()->critical(msg, "qcritical");
        break;
    case QtFatalMsg:
        Logger::getInstance()->fatal(msg, "qfatal");
        abort();
        break;
    }
}

LoggerPrivate::LoggerPrivate(Logger *q) :
    QObject(q), q_ptr(q), lock(new QMutex(QMutex::Recursive)) {}

void LoggerPrivate::setEngineMinimumLevel(
        LoggerEngine *engine, Logger::LogLevel level)
{
    QMutexLocker locker(lock);
    Q_UNUSED(locker);

    if (!engine)
        return;

    toggleEngineForLevel(engine, level, Logger::TraceLevel);
    toggleEngineForLevel(engine, level, Logger::DebugLevel);
    toggleEngineForLevel(engine, level, Logger::InfoLevel);
    toggleEngineForLevel(engine, level, Logger::WarningLevel);
    toggleEngineForLevel(engine, level, Logger::ErrorLevel);
    toggleEngineForLevel(engine, level, Logger::CriticalLevel);
    toggleEngineForLevel(engine, level, Logger::FatalLevel);
    toggleEngineForLevel(engine, level, Logger::WriteLevel);
}

void LoggerPrivate::log(Logger::LogLevel level, const QVariantList &args)
{
    Q_FOREACH(LoggerEngine *eng, logEngineMap)
    {
        if (eng && eng->isInitialized() && eng->isLoggingEnabled()
                && eng->isLogLevelEnabled(level))
            eng->writeFormatted(level, args);
    }
}


Logger::Logger() : QObject(), d_ptr(new LoggerPrivate(this))
{
    qRegisterMetaType<Logger::LogLevel>();
    qRegisterMetaType<Logger::LogLevels>();
    addLoggerEngine(DefaultLoggerName, new BasicSTDLoggerEngine());
    setMinimumLevel(DefaultLoggerName, InfoLevel);
}

QString Logger::logLevelToString(Logger::LogLevel level)
{
    return staticMetaObject.enumerator(0).valueToKey(level);
}

Logger::LogLevel Logger::stringToLogLevel(const QString &level)
{
    bool ok = false;
    const char *key = level.toUtf8().constData();
    int lvl = staticMetaObject.enumerator(0).keyToValue(key, &ok);
    return ok ? static_cast<LogLevel>(lvl) : NoLevels;
}

Logger *Logger::getInstance()
{
    static Logger instance;
    return &instance;
}

void Logger::initLoggerEngine(const QString &engineName)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    LoggerEngine *engine = d->logEngineMap.value(engineName, 0);
    if (engine)
        engine->initLoggerEngine();
}

void Logger::killLoggerEngine(const QString &engineName)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    LoggerEngine *engine = d->logEngineMap.value(engineName, 0);
    if (engine)
        engine->killLoggerEngine();
}

void Logger::installAsMessageHandler()
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    qxtInstallMessageHandler(LoggerMessageHandler);
}

void Logger::removeAsMessageHandler()
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    qxtInstallMessageHandler(0);
}

void Logger::addLoggerEngine(const QString &engineName, LoggerEngine *engine)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    if (engine && !d->logEngineMap.contains(engineName))
    {
        d->logEngineMap.insert(engineName, engine);
        emit loggerEngineAdded(engineName);
    }
}

void Logger::removeLoggerEngine(const QString &engineName)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    LoggerEngine *engine = takeLoggerEngine(engineName);
    if (!engine)
        return;
    engine->killLoggerEngine();
    delete engine;
}

void Logger::removeLoggerEngine(LoggerEngine *engine)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    // Remove them from map, but do nothing with them.
    Q_FOREACH(const QString &k, d->logEngineMap.keys(engine))
        takeLoggerEngine(k);

    // Manage memory only once.
    if (!engine)
        return;
    engine->killLoggerEngine();
    delete engine;
}

LoggerEngine *Logger::takeLoggerEngine(const QString &engineName)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    LoggerEngine *engine = d->logEngineMap.take(engineName);
    if (engine)
        emit loggerEngineRemoved(engineName);
    return engine;
}

LoggerEngine *Logger::engine(const QString &engineName) const
{
    QMutexLocker locker(d_ptr->lock);
    Q_UNUSED(locker);

    LoggerEngine *engine = d_ptr->logEngineMap.value(engineName, 0);
    if (!engine || !isLoggerEngine(engineName))
        return 0;
    return engine;
}

QStringList Logger::allLoggerEngines() const
{
    QMutexLocker locker(d_ptr->lock);
    Q_UNUSED(locker);

    return d_ptr->logEngineMap.keys();
}

QStringList Logger::allEnabledLoggerEngines() const
{
    QMutexLocker locker(d_ptr->lock);
    Q_UNUSED(locker);

    QStringList names;
    Q_FOREACH(const QString &name, allLoggerEngines())
    {
        if (engine(name)->isLoggingEnabled())
            names.append(name);
    }
    return names;
}

QStringList Logger::allEnabledLoggerEngines(Logger::LogLevel level) const
{
    QMutexLocker locker(d_ptr->lock);
    Q_UNUSED(locker);

    QStringList names;
    Q_FOREACH(const QString &name, allLoggerEngines())
    {
        LoggerEngine *eng = engine(name);
        if (eng->isLoggingEnabled() && eng->isLogLevelEnabled(level))
            names.append(name);
    }
    return names;
}

QStringList Logger::allDisabledLoggerEngines() const
{
    QMutexLocker locker(d_ptr->lock);
    Q_UNUSED(locker);

    QStringList names;
    Q_FOREACH(const QString &name, allLoggerEngines())
    {
        if (!engine(name)->isLoggingEnabled())
            names.append(name);
    }
    return names;
}

bool Logger::isLogLevelEnabled(
        const QString &engineName, Logger::LogLevel level) const
{
    QMutexLocker locker(d_ptr->lock);
    Q_UNUSED(locker);

    LoggerEngine *eng = engine(engineName);
    return eng ? eng->isLogLevelEnabled(level) : false;
}

bool Logger::isLoggerEngine(const QString &engineName) const
{
    QMutexLocker locker(d_ptr->lock);
    Q_UNUSED(locker);

    return d_ptr->logEngineMap.contains(engineName);
}

bool Logger::isLoggerEngineEnabled(const QString &engineName) const
{
    QMutexLocker locker(d_ptr->lock);
    Q_UNUSED(locker);

    LoggerEngine *eng = engine(engineName);
    return eng ? eng->isLoggingEnabled() : false;
}

LogStream Logger::stream(Logger::LogLevel level)
{
    return LogStream(this, level, QVariantList());
}

LogStream Logger::trace()
{
    return stream(Logger::TraceLevel);
}

LogStream Logger::debug()
{
    return stream(Logger::DebugLevel);
}

LogStream Logger::info()
{
    return stream(Logger::InfoLevel);
}

LogStream Logger::warning()
{
    return stream(Logger::WarningLevel);
}

LogStream Logger::error()
{
    return stream(Logger::ErrorLevel);
}

LogStream Logger::critical()
{
    return stream(Logger::CriticalLevel);
}

LogStream Logger::fatal()
{
    return stream(Logger::FatalLevel);
}

LogStream Logger::write()
{
    return stream(Logger::WriteLevel);
}

void Logger::enableLogLevels(Logger::LogLevels levels)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    Q_FOREACH(const QString &name, d->logEngineMap.keys())
        enableLogLevels(name, levels);
}

void Logger::enableLogLevels(
        const QString &engineName, Logger::LogLevels levels)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    LoggerEngine *eng = engine(engineName);
    if (eng)
        eng->enableLogLevels(levels);
}

void Logger::enableAllLogLevels()
{
    enableLogLevels(AllLevels);
}

void Logger::enableAllLogLevels(const QString &engineName)
{
    enableLogLevels(engineName, AllLevels);
}

void Logger::enableLoggerEngine(const QString &engineName)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    LoggerEngine *eng = engine(engineName);
    if (eng)
    {
        eng->disableLogging();
        emit loggerEngineEnabled(engineName);
    }
}

void Logger::disableLogLevels(Logger::LogLevels levels)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    Q_FOREACH(const QString &name, d->logEngineMap.keys())
        disableLogLevels(name, levels);
}

void Logger::disableLogLevels(
        const QString &engineName, Logger::LogLevels levels)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    LoggerEngine *eng = engine(engineName);
    if (eng)
        eng->disableLogLevels(levels);
}

void Logger::disableAllLogLevels()
{
    disableLogLevels(AllLevels);
}

void Logger::disableAllLogLevels(const QString &engineName)
{
    disableLogLevels(engineName, AllLevels);
}

void Logger::disableLoggerEngine(const QString &engineName)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    LoggerEngine *eng = engine(engineName);
    if (eng)
    {
        eng->disableLogging();
        emit loggerEngineDisabled(engineName);
    }
}

void Logger::setMinimumLevel(Logger::LogLevel level)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    Q_FOREACH(const QString &name, d->logEngineMap.keys())
        setMinimumLevel(name, level);
}

void Logger::setMinimumLevel(const QString &engineName, Logger::LogLevel level)
{
    Q_D(Logger);
    d->setEngineMinimumLevel(engine(engineName), level);
}

void Logger::log(
        Logger::LogLevel level, const QVariant &message,
        const QVariant &msg1, const QVariant &msg2, const QVariant &msg3,
        const QVariant &msg4, const QVariant &msg5, const QVariant &msg6,
        const QVariant &msg7, const QVariant &msg8, const QVariant &msg9)
{
    Q_D(Logger);

    QMutexLocker locker(d->lock);
    Q_UNUSED(locker);

    QVariantList args;
    args.append(message);
    if (!msg1.isNull())
        args.append(msg1);
    if (!msg2.isNull())
        args.append(msg2);
    if (!msg3.isNull())
        args.append(msg3);
    if (!msg4.isNull())
        args.append(msg4);
    if (!msg5.isNull())
        args.append(msg5);
    if (!msg6.isNull())
        args.append(msg6);
    if (!msg7.isNull())
        args.append(msg7);
    if (!msg8.isNull())
        args.append(msg8);
    if (!msg9.isNull())
        args.append(msg9);
    log(level, args);
}

void Logger::log(Logger::LogLevel level, const QList<QVariant> &args)
{
    Q_D(Logger);
    QMetaObject::invokeMethod(
                d, "log", Qt::AutoConnection, Q_ARG(LogLevel, level),
                Q_ARG(QVariantList, args));
}



}   // namespace Qxt
