/**
 * Project   Graviz
 *
 * @file     CVisualizationController.h
 * @author   Dmytro Sadovyi
 * @date     16.12.2016
 */

#pragma once

#include <QObject>
#include <memory>
#include <map>
#include <QDebug>

#include "View/components/IGravizItem.h"
#include "View/CGraphicView.h"
#include "View/problems/IProblemVisualizer.h"

namespace NView
{

class CVisualizationController : public QObject
{
    Q_OBJECT
public: // methods
    CVisualizationController(std::shared_ptr<CGraphicView> view,
                             std::shared_ptr<IProblemVisualizer> visualizer);
    ~CVisualizationController();

    void finish();

public slots: // methods
    void updateResult(const QString& data);
    void handleInput(const QString& data);

private slots: // methods
    void addPoint(const QPoint& p);

private: // methods
    void update();

signals:
    void sceneChanged(const QString& data);

private: // fields
    std::shared_ptr <CGraphicView> mView;
    std::shared_ptr <IProblemVisualizer> mVisualizer;
    std::map <QString, IGravizItem*> mObjectsMap;
    std::vector <QMetaObject::Connection> mConnections;

    std::map <QString, int> mPointNum;
    QString mCurrentPrefix;
    int mMode;
};

} // namespace NView

