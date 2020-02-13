#include "pipetabwidget.h"
#include "ui_pipetabwidget.h"

PipeTabWidget::PipeTabWidget(int index, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PipeTabWidget)
{
    ui->setupUi(this);
    pipeIndex = index;
    isListening = false;
    server = new QLocalServer(this);
    connect(server, &QLocalServer::newConnection, this, [=]
    {
        connection = server->nextPendingConnection();
        ui->L_STATUS->setText("Connected");
        ui->PB_SEND->setEnabled(true);
        connect(connection, &QLocalSocket::disconnected, [this]
        {
            ui->L_STATUS->setText("Disconnected");
            ui->PB_SEND->setEnabled(false);
        });
        /*connect(connection, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error), [=](QLocalSocket::LocalSocketError socketError)
        {
            // https://doc.qt.io/qt-5/qlocalsocket.html#LocalSocketError-enum
        });*/
        connection->waitForConnected();
        updater = new QTimer(this);
        connect(updater, &QTimer::timeout, this, [=]
        {
            readPipe();
        });
        updater->start(250);
    });
}

PipeTabWidget::~PipeTabWidget()
{
    delete ui;
}

void PipeTabWidget::readPipe()
{
    if(isConnectionValid() && isServerValid())
    {
        if(connection->bytesAvailable())
            display(connection->readAll());
    }
    else
        updater->stop();
}

void PipeTabWidget::display(QByteArray datas)
{
    ui->TB_RECIEVEDDATASHEXA->append(formatInHexa(datas));
    ui->TB_RECIEVEDDATASTEXT->append(datas + "\n\n");
}

QString PipeTabWidget::formatInHexa(QByteArray datas)
{
    QString hexa = datas.toHex().toUpper();
    if(hexa.size() > 2)
        hexa.insert(2, ' ');
    for(int i = 3 ; i < hexa.size() ; i++)
    {
        if(i % 3 == 0 && i != 0)
            hexa.insert(i, " ");
    }
    hexa.insert(hexa.size(), "\n\n");
    if(hexa.size() > 2)
        hexa.remove(2, 1);
    return hexa;
}

bool PipeTabWidget::isConnectionValid()
{
    if(connection)
    {
        if(connection->isValid()    && connection->isOpen() &&
           connection->isReadable() && connection->isWritable())
            return true;
    }
    return false;
}

bool PipeTabWidget::isServerValid()
{
    if(server)
    {
        if(server->isListening())
            return true;
    }
    return false;
}

void PipeTabWidget::on_PB_CONNECT_clicked()
{
    if(!isListening)
    {
        isListening = true;
        pipeName = ui->LE_PIPETOLISTEN->text();
        emit setName(pipeIndex, pipeName + "\nConnected");
        server->listen(pipeName);
        ui->PB_CONNECT->setText("Stop listening");
    }
    else
    {
        isListening = false;
        emit setName(pipeIndex, pipeName + "\nDisconnected");
        server->close();
        ui->PB_CONNECT->setText("Start listening");
    }
}

void PipeTabWidget::on_PB_SEND_clicked()
{
    if(isServerValid() && isConnectionValid())
    {
        QByteArray text;
        text.append(ui->LE_SENDDATA->toPlainText());
        connection->write(text);
    }
}
