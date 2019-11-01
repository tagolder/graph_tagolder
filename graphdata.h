#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <vector>
#include <memory>
#include <cmath>

#include <QColor>
#include <QPoint>
#include <QRect>
#include <QDebug>

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
    double drawDiam = 20.0;
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
    int angle;

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

        if (!isDir)
        {
            edges[i][j].push_back(edge);
            edges[j][i].push_back(edge);
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

    inline void setLoop(std::shared_ptr<Vertex> vert)
    {
        size_t index = indexOfVertex(vert);
        std::shared_ptr<Edge> edge = std::make_shared<Edge>(index, index, 1);
        edge->isNormal = false;
        edge->isLoop = true;
        edge->mousePosition = QPoint(vert->coordX, vert->coordY - 60);
        edge->heightWidth1 = QPoint(60, 30);
        edge->angle = 0;
        this->setEdge(edge);
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
        for (int i = 0; i < edges.size(); i++)
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
        for (int i = 0; i < edges.size(); i++)
        {
            for(int j = 0; j < edges.size(); j++)
            {
                if(i == index || j == index)
                {
                    edges[i][j].clear();
                }
            }
        }
    }

private:
    // Вектор узлов графа
    std::vector<std::shared_ptr<Vertex>> vertexes;

    // Матрица рёбер
    std::vector<std::vector<std::vector<std::shared_ptr<Edge>>>> edges;
};

#endif // GRAPHDATA_H
