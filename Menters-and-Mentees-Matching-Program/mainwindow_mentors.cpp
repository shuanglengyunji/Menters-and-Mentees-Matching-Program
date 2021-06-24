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

    connect(model_mentors, &QAbstractItemModel::dataChanged, this, [this](){
        while (this->model_mentors->canFetchMore()) {
            this->model_mentors->fetchMore();
        }
    });

    ui->tableView_mentors->horizontalHeader()->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(ui->tableView_mentors->horizontalHeader(), &QHeaderView::customContextMenuRequested, this, [this](QPoint pos) {
        int idx = ui->tableView_mentors->horizontalHeader()->logicalIndexAt(pos);
        QMenu contextMenu(this);
        QAction hide_section(QString("Hide %1").arg(ui->tableView_mentors->model()->headerData(idx, Qt::Orientation::Horizontal, Qt::DisplayRole).toString()), this);
        connect(&hide_section, &QAction::triggered, this, [&](){
            ui->tableView_mentors->horizontalHeader()->hideSection(idx);
        });
        contextMenu.addAction(&hide_section);
        QAction show_all("Show all", this);
        connect(&show_all, &QAction::triggered, this, [&](){
            for (int i=0; i<ui->tableView_mentors->horizontalHeader()->count(); i++) {
                ui->tableView_mentors->horizontalHeader()->showSection(i);
            }
        });
        if (ui->tableView_mentors->horizontalHeader()->sectionsHidden()) {
            contextMenu.addAction(&show_all);
        }
        contextMenu.exec(QCursor::pos());
    });
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


