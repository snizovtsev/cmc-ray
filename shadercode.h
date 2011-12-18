#ifndef SHADERCODE_H
#define SHADERCODE_H

#include <QtCore>

#include "serialization.h"

class ShaderCode : public Serializable
{
    QString objectName;
    QString code;

    void construct(Reader* reader);
public:
    explicit ShaderCode(const QString &_objectName, const QString &_code);
    explicit ShaderCode(Reader* reader);
    ShaderCode(QString name, Reader *reader);

    void serialize(Writer *writer) const;

    operator QString() { return code; }
};

#endif // SHADERCODE_H
