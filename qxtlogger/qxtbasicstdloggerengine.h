#ifndef QXTBASICSTDLOGGERENGINE_H
#define QXTBASICSTDLOGGERENGINE_H

#include "qxtloggerengine.h"
class QTextStream;

namespace Qxt
{

class BasicSTDLoggerEnginePrivate;

class QXTLOGGERSHARED_EXPORT BasicSTDLoggerEngine : public LoggerEngine
{
    Q_DECLARE_PRIVATE(LoggerEngine)
    BasicSTDLoggerEnginePrivate * const d_ptr;

public:
    BasicSTDLoggerEngine();
    ~BasicSTDLoggerEngine();

    void initLoggerEngine();
    void killLoggerEngine();
    bool isInitialized() const;

    void writeFormatted(Logger::LogLevel level, const QVariantList &msgs);
    void setLogLevelEnabled(Logger::LogLevels level, bool enable = true);

    QTextStream *stdOutStream() const;
    QTextStream *stdErrStream() const;

protected:
    virtual void writeToStdOut(const QString &level, const QVariantList &msgs);
    virtual void writeToStdErr(const QString &level, const QVariantList &msgs);

};

}   // namespace Qxt

#ifdef QXTLOGGERGLOBALNAMESPACE
typedef Qxt::BasicSTDLoggerEngine QxtBasicSTDLoggerEngine;
#endif

#endif // QXTBASICSTDLOGGERENGINE_H
