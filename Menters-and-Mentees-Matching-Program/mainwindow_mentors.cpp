#include "mainwindow.h"
#include "ui_mainwindow.h"
// mentors

// load
void MainWindow::load_mentors()
{
    // clear exist data
    if ( model_mentors != nullptr )
    {
        delete model_mentors;
        model_mentors = nullptr;
    }

    // link db to mentors QSqlTableModel
    model_mentors = new myMentorsTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_mentors->setTable("mentor");
    model_mentors->setEditStrategy(myMentorsTableModel::OnFieldChange);
    model_mentors->select();
    while(model_mentors->canFetchMore()){
        model_mentors->fetchMore();
    }

    // link mentors QSqlTableModel to QTableView
    ui->tableView_mentors->setModel(model_mentors);
    ui->tableView_mentors->reset();
    ui->tableView_mentors->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_mentors->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView_mentors->resizeColumnsToContents();
    ui->tableView_mentors->resizeRowsToContents();

    connect(model_mentors,&QAbstractItemModel::dataChanged,this,&MainWindow::edit_finished);
}

// search
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
    model_mentors->setFilter(argument);
}

void MainWindow::edit_finished()
{
    /*
    while(model_mentors->canFetchMore())
    {
        model_mentors->fetchMore();
    }

    for(int i=0; i<model_mentors->rowCount(); i++)
    {
        QString train_1 = model_mentors->index(i,18).data().toString();
        QString train_2 = model_mentors->index(i,19).data().toString();
        QString train_3 = model_mentors->index(i,20).data().toString();

        if(train_1 != "y")
        {
            model_mentors->setData(model_mentors->index(i,21),"n");
            model_mentors->setData(model_mentors->index(i,18),"n");
        }

        if(train_2 != "y")
        {
            model_mentors->setData(model_mentors->index(i,21),"n");
            model_mentors->setData(model_mentors->index(i,19),"n");
        }

        if(train_3 != "y")
        {
            model_mentors->setData(model_mentors->index(i,21),"n");
            model_mentors->setData(model_mentors->index(i,20),"n");
        }

        if(train_1 == "y" && train_2 == "y" && train_3 == "y")
        {
            model_mentors->setData(model_mentors->index(i,21),"y");
        }
        else
        {
            model_mentors->setData(model_mentors->index(i,21),"n");
        }

    }
    */

}

/*
// delete
void MainWindow::on_pushButton_mentors_delete_clicked()
{
    while (model_mentors->canFetchMore())
    {
        model_mentors->fetchMore();
    }

    QSqlTableModel *tm = model_mentors;
    if (!tm)
        return;

    QModelIndexList currentSelection = ui->tableView_mentors->selectionModel()->selectedIndexes();
    for (int i = 0; i < currentSelection.count(); ++i) {
        tm->removeRow(currentSelection.at(i).row());
    }

    if (tm){
        if(!tm->submitAll()){
            QSqlError err = tm->lastError();
            QMessageBox::warning(this, tr("Unable to delete"), tr("An error occurred while "
                                       "deleting the connection: ") + err.text());
        }
    }

    load_mentors();
}
*/



