#include "scene.h"

const QString objectName = "scene";

#define foreach_material(var) \
    for (MaterialStorage::const_iterator var = material.begin(); var != material.end(); ++var)

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

    foreach_material(mat)
        delete mat.value();

    foreach (Item* item, items)
        delete item;
}

void Scene::serialize(Writer *writer) const
{
    writer->enterObject(objectName);
    light->serialize(writer);
    eye->serialize(writer);
    fog->serialize(writer);

    foreach_material(mat)
        mat.value()->serialize(writer);

    foreach (Item* item, items)
        item->serialize(writer);

    writer->leaveObject();
}

void Scene::makeShaders(QGLShaderProgram *program)
{
    program->addShaderFromSourceCode(QGLShader::Fragment, shader.arg(*light));
    foreach_material(mat)
        mat.value()->makeShaders(program);

    QString imports;
    QString distanceAt =
            "float distanceAt(vec3 p, out int object) {\n"
            "   float cur, best = 1e10;\n";
    QString colorAt =
            "vec3 colorAt(int object, vec3 point, vec3 normal, vec3 view, vec3 light) {";

    int object = 1;
    foreach (Item* item,  items) {
        imports += QString("float %1(vec3 p);\n").arg(item->entryPoint());
        distanceAt += QString("  if ((cur = %1(p)) < best) { best = cur; object = %2; }\n")
                .arg(item->entryPoint()).arg(object);
        colorAt += QString("  if (object == %1) { return %2_colorAt(point, normal, view, light); }\n")
                .arg(object).arg(item->material());
        ++object;
    }
    distanceAt += "}\n";
    colorAt += "  return vec3(0);\n}\n";

    program->addShaderFromSourceCode(QGLShader::Fragment, imports + distanceAt + colorAt);
}
