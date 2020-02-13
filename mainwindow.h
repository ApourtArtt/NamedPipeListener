#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabBar>
#include "pipetabwidget.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void newPipe();

private slots:
    void on_TABW_PIPES_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
    std::vector<PipeTabWidget *> pipeTabs;
};
#endif // MAINWINDOW_H
