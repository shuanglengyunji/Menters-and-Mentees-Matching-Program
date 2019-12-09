#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // app apth
    QString tmp_path = qApp->applicationDirPath() + "/tmp";
    QDir().mkdir(tmp_path);  //qDebug() << "tmp Path" << tmp_path;

    // init database
    init_database(tmp_path);

    // switch to mentors' page
    ui->stack->setCurrentIndex(0);      // qDebug() << "Switch to Mentors Page";
    ui->actionManage->setChecked(true);
    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(false);
    ui->actionMentors_Grouping->setChecked(false);
    ui->actionMentees_Grouping->setChecked(false);

    // mentors
    connect(ui->checkBox_mentors_gender,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_academic_info,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_type,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_language,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_hall,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_special,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_interests,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_wwvp,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_training,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_round,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);
    connect(ui->checkBox_mentors_confirmation,&QCheckBox::stateChanged,this,&MainWindow::display_mentors_column);

    // mentees
    connect(ui->checkBox_mentees_gender,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_academic_info,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_type,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_language,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_requests,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_special_categories,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
    connect(ui->checkBox_mentees_round,&QCheckBox::stateChanged,this,&MainWindow::display_mentees_column);
}

MainWindow::~MainWindow()
{
    // mentors
    delete model_mentors;

    // mentees
    delete model_mentees;

    // grouping
    delete model_match_mentors;
    delete model_match_mentees_matched;
    delete model_match_mentees_to_be_match;
    delete model_proxy_match_mentors;
    delete model_proxy_match_mentees_to_be_match;
    delete ui;
}

void MainWindow::init_database(QString work_path)
{
    // get database path and database demo path
    QString db_path = work_path + "/" + MY_DATA_BASE_NAME;

    // init database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");         //db.setDatabaseName(db_path);
    if (!db.open()) {
        qDebug() << "Cannot open database";
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"), QMessageBox::Cancel);
        return;
    }

    QSqlQuery query(db);

    // Create New Tables
    query.exec("CREATE TABLE IF NOT EXISTS [mentor] (           \
               group_id         INTEGER NOT NULL DEFAULT 0,         \
               is_confirmed 	CHAR(1) NOT NULL DEFAULT 'n',                     \
               first_name		VARCHAR(20) NOT NULL,           \
               last_name		VARCHAR(20) NOT NULL,           \
               uid				VARCHAR(10) NOT NULL UNIQUE,    \
               wwvp             VARCHAR(10),                        \
               round			INTEGER(1),                     \
               academic_level	VARCHAR(50),                     \
               college			INTEGER(1),                     \
               degree			VARCHAR(100),                   \
               type             INTEGER(1),                         \
               gender			INTEGER(1),                     \
               languages		VARCHAR(50),                    \
               languages_text	TEXT(500),                      \
               hall             VARCHAR(50),                        \
               special			VARCHAR(50),                    \
               interests		TEXT(1000),                     \
               train_1			CHAR(1) NOT NULL DEFAULT 'n',   \
               train_2			CHAR(1) NOT NULL DEFAULT 'n',   \
               train_3			CHAR(1) NOT NULL DEFAULT 'n',   \
               train_complete	CHAR(1) NOT NULL DEFAULT 'n',   \
               PRIMARY KEY(uid)                                 \
           )");

    query.exec("CREATE TABLE IF NOT EXISTS [mentee] (           \
               group_id			INTEGER NOT NULL DEFAULT 0,     \
               first_name			VARCHAR(20) NOT NULL,       \
               last_name			VARCHAR(20) NOT NULL,       \
               uid					VARCHAR(10) NOT NULL UNIQUE,\
               round				INTEGER(1),                 \
               academic_level		VARCHAR(50),                 \
               college				INTEGER(1),                 \
               degree				VARCHAR(100),               \
               type                 INTEGER(1),                 \
               gender				INTEGER(1),                 \
               languages			VARCHAR(50),                \
               languages_text		TEXT(500),                  \
               special_categories	VARCHAR(50),                \
               requests             TEXT(1000),                 \
               PRIMARY KEY(uid)                                 \
           )");
}

void MainWindow::on_actionManage_triggered()
{
    ui->stack->setCurrentIndex(0);      // qDebug() << "Switch to Mentees Grouping Page";

    ui->actionManage->setChecked(true);
    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(false);
    ui->actionMentors_Grouping->setChecked(false);
    ui->actionMentees_Grouping->setChecked(false);
}

void MainWindow::on_actionMentors_Editing_triggered()
{
    ui->stack->setCurrentIndex(1);      // qDebug() << "Switch to Mentors Page";

    ui->actionManage->setChecked(false);
    ui->actionMentors_Editing->setChecked(true);
    ui->actionMentees_Editing->setChecked(false);
    ui->actionMentors_Grouping->setChecked(false);
    ui->actionMentees_Grouping->setChecked(false);

    load_mentors();
}

void MainWindow::on_actionMentees_Editing_triggered()
{
    ui->stack->setCurrentIndex(2);      // qDebug() << "Switch to Mentees Page";

    ui->actionManage->setChecked(false);
    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(true);
    ui->actionMentors_Grouping->setChecked(false);
    ui->actionMentees_Grouping->setChecked(false);

    load_mentees();
}

void MainWindow::on_actionMentors_Grouping_triggered()
{
    ui->stack->setCurrentIndex(3);      // qDebug() << "Switch to Mentors Grouping Page";

    ui->actionManage->setChecked(false);
    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(false);
    ui->actionMentors_Grouping->setChecked(true);
    ui->actionMentees_Grouping->setChecked(false);

    load_group_mentors();
}

void MainWindow::on_actionMentees_Grouping_triggered()
{
    ui->stack->setCurrentIndex(4);      // qDebug() << "Switch to Mentees Grouping Page";

    ui->actionManage->setChecked(false);
    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(false);
    ui->actionMentors_Grouping->setChecked(false);
    ui->actionMentees_Grouping->setChecked(true);
}

/*

// Drop all the tables
str = "DROP TABLE IF EXISTS 'match'";
query.exec(str);
str = "DROP TABLE IF EXISTS 'mentor'";
query.exec(str);
str = "DROP TABLE IF EXISTS 'mentee'";
query.exec(str);

*/






















