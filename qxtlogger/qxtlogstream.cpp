#include "qxtlogstream.h"
#include <QSharedData>

namespace Qxt
{

LogStream::LogStream(
        Logger *owner, Logger::LogLevel level, const QVariantList &data) :
    d_ptr(new LogStreamPrivate(owner, level, data))
{

}

LogStream::LogStream(const LogStream &other) : d_ptr(other.d_ptr)
{
}

LogStream &LogStream::operator<<(const QVariant &value)
{
    d_ptr->data.append(value);
    return *this;
}

}   // namespace Qxt
