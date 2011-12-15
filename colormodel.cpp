#include "colormodel.h"
#include "shadercode.h"

// TODO: документировать ShaderCode как "Shader Value with type X"

class LambertModel: public ColorModel
{
    ShaderCode* color;
public:
    static QString modelName() { return "lambert"; }

    explicit LambertModel(Reader *reader) {
        color = new ShaderCode("color", reader);
        endReading(reader);
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

    void makeShaders(QGLShaderProgram *program)
    {

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

        endReading(reader);
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

    void makeShaders(QGLShaderProgram *program)
    {

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

