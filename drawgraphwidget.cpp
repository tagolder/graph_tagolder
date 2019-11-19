#include "drawgraphwidget.h"
#include "ui_drawgraphwidget.h"

DrawGraphWidget::DrawGraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawGraphWidget)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    setFocus();
    emit setMatrix();
}

DrawGraphWidget::~DrawGraphWidget()
{
    delete ui;
}

void DrawGraphWidget::showVertexMenu(std::shared_ptr<Vertex> vert/*, QPoint pos*/)
{
    QMenu menu;
    QList<QAction*> actions;

    QAction *deleteAction = new QAction("Удалить");
    connect(deleteAction, &QAction::triggered, [=](){
        graphData->removeVertex(vert);
        this->update();
    });
    QAction *loopAction = new QAction("Добавить петлю");
    connect(loopAction, &QAction::triggered, [=](){
        graphData->setLoopM(vert);
        this->update();
    });

    QAction *editNameAction = new QAction("Изменить имя");
    connect(editNameAction, &QAction::triggered, [=](){
        bool ok;
        QString name = QInputDialog::getText(this, tr("Изменение имени вершины"),
                                           tr("Имя : "), QLineEdit::Normal, "", &ok);
        if (ok && !name.isEmpty())
        {
            vert->name = name.toStdString();
        }
        this->update();
    });
    actions.append(deleteAction);
    actions.append(loopAction);
    actions.append(editNameAction);

    menu.exec(actions, this->mapToGlobal(QPoint(vert->coordX, vert->coordY)));
}

void DrawGraphWidget::showEdgeMenu(std::shared_ptr<Edge> edge)
{
    QMenu menu;
    QList<QAction*> actions;

    QAction *deleteAction = new QAction("Удалить");
    connect(deleteAction, &QAction::triggered, [=](){
        //graphData->removeVertex(vert);
        this->update();
    });
    QAction *isDirAction = new QAction("Направить/убрать направление");
    connect(isDirAction, &QAction::triggered, [=](){
        edge->isDir = !edge->isDir;
        this->update();
    });

    QAction *dirToAction = new QAction("Перенаправить");
    connect(dirToAction, &QAction::triggered, [=](){
        edge->dirTo = !edge->dirTo;
        this->update();
    });

    QAction *editWeightAction = new QAction("Изменить вес");
    connect(editWeightAction, &QAction::triggered, [=](){
        bool ok;
        double d = QInputDialog::getDouble(this, tr("Изменение веса ребра"),
                                           tr("Вес : "), edge->weight, -10000, 10000, 2, &ok);
        if (ok)
        {
            edge->weight = d;
        }

        this->update();
    });

    actions.append(deleteAction);
    actions.append(isDirAction);
    actions.append(dirToAction);
    actions.append(editWeightAction);

    menu.exec(actions, this->mapToGlobal(QPoint(edge->mousePosition.rx(), edge->mousePosition.ry())));
}

void DrawGraphWidget::showWidgetMenu(QPoint pos)
{
    QMenu menu;
    QList<QAction*> actions;

    QAction *edgeAction = new QAction("Действия с ребрами");
    connect(edgeAction, &QAction::triggered, [=](){
        needEdge = true;
        this->update();
    });
    QAction *vertexAction = new QAction("Действия с вершинами");
    connect(vertexAction, &QAction::triggered, [=](){
        needEdge = false;
        this->update();
    });

    actions.append(edgeAction);
    actions.append(vertexAction);

    menu.exec(actions, this->mapToGlobal(pos));
}

void DrawGraphWidget::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    painter->fillRect(this->rect(), Qt::gray);

    if (graphData)
    {
        QRect textRect = QRect(0,0,100,35);
        if(needEdge)
        {
            painter->drawText(textRect, "Работа с ребрами");
        }
        else
        {
            painter->drawText(textRect, "Работа с вершинами");
        }
        const std::vector<std::vector<std::vector<std::shared_ptr<Edge>>>> &matrix = graphData->getEdges();
        const double EPS = 1E-10;

        const int N = graphData->getVertexNum();
        const std::vector<std::shared_ptr<Vertex>> &vertexes = graphData->getVertexes();

        for (int i = 0; i < N; i++)
        {
            for (int j = i; j < N ; j++)
            {
                std::vector<std::shared_ptr<Edge>> edgesCell =  matrix.at(i).at(j);
                size_t numEdge = edgesCell.size();
                if (!numEdge)
                {
                    continue;
                }

                std::shared_ptr<Vertex> v1 = vertexes.at(i);
                std::shared_ptr<Vertex> v2 = vertexes.at(j);

                QPoint v1Coords = QPoint(v1->coordX, v1->coordY);
                QPoint v2Coords = QPoint(v2->coordX, v2->coordY);

                painter->setPen(Qt::black);

                for(int k = 0; k < numEdge; k++)
                {
                    painter->setPen(QPen(Qt::black, 1));
                    std::shared_ptr<Edge> closesEdge = edgesCell.at(k);
                    if(closesEdge->isNormal)
                    {
                        painter->drawLine(v1Coords, v2Coords);
                        double delX = v2Coords.rx() - v1Coords.rx();
                        double delY = v2Coords.ry() - v1Coords.ry();
                        QRect rect = QRect(v1Coords.rx() + delX/2 - 15, v1Coords.ry() + delY/2 - 10, 30, 20);
                        painter->drawText(rect, QString::number(closesEdge->weight));
                    }
                    else if(closesEdge->isLoop)
                    {
                        painter->setBrush(Qt::NoBrush);
                        painter->translate(v1->coordX, v1->coordY);
                        painter->rotate(closesEdge->angle);
                        painter->drawEllipse(- closesEdge->heightWidth1.ry()/2, - closesEdge->heightWidth1.rx(),
                                             closesEdge->heightWidth1.ry(), closesEdge->heightWidth1.rx());
                        painter->resetTransform();
                        QRect rect = QRect(closesEdge->mousePosition.rx() - 15, closesEdge->mousePosition.ry() - 10, 30, 20);
                        painter->drawText(rect, QString::number(closesEdge->weight));
                    }
                    else
                    {

                        std::shared_ptr<Vertex> v1 = graphData->getVertex(closesEdge->v1Index);
                        std::shared_ptr<Vertex> v2 = graphData->getVertex(closesEdge->v2Index);

                        double x1 = v1->coordX;
                        double x2 = v2->coordX;
                        double y1 = v1->coordY;
                        double y2 = v2->coordY;

                        double k = (x2-x1)/(y2-y1);

                        double coordX = closesEdge->coords1.rx();
                        double coordY = closesEdge->coords1.ry();
                        double height = closesEdge->heightWidth1.rx();
                        double width = closesEdge->heightWidth1.ry();

                        painter->translate(coordX + width / 2, coordY + height / 2);

                        if(!closesEdge->dirTo && closesEdge->isDir)
                        {
                            if(v1->coordX < v2->coordX)
                            {
                                painter->setPen(QPen(Qt::green, 2));
                            }
                            else
                            {
                                painter->setPen(QPen(Qt::black, 1));
                            }
                        }
                        else
                        {
                            if(v1->coordX < v2->coordX)
                            {
                                painter->setPen(QPen(Qt::black, 1));
                            }
                            else
                            {
                                painter->setPen(QPen(Qt::green, 2));
                            }
                        }

                        if(k < 0)
                        {
                            painter->rotate(180-closesEdge->angle);
                        }
                        else
                        {
                            painter->rotate(closesEdge->angle);
                        }


                        if(closesEdge->nad)
                        {
                            painter->drawArc(-width/2, -height/2, width, height, 180*16, 16 * 90);
                        }
                        else
                        {
                            painter->drawArc(-width/2, -height/2, width, height, 270*16, 16 * 90);
                        }

                        painter->resetTransform();


                        coordX = closesEdge->coords2.rx();
                        coordY = closesEdge->coords2.ry();
                        height = closesEdge->heightWidth2.rx();
                        width = closesEdge->heightWidth2.ry();

                        painter->translate(coordX + width / 2, coordY + height / 2);

                        if(closesEdge->dirTo && closesEdge->isDir)
                        {
                            if(v1->coordX < v2->coordX)
                            {
                                painter->setPen(QPen(Qt::green, 2));
                            }
                            else
                            {
                                painter->setPen(QPen(Qt::black, 1));
                            }
                        }
                        else
                        {
                            if(v1->coordX < v2->coordX)
                            {
                                painter->setPen(QPen(Qt::black, 1));
                            }
                            else
                            {
                                painter->setPen(QPen(Qt::green, 2));
                            }
                        }


                        if(k < 0)
                        {
                            painter->rotate(180-closesEdge->angle);
                        }
                        else
                        {
                            painter->rotate(closesEdge->angle);
                        }

                        if(closesEdge->nad)
                        {
                            painter->drawArc(-width/2, -height/2, width, height, 90*16, 90*16);
                        }
                        else
                        {
                            painter->drawArc(-width/2, -height/2, width, height, 0, 90*16);
                        }
                        painter->resetTransform();
                        QRect rect = QRect(closesEdge->mousePosition.rx() - 15, closesEdge->mousePosition.ry() - 10, 30, 20);
                        painter->setPen(QPen(Qt::black, 1));
                        painter->drawText(rect, QString::number(closesEdge->weight));
                    }


                }
            }
        }

        for (int i = 0; i < N; i++)
        {
            std::shared_ptr<Vertex> vert = vertexes.at(i);
            if (!vert)
            {
                continue;
            }

            const double X = vert->coordX;
            const double Y = vert->coordY;

            const QColor &color = vert->color;
            const double diam = vert->drawDiam;

            painter->setBrush(QBrush(color));

            painter->drawEllipse(QPoint(X, Y), diam / 2.0, diam / 2.0);
        }

        if(needEdge)
        {
            if(selectedVertex)
            {
                painter->drawLine(QPoint(selectedVertex->coordX, selectedVertex->coordY), mouseCoord);
            }
        }
    }
    painter->end();
    delete painter;
}

void DrawGraphWidget::mousePressEvent(QMouseEvent *event)
{
    bool needRepaint = false;

    const double X = event->pos().x();
    const double Y = event->pos().y();

    const int dist = 15;

    std::shared_ptr<Edge> closestEdge = graphData->getEdge(QPoint(X, Y));
    std::shared_ptr<Vertex> closestVert = graphData->getVertex(X, Y, dist);

    bool needShowVertexMenu = false;
    bool needShowEdgeMenu = false;
    bool needShowWidgetMenu = false;

    if (event->button() == Qt::RightButton && closestVert)
    {
        needShowVertexMenu = true;
    }

    if (event->button() == Qt::RightButton && closestEdge)
    {
        needShowEdgeMenu = true;
    }

    if(event->button() == Qt::RightButton && !closestEdge && !closestVert)
    {
        needShowWidgetMenu = true;
    }

    if(event->button() == Qt::LeftButton && closestEdge && !needShowEdgeMenu && !needShowVertexMenu)
    {
        selectedEdge = closestEdge;
    }
    if (event->button() == Qt::LeftButton && closestVert && !needShowVertexMenu && !needShowEdgeMenu)
    {
        selectedVertex = closestVert;
        selectedVertex->color = Qt::red;

        if(needEdge)
        {
            mouseCoord = event->pos();
        }
        needRepaint = true;
    }
    else if(event->button() == Qt::LeftButton && !closestVert && !needShowVertexMenu && !needShowEdgeMenu && !selectedEdge && !needEdge)
    {
        std::shared_ptr<Vertex> v = std::make_shared<Vertex>(X, Y, QString::number(rand() % 356).toStdString());
        graphData->addVertex(v);

        needRepaint = true;
    }

    if (needShowVertexMenu)
    {
        showVertexMenu(closestVert/*, event->pos()*/);
    }

    if(needShowEdgeMenu)
    {
        showEdgeMenu(closestEdge);
    }

    if(needShowWidgetMenu)
    {
        showWidgetMenu(event->pos());
    }

    if (needRepaint)
    {
        this->update();
    }
    emit setMatrix();
}

void DrawGraphWidget::mouseMoveEvent(QMouseEvent *event)
{
    bool needRepaint = false;

    const double X = event->pos().x();
    const double Y = event->pos().y();
    int dist = 15;

    if (event->buttons() & Qt::LeftButton)
    {
        if (selectedVertex)
        {
            if(needEdge)
            {
                std::shared_ptr<Vertex> closestVert = graphData->getVertex(X, Y, dist);

                if(closestVert)
                {
                    newSelectedVertex = closestVert;
                    closestVert->color = Qt::red;

                }
                else
                {
                    if(newSelectedVertex)
                    {
                        newSelectedVertex->color = Qt::green;
                    }
                }
                mouseCoord = QPoint(X, Y);
            }
            else
            {
                selectedVertex->coordX = X;
                selectedVertex->coordY = Y;
                for(int i = 0; i < getGraphData()->getEdges().at(selectedVertex->index).size(); i++)
                {
                    for(int j = 0; j < getGraphData()->getEdges().at(selectedVertex->index).at(i).size(); j++)
                    {
                        std::shared_ptr<Edge> edge = getGraphData()->getEdges().at(selectedVertex->index).at(i).at(j);
                        if(!edge->isNormal)
                        {
                            calculateEdgeMoveVertex(edge);
                        }
                    }
                }
                for(int i = 0; i < getGraphData()->getEdges().at(selectedVertex->index).size(); i++)
                {
                    for(int j = 0; j < getGraphData()->getEdges().at(i).at(selectedVertex->index).size(); j++)
                    {
                        std::shared_ptr<Edge> edge = getGraphData()->getEdges().at(i).at(selectedVertex->index).at(j);
                        if(!edge->isNormal)
                        {
                            calculateEdgeMoveVertex(edge);
                        }
                    }
                }
            }

            needRepaint = true;
        }
        else if(selectedEdge)
        {
            selectedEdge->mousePosition = QPoint(X, Y);
            graphData->calculateEdge(selectedEdge, QPoint(X, Y), needEdge);
            needRepaint = true;
        }
    }

    if (needRepaint)
    {
        this->update();
    }
}

void DrawGraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
    bool needRepaint = false;

    if (selectedVertex)
    {
        if(needEdge)
        {
            const double X = event->pos().rx();
            const double Y = event->pos().ry();

            const int dist = 15;

            std::shared_ptr<Vertex> closestVert = graphData->getVertex(X, Y, dist);

            if(closestVert && closestVert != selectedVertex)
            {
                graphData->setEdge(std::make_shared<Edge>(selectedVertex->index, closestVert->index, 1));
                needRepaint = true;
            }

            mouseCoord = QPoint(X, Y);
        }
        if(newSelectedVertex)
        {
            newSelectedVertex->color = Qt::green;
        }
        selectedVertex->color = Qt::green;

        needRepaint = true;
    }

    if (needRepaint)
    {
        this->update();
    }
    selectedVertex = std::shared_ptr<Vertex>();
    newSelectedVertex = std::shared_ptr<Vertex>();
    selectedEdge = std::shared_ptr<Edge>();

    emit setMatrix();
}


void DrawGraphWidget::calculateEdgeMoveVertex(std::shared_ptr<Edge> edge)
{

    std::shared_ptr<Vertex> v1 = graphData->getVertex(edge->v1Index);
    std::shared_ptr<Vertex> v2 = graphData->getVertex(edge->v2Index);

    double x1 = v1->coordX;
    double x2 = v2->coordX;
    double y1 = v1->coordY;
    double y2 = v2->coordY;


    double X, Y;

    if(edge->isLoop)
    {
        double angle = edge->angle * M_PI / 180;
        X = x1 + edge->heightWidth1.rx() * std::sin(angle);
        Y = y1 - edge->heightWidth1.rx() * std::cos(angle);
        edge->mousePosition = QPoint(X, Y);
    }
    else
    {
        double d = edge->heightWidth1.ry()/2;

        double k =(x2-x1)/(y2-y1);

        if((!(y1 < y2) && edge->upNorm) || (!(y1 > y2) && !edge->upNorm))
        {
            d = -d;
        }

        double coeff = d / std::sqrt(k*k + 1);

        X = x1 + (x2-x1) * edge->a1_a + coeff;
        Y = y1 + (y2-y1) * edge->a1_a - k*(coeff);
        qDebug() << "Зашло в движение вершины изменение ребра в нужное условие";

        graphData->calculateEdge(edge, QPoint(X, Y), needEdge);
    }
}
std::shared_ptr<GraphData> DrawGraphWidget::getGraphData()
{
    return graphData;
}

