#ifndef DRAWGRAPHWIDGET_H
#define DRAWGRAPHWIDGET_H

//#define COEFFICIENT_EQUATION 0.23

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
        emit setMatrix();
    }

    void showVertexMenu(std::shared_ptr<Vertex> vert);
    void showEdgeMenu(std::shared_ptr<Edge> edge);
    void showWidgetMenu(QPoint pos);
    void calculateEdgeMoveVertex(std::shared_ptr<Edge> edge);
    std::shared_ptr<GraphData> getGraphData();
//    void setMatixOnTable();
    void setModelTable(QStandardItemModel &model);
    bool needEdge = false;
//    void setMatrixTable(std::vector<std::vector<double>> &matrix);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:

    Ui::DrawGraphWidget *ui;

    std::shared_ptr<GraphData> graphData;

    std::shared_ptr<Vertex> selectedVertex;

    std::shared_ptr<Edge> selectedEdge;

    std::shared_ptr<Vertex> newSelectedVertex;

    QPoint mouseCoord;

signals:
    void setMatrix();
};

#endif // DRAWGRAPHWIDGET_H
