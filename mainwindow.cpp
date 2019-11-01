#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->table->setModel(model);

    initGraph();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initGraph()
{
    graph = std::make_shared<GraphData>();

    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>(100.0, 100.0, "name1");
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>(100.0, 200.0, "name2");
    std::shared_ptr<Vertex> v3 = std::make_shared<Vertex>(200.0, 150.0, "name3");
    std::shared_ptr<Vertex> v4 = std::make_shared<Vertex>(300.0, 150.0, "name4");
    std::shared_ptr<Vertex> v5 = std::make_shared<Vertex>(350.0, 300.0, "name5");
    graph->addVertex(v1);
    graph->addVertex(v2);
    graph->addVertex(v3);
    graph->addVertex(v4);
    graph->addVertex(v5);

    graph->setEdge(std::make_shared<Edge>(0, 2, 3));
    graph->setEdge(std::make_shared<Edge>(1, 2, 3));
    graph->setEdge(std::make_shared<Edge>(2, 3, 3));
    graph->setEdge(std::make_shared<Edge>(3, 4, 3));

    ui->widget->setGraphData(graph);
    ui->widget->update();
}


void MainWindow::on_spin_valueChanged(int arg1)
{
    model->setColumnCount(arg1);
    model->setRowCount(arg1);
}

void MainWindow::on_openFileBut_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл...", "/home/ya/Документы/graph", "*.tacmatr, *.taclist");
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        if(fileName.indexOf(".taclist") != -1)
        {
            QList<QString> list = QString(file.readAll()).split(" ");
            for(int i = 0; i < list.size(); i++)
            {
                if(!list[i].indexOf("Vertex"))
                {
                    parseVertexFile(list[i]);
                }
                else if(!list[i].indexOf("Edge"))
                {
                    parseEdgeFile(list[i]);
                }
            }
        }
        else
        {
            QList<QString> list;
//          QDataStream in(&file);
            while(!file.atEnd())
            {
                list.append(file.readLine());
            }
            parseMatrix(list);
        }
        ui->widget->update();
        ui->widget->setFocus();
        file.close();
    }
}

void MainWindow::parseVertexFile(QString vertexS)
{
    int i1 = vertexS.indexOf("{");
    int i2 = vertexS.indexOf("(");
    int i3 = vertexS.indexOf(",");
    int i4 = vertexS.indexOf(")");

    float x = vertexS.mid(i2+1, i3-i2-1).toFloat();
    float y = vertexS.mid(i3+1, i4-i3-1).toFloat();
    std::string name = vertexS.mid(i1+1, i2-i1-1).toStdString();

    std::shared_ptr<Vertex> v = std::make_shared<Vertex>(x, y, name);
    graph->addVertex(v);
}

void MainWindow::parseEdgeFile(QString edgeS)
{
    int i = edgeS.indexOf("(") + 1;
    QString s = edgeS.mid(i, edgeS.size() - i - 2);
    QList<QString> list = s.split(",");

    std::shared_ptr<Vertex> vertex1 = graph->getVertex(list[1].toStdString());
    std::shared_ptr<Vertex> vertex2 = graph->getVertex(list[2].toStdString());
    if(vertex1 && vertex2)
    {
        graph->setEdge(std::make_shared<Edge>(vertex1->index, vertex2->index, list[0].toInt()));
    }
}

void MainWindow::parseMatrix(QList<QString> list)
{

}
