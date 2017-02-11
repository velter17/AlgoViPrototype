/**
 * Project   AlgoVi
 *
 * @file     CTerminalSettings.cpp
 * @author   Dmytro Sadovyi
 * @date     28.01.2017
 */

#include "../CTerminalSettings.h"

namespace NSettings
{

CTerminalSettings::CTerminalSettings(const std::string& configPath)
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
        QDomElement x = node.toElement();
        if(x.tagName() == "HomeDir")
            this->setHomeDir(x.text());
        else if(x.tagName() == "Font")
            this->setFont(x.text());
        else if(x.tagName() == "FontSize")
            this->setFontSize(x.text().toInt());
        else if(x.tagName() == "Title")
            this->setTitle(x.text());
        else if(x.tagName() == "HistorySize")
            this->setHistorySize(x.text().toInt());
        else if(x.tagName() == "Colors")
            parseColors(node);
        node = node.nextSibling();
    }
}

void CTerminalSettings::parseColors(QDomNode rootNode)
{
    QDomNode node = rootNode.firstChild();
    while(!node.isNull())
    {
        QDomElement color = node.toElement();
        this->setColor(color.attributeNode("name").value(), color.text());
        node = node.nextSibling();
    }
}

} // namespace NSettings
