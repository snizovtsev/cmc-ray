#include "material.h"

const QString objectName = "material";

Material::Material(Reader *reader)
    : Serializable(objectName, reader)
{
    diffuse = specular = 0;

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
    writer->enterObject(objectName);
    diffuse->serialize(writer);
    specular->serialize(writer);
    writer->leaveObject();
}

void Material::makeShaders(QGLShaderProgram *program)
{
    diffuse->makeShaders(name(), program);
    specular->makeShaders(name(), program);

    QString colorAt = QString("vec3 mat_%1_colorAt(" COLORSPEC ") {\n"
                              "  return %1_diffuse(" COLORCALL ") +"
                              "    %1_specular(" COLORCALL ");"
                              ).arg(name());
    program->addShaderFromSourceCode(QGLShader::Fragment, colorAt);
}
