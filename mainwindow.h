#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "graphdata.h"
#include <QGraphicsSceneMouseEvent>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QDataStream>
#include <QDebug>
#include <QList>
#include <QLayout>
#include <QTableView>
#include <QStringList>
#include <QtGui>
#include <QTextEdit>
#include <QStackedWidget>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initGraph();
    void parseVertexFile(QString);
    void parseEdgeFile(QString);
    void parseMatrix(QList<QString>);
    void setTableOnMatrix();
    void saveAsImage();
    void saveGraph();
    void openGraph();
    void openReference();
private:
    Ui::MainWindow *ui;
    std::vector<std::vector<double>> matrixRes;
    std::shared_ptr<GraphData> currentGraph = std::make_shared<GraphData>();
    unsigned int task = 0;
    bool needSetMatrix = true;
private slots:
    void on_tabWidget_currentChanged(int index);
    void setMatixOnTable();
    void on_table_cellChanged(int row, int column);
};

#endif // MAINWINDOW_H
