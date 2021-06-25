#include "mainwindow.h"
#include "ui_mainwindow.h"

// mentees

// load
void MainWindow::load_mentees()
{
    // clear exist data
    if ( model_mentees != nullptr )
    {
        delete model_mentees;
        model_mentees = nullptr;
    }

    // link db to mentees myMenteesTableModel
    model_mentees = new myMenteesTableModel(this,db);    // model_mentees is a private pointer defined in header file
    model_mentees->setTable("mentee");
    model_mentees->setEditStrategy(myMenteesTableModel::OnFieldChange);
    model_mentees->select();
    while(model_mentees->canFetchMore()){
        model_mentees->fetchMore();
    }

    // link mentees myMenteesTableModel to QTableView
    ui->tableView_mentees->setModel(model_mentees);
    ui->tableView_mentees->reset();
    ui->tableView_mentees->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_mentees->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView_mentees->resizeColumnsToContents();
    ui->tableView_mentees->resizeRowsToContents();

    table_header_menu(ui->tableView_mentees);
}

// search
void MainWindow::on_lineEdit_mentees_search_editingFinished()
{
    QString str = ui->lineEdit_mentees_search->text().trimmed();    // Returns a string that has whitespace removed from the start and the end
    if(str.isEmpty()) {
        model_mentees->setFilter("");
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
    model_mentees->setFilter(argument);
}

/*
// delete
void MainWindow::on_pushButton_mentees_delete_clicked()
{
    while(model_mentees->canFetchMore())
    {
        model_mentees->fetchMore();
    }

    myMenteesTableModel *tm = model_mentees;
    if (!tm)
        return;

    QModelIndexList currentSelection = ui->tableView_mentees->selectionModel()->selectedIndexes();
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

    load_mentees();
}
*/

/*
// clear
void MainWindow::on_pushButton_mentees_clear_clicked()
{
    QSqlQuery query(db);
    query.exec("DELETE FROM mentee");

    load_mentees();
}
*/

/*
// add
void MainWindow::on_pushButton_mentees_add_clicked()
{
    QSqlTableModel *tm = qobject_cast<QSqlTableModel *>(ui->tableView_mentees->model());
    if (!tm)
        return;

    QModelIndex insertIndex = ui->tableView_mentees->currentIndex();
    int row = insertIndex.row() == -1 ? 0 : insertIndex.row();
    tm->insertRow(row);
    insertIndex = tm->index(row, 0);

    ui->tableView_mentees->setCurrentIndex(insertIndex);
    ui->tableView_mentees->edit(insertIndex);
}
*/

//    model_mentees->setHeaderData(0, Qt::Horizontal, "Uni ID");
//    model_mentees->setHeaderData(1, Qt::Horizontal, "First Name");
//    model_mentees->setHeaderData(2, Qt::Horizontal, "Last Name");
//    model_mentees->setHeaderData(3, Qt::Horizontal, "Gender");
//    model_mentees->setHeaderData(4, Qt::Horizontal, "Email");
//    model_mentees->setHeaderData(5, Qt::Horizontal, "Mobile");
//    model_mentees->setHeaderData(6, Qt::Horizontal, "Academic Level");
//    model_mentees->setHeaderData(7, Qt::Horizontal, "College");
//    model_mentees->setHeaderData(8, Qt::Horizontal, "Language");
//    model_mentees->setHeaderData(9, Qt::Horizontal, "Consideration");
//    model_mentees->setHeaderData(10, Qt::Horizontal, "Role");


