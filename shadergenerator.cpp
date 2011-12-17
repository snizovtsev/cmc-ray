#include <QtOpenGL>

#include "shadergenerator.h"

ShaderGenerator::ShaderGenerator(const QString &fileName)
{
    QFile data(fileName);
    if (data.open(QFile::ReadOnly))
        shader = QTextStream(&data).readAll();
}

void ShaderEmitter::operator ()(const QString& source) const
{
    m_program->addShaderFromSourceCode(QGLShader::Fragment, source);
}
