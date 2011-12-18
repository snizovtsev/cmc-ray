#include "shadercode.h"

void ShaderCode::construct(Reader *reader)
{
    objectName = reader->handleObject();
    code = reader->text();
    reader->endObject();
}

ShaderCode::ShaderCode(const QString &_objectName, const QString &_code)
    : objectName(_objectName), code(_code)
{ }

ShaderCode::ShaderCode(Reader *reader)
{
    construct(reader);
}

ShaderCode::ShaderCode(QString name, Reader *reader)
{
    if (reader->child() != name)
        throw SerializeException(name + " element expected");

    construct(reader);

    Q_ASSERT(objectName == name);
}

void ShaderCode::serialize(Writer *writer) const
{
    writer->enterObject(objectName);
    writer->writeText(code);
    writer->leaveObject();
}
