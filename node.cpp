#include "node.h"


node::node()
{

}

node::node(QString n, int x, int y, QObject *parent)
    : QObject(parent), QGraphicsItem()
{
    name = n;
    this -> x = x;
    this -> y = y;
    //connect(this, SIGNAL(move(QString)), SLOT(repaint());
}

node::node(const node& n)
{
    setName(n.name);
    setX(n.x);
    setY(n.y);
}

node& node::operator= (node n)
{
    return n;
}
QString node::getName()
{
    return name;
}

bool node::operator == (const node & right)
{
    return (name == right.name);
}


int node::getX()
{
    return x;
}

int node::getY()
{
    return y;
}

void node::setX(int X)
{
    qDebug() << QString::number(this->x) << " " << QString::number(X);
    this->x = X;
}

void node::setY(int Y)
{
    this->y = Y;
}

void node::setName(QString n)
{
    name = n;
}


QRectF node::boundingRect() const
{
    return QRectF(-8, -8, 16, 16);
}

void node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::black);
    painter->drawEllipse(QRectF(-8, -8, 16, 16));
    painter->drawText(QRectF(-8, 16, 50, 30), name);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPos(mapToScene(event->pos()));
    this->setX(mapToScene(event->pos()).rx());
    this->setY(mapToScene(event->pos()).rx());
    emit move();
    scene()->views();
}



