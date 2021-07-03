#include "mainwindow.h"
#include "ui_mainwindow.h"

// mentees

void MainWindow::init_mentees()
{
    // link db to mentees myMenteesTableModel
    model_mentees = new myMenteesTableModel(this, db);    // model_mentees is a private pointer defined in header file
    model_mentees->setTable("mentee");
    model_mentees->setEditStrategy(myMenteesTableModel::OnFieldChange);
    model_mentees->select();
    while(model_mentees->canFetchMore()){
        model_mentees->fetchMore();
    }

    // link mentees myMenteesTableModel to QTableView
    ui->tableView_mentees->setModel(model_mentees);
    ui->tableView_mentees->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_mentees->setSelectionBehavior(QAbstractItemView::SelectRows);

    table_header_menu(ui->tableView_mentees);
    table_menu(ui->tableView_mentees);
}

// load
void MainWindow::load_mentees()
{
    model_mentees->select();
    ui->tableView_mentees->reset();
    ui->tableView_mentees->resizeColumnsToContents();
    ui->tableView_mentees->resizeRowsToContents();
}

// search
void MainWindow::on_lineEdit_mentees_search_editingFinished()
{
    QString str = ui->lineEdit_mentees_search->text().simplified();    // Returns a string that has whitespace removed from the start and the end
    if(str.isEmpty()) {
        model_mentees->setFilter("");
        return;
    }
    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list
    QStringList out;
    foreach(QString tmp, list)
    {
        out.append(QString("uid LIKE '\%%1\%' OR first_name LIKE '\%%2\%' OR last_name LIKE '\%%3\%'").arg(tmp, tmp, tmp));
    }
    model_mentees->setFilter(out.join(" OR "));
}
