#include "fog.h"

const QString objectName = "fog";

Fog::Fog(Reader *reader)
    : ShaderGenerator(":/fog.frag"),
      Serializable(objectName, reader)
{
    color = density = 0;

    while (reader->hasChild()) {
        if (!density && reader->child() == "density")
            density = new ShaderCode(reader);
        else if (!color && reader->child() == "color")
            color = new ShaderCode(reader);
        else
            throw SerializeException("Unexpected element");
    }

    if (!density || !color)
        throw SerializeException("Density and color should be defined for fog");

    reader->endObject();
}

Fog::~Fog()
{
    delete density;
    delete color;
}

void Fog::serialize(Writer *writer) const
{
    writer->enterObject(objectName);
    density->serialize(writer);
    writer->leaveObject();
}

void Fog::makeShaders(const ShaderEmitter &emitter)
{
    emitter(shader.arg(*density, *color));
}
