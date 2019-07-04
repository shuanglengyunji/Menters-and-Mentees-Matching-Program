#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::init_match_mentors_model()
{
    // Mentors Query Model
    model_match_mentors = new QSqlQueryModel(this);
    QString str = "SELECT * FROM mentor WHERE (is_confirm = 'y')";
    model_match_mentors->setQuery(str,db);

    model_match_mentors->setHeaderData(0, Qt::Horizontal, "Uni ID");
    model_match_mentors->setHeaderData(1, Qt::Horizontal, "First Name");
    model_match_mentors->setHeaderData(2, Qt::Horizontal, "Last Name");
    model_match_mentors->setHeaderData(3, Qt::Horizontal, "Gender");
    model_match_mentors->setHeaderData(4, Qt::Horizontal, "Email");
    model_match_mentors->setHeaderData(5, Qt::Horizontal, "Mobile");
    model_match_mentors->setHeaderData(6, Qt::Horizontal, "Addtional information");
    model_match_mentors->setHeaderData(7, Qt::Horizontal, "Academic Level");
    model_match_mentors->setHeaderData(8, Qt::Horizontal, "College");
    model_match_mentors->setHeaderData(9, Qt::Horizontal, "Language");
    model_match_mentors->setHeaderData(10, Qt::Horizontal, "Training 1");
    model_match_mentors->setHeaderData(11, Qt::Horizontal, "Training 2");
    model_match_mentors->setHeaderData(12, Qt::Horizontal, "Training 3");
    model_match_mentors->setHeaderData(13, Qt::Horizontal, "WWVP Card Num");
    model_match_mentors->setHeaderData(14, Qt::Horizontal, "Confirm");
    model_match_mentors->setHeaderData(15, Qt::Horizontal, "Role");

}

void MainWindow::init_match_mentors_proxy_model()
{
    // Mentors Proxy Model
    model_proxy_match_mentors = new QSortFilterProxyModel(this);
    model_proxy_match_mentors->setSourceModel(model_match_mentors);
}

void MainWindow::init_match_mentors_view()
{
    // set table view
    ui->tableView_match_mentors->setModel(model_proxy_match_mentors);
    ui->tableView_match_mentors->hideColumn(4);
    ui->tableView_match_mentors->hideColumn(5);
    ui->tableView_match_mentors->hideColumn(10);
    ui->tableView_match_mentors->hideColumn(11);
    ui->tableView_match_mentors->hideColumn(12);
    ui->tableView_match_mentors->hideColumn(13);
    ui->tableView_match_mentors->hideColumn(14);
    ui->tableView_match_mentors->hideColumn(15);
    ui->tableView_match_mentors->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_match_mentors->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentors->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_match_mentors->resizeColumnsToContents();
}

// ----------------------------------------------------

void MainWindow::init_match_mentees_to_be_match_model()
{
    // Mentees to be matched
    model_match_mentees_to_be_match = new QSqlQueryModel(this);
    QString str = "SELECT * FROM mentee WHERE (SELECT COUNT(*) as num from match WHERE(match.mentee_id = mentee.uid)) = 0";
    model_match_mentees_to_be_match->setQuery(str,db);

    model_match_mentees_to_be_match->setHeaderData(0, Qt::Horizontal, "Uni ID");
    model_match_mentees_to_be_match->setHeaderData(1, Qt::Horizontal, "First Name");
    model_match_mentees_to_be_match->setHeaderData(2, Qt::Horizontal, "Last Name");
    model_match_mentees_to_be_match->setHeaderData(3, Qt::Horizontal, "Gender");
    model_match_mentees_to_be_match->setHeaderData(4, Qt::Horizontal, "Email");
    model_match_mentees_to_be_match->setHeaderData(5, Qt::Horizontal, "Mobile");
    model_match_mentees_to_be_match->setHeaderData(6, Qt::Horizontal, "Academic Level");
    model_match_mentees_to_be_match->setHeaderData(7, Qt::Horizontal, "College");
    model_match_mentees_to_be_match->setHeaderData(8, Qt::Horizontal, "Language");
    model_match_mentees_to_be_match->setHeaderData(9, Qt::Horizontal, "Consideration");
    model_match_mentees_to_be_match->setHeaderData(10, Qt::Horizontal, "Role");
}

void MainWindow::init_match_mentees_to_be_match_proxy_model()
{
    // mentees proxy model
    model_proxy_match_mentees_to_be_match = new QSortFilterProxyModel(this);
    model_proxy_match_mentees_to_be_match->setSourceModel(model_match_mentees_to_be_match);
}

void MainWindow::init_match_mentees_to_be_match_view()
{
    // set table view
    ui->tableView_match_mentees_to_be_match->setModel(model_proxy_match_mentees_to_be_match);
    ui->tableView_match_mentees_to_be_match->hideColumn(10);
    ui->tableView_match_mentees_to_be_match->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_match_mentees_to_be_match->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentees_to_be_match->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
}

// ----------------------------------------------------

void MainWindow::init_match_mentees_matched_model()
{
    // Mentees matched
    model_match_mentees_matched = new QSqlQueryModel(this);
    QString str = "SELECT * FROM mentee LEFT JOIN match ON match.mentee_id = mentee.uid WHERE match.mentor_id = ''";
    model_match_mentees_matched->setQuery(str,db);

    model_match_mentees_matched->setHeaderData(0, Qt::Horizontal, "Uni ID");
    model_match_mentees_matched->setHeaderData(1, Qt::Horizontal, "First Name");
    model_match_mentees_matched->setHeaderData(2, Qt::Horizontal, "Last Name");
    model_match_mentees_matched->setHeaderData(3, Qt::Horizontal, "Gender");
    model_match_mentees_matched->setHeaderData(4, Qt::Horizontal, "Email");
    model_match_mentees_matched->setHeaderData(5, Qt::Horizontal, "Mobile");
    model_match_mentees_matched->setHeaderData(6, Qt::Horizontal, "Academic Level");
    model_match_mentees_matched->setHeaderData(7, Qt::Horizontal, "College");
    model_match_mentees_matched->setHeaderData(8, Qt::Horizontal, "Language");
    model_match_mentees_matched->setHeaderData(9, Qt::Horizontal, "Consideration");
    model_match_mentees_matched->setHeaderData(10, Qt::Horizontal, "Role");
}

void MainWindow::init_match_mentees_matched_view()
{
    // set table view
    ui->tableView_match_mentees_matched->setModel(model_match_mentees_matched);
    ui->tableView_match_mentees_matched->hideColumn(10);
    ui->tableView_match_mentees_matched->hideColumn(11);
    ui->tableView_match_mentees_matched->hideColumn(12);
    ui->tableView_match_mentees_matched->hideColumn(13);
    ui->tableView_match_mentees_matched->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_match_mentees_matched->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_match_mentees_matched->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_match_mentees_matched->resizeColumnsToContents();
}

// ----------------------------------------------------

void MainWindow::refresh_match()
{
    // Mentors
    QString str = "SELECT * FROM mentor WHERE (is_confirm = 'y')";
    model_match_mentors->setQuery(str,db);
    refresh_match_mentors_view();

    // Mentees to be matched
    str = "SELECT * FROM mentee WHERE (SELECT COUNT(*) as num from match WHERE(match.mentee_id = mentee.uid)) = 0";
    model_match_mentees_to_be_match->setQuery(str,db);
    refresh_match_mentees_to_be_match_view();

    // Mentees Matched
    str = "SELECT * FROM mentee LEFT JOIN match ON match.mentee_id = mentee.uid WHERE match.mentor_id = ''";
    model_match_mentees_matched->setQuery(str,db);
    refresh_match_mentees_matched_view();
}

void MainWindow::refresh_match_mentors_view()
{
    ui->tableView_match_mentors->reset();
    ui->tableView_match_mentors->resizeColumnsToContents();
}

void MainWindow::refresh_match_mentees_to_be_match_view()
{
    ui->tableView_match_mentees_matched->reset();
    ui->tableView_match_mentees_matched->resizeColumnsToContents();
}

void MainWindow::refresh_match_mentees_matched_view()
{
    ui->tableView_match_mentees_to_be_match->reset();
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
}

// ----------------------------------------------------------

void MainWindow::init_match_page()
{
    init_match_mentors_model();
    init_match_mentors_proxy_model();
    init_match_mentors_view();

    init_match_mentees_to_be_match_model();
    init_match_mentees_to_be_match_proxy_model();
    init_match_mentees_to_be_match_view();

    init_match_mentees_matched_model();
    init_match_mentees_matched_view();
}

void MainWindow::on_pushButton_Refresh_clicked()
{
    refresh_match();
}

void MainWindow::on_pushButton_Clear_clicked()
{
    QSqlQuery query(db);
    QString str = "DELETE FROM match";
    query.exec(str);
    refresh_match();
}

void MainWindow::on_pushButton_Up_clicked()
{
    // get u_num
    int row_mentor = ui->tableView_match_mentors->selectionModel()->currentIndex().row();
    QString Mentor_U_Num = model_proxy_match_mentors->index(row_mentor,0).data().toString();

    int row_mentee = ui->tableView_match_mentees_to_be_match->selectionModel()->currentIndex().row();
    QString Mentee_U_Num = model_proxy_match_mentees_to_be_match->index(row_mentee,0).data().toString();

    if(Mentor_U_Num.isEmpty() || Mentee_U_Num.isEmpty())
    {
        return;
    }

    match_manual_add(Mentor_U_Num,Mentee_U_Num);

}

void MainWindow::on_pushButton_Down_clicked()
{
    // get u_num
    int row_mentor = ui->tableView_match_mentors->selectionModel()->currentIndex().row();
    QString Mentor_U_Num = model_proxy_match_mentors->index(row_mentor,0).data().toString();

    int row_mentee = ui->tableView_match_mentees_matched->selectionModel()->currentIndex().row();
    QString Mentee_U_Num = model_match_mentees_matched->record(row_mentee).value(0).toString();

    if(Mentor_U_Num.isEmpty() || Mentee_U_Num.isEmpty())
    {
        return;
    }

    match_manual_remove(Mentor_U_Num,Mentee_U_Num);
}

void MainWindow::on_tableView_match_mentors_clicked(const QModelIndex &index)
{
    // Selected Row
    int row = index.row();
    QString Mentor_U_Num = model_proxy_match_mentors->index(row,0).data().toString();

    // Mentees Matched
    QString str = "SELECT * FROM mentee LEFT JOIN match ON match.mentee_id = mentee.uid WHERE match.mentor_id = '" + Mentor_U_Num + "'";
    model_match_mentees_matched->setQuery(str,db);
    refresh_match_mentees_matched_view();
}

void MainWindow::on_lineEdit_match_search_mentors_editingFinished()
{
    QString str = ui->lineEdit_match_search_mentors->text().trimmed();    // Returns a string that has whitespace removed from the start and the end
    QString argument = "";
    if(str.isEmpty()) {
        argument = "SELECT * FROM mentor WHERE (is_confirm = 'y')";
        model_match_mentors->setQuery(argument,db);
        return;
    }

    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list

    int num = list.size();
    QString tmp = list.at(0);
    argument = "SELECT * FROM mentor WHERE (is_confirm = 'y') AND (uid LIKE '%" + tmp + "%'";
    for (int i=0; i < num ; i++)
    {
        tmp = list.at(i);
        argument = argument
                + " OR " + "uid LIKE '%" + tmp + "%'"               // Uni ID
                + " OR " + "first_name LIKE '%" + tmp + "%'"        // First Name
                + " OR " + "last_name LIKE '%" + tmp + "%'";        // Last Name
    }
    argument = argument + ")";
    qDebug() << argument;
    model_match_mentors->setQuery(argument,db);
    ui->tableView_match_mentors->reset();
    ui->tableView_match_mentors->resizeColumnsToContents();
}

void MainWindow::on_lineEdit_match_search_mentees_editingFinished()
{
    QString str = ui->lineEdit_match_search_mentees->text().trimmed();    // Returns a string that has whitespace removed from the start and the end
    QString argument = "";
    if(str.isEmpty()) {
        argument = "SELECT * FROM mentee WHERE (SELECT COUNT(*) as num from match WHERE(match.mentee_id = mentee.uid)) = 0";
        model_match_mentees_to_be_match->setQuery(argument,db);
        return;
    }

    QStringList list = str.split(QRegExp("[\r\n\t ]+"), QString::SkipEmptyParts);   // get string list

    int num = list.size();
    QString tmp = list.at(0);
    argument = "SELECT * FROM mentee WHERE (SELECT COUNT(*) as num from match WHERE(match.mentee_id = mentee.uid)) = 0 AND (uid LIKE '%" + tmp + "%'";
    for (int i=0; i < num ; i++)
    {
        tmp = list.at(i);
        argument = argument
                + " OR " + "uid LIKE '%" + tmp + "%'"               // Uni ID
                + " OR " + "first_name LIKE '%" + tmp + "%'"        // First Name
                + " OR " + "last_name LIKE '%" + tmp + "%'";        // Last Name
    }
    argument = argument + ")";
    qDebug() << argument;
    model_match_mentees_to_be_match->setQuery(argument,db);
    ui->tableView_match_mentees_to_be_match->reset();
    ui->tableView_match_mentees_to_be_match->resizeColumnsToContents();
}

void MainWindow::on_pushButton_Auto_clicked()
{
    qDebug() << "Auto Match";

    match_in_turn();
}

void MainWindow::match_in_turn()
{
    int gender = ui->comboBox_gender->currentIndex();
    int college = ui->comboBox_college->currentIndex();
    int language = ui->comboBox_language->currentIndex();
    int academiclevel = ui->comboBox_academic_level->currentIndex();
    int consideration = ui->comboBox_consideration->currentIndex();
    int max_mentees_num = ui->spinBox_mentees_num->value();

    switch (gender) {
    case 0:
        gender = 0;
        break;
    case 1:
        gender = 100;
        break;
    case 2:
        gender = 3;
        break;
    case 3:
        gender = 2;
        break;
    case 4:
        gender = 1;
        break;
    default:
        gender = 0;
        break;
    }

    switch (college) {
    case 0:
        college = 0;
        break;
    case 1:
        college = 100;
        break;
    case 2:
        college = 3;
        break;
    case 3:
        college = 2;
        break;
    case 4:
        college = 1;
        break;
    default:
        college = 0;
        break;
    }

    switch (language) {
    case 0:
        language = 0;
        break;
    case 1:
        language = 100;
        break;
    case 2:
        language = 3;
        break;
    case 3:
        language = 2;
        break;
    case 4:
        language = 1;
        break;
    default:
        language = 0;
        break;
    }

    switch (academiclevel) {
    case 0:
        academiclevel = 0;
        break;
    case 1:
        academiclevel = 100;
        break;
    case 2:
        academiclevel = 3;
        break;
    case 3:
        academiclevel = 2;
        break;
    case 4:
        academiclevel = 1;
        break;
    default:
        academiclevel = 0;
        break;
    }

//    qDebug() << gender;
//    qDebug() << college;
//    qDebug() << language;
//    qDebug() << academiclevel;
//    qDebug() << consideration;

    for (int max_mentees_num_tmp = 1; max_mentees_num_tmp <= max_mentees_num; ++max_mentees_num_tmp)
    {
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

        if (model_for_mentors->rowCount() == 0 || model_for_mentees->rowCount() == 0)
        {
            delete model_for_mentors;
            delete model_for_mentees;
            return;
        }

        match(model_for_mentors,model_for_mentees,college,language,gender,academiclevel,consideration,max_mentees_num_tmp);

        delete model_for_mentors;
        delete model_for_mentees;
    }
    refresh_match();
}
