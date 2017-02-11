/**
 * Project   AlgoVi
 *
 * @file     CCommandSettings.cpp
 * @author   Dmytro Sadovyi
 * @date     28.01.2017
 */

#include <QtXml>

#include "../CCommandSettings.h"

namespace NSettings
{

CCommandSettings::CCommandSettings(const std::string& configPath)
{
    QFile file(QString::fromStdString(configPath));
    QDomDocument document;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw std::runtime_error("Failed to open " + configPath);
    }
    document.setContent(&file);
    QDomNode node = document.firstChildElement().firstChild();
    while(!node.isNull())
    {
        QDomElement command = node.toElement();
        this->setCommand(command.attributeNode("name").value(), command.text());
        node = node.nextSibling();
    }
}

QStringList CCommandSettings::getCommandList()
{
    QStringList ret;
    for(const auto& itr : mCommand)
    {
        ret << itr.first;
    }
    return ret;
}


} // namespace NSettings
