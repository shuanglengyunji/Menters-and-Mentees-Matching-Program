#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::algorithm_mentors_group()
{
    int max_num = ui->spinBox_mentors_num->value();                 // 1 - 2
    int round = ui->comboBox_mentor_round->currentIndex();          // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int level = ui->comboBox_mentor_academic_level->currentIndex(); // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int college = ui->comboBox_mentor_college->currentIndex();      // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int type = ui->comboBox_mentor_type->currentIndex();            // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int gender = ui->comboBox_mentor_gender->currentIndex();        // 0 - reave out; 1 - must be same; 2 - high priority; 3 - medium priority; 4 - low priority
    int language = ui->comboBox_mentor_language->currentIndex();    // 0 - reave out; 1 - high priority; 2 - medium priority; 3 - low priority
    // Suikei 02/07/2020 changed special into interests
    int interests = ui->comboBox_mentor_interests->currentIndex();      // 0 - reave out; 1 - high priority; 2 - medium priority; 3 - low priority

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

    // Suikei 02/07/2020 changed speical into interests
    switch (interests) {
    case 0: interests = 0; break;
    case 1: interests = 3; break;
    case 2: interests = 2; break;
    case 3: interests = 1; break;
    default: interests = 0; break;
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

    QSqlQuery query(db);

    QSqlTableModel mentor(this,db);
    mentor.setTable("mentor");
    mentor.setSort(4,Qt::AscendingOrder);

    if (max_num == 2){
        while(1)
        {
            mentor.setFilter("group_id=0  AND wwvp!='n' AND wwvp!='' AND train_complete='y'"); // update: remove 'is_confirmed' restriction
            mentor.select();

            if (mentor.rowCount() == 0)
                break;
            if (mentor.rowCount() == 1){
                QString uid=mentor.record(0).value(4).toString();
                query.exec(QString("UPDATE mentor SET group_id=-1 WHERE uid='%1'").arg(uid));
                break;
            }

            // start group
            QSqlRecord mentor1=mentor.record(0);

            // Suikei 02/07/2020 change index number since add phone to mentors
            QString mentor1id=mentor1.value(4).toString();
            QString mentor1round=mentor1.value(7).toString();
            QString mentor1level=mentor1.value(8).toString();
            QString mentor1college=mentor1.value(9).toString();
            QString mentor1type=mentor1.value(11).toString();
            QString mentor1gender=mentor1.value(12).toString();
            QString mentor1languages=mentor1.value(13).toString();
            QString mentor1interest=mentor1.value(16).toString();	// Suikei 02/07/2020 change special into interest


            QString group_mentor_id;
            int maxscore=0;

            for (int k=1;k<mentor.rowCount();k++)
            {
                // mentor2: the k mentor(score for each mentors except the current one
                QSqlRecord mentor2=mentor.record(k);
                bool enable=true;

                // Suikei 02/07/2020 change index number since add phone to mentors
                QString mentor2id=mentor2.value(4).toString();      // get mentors' id
                QString mentor2round=mentor2.value(7).toString();   // get mentors' round
                QString mentor2level=mentor2.value(8).toString();   // get mentors' level
                QStringList mentor2college=mentor2.value(9).toString().simplified().split(",");     // get mentors' college
                QString mentor2type=mentor2.value(11).toString();   // get mentors' type
                QString mentor2gender=mentor2.value(12).toString(); // get mentors' gender
                QStringList mentor2languages=mentor2.value(13).toString().simplified().split(",");  // get mentors' language
                QStringList mentor2interest=mentor2.value(16).toString().simplified().split(",");        // Suikei 02/07/2020 change special into interest

                bool roundCheck = false;
                bool levelCheck = false;
                bool collegeCheck = false;
                bool typeCheck = false;
                bool genderCheck = false;

                int cscore=0;   // score for each mentor2

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

                // interests
                for(int h=0;h<mentor2interest.count(); h++){
                    if(mentor1interest.contains(mentor2interest.value(h))){
                        cscore+=1*interests;
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
                        // update maximum score
                        maxscore=cscore;
                        //record the mentor with maximum score
                        group_mentor_id=mentor2id;
                    }
                }
            }

            // insert result into database
            if (!group_mentor_id.isEmpty()){
                query.exec(QString("UPDATE mentor SET group_id=(SELECT MAX(group_id)+1 FROM mentor) WHERE uid='%1' OR uid='%2'").arg(mentor1id).arg(group_mentor_id));
            }
            // leave single mentor alone (wait for manual matching)
            else{
                query.exec(QString("UPDATE mentor SET group_id=-1 WHERE uid='%1'").arg(mentor1id));
            }
    //        qDebug()<<"mentor1"+mentor1id+"mentor2"+group_mentor_id;
        }

    //    // change all -1 group into 0 back
        query.exec(QString("UPDATE mentor SET group_id=0 WHERE group_id='-1'"));
    }

    // one mentor per group
    if (max_num == 1){
        while(1){
            mentor.setFilter("group_id=0  AND wwvp!='n' AND wwvp!='' AND train_complete='y'"); // update: remove 'is_confirmed' restriction
            mentor.select();

            if (mentor.rowCount() == 0)
                break;
            if (mentor.rowCount() == 1){
                QString uid=mentor.record(0).value(4).toString();
                query.exec(QString("UPDATE mentor SET group_id=(SELECT MAX(group_id)+1 FROM mentor) WHERE uid='%1'").arg(uid));
                break;
            }
            for (int k=1;k<mentor.rowCount();k++)
            {
                QString uid=mentor.record(k).value(4).toString();
                query.exec(QString("UPDATE mentor SET group_id=(SELECT MAX(group_id)+1 FROM mentor) WHERE uid='%1'").arg(uid));
            }
        }
    }



    delete model_mentors;
    delete model_mentees;


}
