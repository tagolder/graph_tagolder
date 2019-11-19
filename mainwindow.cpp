#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
    ui->centralWidget->setLayout(ui->horizontalLayout_2);

    ui->tabWidget->clear();

    openReference();

    connect(ui->actionSaveAsImage, &QAction::triggered, [=](){
        saveAsImage();
    });
    connect(ui->actionOpen, &QAction::triggered, [=](){
        openGraph();
    });
    connect(ui->actionHelp, &QAction::triggered, [=](){
        openReference();
    });

    ui->actionExit->setShortcuts(QKeySequence::Quit);

    connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    QWidget *w = new QWidget;
    ui->tabWidget->addTab(w, "+");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openReference()
{
    QTextEdit * textEdit = new QTextEdit();

    textEdit->setReadOnly(true);

    QFile file("/home/ya/graph_Tac/graph/reference.txt");
    if (file.open(QIODevice::ReadOnly))
    {
       QString temp(file.readAll());
       textEdit->append(temp);
    }
    ui->tabWidget->addTab(textEdit, "Reference");

    QWidget *firstPageWidget = new QWidget;
    QWidget *secondPageWidget = new QWidget;
    QWidget *thirdPageWidget = new QWidget;

    QStackedWidget *stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(firstPageWidget);
    stackedWidget->addWidget(secondPageWidget);
    stackedWidget->addWidget(thirdPageWidget);
}

void MainWindow::openGraph()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл...", "/home/ya/Документы/graphs", "*.tacmatr, *.taclist");
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
            while(!file.atEnd())
            {
                list.append(file.readLine());
            }
            parseMatrix(list);
        }
        file.close();
    }
}

void MainWindow::parseVertexFile(QString vertexS)
{
    DrawGraphWidget *w = (DrawGraphWidget *)ui->tabWidget->currentWidget();
    std::shared_ptr<GraphData> graph = w->getGraphData();

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
    DrawGraphWidget *w = (DrawGraphWidget *)ui->tabWidget->currentWidget();
    std::shared_ptr<GraphData> graph = w->getGraphData();
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
    if(index == ui->tabWidget->count()-1 && index > 0)
    {

        bool ok;

        QString tabname = QInputDialog::getText(this, tr("Ввод имени графа"),
                                                tr("Имя : "), QLineEdit::Normal, "", &ok);

        if (ok && !tabname.isEmpty())
        {
            ui->tabWidget->setCurrentIndex(0);

            ui->tabWidget->removeTab(index);

            DrawGraphWidget *widget = new DrawGraphWidget;
            std::shared_ptr<GraphData> graph = std::make_shared<GraphData>();
            widget->setGraphData(graph);
            widget->needEdge = false;

            connect(widget, SIGNAL(setMatrix()), this, SLOT(setMatixOnTable()));

            ui->tabWidget->addTab(widget, tabname);

            QWidget *w = new QWidget;
            ui->tabWidget->addTab(w, "+");

            ui->tabWidget->setCurrentIndex(index);
        }

    }
    else if(index > 0 && index != ui->tabWidget->count()-1)
    {
        setMatixOnTable();
    }
}

void MainWindow::setMatixOnTable()
{
    needSetMatrix = false;
    DrawGraphWidget *w = (DrawGraphWidget *)ui->tabWidget->currentWidget();
    const std::vector<std::vector<std::vector<std::shared_ptr<Edge>>>> &matrix = w->getGraphData()->getEdges();
    int size = matrix.size();
    ui->table->setRowCount(size);
    ui->table->setColumnCount(size);
    matrixRes.resize(size);
    QStringList nameList;
    for(int i = 0; i < size; i++)
    {
        std::string name = w->getGraphData()->getVertex(i)->name;
        nameList << QString::fromStdString(name);
        matrixRes[i].resize(size);
    }
    ui->table->setHorizontalHeaderLabels(nameList);
    ui->table->setVerticalHeaderLabels(nameList);
    for(int i = 0; i < size; i++)
    {
        for(int j = i; j < size; j++)
        {
            double summ_ij = 0;
            double summ_ji = 0;
            for(int k = 0; k < matrix.at(i).at(j).size(); k++)
            {
                if(matrix.at(i).at(j).at(k)->isDir)
                {
                    if(matrix.at(i).at(j).at(k)->dirTo)
                    {
                        summ_ij += matrix.at(i).at(j).at(k)->weight;
                    }
                    else
                    {
                        summ_ji += matrix.at(i).at(j).at(k)->weight;
                    }
                }
                else
                {
                    summ_ij += matrix.at(i).at(j).at(k)->weight;
                    summ_ji += matrix.at(i).at(j).at(k)->weight;
                }
            }
            matrixRes[i][j] = summ_ij;
            matrixRes[j][i] = summ_ji;
            ui->table->setItem(i, j, new QTableWidgetItem(QString::number(summ_ij)));
            ui->table->setItem(j, i, new QTableWidgetItem(QString::number(summ_ji)));
        }
    }
    needSetMatrix = true;
}

void MainWindow::setTableOnMatrix()
{
    int size = matrixRes.size();
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            matrixRes[i][j] = ui->table->item(i, j)->text().toDouble();
        }
    }
}

void MainWindow::on_table_cellChanged(int row, int column)
{
    if(needSetMatrix)
    {
        matrixRes[row][column] = ui->table->item(row, column)->text().toDouble();

        DrawGraphWidget *w = (DrawGraphWidget *)ui->tabWidget->currentWidget();

        w->getGraphData()->setMatrixEdges(matrixRes);
        w->update();
    }
}

void MainWindow::saveAsImage()
{   
    DrawGraphWidget *w = (DrawGraphWidget *)ui->tabWidget->currentWidget();
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save File",
                                                    "/home/ya/Изображения/graphs",
                                                    "Images *.png()");
    w->grab().save(fileName + ".png");
}
