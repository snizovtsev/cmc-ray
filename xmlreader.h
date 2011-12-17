#ifndef XMLREADER_H
#define XMLREADER_H

#include "serialization.h"

class XMLReader : public Reader
{
    QXmlStreamReader* xml;
    QFile file;

    QHash <QString, QString> m_attr;

    bool skipUntilStart();
public:
    XMLReader(const QString& fileName);
    ~XMLReader();

    QString attrib(const QString &name) const;
    QString text() const;
    QString handleObject();
    void endObject();
    QString child();
};

#endif // XMLREADER_H
