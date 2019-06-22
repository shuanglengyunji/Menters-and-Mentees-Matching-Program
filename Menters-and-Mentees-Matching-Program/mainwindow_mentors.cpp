#include "mainwindow.h"
#include "ui_mainwindow.h"

// mentors

void MainWindow::init_mentors_page()
{
    // init mentors' table model
    model_mentors = new QSqlTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_mentors->setTable("mentor");
    model_mentors->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_mentors->select();

    model_mentors->setHeaderData(0, Qt::Horizontal, "Uni ID");
    model_mentors->setHeaderData(1, Qt::Horizontal, "First Name");
    model_mentors->setHeaderData(2, Qt::Horizontal, "Last Name");
    model_mentors->setHeaderData(3, Qt::Horizontal, "Gender");
    model_mentors->setHeaderData(4, Qt::Horizontal, "Academic Level");
    model_mentors->setHeaderData(5, Qt::Horizontal, "College");
    model_mentors->setHeaderData(6, Qt::Horizontal, "Language");
    model_mentors->setHeaderData(7, Qt::Horizontal, "Training 1");
    model_mentors->setHeaderData(8, Qt::Horizontal, "Training 2");
    model_mentors->setHeaderData(9, Qt::Horizontal, "WWVP Card Num");
    model_mentors->setHeaderData(10, Qt::Horizontal, "Confirm");
    model_mentors->setHeaderData(11, Qt::Horizontal, "Role");

    // set table view
    ui->tableView_mentors->setModel(model_mentors);
    ui->tableView_mentors->hideColumn(11);
    ui->tableView_mentors->resizeColumnsToContents();
}

void MainWindow::on_lineEdit_mentors_search_editingFinished()
{
    QString str = ui->lineEdit_mentors_search->text().trimmed();    // Returns a string that has whitespace removed from the start and the end
    if(str.isEmpty()) {
        model_mentors->setFilter("");
        return;
    }
    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list
    int num = list.size();
    QString tmp = list.at(0);
    QString argument = "uid LIKE '%" + tmp + "%'";
    for (int i=0; i < num ; i++)
    {
        tmp = list.at(i);
        argument = argument
                + " OR " + "uid LIKE '%" + tmp + "%'"               // Uni ID
                + " OR " + "first_name LIKE '%" + tmp + "%'"        // First Name
                + " OR " + "last_name LIKE '%" + tmp + "%'";        // Last Name
    }
    qDebug() << argument;
    model_mentors->setFilter(argument);
}

void MainWindow::on_pushButton_mentors_add_clicked()
{
    QSqlTableModel *tm = qobject_cast<QSqlTableModel *>(ui->tableView_mentors->model());
    if (!tm)
        return;

    QModelIndex insertIndex = ui->tableView_mentors->currentIndex();
    int row = insertIndex.row() == -1 ? 0 : insertIndex.row();
    tm->insertRow(row);
    insertIndex = tm->index(row, 0);
    ui->tableView_mentors->setCurrentIndex(insertIndex);
    ui->tableView_mentors->edit(insertIndex);

    qDebug() << "Insert Row";
}

void MainWindow::on_pushButton_mentors_delete_clicked()
{
    QSqlTableModel *tm = qobject_cast<QSqlTableModel *>(ui->tableView_mentors->model());
    if (!tm)
        return;

    QModelIndexList currentSelection = ui->tableView_mentors->selectionModel()->selectedIndexes();
    for (int i = 0; i < currentSelection.count(); ++i) {
        tm->removeRow(currentSelection.at(i).row());
    }

    if (tm){
        if(!tm->submitAll()){
            qDebug()<<"Unable to delete"<<endl;
            QSqlError err = tm->lastError();
            QMessageBox::warning(this, tr("Unable to delete"), tr("An error occurred while "
                                       "deleting the connection: ") + err.text());
        }
    }

    qDebug() << "Delete Row";
}

void MainWindow::on_pushButton_mentors_revert_clicked()
{
    QSqlTableModel * tm = qobject_cast<QSqlTableModel *>(ui->tableView_mentors->model());
    if (tm)
        tm->revertAll();

    qDebug() << "Revert";
}

void MainWindow::on_pushButton_mentors_submit_clicked()
{
    QSqlTableModel * tm = qobject_cast<QSqlTableModel *>(ui->tableView_mentors->model());
    if (tm){
        if(!tm->submitAll()){
            qDebug()<<"Unable to submit"<<endl;
            QSqlError err = tm->lastError();
            QMessageBox::warning(this, tr("Unable to submit"), tr("An error occurred while "
                                       "submitting the connection: ") + err.text());
        }
    }

    qDebug() << "Submit All";
}
