#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->TABW_PIPES->setStyleSheet("QTabBar::tab { height: 48px; }");
    QAction *newPipe = ui->menubar->addAction("New Pipe");
    connect(newPipe, &QAction::triggered, this, &MainWindow::newPipe);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newPipe()
{
    pipeTabs.push_back(new PipeTabWidget(static_cast<int>(pipeTabs.size()), this));
    ui->TABW_PIPES->addTab(pipeTabs[pipeTabs.size() - 1], "Empty pipe");
    connect(pipeTabs[pipeTabs.size() - 1], &PipeTabWidget::setName, this, [=](int index, QString name)
    {
        ui->TABW_PIPES->setTabText(index, name);
    });
}

void MainWindow::on_TABW_PIPES_tabCloseRequested(int index)
{
    ui->TABW_PIPES->removeTab(index);
    // delete ui->TABW_PIPES->widget(index); would also work, but Qt does it on his own
}
