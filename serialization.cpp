#include "serialization.h"

#ifdef Q_OS_LINUX
#include "execinfo.h"
#endif

Serializable::Serializable(const QString &objectName, Reader *reader)
{
    QString found;
    if ((found = reader->handleObject()) != objectName)
        throw SerializeException(QString("Unexpected object: ") + found);
}

bool Reader::hasChild()
{
    return !child().isNull();
}

SerializeException::SerializeException(const QString &what)
    : m_what(what)
{
#if 0 //Q_OS_LINUX
    int nptrs;
    const int size = 4096;
    void *buffer[size];
    char** strings;

    nptrs = backtrace(buffer, size);

    strings = backtrace_symbols(buffer, nptrs);
    if (strings != NULL) {
        m_what += '\n';
        for (int i = 0; i < nptrs; ++i)
            m_what += QString(strings[i]) + "\n";
        free(strings);
    }
#endif
}

const char *SerializeException::what() const throw()
{
    return qPrintable(m_what);
}
