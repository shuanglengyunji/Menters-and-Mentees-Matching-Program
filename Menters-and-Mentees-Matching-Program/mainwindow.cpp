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

    // init pages
    init_mentors_page();
    init_mentees_page();
    init_match_page();

    // switch to mentors' page
    ui->stack->setCurrentIndex(0);
    ui->actionMentors_Editing->setChecked(true);     //qDebug() << "Switch to Mentors Page";
}

MainWindow::~MainWindow()
{
    delete model_mentors;
    delete model_mentees;
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
    db = QSqlDatabase::addDatabase("QSQLITE");      //db.setDatabaseName(":memory:");
    db.setDatabaseName(db_path);
    if (!db.open()) {
        qDebug() << "Cannot open database";
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"), QMessageBox::Cancel);
        return;
    }

    QSqlQuery query(db);
    QString str = "";

    // Drop All Tables
    query.exec("DROP TABLE IF EXISTS 'group'");
    query.exec("DROP TABLE IF EXISTS 'mentor'");
    query.exec("DROP TABLE IF EXISTS 'mentee'");

    // Create New Tables
    query.exec("CREATE TABLE IF NOT EXISTS [group] (                             \
               gid         INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,  \
               mentor_id	VARCHAR(10) NOT NULL,                               \
               mentee_id	VARCHAR(10) NOT NULL                                \
           )");
    query.exec("CREATE TABLE IF NOT EXISTS [mentee] (            \
               uid	VARCHAR(10) NOT NULL UNIQUE,                \
               first_name	VARCHAR(20) NOT NULL,               \
               last_name	VARCHAR(20) NOT NULL,               \
               academic_level	VARCHAR(20) NOT NULL,           \
               college	VARCHAR(50) NOT NULL,                   \
               requests	TEXT(500),                          \
               round_1	CHAR(1) NOT NULL,                       \
               round_2	CHAR(1) NOT NULL,                       \
               PRIMARY KEY(uid)                                \
           )");
    query.exec("CREATE TABLE IF NOT EXISTS [mentor] (           \
               uid              VARCHAR(10) NOT NULL UNIQUE,    \
               first_name       VARCHAR(20) NOT NULL,           \
               last_name        VARCHAR(20) NOT NULL,           \
               gender           VARCHAR(20) NOT NULL,           \
               academic_level	VARCHAR(20) NOT NULL,           \
               college          VARCHAR(50) NOT NULL,           \
               degree           VARCHAR(30) NOT NULL,           \
               type             VARCHAR(50) NOT NULL,           \
               languages        VARCHAR(20) NOT NULL,           \
               languages_text	TEXT(500),                      \
               hall             VARCHAR(20) NOT NULL,           \
               special          TEXT(1000),                      \
               interests        TEXT(1000),                      \
               wwvp             VARCHAR(10) NOT NULL,           \
               train_1          CHAR(1) NOT NULL,               \
               train_2          CHAR(1) NOT NULL,               \
               train_3          CHAR(1) NOT NULL,               \
               train_complete	CHAR(1) NOT NULL,               \
               round            TEXT(100) NOT NULL,             \
               is_confirmed     TEXT(500),                      \
               is_grouped       INTEGER NOT NULL DEFAULT 0,     \
               PRIMARY KEY(uid)                                 \
           )");

    qDebug() << "Database Init Success";
}

void MainWindow::on_actionMentors_Editing_triggered()
{
    ui->stack->setCurrentIndex(0);      // qDebug() << "Switch to Mentors Page";

    ui->actionMentors_Editing->setChecked(true);
    ui->actionMentees_Editing->setChecked(false);
    ui->actionMentors_Grouping->setChecked(false);
    ui->actionMentees_Grouping->setChecked(false);
}

void MainWindow::on_actionMentees_Editing_triggered()
{
    ui->stack->setCurrentIndex(1);      // qDebug() << "Switch to Mentees Page";

    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(true);
    ui->actionMentors_Grouping->setChecked(false);
    ui->actionMentees_Grouping->setChecked(false);
}

void MainWindow::on_actionMentors_Grouping_triggered()
{
    ui->stack->setCurrentIndex(2);      // qDebug() << "Switch to Mentors Grouping Page";

    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(false);
    ui->actionMentors_Grouping->setChecked(true);
    ui->actionMentees_Grouping->setChecked(false);
}

void MainWindow::on_actionMentees_Grouping_triggered()
{
    ui->stack->setCurrentIndex(3);      // qDebug() << "Switch to Mentees Grouping Page";

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

// match
str = "CREATE TABLE [match] (                           \
        m_id      INTEGER      PRIMARY KEY              \
                               UNIQUE                   \
                               NOT NULL                 \
                               COLLATE BINARY,          \
        mentor_id VARCHAR (10) REFERENCES mentor (uid)  \
                               NOT NULL,                \
        mentee_id VARCHAR (10) REFERENCES mentee (uid)  \
                               NOT NULL                 \
    )";
// mentee
str = "CREATE TABLE mentee (                            \
        uid            VARCHAR (10) PRIMARY KEY         \
                                    UNIQUE              \
                                    NOT NULL,           \
        first_name     VARCHAR (20) NOT NULL,           \
        last_name      VARCHAR      NOT NULL,           \
        gender         VARCHAR (8)  NOT NULL,           \
        email          VARCHAR (30),                    \
        mobile         VARCHAR (20),                    \
        academic_level VARCHAR (20),                    \
        college        VARCHAR (50),                    \
        languages      VARCHAR (30) NOT NULL            \
                                    DEFAULT English,    \
        consideration  TEXT (200),                      \
        role           VARCHAR      DEFAULT mentee      \
                                    NOT NULL            \
    )";
// mentor
str = "CREATE TABLE mentor (                        \
        uid            VARCHAR (10) PRIMARY KEY     \
                                    UNIQUE          \
                                    NOT NULL,       \
        first_name     VARCHAR (20) NOT NULL,       \
        last_name      VARCHAR (20) NOT NULL,       \
        gender         VARCHAR (20),                \
        email          VARCHAR (30),                \
        mobile         VARCHAR (20),                \
        add_info       TEXT (500),                  \
        academic_level VARCHAR (20),                \
        college        VARCHAR (50),                \
        languages      VARCHAR (30) DEFAULT English \
                                    NOT NULL,       \
        train_1        CHAR (1)     DEFAULT n       \
                                    NOT NULL,       \
        train_2        CHAR (1)     NOT NULL        \
                                    DEFAULT n,      \
        train_3        CHAR (1)     DEFAULT n       \
                                    NOT NULL,       \
        wwvp_card      VARCHAR (10),                \
        is_confirm     CHAR (1)     DEFAULT n       \
                                    NOT NULL,       \
        role           VARCHAR      DEFAULT mentor  \
                                    NOT NULL        \
    )";
*/



