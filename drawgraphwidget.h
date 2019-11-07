#ifndef DRAWGRAPHWIDGET_H
#define DRAWGRAPHWIDGET_H

#define COEFFICIENT_EQUATION 0.23


#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QPolygonF>
#include <cstdlib>

#include "graphdata.h"

namespace Ui {
class DrawGraphWidget;
}

class DrawGraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrawGraphWidget(QWidget *parent = 0);
    ~DrawGraphWidget();

    inline void setGraphData(std::shared_ptr<GraphData> gData)
    {
        graphData = gData;
    }

    void showVertexMenu(std::shared_ptr<Vertex> vert, QPoint pos);
    void showEdgeMenu(std::shared_ptr<Edge> edge, QPoint pos);
    //void calculatePolygon(QPoint v1, QPoint v2, QPoint mousePos);
    void calculateEdgeMoveVertex(std::shared_ptr<Edge> edge);
    std::shared_ptr<GraphData> getGraphData();

    bool needEdge = false;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:

    Ui::DrawGraphWidget *ui;

    std::shared_ptr<GraphData> graphData;

    std::shared_ptr<Vertex> selectedVertex;

    std::shared_ptr<Edge> selectedEdge;

    std::shared_ptr<Vertex> newSelectedVertex;

    QPoint mouseCoord;

};

#endif // DRAWGRAPHWIDGET_H
