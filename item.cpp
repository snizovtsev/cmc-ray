#include "item.h"

const QString objectName = "item";

Item::Item(Reader *reader)
    : Serializable(objectName, reader)
{
    m_name = reader->attrib("name");

    while (reader->hasChild()) {
        if (reader->child() == "code")
            code = new ShaderCode(reader);
        else if (reader->child() == "ambientOcclusion")
            ambientOcclusion = new ShaderCode(reader);
        else if (reader->child() == "softShadows")
            softShadows = new ShaderCode(reader);
        else if (reader->child() == "material") {
            reader->handleObject();
            m_material = reader->text();
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
    writer->pushAttribute("name", m_name);
    writer->enterObject(objectName);

    code->serialize(writer);
    ambientOcclusion->serialize(writer);
    softShadows->serialize(writer);

    writer->enterObject("material");
    writer->writeText(m_material);
    writer->leaveObject();

    writer->leaveObject();
}

void Item::makeShaders(QGLShaderProgram *program)
{
    program->addShaderFromSourceCode(QGLShader::Fragment, *code);

    QString shader = QString("vec3 mat_%1_colorAt(" COLORSPEC ");\n").arg(material());

    shader += "vec3 %1_colorAt(" COLORSPEC ") {\n"
            "   return mat_%2_colorAt(" COLORCALL ");\n"
            "}\n";

    program->addShaderFromSourceCode(QGLShader::Fragment, shader);
}
