#ifndef SHADERGENERATOR_H
#define SHADERGENERATOR_H

class QGLShaderProgram;

#define COLORSPEC "vec3 point, vec3 normal, vec3 view, vec3 light"
#define COLORCALL "point, normal, view, light"

class ShaderEmitter {
    QGLShaderProgram *m_program;
public:
    ShaderEmitter(QGLShaderProgram* program)
        : m_program(program) { }

    void operator()(const QString &source) const;
};

class ShaderGenerator {
protected:
    QString shader;
public:
    ShaderGenerator() { }
    explicit ShaderGenerator(const QString &fileName);
    virtual ~ShaderGenerator() { }

    virtual void makeShaders(const ShaderEmitter &emitter) = 0;
};

#endif // SHADERGENERATOR_H
