#include "item.h"

const QString objectName = "item";

Item::Item(Reader *reader)
    : ShaderGenerator(":/item.frag"),
      Serializable(objectName, reader)
{
    m_name = reader->attrib("name");
    shadowForce = 0;

    while (reader->hasChild()) {
        if (reader->child() == "shader")
            code = new ShaderCode(reader);
        else if (reader->child() == "shadowForce")
            shadowForce = new ShaderCode(reader);
        else if (reader->child() == "material") {
            reader->handleObject();
            m_material = reader->text().simplified();
            reader->endObject();
        } else {
            throw SerializeException("Unexpected element: " + reader->child());
        }
    }

    if (!shadowForce)
        shadowForce = new ShaderCode("shadowForce", "1.0");

    reader->endObject();
}

Item::~Item()
{
    delete code;
    delete shadowForce;
}

void Item::serialize(Writer *writer) const
{
    writer->pushAttribute("name", m_name);
    writer->enterObject(objectName);

    code->serialize(writer);
    shadowForce->serialize(writer);

    writer->enterObject("material");
    writer->writeText(m_material);
    writer->leaveObject();

    writer->leaveObject();
}

void Item::makeShaders(const ShaderEmitter &emitter)
{
    static bool libraryEmitted = false;
    if (!libraryEmitted) {
        emitter(shader);
        libraryEmitted = true;
    }

    // Compile user's distance field shader
    emitter(*code);

    QString shader = QString("vec3 mat_%1_colorAt%4(COLORSPEC);\n").arg(material());
    shader += "float shadowAt(vec3 p, vec3 n, vec3 light);\n";

    shader += "vec3 %1_colorAt%4(COLORSPEC) {\n"
            "   float shadowTerm = 1.0 - (1.0 - shadowAt(point, normal, light)) * (%3);\n"
            "   return shadowTerm * mat_%2_colorAt%4(COLORCALL);\n"
            "}\n";

    emitter(shader.arg(name(), material(), *shadowForce, ""));
    emitter(shader.arg(name(), material(), *shadowForce, "_no_refract"));
}
