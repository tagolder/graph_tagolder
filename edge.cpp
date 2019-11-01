#include "edge.h"

edge::edge()
{

}

edge::edge(QString a, QString b, int w, bool d)
{
    weight = w;
    A = a; B = b;
    direction = d;
}

QString edge::getA()
{
    return A;
}

QString edge::getB()
{
    return B;
}

int edge::getAllNum()
{
    return allnum;
}

void edge::addAllNum()
{
    num++;
}

void edge::setNum(int n)
{
    num = n;
}

void edge::setAllNum(int n)
{
    allnum = n;
}
