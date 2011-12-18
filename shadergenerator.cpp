#include <QtOpenGL>

#include "shadergenerator.h"

ShaderGenerator::ShaderGenerator(const QString &fileName)
{
    QFile data(fileName);
    if (data.open(QFile::ReadOnly))
        shader = QTextStream(&data).readAll();
}

#define COLORSPEC "vec3 point, vec3 normal, vec3 view, vec3 light"
#define COLORCALL "point, normal, view, light"

void ShaderEmitter::operator ()(const QString& source) const
{
    if (!m_program->addShaderFromSourceCode(QGLShader::Fragment,
                                            QString("#define COLORSPEC " COLORSPEC "\n"
                                                    "#define COLORCALL " COLORCALL "\n")
                                            + source))
    {
        qDebug() << "Here is the source:";
        qDebug("%s", qPrintable(source));
    }
}
