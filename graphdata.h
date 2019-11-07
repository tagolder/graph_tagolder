#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <vector>
#include <memory>
#include <cmath>

#include <QColor>
#include <QPoint>
#include <QRect>
#include <QDebug>
#include <QPoint>

struct Vertex
{
    Vertex(const double X, const double Y, const std::string &_name)
        : coordX(X), coordY(Y), name(_name)
    {

    }
    double coordX = 0+.0, coordY = 0.0;
    std::string name = "";
    uint index = 0;
    QColor color = Qt::green;
    double drawDiam = 30.0;
};

struct Edge
{
    Edge(const int _v1Index, const int _v2Index, const double _weight, bool _isDir = false, bool _dirTo = true):
       v1Index(_v1Index), v2Index(_v2Index), weight(_weight), isDir(_isDir), dirTo(_dirTo)
    {

    }
    std::string index;
    int v1Index, v2Index;
    double weight = 1;
    bool isDir = false;
    bool dirTo = true;
    bool isNormal = true;
    bool nad = true;
    bool isLoop = false;
    QColor color = Qt::black;
    QPoint mousePosition = QPoint(0,0);
    QPoint coords1 = QPoint(0,0);
    QPoint coords2 = QPoint(0,0);
    QPoint heightWidth1 = QPoint(0,0);
    QPoint heightWidth2 = QPoint(0,0);
    double a1_a = 0;
    double angle;
    bool upNorm = true;

};


class GraphData
{
public:
    GraphData();

    inline void addVertex(std::shared_ptr<Vertex> vert)
    {
        vertexes.push_back(vert);
        const int N = vertexes.size();
        vert->index = N - 1;

        // Изменение матрицы рёбер
        edges.resize(N);
        for (auto &vec: edges)
        {
            vec.resize(N);
        }
    }

    inline void setVertex(const size_t index, std::shared_ptr<Vertex> vert)
    {
        if (vertexes.size() <= index)
        {
            return;
        }
        vertexes[index] = vert;
    }


    // isDir = false - Ненаправленное ребро, true - направленное
    // dirTo = false - От второго узла к первому, true - от первого ко второму
    inline void setEdge(std::shared_ptr<Edge> edge, bool isDir = false, bool dirTo = true)
    {
        const int i = edge->v1Index;
        const int j = edge->v2Index;

       int n = edges[i][j].size();

        if(!edge->isLoop && edge->isNormal)
        {


           double x1 = getVertex(i)->coordX;
           double y1 = getVertex(i)->coordY;
           double x2 = getVertex(j)->coordX;
           double y2 = getVertex(j)->coordY;
           double d = std::sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

           double coordX1 = x1 + (x2-x1)/2 + std::pow(-1, n)*n*d*std::sin(std::atan((x2-x1)/(y2-y1)) + M_PI/2)/15;
           double coordY1 = y1 + (y2-y1)/2 - (x2-x1)/(y2-y1) * (coordX1 - x1 - (x2-x1)/2);
           qDebug() << QPoint(coordX1, coordY1) << d;

           if(n)
           {
               calculateEdge(edge, QPoint(coordX1, coordY1));
           }

//           if(x2==x1)
//           {
//               coordX1 += 20;
//           }

        }


        if (!isDir && !edge->isLoop)
        {            
            edges.at(i).at(j).push_back(edge);
            edges.at(j).at(i).push_back(edge);
        }
        else if(!isDir && edge->isLoop)
        {
            edges.at(i).at(j).push_back(edge);
        }
        else
        {
           // От первого узла ко второму
            if (dirTo)
            {
                edges[i][j].push_back(edge);
            }
            else
            {
                edges[j][i].push_back(edge);
            }
        }
    }

    inline std::shared_ptr<Vertex> getVertex(const size_t index)
    {
        if (vertexes.size() <= index)
        {
            return std::shared_ptr<Vertex>();
        }
        return vertexes[index];
    }

    inline void setLoopM(std::shared_ptr<Vertex> vert)
    {
        size_t index = indexOfVertex(vert);

        this->setEdge(calculateLoop(index, 1));
    }

    inline void setLoop(std::shared_ptr<Vertex> vert, double weight)
    {
        size_t index = indexOfVertex(vert);

        this->setEdge(calculateLoop(index, weight));
    }

    std::shared_ptr<Edge> calculateLoop(int index, double weight)
    {
        std::shared_ptr<Edge> edge = std::make_shared<Edge>(index, index, weight);
        edge->isNormal = false;
        edge->isLoop = true;
        edge->heightWidth1 = QPoint(60, 30);
        edge->angle = 20*edges[index][index].size();
        double coordX = getVertex(index)->coordX + 60 * std::sin(M_PI * edge->angle/180);
        double coordY = getVertex(index)->coordY - 60 * std::cos(M_PI * edge->angle/180);
        edge->mousePosition = QPoint(coordX, coordY);

        qDebug() << edge->mousePosition << edge->heightWidth1 << edge->angle;

        return edge;
    }


    inline std::shared_ptr<Vertex> getVertex(const std::string s)
    {
        std::shared_ptr<Vertex> resVert;
        for (std::shared_ptr<Vertex> vert : vertexes)
        {
            if (vert->name == s)
            {
                resVert = vert;
            }
        }
        return resVert;
    }

    inline std::shared_ptr<Vertex> getVertex(const double X, const double Y, const double eps)
    {
        std::shared_ptr<Vertex> resVert;
        for (std::shared_ptr<Vertex> vert : vertexes)
        {
            if (std::fabs(vert->coordX - X) < eps && std::fabs(vert->coordY - Y) < eps)
            {
                resVert = vert;
            }
        }

        return resVert;
    }

    inline std::shared_ptr<Edge> getEdge(QPoint mouseCoords)
    {
        std::shared_ptr<Edge> resEdge;
        for (unsigned int i = 0; i < edges.size(); i++)
        {
            for(int j = 0; j < edges.at(i).size(); j++)
            {
                for(std::shared_ptr<Edge> edge : edges.at(i).at(j))
                {
                    if(isDotOnEdge(edge, mouseCoords))
                    {
                        resEdge = edge;
                    }
                }
            }
        }
        return resEdge;
    }

    bool isDotOnEdge(std::shared_ptr<Edge> edge, QPoint mouseCoords)
    {
        double x = mouseCoords.rx();
        double y = mouseCoords.ry();

        if(edge->isNormal)
        {
            std::shared_ptr<Vertex> v1 = getVertex(edge->v1Index);
            std::shared_ptr<Vertex> v2 = getVertex(edge->v2Index);

            if(v1 && v2)
            {
                double x1 = v1->coordX;
                double x2 = v2->coordX;
                double y1 = v1->coordY;
                double y2 = v2->coordY;

                double yRes = ((x2 * y1 - x1 * y2) - (y1 - y2) * x) / (x2 - x1);
                qDebug() << yRes << y;
                if(std::abs(yRes - y) < 8)
                {
                    return true;
                }
            }

        }
        else
        {
            double edgecoordX = edge->mousePosition.rx();
            double edgecoordY = edge->mousePosition.ry();

            if(std::abs(edgecoordX - x) < 5 && std::abs(edgecoordY - y) < 5)
            {
                return true;
            }
        }

        return false;
    }

    inline size_t getVertexNum()
    {
        return vertexes.size();
    }

    inline const std::vector<std::shared_ptr<Vertex>> &getVertexes()
    {
        return vertexes;
    }

    inline const std::vector<std::vector<std::vector<std::shared_ptr<Edge>>>> &getEdges()
    {
        return edges;
    }

    inline int indexOfVertex(std::shared_ptr<Vertex> vert)
    {
        const size_t N = this->getVertexNum();
        for (size_t i = 0; i < N; i++)
        {
            std::shared_ptr<Vertex> v = this->getVertex(i);
            if (v == vert)
            {
                return i;
            }
        }
        return -1;
    }

    inline void removeVertex(std::shared_ptr<Vertex> vert)
    {
        const int index = this->indexOfVertex(vert);
        if (index == -1)
        {
            return;
        }

        vertexes.erase(vertexes.begin() + index);

        for (auto it_col= edges.begin(); it_col != edges.end(); it_col++)
        {
            (*it_col).erase((*it_col).begin() +index);
         }

        edges.erase(edges.begin() + index);

        for(int i = index; i < vertexes.size(); i++)
        {
            vertexes[i]->index = i;
        }

        for(int i = 0; i < edges.size(); i++)
        {
            for(int j = 0; j < edges[i].size(); j++)
            {
                for(auto edge : edges[i][j])
                {
                    edge->v1Index = i;
                    edge->v2Index = j;
                }
            }
        }
    }

    void setMatrixEdges(std::vector<std::vector<double>> &matrix)
    {
        for(int i = 0; i < matrix.size(); i++)
        {
            for(int j = i; j < matrix.size(); j++)
            {
                if(matrix[i][j] != 0 || matrix[j][i] != 0)
                {
                    if(i == j)
                    {
                        setLoop(getVertex(i), matrix[i][j]);
                    }
                    else if(matrix[i][j] == matrix[j][i])
                    {
                        setEdge(std::make_shared<Edge>(i, j, matrix[i][j]));
                    }
                    else
                    {
                        double x1 = getVertex(i)->coordX;
                        double y1 = getVertex(i)->coordY;
                        double x2 = getVertex(j)->coordX;
                        double y2 = getVertex(j)->coordY;
                        double coordX1 = x1 + (x2-x1)/2 + 30 * std::cos(std::atan((x2-x1)/(y2-y1)) + M_PI/2);
                        double coordY1 = y1 + (y2-y1)/2 - (x2-x1)/(y2-y1) * (coordX1 - x1 - (x2-x1)/2);

                        if(matrix[i][j] != 0)
                        {
                            std::shared_ptr<Edge> edge = std::make_shared<Edge>(i, j, matrix[i][j], true, true);

                            if(x2==x1)
                            {
                                coordX1 += 20;
                            }

                            qDebug() << "1: " << QPoint(x1, y1) << " XY: " << QPoint(coordX1, coordY1) << " 2: " << QPoint(x2, y2);

                            edge->isDir = true;
                            edge->dirTo = true;

                            calculateEdge(edge, QPoint(coordX1, coordY1));
                            setEdge(edge);
                        }
                        if(matrix[j][i] != 0)
                        {
                            std::shared_ptr<Edge> edge = std::make_shared<Edge>(j, i, matrix[j][i], true, true);
                            double coordX1 = x1 + (x2-x1)/2 - 30 * std::cos(std::atan((x2-x1)/(y2-y1)) + M_PI/2);
                            double coordY1 = y1 + (y2-y1)/2 - (x2-x1)/(y2-y1) * (coordX1 - x1 - (x2-x1)/2);
                            if(x2==x1)
                            {
                                coordX1 -= 20;
                            }
                            edge->isDir = true;
                            edge->dirTo = false;

                            calculateEdge(edge, QPoint(coordX1, coordY1));
                            setEdge(edge);
                        }
                    }
                }
            }
        }
    }

    void calculateEdge(std::shared_ptr<Edge> edge, QPoint mousePosition, bool needEdge = true)
    {
        std::shared_ptr<Vertex> v1 = getVertex(edge->v1Index);
        std::shared_ptr<Vertex> v2 = getVertex(edge->v2Index);

        if(v1->coordX > v2->coordX)
        {
            std::swap(v1, v2);
        }

        double k = (v2->coordX-v1->coordX)/(v2->coordY-v1->coordY);

        double X = mousePosition.rx();
        double Y = mousePosition.ry();

        double x1 = v1->coordX;
        double x2 = v2->coordX;
        double y1 = v1->coordY;
        double y2 = v2->coordY;

        double yRes ;
//        if(std::abs(x1 - x2) < 10)
//        {
//            yRes = y1 + edge->a1_a * std::abs(y1 - y2);
//        }
//        else
//        {
            yRes = ((x2 * y1 - x1 * y2) - (y1 - y2) * X) / (x2 - x1);
//        }

        double yResLower = -k*(X-x1) + y1;
        double yResUpper = -k*(X-x2) + y2;

        if(edge->isLoop)
        {
            edge->mousePosition = mousePosition;
//            qDebug() << edge->mousePosition << mousePosition;
            double d = std::sqrt((X-x1)*(X-x1) + (Y-y1)*(Y-y1));
            edge->heightWidth1 = QPoint(d, d/2);
            edge->angle = edge->angle = 180 * std::acos(std::abs(Y-v1->coordY) / d)/M_PI;

            if(X > x1 && Y > y1)edge->angle = 180 - edge->angle ;
            if(X < x1 && Y > y1)edge->angle += 180 ;
            if(X < x1 && Y < y1)edge->angle = -edge->angle;

//            qDebug() << edge->heightWidth1 << edge->angle;
        }
        else if((Y >= yResLower && Y <= yResUpper) || (Y <= yResLower && Y >= yResUpper))
        {
            if(Y >= yRes)
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
                if((k > 0 && edge->nad) || (k < 0 && !edge->nad))
                {
                    edge->upNorm = true;
                }
                else
                {
                    edge->upNorm = false;
                }
                qDebug() << edge->upNorm;
            }

            edge->isNormal = false;

            double coordX = x1 + delX * (a1 / a);
            double coordY = y1 + delY * (a1 / a);

            edge->coords1 = QPoint(coordX - d, coordY - a1);
            edge->coords2 = QPoint(coordX - d, coordY - a2);

            edge->heightWidth1 = QPoint(2 * a1, 2 * d);
            edge->heightWidth2 = QPoint(2 * a2, 2 * d);

            double cos_angle = std::abs(y1-y2) / std::sqrt(delX*delX + delY*delY);
            if(cos_angle > 1)
            {
                cos_angle = 1;
            }

            edge->angle = 180 - 180 * std::acos(cos_angle)/M_PI;

        }
    }



private:
    // Вектор узлов графа
    std::vector<std::shared_ptr<Vertex>> vertexes;

    // Матрица рёбер
    std::vector<std::vector<std::vector<std::shared_ptr<Edge>>>> edges;
};

#endif // GRAPHDATA_H
