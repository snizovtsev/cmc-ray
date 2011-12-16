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

    explicit ShaderGenerator(QString fileName) {
        QFile data(fileName);
        if (data.open(QFile::ReadOnly))
            shader = QTextStream(&data).readAll();
    }

    virtual void makeShaders(QGLShaderProgram* program) = 0;
    virtual ~ShaderGenerator() { }
};

class Reader {
public:
    virtual const QString attrib(const QString& name) const = 0;
    virtual const QString& text() const = 0;

    // Переходит к обработке объекта, заполняет attrib
    // Передвигает указатель дальше
    virtual QString handleObject() = 0;
    // Пополняет text, пока не найдет ребенка.
    // После чего ожидается вызов handleObject
    // Повторный вызов ничего не меняет!
    virtual QString child() = 0;
    // Возвращает true, если в обрабатываемом объекте
    // ещё есть необработанные дочерние элементы
    bool hasChild() {
        return !child().isNull();
    }
};

class Writer {
public:
    virtual void enterObject(const QString& name) = 0;
    virtual void pushAttribute(const QString& name, const QString& value);
    virtual void writeText(const QString& text) = 0;
    virtual void leaveObject() = 0;
};

class SerializeException: public std::exception {
    const QString m_what;
public:
    SerializeException(QString what) throw()
        : std::exception(), m_what(what)
    { }

    ~SerializeException() throw() { }

    const char* what() { return qPrintable(m_what); }
};

class Serializable {
public:
    Serializable() { }

    explicit Serializable(QString objectName, Reader* reader) {
        if (reader->handleObject() != objectName)
            throw SerializeException("Unexpected object");
    }

    void endReading(Reader* reader) {
        if (reader->hasChild())
            throw SerializeException("Extra children found");
    }

    virtual void serialize(Writer* writer) const = 0;
    virtual ~Serializable() { }
};

#endif // INTERFACES_H
