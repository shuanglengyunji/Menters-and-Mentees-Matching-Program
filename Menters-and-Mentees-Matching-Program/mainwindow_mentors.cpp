#include "mainwindow.h"
#include "ui_mainwindow.h"

// mentors

void MainWindow::on_lineEdit_mentors_search_editingFinished()
{
    QString str = ui->lineEdit_mentors_search->text();
    if(str.isEmpty())
    {
        model_mentors->setFilter("");
    }
    else
    {
        QString argument = "uid LIKE '%" + str + "%'"   \
                + " OR " + "first_name LIKE '%" + str + "%'"    \
                + " OR " + "last_name LIKE '%" + str + "%'";
        qDebug() << argument;
        model_mentors->setFilter(argument);
    }
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
