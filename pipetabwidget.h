#ifndef PIPETABWIDGET_H
#define PIPETABWIDGET_H

#include <QWidget>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTimer>

namespace Ui {
class PipeTabWidget;
}

class PipeTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PipeTabWidget(int index, QWidget *parent = nullptr);
    ~PipeTabWidget();

signals:
    void setName(int index, QString name);

private slots:
    void on_PB_CONNECT_clicked();
    void on_PB_SEND_clicked();
    void readPipe();

private:
    void display(QByteArray datas);
    QString formatInHexa(QByteArray datas);
    bool isConnectionValid();
    bool isServerValid();
    Ui::PipeTabWidget *ui;
    QLocalServer *server;
    QLocalSocket *connection;
    QTimer *updater;
    bool isListening;
    QString pipeName;
    int pipeIndex;
};

#endif // PIPETABWIDGET_H
