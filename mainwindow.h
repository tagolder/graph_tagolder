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
protected:
    void keyReleaseEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;
    std::vector<std::vector<double>> matrixRes;
    QStandardItemModel *model = new QStandardItemModel();
    std::shared_ptr<GraphData> current_graph = std::make_shared<GraphData>();
    std::shared_ptr<GraphData> graph = std::make_shared<GraphData>();
    //std::vector<std::shared_ptr<GraphData> > graphs;
    unsigned int task = 0;
private slots:
    void on_spin_valueChanged(int arg1);
    //void on_pushButton_clicked();
    void on_openFileBut_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_toolButton_clicked();
    void on_edgeButtonTask_clicked();
    void on_vertexButtonTask_clicked();
    void setMatixOnTable();
};

#endif // MAINWINDOW_H
