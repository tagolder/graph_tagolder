#ifndef NODE_H
#define NODE_H


#include <QString>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsItem>
#include <QPointF>

class node : public QObject, public QGraphicsItem
{
Q_OBJECT
private:
    QString name;
    int x, y;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void move();
public:
    node();
    node(const node&);
    node &operator= (node);
    explicit node(QString, int x = -1, int y = -1, QObject* parent = 0);
    QString getName();
    int getX();
    int getY();
    void setX(int);
    void setY(int);
    void setName(QString);
    bool operator == (const node&);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private slots:
    //void mouseMove(QMouseEvent* event);

};

#endif // NODE_H
