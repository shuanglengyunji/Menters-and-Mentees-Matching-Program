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


void MainWindow::MainWindow::match(QSqlQueryModel *mentor, QSqlQueryModel *mentee, int college, int language, int gender, int academiclevel, int consideration, int max_mentees_num)
{
    QSqlQuery q("", db);                                                            //link to database
    QList <int> menteenumber;                                                       //number of mentees for each mentor
    QString searchs("SELECT COUNT(mentor_id) FROM match WHERE (mentor_id = \'");    //initial search command
    QString searche("\')");
    int cscore=0,hscore=0,hid=0,lscore=0,ccnt=0;
    bool collegecheck=false,languagecheck=false,gendercheck=false,academiclevelcheck=false,enable=false,matched=false;

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

        for (int k=0;k<mentee->rowCount();k++) {
        QSqlRecord tempmentee=mentee->record(k);
        QString tempconsideration=tempmentee.value(9).toString();
        if(!tempconsideration.isEmpty() && consideration==1){
            ccnt++;
        }
    }

    int maxassign=(mentee->rowCount()-ccnt)/mentor->rowCount();                   //calculate max menteenumbers

    qDebug()<<maxassign<<endl;

    for (int k=0;k<mentee->rowCount();k++) {                                        //start matching
        QSqlRecord tempmentee=mentee->record(k);                                    //get mentee data
        QString menteeid=tempmentee.value(0).toString();
        QString mentorid;
        QString tempmenteecollege=tempmentee.value(7).toString();
        tempmenteecollege.remove(" ");
        QStringList collegementee=tempmenteecollege.split(',');
        QString tempmenteelanguage=tempmentee.value(8).toString();
        tempmenteelanguage.remove(" ");
        QStringList languagementee=tempmenteelanguage.split(',');
        QString academiclevelmentee=tempmentee.value(6).toString();
        academiclevelmentee.remove(" ");
        QString gendementee=tempmentee.value(3).toString();
        QString tempconsideration=tempmentee.value(9).toString();
        bool isfull=false;
        if(!tempconsideration.isEmpty() && consideration==1)                        //if there is some consideration and it need to be considered, this mentee will not be auto-assign
            continue;

        hscore=0;                                                                   //initial assign variable
        lscore=1000;
        hid=0;
        matched=false;



        for (int i=0;i<mentor->rowCount();i++) {                                    //start scoring

            if(menteenumber[i]>=maxassign){                                         //if mentor have enough mentees. skip
                if(hid==i) hid++;
                if(hid>=menteenumber.count()) {isfull=true;hid--;}
                continue;
            }

            QSqlRecord tmpmentor=mentor->record(i);                                 //get mentor data
            QString mentorid=tmpmentor.value(0).toString();
            QString tempmentorcollege=tmpmentor.value(7).toString();
            tempmentorcollege.remove(" ");
            QStringList collegementor=tempmentorcollege.split(',');
            QString tempmentorlanguage=tmpmentor.value(8).toString();
            tempmentorlanguage.remove(" ");
            QStringList languagementor=tempmentorlanguage.split(',');
            QString academiclevelmentor=tmpmentor.value(6).toString();
            academiclevelmentor.remove(" ");
            QString gendementor=tmpmentor.value(3).toString();

            collegecheck=false;
            languagecheck=false;
            gendercheck=true;
            academiclevelcheck=true;
            enable=true;
            cscore=0;
            for(int h=0;h<collegementee.count();h++){                               //scoring
                if(collegementor.contains(collegementee.value(h))){
                    if(college!=100){
                        cscore+=1*college;
                    }
                    else {
                        collegecheck=true;
                    }
                }
            }

            qDebug()<<"Mentee: "<<menteeid<<"to mentor: "<<mentorid<<", Score now considered college:"<<cscore<<endl;

            for(int h=0;h<languagementee.count();h++){
                if(collegementor.contains(languagementee.value(h))){
                    if(language!=100){
                        cscore+=1*language;
                    }
                    else {
                        languagecheck=true;
                    }
                }
            }

            qDebug()<<"Mentee: "<<menteeid<<"to mentor: "<<mentorid<<", Score now considered language:"<<cscore<<endl;

            if(academiclevelmentee == academiclevelmentor){
                if(academiclevel!=100){
                    cscore+=1*academiclevel;
                }
            }
            else {
                academiclevelcheck=false;
            }

            qDebug()<<"Mentee: "<<menteeid<<"to mentor: "<<mentorid<<", Score now considered level of study:"<<cscore<<endl;

            if(gendementee==gendementor){
                if(gender!=100){
                    cscore+=1*gender;
                }
            }
            else {
                gendercheck=false;
            }

            qDebug()<<"Mentee: "<<menteeid<<"to mentor: "<<mentorid<<", Score now considered gender:"<<cscore<<endl;

            //judge if this mentor absolute same
            if(college==100 && collegecheck==false){
                enable=false;
            }
            if(language==100 && languagecheck==false){
                enable=false;
            }
            if(gender==100 && gendercheck==false){
                enable=false;
            }
            if(academiclevel==100 && academiclevelcheck==false){
                enable=false;
            }

            if(cscore>hscore && enable){                                                      //final mentor
                hscore=cscore;
                hid=i;
                matched=true;
            }
            if(cscore<lscore && enable){
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

        if(matched){
            menteenumber[hid]+=1;                                                       //process matching
            QString inser="insert into match (mentor_id,mentee_id) values (";
            QSqlRecord tmpmentor=mentor->record(hid);
            mentorid=tmpmentor.value(0).toString();
            inser.append("\'"+mentorid+"\'"+",\'"+menteeid+"\')");

            qDebug()<<"Mentee: "<<menteeid<<", Highest score: "<<hscore<<" at mentor: "<<mentorid<<", Lowest score: "<<lscore<<" , "<<inser<<endl;

            q.exec(inser);
        }
        else {
            continue;
        }

    }
}

