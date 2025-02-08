#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Define render time interval (ms)
#define RENDER_TIME_INTERVAL 100

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include "maze.h"
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event); // Renders the maze

private slots:
    void on_sizesetButton_clicked();

    void on_GenerateButton_clicked();

    void on_SolveButton_clicked();

    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void on_destisetButton_clicked();

    void on_depsetButton_clicked();

private:
    Ui::MainWindow *ui;

    QPainter *paint;
    QTimer *timer;

    int row=10,column=10; // Maze's size
    maze *M; // An pointer to the instance of the class maze
};
#endif // MAINWINDOW_H
