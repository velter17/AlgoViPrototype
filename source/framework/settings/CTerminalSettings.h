/**
 * Project   Graviz
 *
 * @file     CTerminalSettings.h
 * @author   Dmytro Sadovyi
 * @date     28.01.2017
 */

#pragma once

#include <QString>
#include <QtXml>

#include "HelperFunctions.h"

namespace NSettings
{

DECLARE_CLASS_HEAD(TerminalSettings, "configs/terminalConfig.xml")
    DECLARE_ATTRIBUTE(TerminalSettings, HomeDir,     QString)
    DECLARE_ATTRIBUTE(TerminalSettings, Title,       QString)
    DECLARE_ATTRIBUTE(TerminalSettings, Font,        QString)
    DECLARE_ATTRIBUTE(TerminalSettings, FontSize,    int)
    DECLARE_ATTRIBUTE(TerminalSettings, HistorySize, int)
    DECLARE_ATTRIBUTE_MAP(TerminalSettings, Color, QString, QString)

private: // methods
    void parseColors(QDomNode rootNode);

DECLARE_CLASS_END()

} // namespace NSettings

