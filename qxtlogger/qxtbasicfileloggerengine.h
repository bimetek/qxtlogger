#ifndef QXTBASICFILELOGGERENGINE_H
#define QXTBASICFILELOGGERENGINE_H

#include "qxtabstractfileloggerengine.h"

namespace Qxt
{

class BasicFileLoggerEnginePrivate;

class QXTLOGGERSHARED_EXPORT BasicFileLoggerEngine :
        public AbstractFileLoggerEngine
{
    Q_DECLARE_PRIVATE(BasicFileLoggerEngine)
    BasicFileLoggerEnginePrivate * const d_ptr;

public:
    BasicFileLoggerEngine(const QString &fileName = QString());
    ~BasicFileLoggerEngine();

    QString dateFormat() const;
    void setDateFormat(const QString &format);

protected:
    virtual void writeToFile(const QString &level,
                             const QVariantList &messages);
};

}   // namespace Qxt

#ifdef QXTLOGGERGLOBALNAMESPACE
typedef Qxt::BasicFileLoggerEngine QxtBasicFileLoggerEngine;
#endif

#endif // QXTBASICFILELOGGERENGINE_H
