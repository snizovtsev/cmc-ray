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

/**
  @class LambertModel
  Реализует модель Ламберта для диффузного света.
  */
class LambertModel: public ColorModel
{
    /// Основной цвет.
    /// Шейдерное выражение типа <b>vec3</b>
    ShaderCode* color;
public:
    /// Имя модели.
    static QString modelName() { return "lambert"; }

    /// Создаёт модель сериализацией
    /// @param reader - сериализатор.
    explicit LambertModel(Reader *reader) {
        color = new ShaderCode("color", reader);
        reader->endObject();
    }

    /// Виртуальный деструктор.
    ~LambertModel() {
        delete color;
    }

    void serialize(Writer *writer) const {
        writer->pushAttribute("model", modelName());
        writer->enterObject("diffuse");
        color->serialize(writer);
        writer->leaveObject();
    }

    void makeShaders(const QString& matName, const ShaderEmitter &emitter) {
        ColorModel::makeShaders(emitter);
        const char* shader = "vec3 lambert(vec3 normal, vec3 light, vec3 color);\n"
                             "vec3 %1_diffuse(COLORSPEC)\n"
                             "{ return lambert(normal, light, %2); }\n";
        emitter(color->require() + "\n" +
                QString(shader).arg(matName, *color));
    }
};

/**
  @class PhongModel
  Реализует модель Фонга для бликового света.
  */
class PhongModel: public ColorModel
{
    /// Цвет блика.
    /// Шейдерное выражение типа <b>vec3</b>
    ShaderCode* color;
    /// Сила бликов.
    /// Шейдерное выражение типа <b>float</b>
    ShaderCode* shininess;
public:
    /// Имя модели.
    static QString modelName() { return "phong"; }

    /// Создаёт модель сериализацией.
    /// @param reader - сериализатор.
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

    void makeShaders(const QString& matName, const ShaderEmitter &emitter) {
        ColorModel::makeShaders(emitter);
        const char* shader = "vec3 phong(vec3 normal, vec3 view, vec3 color, float shininness);\n"
                             "vec3 %1_specular(COLORSPEC)\n"
                             "{ return phong(normal, view, %2, %3); }\n";
        emitter(color->require() + shininess->require() + "\n" +
                QString(shader).arg(matName, *color, *shininess));
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

