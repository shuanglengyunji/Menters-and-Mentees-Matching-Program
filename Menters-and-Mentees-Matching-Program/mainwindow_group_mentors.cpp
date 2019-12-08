#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::load_group_mentors()
{
    // mentors to be grouped

    if (model_group_mentors_to_be_grouped != nullptr)
    {
        delete model_group_mentors_to_be_grouped;
    }

    model_group_mentors_to_be_grouped = new QSqlQueryModel(this);
    model_group_mentors_to_be_grouped->setQuery("SELECT * FROM mentor WHERE is_grouped = 0",db);

    ui->tableView_group_mentor_to_be_group->setModel(model_group_mentors_to_be_grouped);
    ui->tableView_group_mentor_to_be_group->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_group_mentor_to_be_group->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_group_mentor_to_be_group->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_group_mentor_to_be_group->resizeColumnsToContents();

    if (model_group_mentors_grouped != nullptr)
    {
        delete model_group_mentors_grouped;
    }

    // mentors grouped

    model_group_mentors_grouped = new QSqlQueryModel(this);
    model_group_mentors_grouped->setQuery("SELECT mentor_id FROM groups",db);

    //int row_num = model_group_mentors_grouped->rowCount();


    ui->tableView_group_mentor_grouped->setModel(model_group_mentors_grouped);
    ui->tableView_group_mentor_grouped->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_group_mentor_grouped->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_group_mentor_grouped->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::on_tableView_group_mentor_to_be_group_clicked(const QModelIndex &index)
{
    // qDebug() << ui->tableView_group_mentor_to_be_group->selectionModel()->selection().size();
}
