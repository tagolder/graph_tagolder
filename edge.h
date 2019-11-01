#ifndef EDGE_H
#define EDGE_H

#include <QString>

class edge
{
private:
    QString A, B; //хранятся имена вершин

    bool direction;
    int weight;
    int num = 0;
    int allnum = 0;
public:
    edge();
    edge(QString, QString, int w = 1, bool b = true);
    QString getA();
    QString getB();
    void addAllNum();
    int getAllNum();
    void setNum(int);
    void setAllNum(int);
};

#endif // EDGE_H
