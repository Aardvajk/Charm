#include "Settings.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include <QtCore/QFile>
#include <QtCore/QRect>
#include <QtCore/QTextStream>

#include "debug/Assert.h"

namespace
{

QString
rectToString(const QRect &r)
{
    return QString("%1 %2 %3 %4").arg(r.left()).arg(r.top()).arg(r.width()).arg(r.height());
}

QRect
stringToRect(const QString &s)
{
    QString t = s;
    QTextStream ts(&t);

    int x, y, w, h;
    ts >> x >> y >> w >> h;

    return QRect(x, y, w, h);
}

QVariant
createValue(QDomElement &element)
{
    QString type = element.attribute("type");

    if(type == "int")
    {
        return element.firstChild().toText().data().toInt();
    }
    else if(type == "float")
    {
        return element.firstChild().toText().data().toFloat();
    }
    else if(type == "string")
    {
        return element.firstChild().toText().data();
    }
    else if(type == "bool")
    {
        return element.firstChild().toText().data() == "1";
    }
    else if(type == "rect")
    {
        return stringToRect(element.firstChild().toText().data());
    }
    else if(type == "list")
    {
        QList<QVariant> values;
        for(int i = 0; i < element.childNodes().count(); ++i)
        {
            QDomElement child = element.childNodes().at(i).toElement();
            values.append(createValue(child));
        }

        return values;
    }

    return QVariant();
}

void
load(QDomNode &node, Settings *settings)
{
    for(int i = 0; i < node.childNodes().count(); ++i)
    {
        QDomElement element = node.childNodes().at(i).toElement();

        Settings *value = settings->add(element.tagName(), QVariant());

        QVariant v = createValue(element);

        if(v.isValid())
        {
            value->setValue(v);
        }
        else
        {
            load(element, value);
        }
    }
}

void
createNode(QDomDocument &doc, QDomElement &valueElement, const QVariant &value)
{
    if(value.type() == QVariant::Int)
    {
        valueElement.setAttribute("type", "int");
        valueElement.appendChild(doc.createTextNode(QString::number(value.toInt())));
    }
    else if(value.type() == QVariant::Double || value.type() == static_cast<QVariant::Type>(QMetaType::Float))
    {
        valueElement.setAttribute("type", "float");
        valueElement.appendChild(doc.createTextNode(QString::number(value.toFloat())));
    }
    else if(value.type() == QVariant::String)
    {
        valueElement.setAttribute("type", "string");
        valueElement.appendChild(doc.createTextNode(value.toString()));
    }
    else if(value.type() == QVariant::Bool)
    {
        valueElement.setAttribute("type", "bool");
        valueElement.appendChild(doc.createTextNode(value.toBool() ? "1" : "0"));
    }
    else if(value.type() == QVariant::Rect)
    {
        valueElement.setAttribute("type", "rect");
        valueElement.appendChild(doc.createTextNode(rectToString(value.toRect())));
    }
    else if(value.type() == QVariant::List)
    {
        valueElement.setAttribute("type", "list");
        foreach(const QVariant &v, value.toList())
        {
            QDomElement element = doc.createElement("Item");
            valueElement.appendChild(element);

            createNode(doc, element, v);
        }
    }
}

void
save(QDomDocument &doc, QDomElement &root, Settings *settings)
{
    for(int i = 0; i < settings->childCount(); ++i)
    {
        Settings *value = settings->child(i);

        QDomElement valueElement = doc.createElement(value->name());
        root.appendChild(valueElement);

        if(value->childCount())
        {
            save(doc, valueElement, value);
        }
        else
        {
            createNode(doc, valueElement, value->value());
        }
    }
}

}

Settings::~Settings()
{
    qDeleteAll(nodeChildren);
}

void
Settings::clear()
{
    if(!nodeChildren.isEmpty())
    {
        qDeleteAll(nodeChildren);
        nodeChildren.clear();
    }
}

void
Settings::setName(const QString &name)
{
    nodeName = name;
}

void
Settings::setValue(const QVariant &value)
{
    clear();
    nodeValue = value;
}

Settings*
Settings::add(const QString &name, const QVariant &value)
{
    nodeChildren.append(new Settings(name, value));
    return nodeChildren.back();
}

Settings*
Settings::child(const QString &name)
{
    for(int i = 0; i < nodeChildren.count(); ++i)
    {
        if(nodeChildren[i]->nodeName == name)
        {
            return nodeChildren[i];
        }
    }

    return add(name, QVariant());
}

Settings*
Settings::child(int index)
{
    Assert(index >= 0 && index < nodeChildren.count());

    return nodeChildren[index];
}

bool
Settings::contains(const QString &name) const
{
    for(int i = 0; i < nodeChildren.count(); ++i)
    {
        if(nodeChildren[i]->nodeName == name)
        {
            return true;
        }
    }

    return false;
}

bool
Settings::loadFromXml(const QString &path, Settings *settings)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QDomDocument doc;
        doc.setContent(file.readAll());

        QDomNode node = doc.firstChild();
        load(node, settings);

        return true;
    }

    return false;
}

bool
Settings::saveToXml(const QString &path, Settings *settings)
{
    QDomDocument doc;
    doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");

    QDomElement root = doc.createElement("Root");
    doc.appendChild(root);

    save(doc, root, settings);

    QFile file(path);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(doc.toByteArray());
        return true;
    }

    return false;
}
