#include "scene.h"

const QString objectName = "scene";

Scene::Scene(Reader* reader)
    : ShaderGenerator(":/scene.frag"),
      Serializable(objectName, reader)
{
    light = 0;
    eye = 0;
    fog = 0;

    while (reader->hasChild()) {
        if (reader->child() == "fog") {
            fog = new Fog(reader);
        } else if (reader->child() == "light") {
            light = new ShaderCode(reader);
        } else if (reader->child() == "eye") {
            eye = new ShaderCode(reader);
        } else if (reader->child() == "material") {
            Material* mat = new Material(reader);
            if (material.contains(mat->name())) {
                delete mat;
                throw SerializeException("Material redeclaration");
            }
            material.insert(mat->name(), mat);
        } else if (reader->child() == "item") {
            items += new Item(reader);
        } else {
            throw SerializeException("Unexpected children");
        }
    }

    if (!fog || !light || !eye)
        throw SerializeException("Fog, light and eye are required");

    endReading(reader);
}

Scene::~Scene()
{
    delete light;
    delete eye;
    delete fog;

    for (MaterialStorage::const_iterator i = material.begin(); i != material.end(); ++i)
        delete i.value();

    foreach (Item* item, items)
        delete item;
}

void Scene::serialize(Writer *writer) const
{
    writer->enterObject(objectName);
    light->serialize(writer);
    eye->serialize(writer);
    fog->serialize(writer);

    for (MaterialStorage::const_iterator i = material.begin(); i != material.end(); ++i) {
        writer->pushAttribute("name", i.key());
        i.value()->serialize(writer);
        ++i;
    }

    foreach (Item* item, items)
        item->serialize(writer);

    writer->leaveObject();
}

void Scene::makeShaders(QGLShaderProgram *program)
{
    program->addShaderFromSourceCode(QGLShader::Fragment, shader);
}
