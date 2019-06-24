#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // app apth
    QString tmp_path = qApp->applicationDirPath() + "/tmp";
    QDir().mkdir(tmp_path);
    qDebug() << "tmp Path" << tmp_path;

    // init database
    init_database(tmp_path);

    // init pages
    init_mentors_page();
    init_mentees_page();
    init_match_page();

    // switch to mentors' page
    ui->stack->setCurrentIndex(0);
    ui->actionManage_Mentors->setChecked(true);
    qDebug() << "Switch to Mentors Page";

}

MainWindow::~MainWindow()
{
    delete model_mentors;
    delete model_mentees;
    delete model_match_mentors;
    delete model_match_mentees_matched;
    delete model_match_mentees_to_be_match;
    delete ui;
}

void MainWindow::init_database(QString work_path)
{
    // get database path and database demo path
    QString db_path = work_path + "/" + MY_DATA_BASE_NAME;
    QString db_demo_path = work_path + "/" + MY_DATABASE_DEMO_NAME;

    // init database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    //db.setDatabaseName(db_path);
    //db.setDatabaseName(db_demo_path);
    if (!db.open()) {
        qDebug() << "Cannot open database";
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"), QMessageBox::Cancel);
        return;
    }

    QSqlQuery query(db);
    QString str = "";

    // Drop all the tables
    str = "DROP TABLE IF EXISTS 'match'";
    query.exec(str);

    str = "DROP TABLE IF EXISTS 'mentor'";
    query.exec(str);

    str = "DROP TABLE IF EXISTS 'mentee'";
    query.exec(str);

    // match
    str = "CREATE TABLE [match] ( m_id INTEGER PRIMARY KEY UNIQUE NOT NULL, \
            mentor_id VARCHAR (10) REFERENCES [mentor ] (uid) NOT NULL, \
            mentee_id VARCHAR (10) REFERENCES mentee (uid) NOT NULL)";
    query.exec(str);

    // mentor
    str = "CREATE TABLE mentor ( uid VARCHAR (10) PRIMARY KEY UNIQUE NOT NULL,  \
            first_name     VARCHAR (20) NOT NULL,   \
            last_name      VARCHAR (20) NOT NULL,   \
            gender         VARCHAR (20) NOT NULL,   \
            academic_level VARCHAR (20) NOT NULL,   \
            college        VARCHAR (50) NOT NULL,   \
            languages      VARCHAR (30) NOT NULL DEFAULT English,   \
            train_1        CHAR (1)     DEFAULT n NOT NULL, \
            train_2        CHAR (1)     NOT NULL DEFAULT n, \
            wwvp_card      VARCHAR (10),    \
            is_confirm     CHAR (1)     DEFAULT n NOT NULL, \
            role           VARCHAR      DEFAULT mentor NOT NULL)";
    query.exec(str);

    // mentee
    str = "CREATE TABLE mentee ( uid VARCHAR (10) PRIMARY KEY UNIQUE NOT NULL, \
                first_name     VARCHAR (20) NOT NULL,   \
                last_name      VARCHAR      NOT NULL,   \
                gender         VARCHAR (8)  NOT NULL,   \
                academic_level VARCHAR (20) NOT NULL,   \
                college        VARCHAR (50) NOT NULL,   \
                languages      VARCHAR (30) NOT NULL DEFAULT English,   \
                consideration  TEXT (200), role VARCHAR DEFAULT mentee NOT NULL)";
    query.exec(str);


    qDebug() << "Database Init Success";
}

void MainWindow::on_actionManage_Mentors_triggered()
{
    ui->stack->setCurrentIndex(0);
    qDebug() << "Switch to Mentors Page";

    ui->actionManage_Mentors->setChecked(true);
    ui->actionManage_Mentees->setChecked(false);
    ui->actionManage_Matching->setChecked(false);
}

void MainWindow::on_actionManage_Mentees_triggered()
{
    ui->stack->setCurrentIndex(1);
    qDebug() << "Switch to Mentees Page";

    ui->actionManage_Mentors->setChecked(false);
    ui->actionManage_Mentees->setChecked(true);
    ui->actionManage_Matching->setChecked(false);
}

void MainWindow::on_actionManage_Matching_triggered()
{
    ui->stack->setCurrentIndex(2);
    qDebug() << "Switch to Matching Page";

    ui->actionManage_Mentors->setChecked(false);
    ui->actionManage_Mentees->setChecked(false);
    ui->actionManage_Matching->setChecked(true);

}


