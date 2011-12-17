#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtCore>
#include <QtOpenGL>

#define COLORSPEC "vec3 point, vec3 normal, vec3 view, vec3 light"
#define COLORCALL "point, normal, view, light"

class ShaderGenerator {
protected:
    QString shader;
public:
    ShaderGenerator() { }

    explicit ShaderGenerator(const QString &fileName);
    virtual void makeShaders(QGLShaderProgram* program) = 0;
    virtual ~ShaderGenerator() { }
};

class Reader {
public:
    virtual QString attrib(const QString& name) const = 0;
    virtual QString text() const = 0;

    // Переходит к обработке объекта, заполняет attrib
    // Передвигает указатель дальше
    virtual QString handleObject() = 0;
    // bla-bla-bla
    virtual void endObject() = 0;
    // Пополняет text, пока не найдет ребенка.
    // После чего ожидается вызов handleObject
    // Повторный вызов ничего не меняет!
    virtual QString child() = 0;
    // Возвращает true, если в обрабатываемом объекте
    // ещё есть необработанные дочерние элементы
    bool hasChild();
};

class Writer {
public:
    virtual void enterObject(const QString& name) = 0;
    virtual void pushAttribute(const QString& name, const QString& value) = 0;
    virtual void writeText(const QString& text) = 0;
    virtual void leaveObject() = 0;
};

class SerializeException: public std::exception {
    QString m_what;
public:
    SerializeException(QString what);
    ~SerializeException() throw() { }

    const char* what() const throw();
};

class Serializable {
public:
    Serializable() { }
    explicit Serializable(const QString &objectName, Reader* reader);
    virtual ~Serializable() { }

    virtual void serialize(Writer* writer) const = 0;
};

#endif // INTERFACES_H
