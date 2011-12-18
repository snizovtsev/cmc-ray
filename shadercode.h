#ifndef SHADERCODE_H
#define SHADERCODE_H

#include <QtCore>

#include "serialization.h"

class ShaderCode : public Serializable
{
    QString objectName;
    QString code;
    QString m_require;

    void construct(Reader* reader);
public:
    explicit ShaderCode(const QString &_objectName, const QString &_code);
    explicit ShaderCode(Reader* reader);
    ShaderCode(QString name, Reader *reader);

    void serialize(Writer *writer) const;

    operator QString() { return code; }
    QString require() const { return m_require; }
};

#endif // SHADERCODE_H
