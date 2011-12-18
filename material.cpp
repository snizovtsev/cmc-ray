#include "material.h"

const QString objectName = "material";

Material::Material(Reader *reader)
    : ShaderGenerator(":/refract.frag"),
      Serializable(objectName, reader)
{
    diffuse = specular = 0;
    refract = 0;
    m_name = reader->attrib("name");

    while (reader->hasChild()) {
        QString child = reader->child();

        if (!diffuse && child == "diffuse") {
            reader->handleObject();
            diffuse = ColorModel::createDiffuse(reader->attrib("model"), reader);
        } else if (!specular && child == "specular") {
            reader->handleObject();
            specular = ColorModel::createSpecular(reader->attrib("model"), reader);
        } else if (!refract && child == "refract") {
            refract = new Refract(m_name, reader);
        } else {
            throw SerializeException("Unknown children");
        }
    }

    if (!diffuse || !specular)
        throw SerializeException("Both diffuse and specular models should be defined");

    reader->endObject();
}

Material::~Material()
{
    delete diffuse;
    delete specular;
    delete refract;
}

void Material::serialize(Writer *writer) const
{
    writer->pushAttribute("name", name());
    writer->enterObject(objectName);
    diffuse->serialize(writer);
    specular->serialize(writer);
    if (refract)
        refract->serialize(writer);
    writer->leaveObject();
}

void Material::makeShaders(const ShaderEmitter &emitter)
{
    diffuse->makeShaders(name(), emitter);
    specular->makeShaders(name(), emitter);
    if (refract)
        refract->makeShaders(emitter);

    QString colorAt = QString("vec3 %1_diffuse(COLORSPEC);\n"
                              "vec3 %1_specular(COLORSPEC);\n"
                              "vec3 mat_%1_colorAt(COLORSPEC) {\n"
                              "  return %1_diffuse(COLORCALL) +\n"
                              "    %1_specular(COLORCALL);"
                              "}\n").arg(name());
    emitter(colorAt);
}
