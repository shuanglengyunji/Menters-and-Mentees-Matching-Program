#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::MainWindow::algorithm_mentees_match()
{
    int u18 = ui->comboBox_u18->currentIndex();     // 0 - Leave Out    1 - Manual Match
    int round = ui->comboBox_round->currentIndex();
    int level = ui->comboBox_academic_level->currentIndex();
    int college = ui->comboBox_college->currentIndex();
    int type = ui->comboBox_type->currentIndex();
    int gender = ui->comboBox_gender->currentIndex();
    int language = ui->comboBox_language->currentIndex();
    int interest = ui->comboBox_interests->currentIndex();
    int importance = ui->comboBox_importance->currentIndex();      // 0 - Leave Out    1 - Manual Match
    int personal_mentees_num = ui->comboBox_personal_mentees_num->currentIndex();   // 0 - Leave Out    1 - Enable
    int max_mentees_num = ui->spinBox_mentees_num->value();

    switch (round) {
    case 0: round = 0; break;
    case 1: round = 1; break;   // must be same
    case 2: round = 4; break;
    case 3: round = 3; break;
    case 4: round = 2; break;
    default: round = 0; break;
    }

    switch (level) {
    case 0: level = 0; break;
    case 1: level = 1; break;   // must be same
    case 2: level = 4; break;
    case 3: level = 3; break;
    case 4: level = 2; break;
    default: level = 0; break;
    }

    switch (college) {
    case 0: college = 0; break;
    case 1: college = 1; break;   // must be same
    case 2: college = 4; break;
    case 3: college = 3; break;
    case 4: college = 2; break;
    default: college = 0; break;
    }

    switch (type) {
    case 0: type = 0; break;
    case 1: type = 1; break;   // must be same
    case 2: type = 4; break;
    case 3: type = 3; break;
    case 4: type = 2; break;
    default: type = 0; break;
    }

    switch (gender) {
    case 0: gender = 0; break;
    case 1: gender = 1; break;   // must be same
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

    while(1) {

        bool end = true;

        QSqlQueryModel mentor(this);
        mentor.setQuery("SELECT * FROM mentor WHERE wwvp = 1 AND train_complete = 1;", db);
        while(mentor.canFetchMore()){
            mentor.fetchMore();
        }
        for (int k=0;k<mentor.rowCount();k++)  // loop each mentor
        {
            // pick up a mentor
            QSqlRecord r = mentor.record(k);

            // count
            QSqlQueryModel mentee_count_model(this);
            mentee_count_model.setQuery(QString("SELECT count(*) FROM mentee WHERE mentee.mentor_uid = '%1';").arg(r.value("uid").toString()), db);
            int mentee_count = mentee_count_model.record(0).value(0).toInt();

            if (personal_mentees_num && mentee_count >= mentor.record(k).value("capacity").toInt()) {
                continue;
            };

            if (mentee_count >= max_mentees_num) {
                continue;
            }

            // parse mentor
            QString mentoruid = r.value("uid").toString();
            int mentorround = r.value("round").toInt();
            int mentorlevel = r.value("academic_level").toInt();
            QStringList mentorcollege = r.value("college").toString().split(",");
            int mentortype = r.value("type").toInt();
            int mentorgender = r.value("gender").toInt();
            QStringList mentorlanguages = r.value("languages").toString().split(",");
            QStringList mentorinterest = r.value("interests").toString().split(",");

            // fetch mentees
            QSqlQueryModel mentee(this);
            QString mentee_query = "SELECT * FROM mentee WHERE mentor_uid IS NULL ";
            if (u18) {
                mentee_query += "AND u18 = 0 ";
            }
            if (importance) {
                mentee_query += "AND importance = 0 ";
            }
            mentee.setQuery(mentee_query, db);
            while(mentee.canFetchMore()){
                mentee.fetchMore();
            }

            // loop over mentees
            QString matched_mentee_uid;
            int maxscore = -1;
            for (int i=0; i<mentee.rowCount(); i++) {
                QString menteeuid = mentee.record(i).value("uid").toString();
                int menteeround = mentee.record(i).value("round").toInt();
                int menteelevel = mentee.record(i).value("academic_level").toInt();
                QStringList menteecollege = mentee.record(i).value("college").toString().split(",");
                int menteetype = mentee.record(i).value("type").toInt();
                int menteegender = mentee.record(i).value("gender").toInt();
                QStringList menteelanguages = mentee.record(i).value("languages").toString().split(",");
                QStringList menteeinterest = mentee.record(i).value("interests").toString().split(",");

                int score=0;   // initial score for each mentees

                if (score>=0) {
                    if (round == 1){             // round
                        if (menteeround == mentorround) {
                            score += 4;
                        } else {
                            score = -1000;
                        }
                    } else {
                        score += (menteeround == mentorround) * round;
                    }
                }

                if (score>=0) {
                    if(level == 1){             // level
                        if (menteelevel == mentorlevel) {
                            score += 4;
                        } else {
                            score = -1000;
                        }
                    } else {
                        score += (menteelevel == mentorlevel) * level;
                    }
                }

                if (score>=0) {
                    if(type == 1){             // type
                        if (menteetype == mentortype) {
                            score += 4;
                        } else {
                            score = -1000;
                        }
                    } else {
                        score += (menteetype == mentortype) * type;
                    }
                }

                if (score>=0) {
                    if(gender == 1){             // gender
                        if (menteegender == mentorgender) {
                            score += 4;
                        } else {
                            score = -1000;
                        }
                    } else {
                        score += (menteegender == mentorgender) * gender;
                    }
                }

                if (score>=0) {                 // college
                    int college_score = 0;
                    foreach(QString college, menteecollege) {
                        if (mentorcollege.contains(college)) {
                            college_score+=1;
                        }
                    }
                    if(college == 1) {
                        if (college_score == 0) {
                            score = -1000;
                        } else {
                            score += 4*college_score;
                        }
                    } else {
                        score += college*college_score;
                    }
                }

                if (score>=0) {
                    int language_score = 0;
                    foreach(QString language, menteelanguages) {
                        if (mentorlanguages.contains(language)) {
                            language_score+=1;
                        }
                    }
                    score += language*language_score;
                }

                if (score>=0) {
                    int interest_score = 0;
                    foreach(QString interest, menteeinterest) {
                        if (mentorinterest.contains(interest)) {
                            interest_score += 1;
                        }
                    }
                    score += interest*interest_score;
                }

                // store the maximum score
                if (score>=0)
                {
                    if (score>=maxscore){
                        maxscore=score;
                        matched_mentee_uid=menteeuid;
                    }
                }
            }

            if (maxscore>=0 && !matched_mentee_uid.isEmpty()){
                QSqlQuery query(db);
                query.exec(QString("UPDATE mentee SET mentor_uid='%1' WHERE uid='%2'").arg(mentoruid).arg(matched_mentee_uid));
                end = false;
            }
        }

        if (end) {
            break;
        }
    }
}

