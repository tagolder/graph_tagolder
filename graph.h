#ifndef GRAPH_H
#define GRAPH_H

#define Radius 8

#include "edge.h"
#include "node.h"
#include <QPen>
#include <QString>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsLineItem>
#include <QGraphicsView>

class Graph : public QGraphicsView
{
Q_OBJECT
public slots:
    void rePaint();
private:
    QGraphicsScene* scene = new QGraphicsScene();
    QMap<node> nodes;
    QVector<edge> edges;
    QVector<QVector<int> > matrix_inc;
    QVector<QVector<int> > matrix_sme;
    int weight = 0;
    bool fix = false;
public:
    explicit Graph(QWidget *parent = 0);
    Graph(int);
//    void setObjectName(QString);
    node getNode(QString);
    void addNode(node);
    void addEdge(const edge &);
    void setFix(bool);
    bool getFix();
};

#endif // GRAPH_H
