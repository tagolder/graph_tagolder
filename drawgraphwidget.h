#ifndef DRAWGRAPHWIDGET_H
#define DRAWGRAPHWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QPolygonF>
#include <cstdlib>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QInputDialog>

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
//        graphDataVector.push_back(gData);
        emit setMatrix();
    }

    void showVertexMenu(std::shared_ptr<Vertex> vert);
    void showEdgeMenu(std::shared_ptr<Edge> edge);
    void showWidgetMenu(QPoint pos);
    void calculateEdgeMoveVertex(std::shared_ptr<Edge> edge);
    std::shared_ptr<GraphData> getGraphData();
    void setMatrixEdges(std::vector<std::vector<double>> &table);
    void unReDo(bool dirTo);
    void addGraphDataInVector();
    bool needEdge = false;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:

    Ui::DrawGraphWidget *ui;

    std::shared_ptr<GraphData> graphData;

    unsigned int graphDataIndex = 0;

    std::vector<std::shared_ptr<GraphData>> graphDataVector;

    std::shared_ptr<Vertex> selectedVertex;

    std::shared_ptr<Edge> selectedEdge;

    std::shared_ptr<Vertex> newSelectedVertex;

    QPoint mouseCoord;

signals:
    void setMatrix();
};

#endif // DRAWGRAPHWIDGET_H
