#include "drawgraphwidget.h"
#include "ui_drawgraphwidget.h"

DrawGraphWidget::DrawGraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawGraphWidget)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    setFocus();
}

DrawGraphWidget::~DrawGraphWidget()
{
    delete ui;
}

void DrawGraphWidget::showVertexMenu(std::shared_ptr<Vertex> vert, QPoint pos)
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
        graphData->setLoop(vert);
        this->update();
    });
    actions.append(deleteAction);
    actions.append(loopAction);

    menu.exec(actions, this->mapToGlobal(pos));
}

void DrawGraphWidget::showEdgeMenu(std::shared_ptr<Edge> edge, QPoint pos)
{

}

void DrawGraphWidget::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    painter->fillRect(this->rect(), Qt::gray);

    if (graphData)
    {
        const std::vector<std::vector<std::vector<std::shared_ptr<Edge>>>> &matrix = graphData->getEdges();
        const double EPS = 1E-10;

        const int N = graphData->getVertexNum();
        const std::vector<std::shared_ptr<Vertex>> &vertexes = graphData->getVertexes();

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                std::vector<std::shared_ptr<Edge>> edgesCell =  matrix.at(i).at(j);
                size_t numEdge = edgesCell.size();
                if (!numEdge)
                {
                    continue;
                }

                std::shared_ptr<Vertex> v1 = vertexes.at(i);
                std::shared_ptr<Vertex> v2 = vertexes.at(j);

                QPointF v1Coords = QPointF(v1->coordX, v1->coordY);
                QPointF v2Coords = QPointF(v2->coordX, v2->coordY);

                painter->setPen(Qt::black);

                for(int k = 0; k < numEdge; k++)
                {
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
                        painter->drawText(rect, QString::number(closesEdge->weight));
                    }


                }

                // Если веса одинаковые то ребро ненаправленное
//                if (mirrorWeight > EPS && weight > EPS)
//                {

//                }
//                else if (weight > EPS)
//                {
//                    painter->drawEllipse(v2Coords, 2.0, 2.0);
//                }
//                else if (mirrorWeight > EPS)
//                {
//                    painter->drawEllipse(v1Coords, 2.0, 2.0);
//                }
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

            painter->drawEllipse(QPointF(X, Y), diam / 2.0, diam / 2.0);
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

    bool needShowMenu = false;
    if (event->buttons() & Qt::RightButton)
    {
        needShowMenu = true;
    }

    selectedEdge = graphData->getEdge(QPoint(X, Y));

    std::shared_ptr<Vertex> closestVert = graphData->getVertex(X, Y, dist);

    if (closestVert && !needShowMenu)
    {
        selectedVertex = closestVert;
        selectedVertex->color = Qt::red;

        if(needEdge)
        {
            mouseCoord = event->pos();
        }
        needRepaint = true;
    }
    else if(!closestVert && !needShowMenu && !selectedEdge)
    {
        std::shared_ptr<Vertex> v = std::make_shared<Vertex>(X, Y, QString::number(rand() % 356).toStdString());
        graphData->addVertex(v);

        needRepaint = true;
    }

    if (needShowMenu)
    {
        showVertexMenu(closestVert, event->pos());
    }

    if (needRepaint)
    {
        this->update();
    }
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
                for(std::vector<std::shared_ptr<Edge>> edgeVec : graphData->getEdges().at(selectedVertex->index))
                {
                    for(std::shared_ptr<Edge> edge : edgeVec)
                    {
                        if(!edge->isNormal && !edge->isLoop)
                        {
                            calculateEdgeMoveVertex(edge);
                        }
                        else if(!edge->isNormal && edge->isLoop)
                        {
                            coordsMouseLoop(edge);
                        }
                    }
                }
            }

            needRepaint = true;
        }
        else if(selectedEdge)
        {
            selectedEdge->mousePosition = QPoint(X, Y);
            calculateEdge(selectedEdge, QPoint(X, Y));
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

            if(closestVert)
            {
                graphData->setEdge(std::make_shared<Edge>(selectedVertex->index, closestVert->index, 1));
                //needEdge = false;
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
}

void DrawGraphWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        needEdge = true;
    }
}

void DrawGraphWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        needEdge = false;
    }

}

void DrawGraphWidget::calculateEdge(std::shared_ptr<Edge> edge, QPoint mousePosition)
{
    std::shared_ptr<Vertex> v1 = graphData->getVertex(edge->v1Index);
    std::shared_ptr<Vertex> v2 = graphData->getVertex(edge->v2Index);

    double k = (v2->coordX-v1->coordX)/(v2->coordY-v1->coordY);

    if(v1->coordX > v2->coordX)
    {
        std::swap(v1, v2);
    }

    double X = mousePosition.rx();
    double Y = mousePosition.ry();

    double x1 = v1->coordX;
    double x2 = v2->coordX;
    double y1 = v1->coordY;
    double y2 = v2->coordY;

    double yRes = ((x2 * y1 - x1 * y2) - (y1 - y2) * X) / (x2 - x1);

    double yResLower = -k*(X-x1) + y1;
    double yResUpper = -k*(X-x2) + y2;

    if(edge->isLoop)
    {
        edge->mousePosition = mousePosition;
        qDebug() << edge->mousePosition.rx() << edge->mousePosition.ry() << mousePosition.rx() << mousePosition.ry();
        double d = std::sqrt((X-x1)*(X-x1) + (Y-y1)*(Y-y1));
        edge->heightWidth1 = QPoint(d, d/2);

        edge->angle = edge->angle = 180 * std::acos(std::abs(Y-v1->coordY) / d)/M_PI;

        if(X > x1 && Y > y1)edge->angle = 180 - edge->angle ;
        if(X < x1 && Y > y1)edge->angle += 180 ;
        if(X < x1 && Y < y1)edge->angle = -edge->angle;

    }
    else if((Y > yResLower && Y < yResUpper) || (Y < yResLower && Y > yResUpper))
    {
        if(Y > yRes)
        {
            edge->nad = false;
        }
        else
        {
            edge->nad = true;
        }

        edge->mousePosition = mousePosition;


        double delX = x2 - x1;
        double delY = y2 - y1;
        double a = std::sqrt(delX * delX + delY * delY);

        double b_2 = (X - x1)*(X - x1) + (Y - y1)*(Y - y1);
        double c_2 = (X - x2)*(X - x2) + (Y - y2)*(Y - y2);
        double a1 = (a * a + b_2 - c_2) / (2 * a);
        double a2 = a - a1;

        double d = edge->heightWidth1.ry()/2;

        if(needEdge)
        {
            edge->a1_a = a1 / a;
            d = std::sqrt(std::abs(b_2 - a1 * a1));
        }


        edge->isNormal = false;

        double coordX = x1 + delX * (a1 / a);
        double coordY = y1 + delY * (a1 / a);

        edge->coords1 = QPoint(coordX - d, coordY - a1);
        edge->coords2 = QPoint(coordX - d, coordY - a2);

        edge->heightWidth1 = QPoint(2 * a1, 2 * d);
        edge->heightWidth2 = QPoint(2 * a2, 2 * d);

        edge->angle = 180 - 180 * std::acos(std::abs(X-coordX) / d)/M_PI;
    }
}

void DrawGraphWidget::calculateEdgeMoveVertex(std::shared_ptr<Edge> edge)
{

    std::shared_ptr<Vertex> v1 = graphData->getVertex(edge->v1Index);
    std::shared_ptr<Vertex> v2 = graphData->getVertex(edge->v2Index);

    double x1 = v1->coordX;
    double x2 = v2->coordX;
    double y1 = v1->coordY;
    double y2 = v2->coordY;


    double d = edge->heightWidth1.ry()/2;

    double k = (x2-x1)/(y2-y1);

    double X1 = x1 + (x2-x1)* edge->a1_a;
    double Y1 = y1 + (y2-y1) * edge->a1_a;

    double  coeff = d / std::sqrt(k*k + 1);

//    if(x1 == x2)
//    {
//        edge->nad = !edge->nad;
//    }

    if((k > 0 && !edge->nad) || (k < 0 && edge->nad))
    {
        coeff = -coeff;
    }
    double X2, Y2;

    X2 = X1 + coeff;
    Y2 = Y1 - k*(coeff);


    calculateEdge(edge, QPoint(X2, Y2));
}

void DrawGraphWidget::coordsMouseLoop(std::shared_ptr<Edge> edge)
{
    std::shared_ptr<Vertex> v = graphData->getVertex(edge->v1Index);
    double angle = edge->angle * M_PI / 180;
    double x1 = v->coordX;
    double y1 = v->coordY - edge->heightWidth1.rx();
    double X = x1 * std::cos(angle) - y1 * std::sin(angle);
    double Y = x1 * std::sin(angle) + y1 * std::cos(angle);

    calculateEdge(edge, QPoint(X, Y));
}

std::shared_ptr<GraphData> DrawGraphWidget::getGraphData()
{
    return graphData;
}



