#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <QtCore>

/**
  @class SerializeException
  Общий класс для исключений, возникающих при сериализации.
  Это исключение должно возбуждаться при любых ошибках сериализации:
  как синтаксических, так и семантических.
  */
class SerializeException: public std::exception {
    /// Хранит описание ошибки.
    QString m_what;
public:
    /// Стандартный конструктор.
    SerializeException(const QString &what);
    /// Виртуальный деструктор.
    /// Нужен для правильной перезгрузки деструктора из std::exception учитывая throws.
    ~SerializeException() throw() { }

    /// Описание ошибки.
    const char* what() const throw();
};

/**
  @interface Reader
  Интерфейс для сериализаторов чтения.
  Должен передаваться в конструктор Serializable класса для его загрузки.
  Подразумевает два типа объекта - контейнер типов Serializable и текст.
  Для любого типа объекта можно задавать именованные атрибуты.
  */
class Reader {
public:
    /// Значение атрибута.
    /// @warning Должен вызываться сразу после handleObject()
    /// @param name - имя аттрибута
    virtual QString attrib(const QString& name) const = 0;

    /// Значение текстового объекта.
    /// @warning Должен вызываться сразу после handleObject()
    virtual QString text() const = 0;

    /// Переходит к обработке следующего дочернего объекта и считывает его аттрибуты.
    /// Для каждого вызова handleObject() должен быть парный вызов endObject().
    /// @returns Имя обрабатываемого объекта.
    /// @throws SerializationException если дочерних объектов больше нет.
    virtual QString handleObject() = 0;

    /// Завершает обработку текущего объекта.
    /// @throws SerializationException если остались необработанные дочерние объекты.
    virtual void endObject() = 0;

    /// Читает данные, пока не встретит дочерний объект.
    /// @return Имя дочернего объекта или пустую строку, если их нет.
    virtual QString child() = 0;

    /// Эквивалентно !child().isEmpty()
    /// @return true, если есть необработанные дочерние объекты,
    ///   false иначе.
    bool hasChild();

    /// Виртуальный деструктор
    virtual ~Reader() { }
};

/**
  @interface Writer
  Интерфейс для сериализаторов записи.
  Передаётся в метод Serializable::serialize() для сохранения текущего состояния класса.
  */
class Writer {
public:
    /// Начинает сериализацию объекта и сохраняет его аттрибуты.
    /// Для каждого вызова enterObject() должен быть парный вызов leaveObject().
    /// Аттрибуты задаются перед вызовом enterObject() методом pushAttribute().
    /// @param name - имя сериализуемого объекта
    virtual void enterObject(const QString& name) = 0;

    /// Создаёт аттрибут.
    /// @param name - имя аттрибута.
    /// @param value - значение аттрибута.
    virtual void pushAttribute(const QString& name, const QString& value) = 0;

    /// Выводит текст текущего объекта.
    virtual void writeText(const QString& text) = 0;

    /// Завершает сериализацию объекта.
    virtual void leaveObject() = 0;

    /// Виртуальный деструктор
    virtual ~Writer() { }
};

/**
  @interface Serializable
  Интерфейс для классов, поддерживающих сериализацию.
  */

class Serializable {
public:
    /// Стандартный конструктор.
    /// Ничего не делает.
    Serializable() { }

    /// Конструктор, начинающий сериализацию объекта objectName.
    /// @param objectName - имя объекта.
    /// @param reader - сериализатор чтения.
    /// @throws SerializationException если текущий объект не objectName.
    explicit Serializable(const QString &objectName, Reader* reader);

    /// Виртуальный деструктор.
    virtual ~Serializable() { }

    /// Сериализует объект.
    /// Должен самостоятельно вызвать Writer::enterObject() и Writer::leaveObject().
    /// @param writer - сериализатор записи.
    virtual void serialize(Writer* writer) const = 0;
};

#endif // SERIALIZATION_H
