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
    if (!m_program->addShaderFromSourceCode(QGLShader::Fragment, source)) {
        qDebug() << "Here is the source:";
        qDebug("%s", qPrintable(source));
    }
}
