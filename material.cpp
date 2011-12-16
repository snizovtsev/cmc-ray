#include "material.h"

const QString objectName = "material";

Material::Material(Reader *reader)
    : Serializable(objectName, reader)
{
    diffuse = specular = 0;
    dynamic = reader->attrib("dynamic") == "true";

    while (reader->hasChild()) {
        if (!diffuse && reader->child() == "diffuse") {
            reader->handleObject();
            diffuse = ColorModel::createDiffuse(reader->attrib("model"), reader);
        } else if (!specular && reader->child() == "specular") {
            reader->handleObject();
            specular = ColorModel::createSpecular(reader->attrib("model"), reader);
        } else {
            throw SerializeException("Unknown children");
        }
    }

    if (!diffuse || !specular)
        throw SerializeException("Both diffuse and specular models should be defined");

    endReading(reader);
}

Material::~Material()
{
    delete diffuse;
    delete specular;
}

void Material::serialize(Writer *writer) const
{
    writer->pushAttribute("name", name());
    writer->pushAttribute("dynamic", dynamic ? "true" : "false");
    writer->enterObject(objectName);
    diffuse->serialize(writer);
    specular->serialize(writer);
    writer->leaveObject();
}

void Material::makeShaders(QGLShaderProgram *program)
{
}
