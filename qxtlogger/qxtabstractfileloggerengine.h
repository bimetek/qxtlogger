#ifndef QXTABSTRACTFILELOGGERENGINE_H
#define QXTABSTRACTFILELOGGERENGINE_H

#include "qxtabstractiologgerengine.h"

namespace Qxt
{

class AbstractFileLoggerEnginePrivate;

class QXTLOGGERSHARED_EXPORT AbstractFileLoggerEngine :
        public AbstractIOLoggerEngine
{
    Q_DECLARE_PRIVATE(AbstractFileLoggerEngine)
    AbstractFileLoggerEnginePrivate * const d_ptr;

public:
    AbstractFileLoggerEngine(const QString &fileName, QIODevice::OpenMode mode);
    ~AbstractFileLoggerEngine();

    virtual void initLoggerEngine();
    virtual void killLoggerEngine();
    virtual bool isInitialized() const;

    virtual void writeFormatted(Logger::LogLevel level,
                                const QVariantList &messages);

    void setLogFileName(const QString &fileName);
    QString logFileName() const;

protected:
    virtual void writeToFile(const QString &level,
                             const QVariantList &messages) = 0;
};

}   // namespace Qxt

#ifdef QXTLOGGERGLOBALNAMESPACE
typedef Qxt::AbstractFileLoggerEngine QxtAbstractFileLoggerEngine;
#endif

#endif // QXTABSTRACTFILELOGGERENGINE_H
