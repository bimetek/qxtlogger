#ifndef QXTLOGSTREAM_H
#define QXTLOGSTREAM_H

#include <QExplicitlySharedDataPointer>
#include <QVariant>
#include "qxtlogger.h"

namespace Qxt
{

class LogStreamPrivate;

class QXTLOGGERSHARED_EXPORT LogStream
{
    const QExplicitlySharedDataPointer<LogStreamPrivate> d_ptr;
    friend class Logger;

    LogStream(Logger *owner, Logger::LogLevel level, const QVariantList &data);

public:
    LogStream(const LogStream &other);
    LogStream &operator<< (const QVariant &value);
};

class LogStreamPrivate : public QSharedData
{
public:
    LogStreamPrivate(
            Logger *owner, Logger::LogLevel level, const QVariantList &data) :
        owner(owner), level(level), data(data) {}
    ~LogStreamPrivate() { owner->log(level, data); }

    Logger *owner;
    Logger::LogLevel level;
    QVariantList data;
};

}   // namespace Qxt

#ifdef QXTLOGGERGLOBALNAMESPACE
typedef Qxt::LogStream QxtLogStream;
#endif

#endif // QXTLOGSTREAM_H
