#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::MainWindow::algorithm_mentees_match()
{
    int round = ui->comboBox_round->currentIndex();
    int level = ui->comboBox_academic_level->currentIndex();
    int college = ui->comboBox_college->currentIndex();
    int type = ui->comboBox_type->currentIndex();
    int gender = ui->comboBox_gender->currentIndex();
    int language = ui->comboBox_language->currentIndex();
    // Suikei 02/07/2020 change spcial into interest
    int interest = ui->comboBox_interests->currentIndex();
    // Suikei 03/07/2020 add under 18 option
    int u18 = ui->comboBox_u18->currentIndex();     // 0 - Leave Out    1 - Manual Match
    // int request = ui->comboBox_request->currentIndex();     // 0 - Leave Out    1 - Manual Match
    // Suikei -3/07/2020 add importance
    int importance = ui->comboBox_importance->currentIndex();      // 0 - Leave Out    1 - Manual Match
    int max_mentees_num = ui->spinBox_mentees_num->value();

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

    // Suikei 02/07/2020 change spcial into interest
    switch (interest) {
    case 0: interest = 0; break;
    case 1: interest = 3; break;
    case 2: interest = 2; break;
    case 3: interest = 1; break;
    default: interest = 0; break;
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


    QSqlQuery query(db);

    QSqlTableModel mentor(this,db);
    mentor.setTable("mentor");
    mentor.setSort(0,Qt::AscendingOrder); // use group id as order

    QSqlTableModel mentee(this,db);
    mentee.setTable("mentee");
    mentee.setSort(4,Qt::AscendingOrder); // use uid as order

    int group_count;
    QSqlQueryModel querymodel;
    querymodel.setQuery("SELECT MAX(group_id) FROM mentor",db);
    group_count = querymodel.record(0).value(0).toInt(); // find the total group numbers

    for (int k=0;k<max_mentees_num;k++)  // each loop assign each group 1 mentee
    {
        for (int k=0;k<group_count;k++)  // loop each group's mentors
        {
            QString search_mentor = "group_id=";
            QString group_id = QVariant(k+1).toString();
            search_mentor.append(group_id);

            mentor.setFilter(search_mentor);
            mentor.select();
            while(mentor.canFetchMore()){// find mentors of each group
                mentor.fetchMore();
            }

            // if current group has already maximum mentees, skip current group
            QSqlQueryModel mentee_count_model;
            QString count_mentee_group = "SELECT COUNT(uid) FROM mentee WHERE group_id=";
            count_mentee_group.append(group_id);
            mentee_count_model.setQuery(count_mentee_group,db);
            int current_group_mentee_count = mentee_count_model.record(0).value(0).toInt();
            if (current_group_mentee_count >= max_mentees_num)
                continue;

            // Suikei 02/07/2020 change index number
            QString mentorsgroup = mentor.record(0).value(0).toString(); // record mentor's group id for update the mentee group id
            QString mentorsround = mentor.record(0).value(7).toString(); // record mentor's details
            QString mentorslevel = mentor.record(0).value(8).toString();
            QString mentorscollege = mentor.record(0).value(9).toString();
            QString mentorstype = mentor.record(0).value(11).toString();
            QString mentorsgender = mentor.record(0).value(12).toString();
            QString mentorslanguages = mentor.record(0).value(13).toString();
            QString mentorsinterest = mentor.record(0).value(16).toString();

            if (mentor.rowCount() == 2){    // if only one mentor, just record his info, else, merge two mentors info
                // Suikei 02/07/2020 change index number
                mentorsround.append(",").append(mentor.record(1).value(7).toString());
                mentorslevel.append(",").append(mentor.record(1).value(8).toString());
                mentorscollege.append(",").append(mentor.record(1).value(9).toString());
                mentorstype.append(",").append(mentor.record(1).value(11).toString());
                mentorsgender.append(",").append(mentor.record(1).value(12).toString());
                mentorslanguages.append(",").append(mentor.record(1).value(13).toString());
                mentorsinterest.append(",").append(mentor.record(1).value(16).toString());
            }

            // loop each mentees and find the closest one
            // leave request
            // Suikei 03/07/2020 leave request and u18 or not
            if (u18 == 1 && importance == 1){ // leave both u18=true and importance=true(manually match u18 and request)
                mentee.setFilter("group_id=0 AND u18=0 AND importance=0");// only auto match 18+ and not important
            } else if (u18 == 1 && importance == 0){ // only leave u18=true(manually match u18)
                mentee.setFilter("group_id=0 AND u18=0");// auto match all 18+
            } else if (u18 == 0 && importance == 1){ // only leave importance=true(manualy match request)
                mentee.setFilter("group_id=0 AND importance=0");// auto match all not important
            } else{ // auto match all (u18==0, importance==0)
                mentee.setFilter("group_id=0");
            }

            mentee.select();
            while(mentee.canFetchMore()){
                mentee.fetchMore();
            }
            if (mentee.rowCount() == 0)
                break;

            QString match_mentee_id;
            int maxscore = 0;

            for (int k=0;k<mentee.rowCount();k++) {             // get each mentee's info
                // Suikei 02/07/2020 change index number
                QString menteeid = mentee.record(k).value(3).toString();
                QString menteeround = mentee.record(k).value(5).toString();
                QString menteelevel = mentee.record(k).value(6).toString();
                QStringList menteecollege = mentee.record(k).value(7).toString().simplified().split(",");
                QString menteetype = mentee.record(k).value(9).toString();
                QString menteegender = mentee.record(k).value(10).toString();
                QStringList menteelanguages = mentee.record(k).value(11).toString().simplified().split(",");
                QStringList menteeinterest = mentee.record(k).value(13).toString().simplified().split(",");

                bool enable=true;       // initial judge parameters
                bool roundCheck = false;
                bool levelCheck = false;
                bool collegeCheck = false;
                bool typeCheck = false;
                bool genderCheck = false;

                int cscore=0;   // initial score for each mentees

                if(mentorsround.contains(menteeround)){             // round
                    if(round!=1){
                        cscore+=1*round;
                    }
                    else {
                        roundCheck=true;
                    }
                }
                if(mentorslevel.contains(menteelevel)){             // level
                    if(level!=1){
                        cscore+=1*level;
                    }
                    else {
                        levelCheck=true;
                    }
                }
                for(int h=0;h<menteecollege.count();h++){           // college
                    if(mentorscollege.contains(menteecollege.value(h))){
                        if(college!=1){
                            cscore+=1*college;
                        }
                        else {
                            collegeCheck=true;
                        }
                    }
                }
                if(mentorstype.contains(menteetype)){               // type
                    if(type!=1){
                        cscore+=1*type;
                    }
                    else {
                        typeCheck=true;
                    }
                }
                if(mentorsgender.contains(menteegender)){           // gender
                    if(gender!=1){
                        cscore+=1*gender;
                    }
                    else {
                        genderCheck=true;
                    }
                }
                for(int h=0;h<menteelanguages.count(); h++){        // languages
                    if(mentorslanguages.contains(menteelanguages.value(h))){
                        cscore+=1*language;
                    }
                }
                // Suikei 06/07/2020 change special to interest
                for(int h=0;h<menteeinterest.count(); h++){          // special categories
                    if(mentorsinterest.contains(menteeinterest.value(h))){
                        cscore+=1*interest;
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
                        match_mentee_id=menteeid;
                    }
                }
            }
            // record mentee with mentors group
            if (!match_mentee_id.isEmpty()){
                int groupid = mentorsgroup.toInt();
                query.exec(QString("UPDATE mentee SET group_id=%1 WHERE uid='%2'").arg(groupid).arg(match_mentee_id));
            }
        }
    }

    delete model_mentors;
    delete model_mentees;
}

