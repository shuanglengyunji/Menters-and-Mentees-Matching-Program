#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::refresh()
{
    // Mentors
    QString str = "SELECT * FROM mentor WHERE (is_confirm = 'y')";
    model_match_mentors->setQuery(str,db);
    ui->tableView_match_mentors->reset();
    ui->tableView_match_mentors->resizeColumnsToContents();

    // Mentees to be matched
    str = "SELECT * FROM mentee WHERE (SELECT COUNT(*) as num from match WHERE(match.mentee_id = mentee.uid)) = 0";
    model_match_mentees_to_be_match->setQuery(str,db);
    ui->tableView_match_mentees_to_be_match->reset();
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();

    // Mentees Matched
    str = "SELECT * FROM mentee LEFT JOIN match ON match.mentee_id = mentee.uid WHERE match.mentor_id = ''";
    model_match_mentees_matched->setQuery(str,db);
    ui->tableView_match_mentees_matched->reset();
    ui->tableView_match_mentees_matched->resizeColumnsToContents();
}

// ----------------------------------------------------------

void MainWindow::init_match_page()
{
    // init QSqlQueryModel model

    // Mentors
    model_match_mentors = new QSqlQueryModel(this);
    QString str = "SELECT * FROM mentor WHERE (is_confirm = 'y')";
    model_match_mentors->setQuery(str,db);

    model_match_mentors->setHeaderData(0, Qt::Horizontal, "Uni ID");
    model_match_mentors->setHeaderData(1, Qt::Horizontal, "First Name");
    model_match_mentors->setHeaderData(2, Qt::Horizontal, "Last Name");
    model_match_mentors->setHeaderData(3, Qt::Horizontal, "Gender");
    model_match_mentors->setHeaderData(4, Qt::Horizontal, "Academic Level");
    model_match_mentors->setHeaderData(5, Qt::Horizontal, "College");
    model_match_mentors->setHeaderData(6, Qt::Horizontal, "Language");
    model_match_mentors->setHeaderData(7, Qt::Horizontal, "Training 1");
    model_match_mentors->setHeaderData(8, Qt::Horizontal, "Training 2");
    model_match_mentors->setHeaderData(9, Qt::Horizontal, "WWVP Card Num");
    model_match_mentors->setHeaderData(10, Qt::Horizontal, "Confirm");
    model_match_mentors->setHeaderData(11, Qt::Horizontal, "Role");

    // proxy model
    model_proxy_match_mentors = new QSortFilterProxyModel(this);
    model_proxy_match_mentors->setSourceModel(model_match_mentors);

    // set table view
    ui->tableView_match_mentors->setModel(model_proxy_match_mentors);
    ui->tableView_match_mentors->hideColumn(7);
    ui->tableView_match_mentors->hideColumn(8);
    ui->tableView_match_mentors->hideColumn(9);
    ui->tableView_match_mentors->hideColumn(10);
    ui->tableView_match_mentors->hideColumn(11);
    ui->tableView_match_mentors->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_match_mentors->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentors->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_match_mentors->resizeColumnsToContents();

    // Mentees to be matched
    model_match_mentees_to_be_match = new QSqlQueryModel(this);
    str = "SELECT * FROM mentee WHERE (SELECT COUNT(*) as num from match WHERE(match.mentee_id = mentee.uid)) = 0";
    model_match_mentees_to_be_match->setQuery(str,db);

    model_match_mentees_to_be_match->setHeaderData(0, Qt::Horizontal, "Uni ID");
    model_match_mentees_to_be_match->setHeaderData(1, Qt::Horizontal, "First Name");
    model_match_mentees_to_be_match->setHeaderData(2, Qt::Horizontal, "Last Name");
    model_match_mentees_to_be_match->setHeaderData(3, Qt::Horizontal, "Gender");
    model_match_mentees_to_be_match->setHeaderData(4, Qt::Horizontal, "Academic Level");
    model_match_mentees_to_be_match->setHeaderData(5, Qt::Horizontal, "College");
    model_match_mentees_to_be_match->setHeaderData(6, Qt::Horizontal, "Language");
    model_match_mentees_to_be_match->setHeaderData(7, Qt::Horizontal, "Consideration");
    model_match_mentees_to_be_match->setHeaderData(8, Qt::Horizontal, "Role");

    // proxy model
    model_proxy_match_mentees_to_be_match = new QSortFilterProxyModel(this);
    model_proxy_match_mentees_to_be_match->setSourceModel(model_match_mentees_to_be_match);

    // set table view
    ui->tableView_match_mentees_to_be_match->setModel(model_proxy_match_mentees_to_be_match);
    ui->tableView_match_mentees_to_be_match->hideColumn(8);
    ui->tableView_match_mentees_to_be_match->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_match_mentees_to_be_match->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentees_to_be_match->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();

    // Mentees matched
    model_match_mentees_matched = new QSqlQueryModel(this);
    str = "SELECT * FROM mentee LEFT JOIN match ON match.mentee_id = mentee.uid WHERE match.mentor_id = ''";
    model_match_mentees_matched->setQuery(str,db);

    model_match_mentees_matched->setHeaderData(0, Qt::Horizontal, "Uni ID");
    model_match_mentees_matched->setHeaderData(1, Qt::Horizontal, "First Name");
    model_match_mentees_matched->setHeaderData(2, Qt::Horizontal, "Last Name");
    model_match_mentees_matched->setHeaderData(3, Qt::Horizontal, "Gender");
    model_match_mentees_matched->setHeaderData(4, Qt::Horizontal, "Academic Level");
    model_match_mentees_matched->setHeaderData(5, Qt::Horizontal, "College");
    model_match_mentees_matched->setHeaderData(6, Qt::Horizontal, "Language");
    model_match_mentees_matched->setHeaderData(7, Qt::Horizontal, "Consideration");
    model_match_mentees_matched->setHeaderData(8, Qt::Horizontal, "Role");

    // set table view
    ui->tableView_match_mentees_matched->setModel(model_match_mentees_matched);
    ui->tableView_match_mentees_matched->hideColumn(8);
    ui->tableView_match_mentees_matched->hideColumn(9);
    ui->tableView_match_mentees_matched->hideColumn(10);
    ui->tableView_match_mentees_matched->hideColumn(11);
    ui->tableView_match_mentees_matched->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_match_mentees_matched->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentees_matched->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_match_mentees_matched->resizeColumnsToContents();
}

void MainWindow::on_pushButton_Auto_clicked()
{
    bool college = ui->checkBox_College->isChecked();
    bool language = ui->checkBox_Language->isChecked();
    bool gender = ui->checkBox_Gender->isChecked();
    bool academiclevel = ui->checkBox_AcademicLevel->isChecked();
    bool consideration = ui->checkBox_Consideration->isChecked();

    QSqlQueryModel * model_for_mentors = new QSqlQueryModel(this);
    QString str = "SELECT * FROM mentor WHERE (is_confirm = 'y')";
    model_for_mentors->setQuery(str,db);
    while(model_for_mentors->canFetchMore())
    {
        model_for_mentors->fetchMore();
    }

    QSqlQueryModel * model_for_mentees = new QSqlQueryModel(this);
    str = "SELECT * FROM mentee WHERE (SELECT COUNT(*) as num from match WHERE(match.mentee_id = mentee.uid)) = 0";
    model_for_mentees->setQuery(str,db);
    while(model_for_mentees->canFetchMore())
    {
        model_for_mentees->fetchMore();
    }

    match(model_for_mentors,model_for_mentees,college,language,gender,academiclevel,consideration);

    delete model_for_mentors;
    delete model_for_mentees;

    refresh();
}

void MainWindow::on_pushButton_Refresh_clicked()
{
    refresh();
}

void MainWindow::on_pushButton_Clear_clicked()
{
    QSqlQuery query(db);
    QString str = "DELETE FROM match";
    query.exec(str);
    refresh();
}

void MainWindow::on_pushButton_Up_clicked()
{
    // get u_num
    int row_mentor = ui->tableView_match_mentors->selectionModel()->currentIndex().row();
    QString Mentor_U_Num = model_match_mentors->record(row_mentor).value(0).toString();

    int row_mentee = ui->tableView_match_mentees_to_be_match->selectionModel()->currentIndex().row();
    QString Mentee_U_Num = model_match_mentees_to_be_match->record(row_mentee).value(0).toString();
    qDebug() << Mentor_U_Num << Mentee_U_Num;

    if(Mentor_U_Num.isEmpty() || Mentee_U_Num.isEmpty())
    {
        return;
    }

    //INSERT INTO match(mentor_id, mentee_id) VALUES('u1234567', 'u7777777')
    QString str = "INSERT INTO match(mentor_id, mentee_id) VALUES('" + Mentor_U_Num + "', '" + Mentee_U_Num + "')";
    QSqlQuery query(db);
    query.exec(str);
    qDebug() << str;

    // show

    // Mentees to be matched
    str = "SELECT * FROM mentee WHERE (SELECT COUNT(*) as num from match WHERE(match.mentee_id = mentee.uid)) = 0";
    model_match_mentees_to_be_match->setQuery(str,db);
    ui->tableView_match_mentees_to_be_match->reset();
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();

    // Mentees Matched
    str = "SELECT * FROM mentee LEFT JOIN match ON match.mentee_id = mentee.uid WHERE match.mentor_id = '" + Mentor_U_Num + "'";
    model_match_mentees_matched->setQuery(str,db);
    ui->tableView_match_mentees_matched->reset();
    ui->tableView_match_mentees_matched->resizeColumnsToContents();

}

void MainWindow::on_pushButton_Down_clicked()
{
    // get u_num
    int row_mentor = ui->tableView_match_mentors->selectionModel()->currentIndex().row();
    QString Mentor_U_Num = model_match_mentors->record(row_mentor).value(0).toString();

    int row_mentee = ui->tableView_match_mentees_matched->selectionModel()->currentIndex().row();
    QString Mentee_U_Num = model_match_mentees_matched->record(row_mentee).value(0).toString();
    qDebug() << Mentor_U_Num << Mentee_U_Num;

    if(Mentor_U_Num.isEmpty() || Mentee_U_Num.isEmpty())
    {
        return;
    }

    //INSERT INTO match(mentor_id, mentee_id) VALUES('u1234567', 'u7777777')
    QString str = "DELETE FROM match WHERE mentee_id = '" + Mentee_U_Num + "'";
    QSqlQuery query(db);
    query.exec(str);
    qDebug() << str;

    // show

    // Mentees to be matched
    str = "SELECT * FROM mentee WHERE (SELECT COUNT(*) as num from match WHERE(match.mentee_id = mentee.uid)) = 0";
    model_match_mentees_to_be_match->setQuery(str,db);
    ui->tableView_match_mentees_to_be_match->reset();
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();

    // Mentees Matched
    str = "SELECT * FROM mentee LEFT JOIN match ON match.mentee_id = mentee.uid WHERE match.mentor_id = '" + Mentor_U_Num + "'";
    model_match_mentees_matched->setQuery(str,db);
    ui->tableView_match_mentees_matched->reset();
    ui->tableView_match_mentees_matched->resizeColumnsToContents();
}

void MainWindow::on_tableView_match_mentors_clicked(const QModelIndex &index)
{
    // Selected Row
    int row = index.row();
    QString Mentor_U_Num = model_match_mentors->record(row).value(0).toString();

    // Mentees Matched
    QString str = "SELECT * FROM mentee LEFT JOIN match ON match.mentee_id = mentee.uid WHERE match.mentor_id = '" + Mentor_U_Num + "'";
    model_match_mentees_matched->setQuery(str,db);
    ui->tableView_match_mentees_matched->reset();
    ui->tableView_match_mentees_matched->resizeColumnsToContents();
}

void MainWindow::on_lineEdit_match_search_mentors_editingFinished()
{
    QString str = ui->lineEdit_match_search_mentors->text().trimmed();    // Returns a string that has whitespace removed from the start and the end
    if(str.isEmpty()) {
        model_proxy_match_mentors->setFilterRegExp("");
        return;
    }

    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list

//    int num = list.size();
//    for (int i=0; i < num ; i++)
//    {
//        QString tmp = list.at(i);
//    }

    QString tmp = list.at(0);
    model_proxy_match_mentors->setFilterRegExp(tmp);
}

void MainWindow::on_lineEdit_match_search_mentees_editingFinished()
{

}

