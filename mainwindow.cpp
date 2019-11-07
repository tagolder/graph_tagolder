#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->table->setModel(model);

    initGraph();

    ui->edgeButtonTask->setStyleSheet("background-color: white");
    ui->vertexButtonTask->setStyleSheet("background-color: grey");
    ui->toolButton->setStyleSheet("background-color: white");

    ui->tabWidget->removeTab(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initGraph()
{
    graph = std::make_shared<GraphData>();

    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>(100.0, 100.0, "name1");
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>(150.0, 200.0, "name2");
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

    std::vector<std::vector<double>> matrix{
        {0,5,0,0,0},
        {6,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
      };
    graph->setMatrixEdges(matrix);

    //ui->widget->update();
    ui->tab_1->setGraphData(graph);
    //ui->tabWidget->setUpdatesEnabled(false);
    ui->tabWidget->setTabsClosable(true);
    connect(ui->tab_1, SIGNAL(setMatrix()), this, SLOT(setMatixOnTable()));
//    ui->tab_1->setModelTable(ui->tab_1->model);
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
        //ui->tabWidget->update();
        //ui->wi->setFocus();
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

void MainWindow::on_tabWidget_currentChanged(int index)
{

}

void MainWindow::on_toolButton_clicked()
{
    DrawGraphWidget *w = (DrawGraphWidget *)ui->tabWidget->currentWidget();
    w->needEdge = false;

    std::shared_ptr<GraphData> g = std::make_shared<GraphData>();
    DrawGraphWidget *widget = new DrawGraphWidget();
    widget->setGraphData(g);
    connect(widget, SIGNAL(setMatrix()), this, SLOT(setMatixOnTable()));

    ui->tabWidget->addTab(widget, QString::number(ui->tabWidget->count()));

    ui->edgeButtonTask->setStyleSheet("background-color: white");
    ui->vertexButtonTask->setStyleSheet("background-color: white");
    ui->toolButton->setStyleSheet("background-color: grey");
}

void MainWindow::on_edgeButtonTask_clicked()
{
    DrawGraphWidget *w = (DrawGraphWidget *)ui->tabWidget->currentWidget();
    w->needEdge = true;
    ui->edgeButtonTask->setStyleSheet("background-color: grey");
    ui->vertexButtonTask->setStyleSheet("background-color: white");
    ui->toolButton->setStyleSheet("background-color: white");

}

void MainWindow::on_vertexButtonTask_clicked()
{
    DrawGraphWidget *w = (DrawGraphWidget *)ui->tabWidget->currentWidget();
    w->needEdge = false;

    ui->edgeButtonTask->setStyleSheet("background-color: white");
    ui->vertexButtonTask->setStyleSheet("background-color: grey");
    ui->toolButton->setStyleSheet("background-color: white");
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_1)
    {
        on_edgeButtonTask_clicked();
    }
    if(event->key() == Qt::Key_3)
    {
        on_vertexButtonTask_clicked();
    }
    if(event->key() == Qt::Key_2)
    {
        on_toolButton_clicked();
    }

}

void MainWindow::setMatixOnTable()
{
    DrawGraphWidget *w = (DrawGraphWidget *)ui->tabWidget->currentWidget();
    const std::vector<std::vector<std::vector<std::shared_ptr<Edge>>>> &matrix = w->getGraphData()->getEdges();
    int size = matrix.size();
    model->setRowCount(size);
    model->setColumnCount(size);
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            double summ = 0;
            for(int k = 0; k < matrix.at(i).at(j).size(); k++)
            {
                summ += matrix.at(i).at(j).at(k)->weight;
            }
            QStandardItem *item = new QStandardItem(QString::number(summ));
            model->setItem(i, j, item);
        }
    }
}
