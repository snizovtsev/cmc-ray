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
        QString child = reader->child();

        if (child == "fog") {
            fog = new Fog(reader);
        } else if (child == "light") {
            light = new ShaderCode(reader);
        } else if (child == "eye") {
            eye = new ShaderCode(reader);
        } else if (child == "material") {
            Material* mat = new Material(reader);
            if (material.contains(mat->name())) {
                delete mat;
                throw SerializeException("Material redeclaration");
            }
            material.insert(mat->name(), mat);
        } else if (child == "item") {
            items += new Item(reader);
        } else {
            throw SerializeException("Unexpected children");
        }
    }

    if (!fog || !light || !eye)
        throw SerializeException("Fog, light and eye are required");

    reader->endObject();
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

void Scene::makeShaders(const ShaderEmitter &emitter)
{
    fog->makeShaders(emitter);

    foreach_material(mat)
        mat.value()->makeShaders(emitter);

    QString imports;
    QString distanceAt =
            "float distanceAt(vec3 p, out int object) {\n"
            "   float cur, best = 1e10;\n"
            "   object = -2; // noneObject\n";
    QString colorAt =
            "vec3 colorAt%1(int object, COLORSPEC) {\n";

    int object = 1;
    foreach (Item* item,  items) {
        item->makeShaders(emitter);
        imports += QString("float %1(vec3 p);\n").arg(item->name());
        imports += QString("vec3 %1_colorAt%2(COLORSPEC);\n").arg(item->name(), "%1");
        distanceAt += QString("  cur = %1(p); if (cur < best) { best = cur; object = %2; }\n")
                .arg(item->name()).arg(object);
        colorAt += QString("  if (object == %1) { return %2_colorAt%3(COLORCALL); }\n")
                .arg(QString::number(object), item->name(), "%1");
        ++object;
    }
    distanceAt += "  return best;\n}\n";
    colorAt += "  return vec3(0);\n}\n";

    emitter(imports.arg("") + imports.arg("_no_refract") + distanceAt +
            colorAt.arg("") + colorAt.arg("_no_refract"));
    emitter(shader.arg(*light, *eye));
}
