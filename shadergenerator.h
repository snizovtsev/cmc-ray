#ifndef SHADERGENERATOR_H
#define SHADERGENERATOR_H

class QGLShaderProgram;

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
