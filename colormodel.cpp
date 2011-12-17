#include "colormodel.h"
#include "shadercode.h"

ColorModel::ColorModel()
    : ShaderGenerator(":/colormodel.frag")
{ }

void ColorModel::makeShaders(const ShaderEmitter &emitter)
{
    static bool registerLibrary = true;
    if (registerLibrary) {
        emitter(shader);
        registerLibrary = false;
    }
}

class LambertModel: public ColorModel
{
    ShaderCode* color;
public:
    static QString modelName() { return "lambert"; }

    explicit LambertModel(Reader *reader) {
        color = new ShaderCode("color", reader);
        reader->endObject();
    }

    ~LambertModel() {
        delete color;
    }

    void serialize(Writer *writer) const {
        writer->pushAttribute("model", modelName());
        writer->enterObject("diffuse");
        color->serialize(writer);
        writer->leaveObject();
    }

    void makeShaders(const QString& itemName, const ShaderEmitter &emitter) {
        ColorModel::makeShaders(emitter);
        const char* shader = "vec3 lambert(vec3 normal, vec3 light, vec3 color);\n"
                             "vec3 %1_diffuse(" COLORSPEC ")\n"
                             "{ return lambert(normal, light, %2); }\n";
        emitter(QString(shader).arg(itemName, *color));
    }
};

class PhongModel: public ColorModel
{
    ShaderCode* color;
    ShaderCode* shininess;
public:
    static QString modelName() { return "phong"; }

    explicit PhongModel(Reader *reader) {
        color = shininess = 0;

        while (reader->hasChild()) {
            if (reader->child() == "color")
                color = new ShaderCode(reader);
            else if (reader->child() == "shininess")
                shininess = new ShaderCode(reader);
            else
                throw SerializeException("Unexpected element: " + reader->child());
        }

        if (!color || !shininess)
            throw SerializeException("Color or shininess are not defined");

        reader->endObject();
    }

    ~PhongModel() {
        delete color;
        delete shininess;
    }

    void serialize(Writer *writer) const {
        writer->pushAttribute("model", modelName());
        writer->enterObject("specular");
        color->serialize(writer);
        shininess->serialize(writer);
        writer->leaveObject();
    }

    void makeShaders(const QString& itemName, const ShaderEmitter &emitter) {
        ColorModel::makeShaders(emitter);
        const char* shader = "vec3 phong(vec3 normal, vec3 view, vec3 color, float shininness);\n"
                             "vec3 %1_specular(" COLORSPEC ")\n"
                             "{ return phong(normal, view, %2, %3); }\n";
        emitter(QString(shader).arg(itemName, *color, *shininess));
    }
};

ColorModel* ColorModel::createDiffuse(const QString &name, Reader* reader)
{
    if (name == "lambert")
        return new LambertModel(reader);
    else
        throw SerializeException("Unknown diffuse model: " + name);
}

ColorModel* ColorModel::createSpecular(const QString &name, Reader* reader)
{
    if (name == "phong")
        return new PhongModel(reader);
    else
        throw SerializeException("Unknown specular model: " + name);
}

