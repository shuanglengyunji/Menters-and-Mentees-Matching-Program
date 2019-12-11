#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::algorithm_mentors_group()
{
//    int max_num = ui->spinBox_mentors_num->value();                 // 1 - 5
    int round = ui->comboBox_mentor_round->currentIndex();          // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int level = ui->comboBox_mentor_academic_level->currentIndex(); // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int college = ui->comboBox_mentor_college->currentIndex();      // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int type = ui->comboBox_mentor_type->currentIndex();            // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int gender = ui->comboBox_mentor_gender->currentIndex();        // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int language = ui->comboBox_mentor_language->currentIndex();    // 0 - reave out; 1 - high priority; 2 - medium priority; 3 - low priority
    int special = ui->comboBox_mentor_special->currentIndex();      // 0 - reave out; 1 - high priority; 2 - medium priority; 3 - low priority

    switch (round) {
    case 0: round = 0; break;
    case 1: round = 1; break;
    case 2: round = 4; break;
    case 3: round = 3; break;
    case 4: round = 2; break;
    default: round = 0; break;
    }

    switch (level) {
    case 0: level = 0; break;
    case 1: level = 1; break;
    case 2: level = 4; break;
    case 3: level = 3; break;
    case 4: level = 2; break;
    default: level = 0; break;
    }

    switch (college) {
    case 0: college = 0; break;
    case 1: college = 1; break;
    case 2: college = 4; break;
    case 3: college = 3; break;
    case 4: college = 2; break;
    default: college = 0; break;
    }

    switch (type) {
    case 0: type = 0; break;
    case 1: type = 1; break;
    case 2: type = 4; break;
    case 3: type = 3; break;
    case 4: type = 2; break;
    default: type = 0; break;
    }

    switch (gender) {
    case 0: gender = 0; break;
    case 1: gender = 1; break;
    case 2: gender = 4; break;
    case 3: gender = 3; break;
    case 4: gender = 2; break;
    default: gender = 0; break;
    }

    switch (language) {
    case 0: language = 0; break;
    case 1: language = 3; break;
    case 2: language = 2; break;
    case 3: language = 1; break;
    default: language = 0; break;
    }

    switch (special) {
    case 0: special = 0; break;
    case 1: special = 3; break;
    case 2: special = 2; break;
    case 3: special = 1; break;
    default: special = 0; break;
    }



    QSqlTableModel * model_mentors = new QSqlTableModel(this,db);
    model_mentors->setTable("mentor");
    model_mentors->select();
    while(model_mentors->canFetchMore()){
        model_mentors->fetchMore();
    }

    QSqlTableModel * model_mentees = new QSqlTableModel(this,db);
    model_mentees->setTable("mentee");
    model_mentees->select();
    while(model_mentees->canFetchMore()){
        model_mentees->fetchMore();
    }

    // ------------------------------------------------------

    // get mentors
    QSqlQueryModel mentor;
    QSqlQuery query(db);

    while(1)
    {
        mentor.setQuery("SELECT * FROM mentor WHERE group_id = 0",db);
        if (mentor.rowCount() == 0)
            break;

        if (mentor.rowCount() == 1){
            QString uid=mentor.record(0).value(4).toString();
            query.exec(QString("UPDATE mentor WHERE uid='%1' SET group_id=(SELECT MAX(group_id)+1 FROM mentor)").arg(uid));
        }

        // start group
        QSqlRecord mentor1=mentor.record(0);

        QString mentor1id;
        QString group_mentor_id;
        int maxscore=0;
        bool enable=true;

        for (int k=1;k<mentor.rowCount();k++) {
            // mentor2: the k mentor(score for each mentors except the current one
            QSqlRecord mentor2=mentor.record(k);

            // get mentors' id
            mentor1id=mentor1.value(4).toString();
            QString mentor2id=mentor2.value(4).toString();

            // get mentors' round
            QString mentor1round=mentor1.value(6).toString();
            QString mentor2round=mentor2.value(6).toString();

            // get mentors' level
            QString mentor1level=mentor1.value(7).toString();
            QString mentor2level=mentor2.value(7).toString();

            // get mentors' college
            QString mentor1college=mentor1.value(8).toString();

            QString collegementor2=mentor1.value(8).toString();
            collegementor2.remove(" ");
            QStringList mentor2college=collegementor2.split(',');

            // get mentors' type
            QString mentor1type=mentor1.value(10).toString();
            QString mentor2type=mentor2.value(10).toString();

            // get mentors' gender
            QString mentor1gender=mentor1.value(11).toString();
            QString mentor2gender=mentor2.value(11).toString();

            // get mentors' language
            QString mentor1languages=mentor1.value(12).toString();

            QString languagesmentor2=mentor2.value(12).toString();
            languagesmentor2.remove(" ");
            QStringList mentor2languages=languagesmentor2.split(',');

            // get mentors' special categories
            QString mentor1special=mentor1.value(15).toString();

            QString specialmentor2=mentor2.value(15).toString();
            specialmentor2.remove(" ");
            QStringList mentor2special=specialmentor2.split(',');


            bool roundCheck = false;
            bool levelCheck = false;
            bool collegeCheck = false;
            bool typeCheck = false;
            bool genderCheck = false;
            bool languageCheck = false;
            bool specialCheck = false;

            // if reave out, status are true
            if (round == 0){roundCheck = true;}
            if (level == 0){levelCheck = true;}
            if (college == 0){collegeCheck = true;}
            if (type == 0){typeCheck = true;}
            if (gender == 0){genderCheck = true;}
            if (language == 0){languageCheck = true;}
            if (special == 0){specialCheck = true;}



            // score for each mentor2
            int cscore=0;

            // round
            if(mentor1round == mentor2round){
                if(round!=1){
                    cscore+=1*round;
                }
                else{
                    roundCheck=true;
                }
            }

            // level
            if(mentor1level == mentor2level){
                if(level!=1){
                    cscore+=1*level;
                }
                else{
                    levelCheck=true;
                }
            }

            // college
            for(int h=0;h<mentor2college.count();h++){
                if(mentor1college.contains(mentor2college.value(h))){
                    if(college!=1){
                        cscore+=1*college;
                    }
                    else {
                        collegeCheck=true;
                    }
                }
            }

            // type
            if(mentor1type == mentor2type){
                if(type!=1){
                    cscore+=1*type;
                }
                else{
                    typeCheck=true;
                }
            }

            // gender
            if(mentor1gender == mentor2gender){
                if(gender!=1){
                    cscore+=1*gender;
                }
                else{
                    genderCheck=true;
                }
            }

            // language
            for(int h=0;h<mentor2languages.count(); h++){
                if(mentor1languages.contains(mentor2languages.value(h))){
                    cscore+=1*language;
                }
            }

            // special categories
            for(int h=0;h<mentor2special.count(); h++){
                if(mentor1special.contains(mentor2special.value(h))){
                    cscore+=1*special;
                }
            }


            // final check

            if(round==1 && roundCheck==false){enable=false;}
            if(level==1 && levelCheck==false){enable=false;}
            if(college==1 && collegeCheck==false){enable=false;}
            if(type==1 && typeCheck==false){enable=false;}
            if(gender==1 && genderCheck==false){enable=false;}
            // store the maximum score
            if (enable)
            {
                if (cscore>=maxscore){
                    // update minimum score
                    maxscore=cscore;

                    //record the mentor with minimum score
                    group_mentor_id=mentor2id;

                }
            }

        }
        // insert result into database
        if (group_mentor_id != nullptr){
            query.exec(QString("UPDATE mentor SET group_id=(SELECT MAX(group_id)+1 FROM mentor) WHERE uid='%1' OR uid='%2'").arg(mentor1id).arg(group_mentor_id));
        }
        else{
            query.exec(QString("UPDATE mentor SET group_id=(SELECT MAX(group_id)+1 FROM mentor) WHERE uid='%1'").arg(mentor1id));

        }
    }






    // Example:

//    QSqlQuery query(db);
//    if ( !query.exec("your sql sentence goes here") )
//    {
//        qDebug() << query.lastQuery();
//        qDebug() << query.lastError();
//    }

//    QSqlQueryModel querymodel;
//    querymodel.setQuery("your sql sentence goes here",db);
//    querymodel.record(23333).value(6666).toString();            // how to read a data








    // ------------------------------------------------------

    delete model_mentors;
    delete model_mentees;

}
