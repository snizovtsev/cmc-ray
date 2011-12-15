#include "item.h"

const QString objectName = "item";

Item::Item(Reader *reader)
    : Serializable(objectName, reader)
{
    name = reader->attrib("name");

    while (reader->hasChild()) {
        if (reader->child() == "code")
            code = new ShaderCode(reader);
        else if (reader->child() == "ambientOcclusion")
            ambientOcclusion = new ShaderCode(reader);
        else if (reader->child() == "softShadows")
            softShadows = new ShaderCode(reader);
        else if (reader->child() == "material") {
            reader->handleObject();
            material = reader->text();
            endReading(reader);
        } else {
            throw SerializeException("Unexpected element: " + reader->child());
        }
    }

    endReading(reader);
}

Item::~Item()
{
    delete code;
    delete ambientOcclusion;
    delete softShadows;
}

void Item::serialize(Writer *writer) const
{
    writer->pushAttribute("name", name);
    writer->enterObject(objectName);

    code->serialize(writer);
    ambientOcclusion->serialize(writer);
    softShadows->serialize(writer);

    writer->enterObject("material");
    writer->writeText(material);
    writer->leaveObject();

    writer->leaveObject();
}

void Item::makeShaders(QGLShaderProgram *program)
{
}
