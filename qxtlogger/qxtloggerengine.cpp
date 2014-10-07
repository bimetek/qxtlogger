#include "qxtloggerengine.h"

namespace Qxt
{

class LoggerEnginePrivate
{
    Q_DECLARE_PUBLIC(LoggerEngine)
    LoggerEngine *q_ptr;

public:
    LoggerEnginePrivate(LoggerEngine *q) :
        q_ptr(q), logLevels(Logger::AllLevels), isLogging(false) {}

    Logger::LogLevels logLevels;
    bool isLogging;
};

LoggerEngine::LoggerEngine() :
    d_ptr(new LoggerEnginePrivate(this))
{
}

LoggerEngine::~LoggerEngine()
{
    Logger::getInstance()->removeLoggerEngine(this);
    delete d_ptr;
}

void LoggerEngine::setLoggingEnabled(bool enable)
{
    Q_D(LoggerEngine);
    d->isLogging = enable;
}

bool LoggerEngine::isLoggingEnabled() const
{
    return d_ptr->isLogging;
}

void LoggerEngine::setLogLevelsEnabled(Logger::LogLevels levels, bool enable)
{
    Q_D(LoggerEngine);
    if (enable)
        d->logLevels |= levels;
    else
        d->logLevels &= ~levels;
}

bool LoggerEngine::isLogLevelEnabled(Logger::LogLevel level) const
{
    return d_ptr->logLevels & level;
}

}   // namespace Qxt
