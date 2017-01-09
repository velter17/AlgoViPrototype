/**
 * Project   Graviz
 *
 * @file     CHintProvider.h
 * @author   Dmytro Sadovyi
 * @date     15.12.2016
 */

#pragma once

#include <map>
#include <QStringList>
#include <QPair>

#include "Types.h"

namespace NController
{

typedef QPair<QStringList, int> tHint;

class CHintProvider
{
public: // methods
    CHintProvider();

    tHint getHints(const QString& input);

private: // methods
    template <TCommandHintName cmd>
    tHint hints(const QString& input);

    tHint processHints(const QStringList& list, const QString& str);

private: // fields
    typedef tHint (CHintProvider::*THintHandler)(const QString& args);

    std::map <QString, THintHandler> mFunc;
    QStringList mAppList;
};

} // namespace NController

