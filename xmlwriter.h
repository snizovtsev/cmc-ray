#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QtCore>

#include "interfaces.h"

class XMLWriter : public Writer
{
    QXmlStreamWriter* xml;
    QXmlStreamAttributes attr;

public:
    explicit XMLWriter(const QString &fileName);
    ~XMLWriter();

    void enterObject(const QString &name);
    void pushAttribute(const QString &name, const QString &value);
    void writeText(const QString &text);
    void leaveObject();
};

#endif // XMLWRITER_H
