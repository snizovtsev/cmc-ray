#include "refract.h"

const char* objectName = "refract";
const int defaultRefractLimit = 150;
const int defaultReflectLimit = 150;

Refract::Refract(const QString &id, Reader *reader)
    : ShaderGenerator(":/refract.frag"),
      Serializable(objectName, reader),
      m_id(id),
      reflectLimit(defaultReflectLimit),
      refractLimit(defaultRefractLimit)
{
    indexOfRefraction = 0;
    m_factor = 0;

    while (reader->hasChild()) {
        QString child = reader->child();
        if (child == "indexOfRefraction") {
            indexOfRefraction = new ShaderCode(reader);
        } else if (child == "factor") {
            m_factor = new ShaderCode(reader);
        } else if (child == "reflectLimit") {
            reader->handleObject();
            bool ok; reflectLimit = reader->text().simplified().toInt(&ok);
            reader->endObject();
            if (!ok || reflectLimit < 0)
                throw SerializeException("Invalid value in reflectLimit");
        } else if (child == "refractLimit") {
            reader->handleObject();
            bool ok; refractLimit = reader->text().simplified().toInt(&ok);
            reader->endObject();
            if (!ok || refractLimit < 0)
                throw SerializeException("Invalid value in refractLimit");
        } else {
            throw SerializeException("Unknown element");
        }
    }

    if (!indexOfRefraction || !m_factor)
        throw SerializeException("indexOfRefraction or factor is not defined");

    reader->endObject();
}

Refract::~Refract()
{
    delete indexOfRefraction;
    delete m_factor;
}

void Refract::serialize(Writer *writer) const
{
    writer->enterObject(objectName);
    indexOfRefraction->serialize(writer);
    m_factor->serialize(writer);
    if (reflectLimit != defaultReflectLimit) {
        writer->enterObject("reflectLimit");
        writer->writeText(QString::number(reflectLimit));
        writer->leaveObject();
    }
    if (refractLimit != defaultRefractLimit) {
        writer->enterObject("refractLimit");
        writer->writeText(QString::number(refractLimit));
        writer->leaveObject();
    }
    writer->leaveObject();
}

void Refract::makeShaders(const ShaderEmitter &emitter)
{
    static bool libraryEmitted = false;
    if (!libraryEmitted) {
        emitter(shader);
        libraryEmitted = true;
    }

    QString shader = "vec3 trace_refraction(COLORSPEC, float indexOfRefraction, int reflectLimit, int refractLimit);\n";
    shader += QString(
                "vec3 %1_refracted(COLORSPEC) {\n"
                "  return trace_refraction(COLORCALL, %2, %3, %4);\n"
                "}\n")
            .arg(m_id, *indexOfRefraction, QString::number(reflectLimit), QString::number(refractLimit));
    emitter(shader);
}
