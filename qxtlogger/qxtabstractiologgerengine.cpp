#include "qxtabstractiologgerengine.h"

namespace Qxt
{

class AbstractIOLoggerEnginePrivate
{
    Q_DECLARE_PUBLIC(AbstractIOLoggerEngine)
    AbstractIOLoggerEngine * const q_ptr;

public:
    AbstractIOLoggerEnginePrivate(AbstractIOLoggerEngine *q, QIODevice *dev) :
        q_ptr(q), device(dev) {}

    QIODevice *device;
};

AbstractIOLoggerEngine::AbstractIOLoggerEngine(QIODevice *device) :
    LoggerEngine(), d_ptr(new AbstractIOLoggerEnginePrivate(this, device))
{

}

AbstractIOLoggerEngine::~AbstractIOLoggerEngine()
{
    delete d_ptr;
}

void AbstractIOLoggerEngine::setDevice(QIODevice *device)
{
    Q_D(AbstractIOLoggerEngine);
    d->device = device;
}

QIODevice *AbstractIOLoggerEngine::device() const
{
    return d_ptr->device;
}

}   // Qxt
