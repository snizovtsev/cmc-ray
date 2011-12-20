/**
  @mainpage Принцип работы трассировщика лучей.
    На вход даётся файл, из которого сериализуются классы, реализующие
  ShaderGenerator. Далее каждый из них генерирует один или более
  фрагментных шейдеров, которые потом линкуются вместе.

    Для генерации теней прямой трассировки лучей не происходит. Вместо
  этого они имитируются при помощи Ambient Occlusion и Soft Shadow. Т.е.
  делаются выборки из точки по направлению нормали и по направлению света
  соответвенно, после чего считается магическая формула.

    Возможна имитация стеклянных предметов. Для этого задаётся коэфициент
  прозрачности (factor) и коэфициент преломления (indexOfRefraction). Далее
  происходит трассировка отражённого и преломлённого луча, но они уже не
  трассируются повторно, если встретят снова стеклянный объект. Далее
  отражённый и преломлённый свет смешиваются по коефициенту Фреснеля.

    Архитектура позволяет прозрачно реализовать несколько моделей света
  (сейчас реализованы только модель Ламберта и Фонга), процедурные текстуры,
  несложно доработать движёк для поддержки процедурного Bump Mapping-а.

  @bug После перехода на новую архитектуру, в которой шейдеры генерируются, код
  стал менее оптимален и некоторые компиляторы GLSL падают на этапе линковки.
  Если это произошло, можно попытать уменьшить количество объектов или материалов.

  @author Сергей Низовцев
  */

#include <QtGui/QApplication>
#include "widget.h"

#include "xmlreader.h"
#include "xmlwriter.h"
#include "binaryreader.h"
#include "binarywriter.h"

static void testSerialization()
{
    XMLReader xml_in("scene.xml");
    Scene scene1(&xml_in);

    {XMLWriter xml_out1("test/scene-out1.xml");
    scene1.serialize(&xml_out1);}

    {BinaryWriter bin_out("test/scene.bin");
    scene1.serialize(&bin_out);}

    BinaryReader bin_in("test/scene.bin");
    Scene scene2(&bin_in);

    {XMLWriter xml_out2("test/scene-out2.xml");
    scene2.serialize(&xml_out2);}
}

int main(int argc, char *argv[])
{
    testSerialization();

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
