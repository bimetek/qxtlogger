#ifndef QXTLOGGER_P_H
#define QXTLOGGER_P_H

#include <QObject>
#include "qxtlogger.h"
class QMutex;

namespace Qxt
{

class QXTLOGGERSHARED_EXPORT LoggerPrivate : public QObject
{
    Q_DECLARE_PUBLIC(Logger)
    Logger * const q_ptr;

public:
    LoggerPrivate(Logger *q);

    inline void toggleEngineForLevel(
            LoggerEngine *e, Logger::LogLevel level, Logger::LogLevel criteria)
    {
        e->setLogLevelsEnabled(criteria, (criteria > level));
    }

    void setEngineMinimumLevel(LoggerEngine *engine, Logger::LogLevel level);

    QMutex *lock;
    QHash<QString, LoggerEngine *> logEngineMap;

    Q_INVOKABLE void log(Logger::LogLevel level, const QVariantList &args);
};

}   // namespace Qxt

#ifdef QXTLOGGERGLOBALNAMESPACE
typedef Qxt::LoggerPrivate QxtLoggerPrivate;
#endif

#endif // QXTLOGGER_P_H
