/**
 * Project   Graviz
 *
 * @file     CSolverSettings.cpp
 * @author   Dmytro Sadovyi
 * @date     28.01.2017
 */


#include "../CSolverSettings.h"

namespace NSettings
{

CSolverSettings::CSolverSettings(const std::string& configPath)
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
        if(command.text() == "Compilers")
            parseCompilers(node);
        else if(command.text() == "Solvers")
            parseSolvers(node);
        node = node.nextSibling();
    }
}

void CSolverSettings::parseCompilers(QDomNode rootNode)
{
    QDomNode node = rootNode.firstChild();
    while(!node.isNull())
    {
        QDomElement compiler = node.toElement();
        this->setCompiler(compiler.attributeNode("name").value(), compiler.text());
        node = node.nextSibling();
    }
}

void CSolverSettings::parseSolvers(QDomNode rootNode)
{
    QDomNode node = rootNode.firstChildElement();
    while(!node.isNull())
    {
        QDomElement solver = node.toElement();
        this->setSolver(solver.attributeNode("name").value(), QPair<QString, bool>(
                            solver.text(), solver.attributeNode("type").value() == "compiler"));
        node = node.nextSibling();
    }
}

QStringList CSolverSettings::getSolverList()
{
    QStringList ret;
    for(const auto& itr : mSolver)
    {
        ret << itr.first;
    }
    return ret;
}


} // namespace NSettings
