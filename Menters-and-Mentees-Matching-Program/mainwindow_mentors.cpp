#include "mainwindow.h"
#include "ui_mainwindow.h"

// mentors

void MainWindow::init_mentors()
{
    // link db to mentors QSqlTableModel
    model_mentors = new myMentorsTableModel(this, db);    // model_mentors is a private pointer defined in header file
    model_mentors->setTable("mentor");
    model_mentors->setEditStrategy(myMentorsTableModel::OnFieldChange);
    model_mentors->select();
    while(model_mentors->canFetchMore()){
        model_mentors->fetchMore();
    }

    // link mentors QSqlTableModel to QTableView
    ui->tableView_mentors->setItemDelegateForColumn(4, &delegate);
    ui->tableView_mentors->setItemDelegateForColumn(16, &delegate);
    ui->tableView_mentors->setItemDelegateForColumn(17, &delegate);
    ui->tableView_mentors->setItemDelegateForColumn(18, &delegate);
    ui->tableView_mentors->setModel(model_mentors);
    ui->tableView_mentors->horizontalHeader()->setMaximumSectionSize(400);
    ui->tableView_mentors->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(model_mentors, &QAbstractItemModel::dataChanged, this, [this](){

        while(model_mentors->canFetchMore())
        {
            model_mentors->fetchMore();
        }

        for(int i=0; i<model_mentors->rowCount(); i++)
        {
            int train_1 = model_mentors->index(i,16).data(Qt::EditRole).toInt();
            int train_2 = model_mentors->index(i,17).data(Qt::EditRole).toInt();
            int train_3 = model_mentors->index(i,18).data(Qt::EditRole).toInt();

            if(train_1 == 1 && train_2 == 1 && train_3 == 1)
            {
                model_mentors->setData(model_mentors->index(i,19), 1, Qt::EditRole);
            }
            else
            {
                model_mentors->setData(model_mentors->index(i,19), 0, Qt::EditRole);
            }
        }
    });

    table_header_menu(ui->tableView_mentors);
    table_menu(ui->tableView_mentors);
}

// load
void MainWindow::load_mentors()
{
    model_mentors->select();
    ui->tableView_mentors->reset();
    ui->tableView_mentors->resizeColumnsToContents();
    ui->tableView_mentors->resizeRowsToContents();
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


