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
    int row = ui->tableView_mentors->selectionModel()->currentIndex().row();
    model_mentors->insertRow(row);
    qDebug() << "Insert Row";
}

void MainWindow::on_pushButton_mentors_delete_clicked()
{
    int row = ui->tableView_mentors->selectionModel()->currentIndex().row();
    model_mentors->removeRow(row);
    model_mentors->submit();
    qDebug() << "Delete Row";
}

void MainWindow::on_pushButton_mentors_revert_clicked()
{

}

void MainWindow::on_pushButton_mentors_submit_clicked()
{

}
