#ifndef QXTABSTRACTIOLOGGERENGINE_H
#define QXTABSTRACTIOLOGGERENGINE_H

#include "qxtloggerengine.h"

namespace Qxt
{

class AbstractIOLoggerEnginePrivate;

class QXTLOGGERSHARED_EXPORT AbstractIOLoggerEngine : public LoggerEngine
{
    Q_DECLARE_PRIVATE(AbstractIOLoggerEngine)
    AbstractIOLoggerEnginePrivate * const d_ptr;

public:
    AbstractIOLoggerEngine(QIODevice *device = 0);
    ~AbstractIOLoggerEngine();

    void setDevice(QIODevice *device);
    QIODevice *device() const;
};

}   // namespace Qxt

#ifdef QXTLOGGERGLOBALNAMESPACE
typedef Qxt::AbstractIOLoggerEngine QxtAbstractIOLoggerEngine;
#endif

#endif // QXTABSTRACTIOLOGGERENGINE_H
