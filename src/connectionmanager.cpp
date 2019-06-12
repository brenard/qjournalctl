#include "connectionmanager.h"
#include "ui_connectionmanager.h"
#include "connectiondialog.h"

ConnectionManager::ConnectionManager(QWidget *parent, SSHConnectionSerializer *sshConnectionSerializer) :
    QDialog(parent),
    ui(new Ui::ConnectionManager)
{
    ui->setupUi(this);

    this->sshConnectionSerializer = sshConnectionSerializer;

    // Show saved connections
    connectionsModel = new QStandardItemModel(0, 1);
    connectionsModel->setHorizontalHeaderItem(0, new QStandardItem("Connection"));
    ui->listView->setModel(connectionsModel);
    refreshSavedConnections();
}

ConnectionManager::~ConnectionManager()
{
    delete connectionsModel;
    delete ui;
}

void ConnectionManager::refreshSavedConnections()
{
    connectionsModel->clear();

    // Add known connections
    for(SSHConnectionSettings *settings : *sshConnectionSerializer->getConnectionsVector()){
        connectionsModel->appendRow(new QStandardItem(settings->getName()));
    }

    // Update ui
    ui->listView->update();
}

void ConnectionManager::on_newConnectionButton_clicked()
{
    // We can omit current SSHSettings since we don't want to connect to another host from there
    ConnectionDialog dialog(nullptr, nullptr, sshConnectionSerializer, true);
    dialog.exec();
    refreshSavedConnections();
}

void ConnectionManager::on_editConnectionButton_clicked()
{
    int id = ui->listView->selectionModel()->selectedIndexes().first().row();

    ConnectionDialog dialog(sshConnectionSerializer, id);
    dialog.exec();
    refreshSavedConnections();
}

void ConnectionManager::on_removeConnectionButton_clicked()
{
    int id = ui->listView->selectionModel()->selectedIndexes().first().row();

    sshConnectionSerializer->remove(id);
    refreshSavedConnections();
}

void ConnectionManager::on_listView_doubleClicked()
{
    on_editConnectionButton_clicked();
}

void ConnectionManager::on_closeButton_clicked()
{
    close();
}