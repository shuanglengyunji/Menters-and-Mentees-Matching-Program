#include "mainwindow.h"
#include "ui_mainwindow.h"

// mentees

void MainWindow::on_lineEdit_mentees_search_editingFinished()
{
    QString str = ui->lineEdit_mentees_search->text();
    if(str.isEmpty())
    {
        model_mentees->setFilter("");
    }
    else
    {
        QString argument = "uid LIKE '%" + str + "%'"   \
                + " OR " + "first_name LIKE '%" + str + "%'"    \
                + " OR " + "last_name LIKE '%" + str + "%'";
        qDebug() << argument;
        model_mentees->setFilter(argument);
    }
}

void MainWindow::on_pushButton_mentees_add_clicked()
{

}

void MainWindow::on_pushButton_mentees_delete_clicked()
{

}

void MainWindow::on_pushButton_mentees_revert_clicked()
{

}

void MainWindow::on_pushButton_mentees_submit_clicked()
{

}
