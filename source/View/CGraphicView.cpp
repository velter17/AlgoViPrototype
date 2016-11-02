#include "View/CGraphicView.h"
#include <QDebug>
#include <vector>
#include <QProcess>
#include <fstream>

#define DEFAULT_WIN_HEIGHT 1000
#define DEFAULT_WIN_WIDTH 1000

CGraphicView::CGraphicView()
    : mSolverEnabled(false)
{
    this->setSceneRect(0, 0, DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT);
}

CGraphicView::~CGraphicView()
{

}

void CGraphicView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}

void CGraphicView::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawForeground(painter, rect);
}

