#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::match(QSqlQueryModel *mentor, QSqlQueryModel *mentee, bool college, bool language, bool gender, bool academiclevel, bool consideration)
{
    QSqlQuery q("", db);                                                            //link to database
    QList <int> menteenumber;                                                       //number of mentees for each mentor
    QString searchs("SELECT COUNT(mentor_id) FROM match WHERE (mentor_id = \'");    //initial search command
    QString searche("\')");
    int rcollege,rlanguage,rgender,racademiclevel,rconsideration;                   //declare rule variables
    int cscore=0,hscore=0,hid=0;
    int maxassign=mentee->rowCount()/mentor->rowCount()+1;                          //calculate max menteenumbers

    qDebug()<<maxassign<<endl;

    for (int i=0;i<mentor->rowCount();i++) {                                        //get pre-assigned mentees
        QSqlRecord tmpmentor=mentor->record(i);
        QString mentorid=tmpmentor.value(0).toString();

        qDebug()<<mentorid<<endl;
        qDebug()<<searchs+mentorid+searche<<endl;

        q.exec(searchs+mentorid+searche);
        while(q.next()){
            qDebug()<<q.value(0).toInt()<<endl;
            menteenumber.append(q.value(0).toInt());
        }
    }
    if(college){                                                                    //ensure rule variable values
        rcollege=1;
    } else {
        rcollege=0;
    }
    if(language){
        rlanguage=1;
    } else {
        rlanguage=0;
    }
    if(gender){
        rgender=1;
    } else {
        rgender=0;
    }
    if(academiclevel){
        racademiclevel=1;
    } else {
        racademiclevel=0;
    }
    if(consideration){
        rconsideration=1;
    } else {
        rconsideration=0;
    }

    for (int k=0;k<mentee->rowCount();k++) {                                        //start matching
        QSqlRecord tempmentee=mentee->record(k);                                    //get mentee data
        QString menteeid=tempmentee.value(0).toString();
        QString mentorid;

        qDebug()<<menteeid<<endl;

        QString tempmenteecollege=tempmentee.value(5).toString();
        QStringList collegementee=tempmenteecollege.split('|');
        QString tempmenteelanguage=tempmentee.value(6).toString();
        QStringList languagementee=tempmenteelanguage.split('|');
        QString academiclevelmentee=tempmentee.value(4).toString();
        academiclevelmentee.remove(" ");
        QString gendementee=tempmentee.value(3).toString();
        QString consideration=tempmentee.value(7).toString();
        if(!consideration.isEmpty() && rconsideration)                              //if there is some consideration and it need to be considered, this mentee will not be auto-assign
            continue;

        cscore=0;                                                                   //initial assign variable
        hscore=0;
        hid=0;



        for (int i=0;i<mentor->rowCount();i++) {                                    //start scoring
            if(menteenumber[i]>=maxassign){                                          //if mentor have enough mentees. skip
                if(hid==i) hid++;
                continue;
            }

            QSqlRecord tmpmentor=mentor->record(i);                                 //get mentor data
            QString mentorid=tmpmentor.value(0).toString();
            QString tempmentorcollege=tmpmentor.value(5).toString();
            QStringList collegementor=tempmentorcollege.split('|');
            QString tempmentorlanguage=tmpmentor.value(6).toString();
            QStringList languagementor=tempmentorlanguage.split('|');
            QString academiclevelmentor=tmpmentor.value(4).toString();
            academiclevelmentor.remove(" ");
            QString gendementor=tmpmentor.value(3).toString();

            for(int h=0;h<collegementee.count();h++){                               //scoring
                if(collegementor.contains(collegementee.value(h))){
                    cscore+=1*rcollege;
                }
            }
            for(int h=0;h<languagementee.count();h++){
                if(collegementor.contains(languagementee.value(h))){
                    cscore+=1*rlanguage;
                }
            }
            if(academiclevelmentee == academiclevelmentor)
                cscore+=1*racademiclevel;
            if(gendementee==gendementor)
                cscore+=1*rgender;

            if(cscore>hscore){                                                      //final mentor
                hscore=cscore;
                hid=i;
            }

        }

        menteenumber[hid]+=1;                                                       //process matching
        QString inser="insert into match (mentor_id,mentee_id) values (";
        QSqlRecord tmpmentor=mentor->record(hid);
        mentorid=tmpmentor.value(0).toString();
        inser.append("\'"+mentorid+"\'"+",\'"+menteeid+"\')");

        qDebug()<<inser<<endl;

        q.exec(inser);

    }
}

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

    // set table view
    ui->tableView_match_mentors->setModel(model_match_mentors);
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

    // set table view
    ui->tableView_match_mentees_to_be_match->setModel(model_match_mentees_to_be_match);
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
    match(model_match_mentors,model_match_mentees_to_be_match,college,language,gender,academiclevel,consideration);
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

