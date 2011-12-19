#ifndef SHADERGENERATOR_H
#define SHADERGENERATOR_H

class QGLShaderProgram;

/**
  @class ShaderEmitter
  Компилирует фрагментный шейдер.
  Класс-обёртка для QGLShaderProgram, созданный для упрощения API, отладки,
  добавления констант и макроопределений. В данный момент автоматически
  определяет макросы COLORSPEC и COLORCALL.
  */
class ShaderEmitter {
    /// Хранит откомпилированные шейдеры.
    QGLShaderProgram *m_program;
public:
    /// Стандартный конструктор.
    /// @param program - шейдерная программа, в которую будут добавляться
    ///   созданные шейдеры.
    ShaderEmitter(QGLShaderProgram* program)
        : m_program(program) { }

    /// Компилирует и добавляет к программе шейдер.
    /// В случае ошибки выдаёт диагностическое сообщение в stderr.
    /// @param source - код шейдера
    void operator()(const QString &source) const;
};

/**
  @interface ShaderGenerator
  Интерфейс для классов, которые будут создавать шейдеры.
  */
class ShaderGenerator {
protected:
    /// Код стандартного шейдера.
    /// Многие классы создают шейдер из файла-шаблона. Чтобы упростить их
    /// загрузку создан специальный конструктор и эта переменная.
    QString shader;
public:
    /// Стандартный конструктор. Ничего не делает.
    ShaderGenerator() { }
    /// Конструктор, который загружает код шейдера из файла в #shader.
    /// @param fileName - файл, из которого нужно загрузить шаблон шейдера.
    explicit ShaderGenerator(const QString &fileName);
    /// Виртуальный деструктор.
    virtual ~ShaderGenerator() { }

    /// Генерирует шейдеры.
    /// @param emitter - программа, в которую добавить созданные шейдеры.
    virtual void makeShaders(const ShaderEmitter &emitter) = 0;
};

#endif // SHADERGENERATOR_H
