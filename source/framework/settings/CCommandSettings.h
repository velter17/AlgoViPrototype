/**
 * Project   Graviz
 *
 * @file     CCommandSettings.h
 * @author   Dmytro Sadovyi
 * @date     28.01.2017
 */

#pragma once

#include <QString>

#include "HelperFunctions.h"

namespace NSettings
{

DECLARE_CLASS_HEAD(CommandSettings, "configs/commandConfig.xml")
    DECLARE_ATTRIBUTE_MAP(CommandSettings, Command, QString, QString)

public: // methods
    QStringList getCommandList();

DECLARE_CLASS_END()

} // namespace NSettings

