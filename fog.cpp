#include "fog.h"

const QString objectName = "fog";

Fog::Fog(Reader *reader)
    : ShaderGenerator(":/fog.frag"),
      Serializable(objectName, reader)
{
    while (reader->hasChild()) {
        if (!intensity && reader->child() == "intensity")
            intensity = new ShaderCode(reader);
        else if (!color && reader->child() == "color")
            color = new ShaderCode(reader);
        else
            throw SerializeException("Unexpected element");
    }

    if (!intensity || !color)
        throw SerializeException("");

    endReading(reader);
}

Fog::~Fog()
{
    delete intensity;
    delete color;
}

void Fog::serialize(Writer *writer) const
{
    writer->enterObject(objectName);
    intensity->serialize(writer);
    writer->leaveObject();
}

void Fog::makeShaders(QGLShaderProgram *program)
{
    program->addShaderFromSourceCode(QGLShader::Fragment,
                                     shader.arg(*intensity, *color));
}
