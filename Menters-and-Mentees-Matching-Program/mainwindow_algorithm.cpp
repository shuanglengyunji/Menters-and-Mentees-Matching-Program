#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::match_manual_add(QString Mentor_U_Num, QString Mentee_U_Num)
{
    //INSERT INTO match(mentor_id, mentee_id) VALUES('u1234567', 'u7777777')
    QString str = "INSERT INTO match(mentor_id, mentee_id) VALUES('" + Mentor_U_Num + "', '" + Mentee_U_Num + "')";
    QSqlQuery query(db);
    query.exec(str);
    qDebug() << str;

    // show

    // Mentees to be matched
    str = "SELECT * FROM mentee WHERE (SELECT COUNT(*) as num from match WHERE(match.mentee_id = mentee.uid)) = 0";
    model_match_mentees_to_be_match->setQuery(str,db);
    refresh_match_mentees_to_be_match_view();

    // Mentees Matched
    str = "SELECT * FROM mentee LEFT JOIN match ON match.mentee_id = mentee.uid WHERE match.mentor_id = '" + Mentor_U_Num + "'";
    model_match_mentees_matched->setQuery(str,db);
    refresh_match_mentees_matched_view();
}

void MainWindow::match_manual_remove(QString Mentor_U_Num, QString Mentee_U_Num)
{
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

void MainWindow::match(QSqlQueryModel *mentor, QSqlQueryModel *mentee, bool college, bool language, bool gender, bool academiclevel, bool consideration)
{
    QSqlQuery q("", db);                                                            //link to database
    QList <int> menteenumber;                                                       //number of mentees for each mentor
    QString searchs("SELECT COUNT(mentor_id) FROM match WHERE (mentor_id = \'");    //initial search command
    QString searche("\')");
    int rcollege,rlanguage,rgender,racademiclevel,rconsideration;                   //declare rule variables
    int cscore=0,hscore=0,hid=0,lscore=0,ccnt=0;

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
    for (int k=0;k<mentee->rowCount();k++) {
        QSqlRecord tempmentee=mentee->record(k);
        QString consideration=tempmentee.value(7).toString();
        if(!consideration.isEmpty() && rconsideration){
            ccnt++;
        }
    }

    int maxassign=(mentee->rowCount()-ccnt)/mentor->rowCount();                   //calculate max menteenumbers
    qDebug()<<maxassign<<endl;

    for (int k=0;k<mentee->rowCount();k++) {                                        //start matching
        QSqlRecord tempmentee=mentee->record(k);                                    //get mentee data
        QString menteeid=tempmentee.value(0).toString();
        QString mentorid;
        QString tempmenteecollege=tempmentee.value(5).toString();
        tempmenteecollege.remove(" ");
        QStringList collegementee=tempmenteecollege.split(',');
        QString tempmenteelanguage=tempmentee.value(6).toString();
        tempmenteelanguage.remove(" ");
        QStringList languagementee=tempmenteelanguage.split(',');
        QString academiclevelmentee=tempmentee.value(4).toString();
        academiclevelmentee.remove(" ");
        QString gendementee=tempmentee.value(3).toString();
        QString consideration=tempmentee.value(7).toString();
        bool isfull=false;
        if(!consideration.isEmpty() && rconsideration)                              //if there is some consideration and it need to be considered, this mentee will not be auto-assign
            continue;

        hscore=0;                                                                   //initial assign variable
        lscore=1000;
        hid=0;




        for (int i=0;i<mentor->rowCount();i++) {                                    //start scoring

            if(menteenumber[i]>=maxassign){                                         //if mentor have enough mentees. skip
                if(hid==i) hid++;
                if(hid>=menteenumber.count()) {isfull=true;hid--;}
                continue;
            }

            QSqlRecord tmpmentor=mentor->record(i);                                 //get mentor data
            QString mentorid=tmpmentor.value(0).toString();
            QString tempmentorcollege=tmpmentor.value(5).toString();
            tempmentorcollege.remove(" ");
            QStringList collegementor=tempmentorcollege.split(',');
            QString tempmentorlanguage=tmpmentor.value(6).toString();
            tempmentorlanguage.remove(" ");
            QStringList languagementor=tempmentorlanguage.split(',');
            QString academiclevelmentor=tmpmentor.value(4).toString();
            academiclevelmentor.remove(" ");
            QString gendementor=tmpmentor.value(3).toString();

            cscore=0;
            for(int h=0;h<collegementee.count();h++){                               //scoring
                if(collegementor.contains(collegementee.value(h))){
                    cscore+=1*rcollege;
                }
            }

            qDebug()<<"Mentee: "<<menteeid<<"to mentor: "<<mentorid<<", Score now considered college:"<<cscore<<endl;

            for(int h=0;h<languagementee.count();h++){
                if(collegementor.contains(languagementee.value(h))){
                    cscore+=1*rlanguage;
                }
            }

            qDebug()<<"Mentee: "<<menteeid<<"to mentor: "<<mentorid<<", Score now considered language:"<<cscore<<endl;

            if(academiclevelmentee == academiclevelmentor)
                cscore+=1*racademiclevel;

            qDebug()<<"Mentee: "<<menteeid<<"to mentor: "<<mentorid<<", Score now considered level of study:"<<cscore<<endl;

            if(gendementee==gendementor)
                cscore+=1*rgender;

            qDebug()<<"Mentee: "<<menteeid<<"to mentor: "<<mentorid<<", Score now considered gender:"<<cscore<<endl;

            if(cscore>hscore){                                                      //final mentor
                hscore=cscore;
                hid=i;
            }
            if(cscore<lscore){
                lscore=cscore;
            }
        }

        if(hscore==lscore || isfull){
            for (int h=0;h<menteenumber.count();h++) {
                if(menteenumber[h]<menteenumber[hid]){
                    hid=h;
                }
            }
        }

        menteenumber[hid]+=1;                                                       //process matching
        QString inser="insert into match (mentor_id,mentee_id) values (";
        QSqlRecord tmpmentor=mentor->record(hid);
        mentorid=tmpmentor.value(0).toString();
        inser.append("\'"+mentorid+"\'"+",\'"+menteeid+"\')");

        qDebug()<<"Mentee: "<<menteeid<<", Highest score: "<<hscore<<" at mentor: "<<mentorid<<", Lowest score: "<<lscore<<" , "<<inser<<endl;

        q.exec(inser);

    }
}
