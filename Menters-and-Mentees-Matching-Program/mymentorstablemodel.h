#ifndef MYMENTORSTABLEMODEL_H
#define MYMENTORSTABLEMODEL_H

#include <QObject>
#include <QDebug>
#include <qsqltablemodel.h>
#include "parser.h"

class myMentorsTableModel : public QSqlTableModel
{
public:
    myMentorsTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase(), bool use_group_id_as_row_number = false) {
        this->setTable("mentor");
        this->setEditStrategy(QSqlTableModel::OnFieldChange);
        this->select();
        while(this->canFetchMore()){
            this->fetchMore();
        }
        this->use_group_id_as_row_number = use_group_id_as_row_number;
    };
    QVariant headerData(int section,  Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void init_table();
    parser get_parser(int idx) {
        for(int i=0; i<this->parser_list.size(); i++) {
            parser p = parser_list[i];
            if (p.get_index() == idx) {
                return p;
            }
        }
        qDebug() << "unexpected parser index";
        exit(1);
    }

private:
    bool use_group_id_as_row_number;
    QString table_init_query = "CREATE TABLE IF NOT EXISTS [mentor] (           \
        first_name		VARCHAR(1000),                    \
        last_name		VARCHAR(1000),                    \
        uid				VARCHAR(20) NOT NULL UNIQUE,    \
        phone			VARCHAR(1000),                    \
        wwvp            INTEGER(1),                    \
        round			INTEGER(1),                     \
        academic_level	INTEGER(1),                     \
        college			VARCHAR(50),                     \
        degree			VARCHAR(1000),                   \
        type            INTEGER(1),                         \
        gender			INTEGER(1),                     \
        languages		VARCHAR(50),                    \
        languages_text	TEXT(1000),                      \
        hall            VARCHAR(1000),                        \
        interests		VARCHAR(50),                     \
        capacity		INTEGER,                     \
        train_1			INTEGER(1),                     \
        train_2			INTEGER(1),                     \
        train_3			INTEGER(1),                     \
        train_complete	INTEGER(1),                     \
        PRIMARY KEY(uid)                                 \
    )";
    QList<parser> parser_list = {
        parser(1, parser::Mode::pass_through, "First Name", "first_name"),
        parser(2, parser::Mode::pass_through, "Last Name", "last_name"),
        parser(3, parser::Mode::pass_through, "Uni ID", "uid"),
        parser(4, parser::Mode::pass_through, "Phone", "phone"),
        parser(5, parser::Mode::yes_or_no, "WWVP", "wwvp"),
        parser(6, parser::Mode::string_matching, "Round", "round",
            QStringList(QList<QString>() << "Round 1" << "Round 2")),
        parser(7, parser::Mode::string_matching, "UG/PG", "academic_level",
            QStringList(QList<QString>() << "Postgraduate (coursework)" << "Undergraduate")),
        parser(8, parser::Mode::string_matching_multiple, "Academic College", "college",
            QStringList(QList<QString>() << "College of Asia and the Pacific"
            << "College of Arts and Social Sciences" << "College of Business and Economics"
            << "College of Engineering and Computer Science" << "College of Law"
            << "College of Science" << "College of Health and Medicine"
        )),
        parser(9, parser::Mode::pass_through, "Degree", "degree"),
        parser(10, parser::Mode::string_matching, "Dom/Int", "type", QStringList(QList<QString>() << "Domestic" << "International")),
        parser(11, parser::Mode::string_matching, "Gender", "gender", QStringList(QList<QString>() << "Female" << "Male" << "Other" << "Prefer not to say")),
        parser(12, parser::Mode::string_matching_multiple, "Languages", "languages",
            QStringList(QList<QString>() << "Hindi" << "Vietnamese" << "German" << "Korean"
            << "Tamil" << "Mandarin (Chinese)" << "Spanish" << "Cantonese" << "Indonesian" << "Japanese" << "Urdu" << "Other (please specify)"
            << "I only speak English"
        )),
        parser(13, parser::Mode::pass_through, "Language - Text", "languages_text"),
        parser(14, parser::Mode::pass_through, "Residential Hall", "hall"),
        parser(15, parser::Mode::string_matching_multiple, "Interests", "interests",
            QStringList(QList<QString>() << "Travel" << "Comics, manga, and anime"
            << "Learning languages" << "Science fiction and fantasy" << "Gardening" << "Hiking, nature, and outdoor recreation"
            << "Cooking or food" << "Watching movies" << "Performing arts - theatre, dance, etc." << "Visual arts - drawing, painting, craft, etc."
            << "Playing or creating music" << "Playing sport"),
               true
               ),
        parser(16, parser::Mode::pass_through, "Capacity", "capacity"),
        parser(17, parser::Mode::yes_or_no, "Training 1", "train_1"),
        parser(18, parser::Mode::yes_or_no, "Training 2", "train_2"),
        parser(19, parser::Mode::yes_or_no, "Training 3", "train_3"),
        parser(20, parser::Mode::yes_or_no, "Training Complete", "train_complete"),
    };
};

#endif // MYMENTORSTABLEMODEL_H
