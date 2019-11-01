#include "graph.h"

Graph::Graph(QWidget *parent) : QGraphicsView(parent)
{
    setScene(scene);
}


Graph::Graph(int w)
{
    weight = w;
    setScene(scene);
}

void Graph::addEdge(const edge &ed)
{
    for(edge &e : edges)
    {
        if((e.getA() == ed.getA()) & (e.getB() == ed.getB()))
        {
            e.addAllNum();
            ed.setNum(e.getAllNum());
            ed.setAllNum(e.getAllNum());
        }
    }
    edges.append(ed);

    const QString &nodeA = ed.getA();
    const QString &nodeB = ed.getB();

    if(nodes.indexOf(getNode(nodeA)) == -1)
    {
        nodes.append( node(ed.getA()) );
    }
    if(nodes.indexOf(getNode(ed.getB())) == -1)nodes.append(node(ed.getB()));

}

void Graph::addNode(node n)
{
    if(nodes.indexOf(n) == -1)
    {
        n.setPos(n.getX(), n.getY());
        scene->addItem(&n);
//        connect(item, SIGNAL(move()), SLOT(rePaint()));
        nodes.append(n);
       // connect()
        //connect(nodes.at(nodes.size()-1), SIGNAL())
    }
    //QGraphicsItem = new node(n)
}


bool Graph::getFix()
{
    return fix;
}

void Graph::setFix(bool b)
{
    fix = b;
}

void Graph::rePaint()
{
    scene->clear();
    for(edge e : edges)
    {
        QGraphicsItem *item = new QGraphicsLineItem(getNode(e.getA()).getX(), getNode(e.getA()).getY(), getNode(e.getB()).getX(), getNode(e.getB()).getY());
        scene->addItem(item);
    }
}

node Graph::getNode(QString name)
{
    for(node n : nodes)
    {
        if(n.getName() == name)
        {
            return n;
        }
    }
    return node();
}

