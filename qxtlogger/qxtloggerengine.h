#ifndef QXTLOGGERENGINE_H
#define QXTLOGGERENGINE_H

#include <QtGlobal>
#include "qxtlogger.h"

namespace Qxt
{

class LoggerEnginePrivate;

class QXTLOGGERSHARED_EXPORT LoggerEngine
{
    Q_DECLARE_PRIVATE(LoggerEngine)
    LoggerEnginePrivate *d_ptr;

public:
    LoggerEngine();
    virtual ~LoggerEngine();

    virtual void initLoggerEngine() = 0;
    virtual void killLoggerEngine() = 0;
    virtual bool isInitialized() const = 0;

    virtual void writeFormatted(Logger::LogLevel level,
                                const QVariantList &messages) = 0;

    virtual void setLoggingEnabled(bool enable = true);
    bool isLoggingEnabled() const;
    inline void enableLogging()  { setLoggingEnabled(true);  }
    inline void disableLogging() { setLoggingEnabled(false); }

    virtual void setLogLevelsEnabled(
            Logger::LogLevels levels, bool enable = true);
    bool isLogLevelEnabled(Logger::LogLevel level) const;
    inline void enableLogLevels(Logger::LogLevels levels);
    inline void disableLogLevels(Logger::LogLevels levels);
};

void LoggerEngine::enableLogLevels(Logger::LogLevels levels)
{
    setLogLevelsEnabled(levels, true);
}

void LoggerEngine::disableLogLevels(Logger::LogLevels levels)
{
    setLogLevelsEnabled(levels, false);
}

}   // namespace Qxt

#ifdef QXTLOGGERGLOBALNAMESPACE
typedef Qxt::LoggerEngine QxtLoggerEngine;
#endif

#endif // QXTLOGGERENGINE_H
