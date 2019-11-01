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

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model = new QStandardItemModel();
    std::shared_ptr<GraphData> graph;
private slots:
    void on_spin_valueChanged(int arg1);
    //void on_pushButton_clicked();
    void on_openFileBut_clicked();
};

#endif // MAINWINDOW_H
