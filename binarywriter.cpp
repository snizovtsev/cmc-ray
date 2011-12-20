#include "binarywriter.h"

enum TokenType {
    START_ELEMENT,
    END_ELEMENT,
    BODY_ELEMENT,
    TEXT_ELEMENT
};

BinaryWriter::BinaryWriter(const QString &fileName)
    : file(fileName)
{
    if (!file.open(QFile::WriteOnly))
        throw SerializeException(QString("Can't open %1 for writing").arg(fileName));
    stream.setDevice(&file);
}

void BinaryWriter::enterObject(const QString &name)
{
    stream << START_ELEMENT;
    stream << name;

    stream << BODY_ELEMENT;
    stream << attrs.count();
    foreach (typeof(*attrs.begin()) attr, attrs)
        stream << attr.first << attr.second;
    attrs.clear();
}

void BinaryWriter::pushAttribute(const QString &name, const QString &value)
{
    attrs += QPair<QString,QString>(name, value);
}

void BinaryWriter::writeText(const QString &text)
{
    stream << TEXT_ELEMENT;
    stream << text;
}

void BinaryWriter::leaveObject()
{
    stream << END_ELEMENT;
}
