#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // app apth
    QString path = qApp->applicationDirPath();
    qDebug() << "App Dir:" << path;

    // tmp path
    QString tmp_path = path + "/tmp";
    qDebug() << "tmp Path" << tmp_path;
    QDir().mkdir(tmp_path);

    // database path
    QString db_path = tmp_path + "/" + MY_DATA_BASE_NAME;
    if(QFile().exists(db_path))
    {
        QFile().remove(db_path);
        qDebug() << "Old database removed";
    }
    qDebug() << "Database Path:" << db_path;

    // database demo path
    QString db_demo_path = tmp_path + "/" + MY_DATABASE_DEMO_NAME;
    qDebug() << "Database Demo Path:" << db_demo_path;

    // init database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_path);
    //db.setDatabaseName(db_demo_path);
    if (!db.open()) {
        qDebug() << "Cannot open database";
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"), QMessageBox::Cancel);
        return;
    }

    QSqlQuery query(db);


    // match
    QString str = "CREATE TABLE [match] ( m_id INTEGER PRIMARY KEY UNIQUE NOT NULL, \
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

    // display mentors' table
    model_mentors = new QSqlTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_mentors->setTable("mentor");
    model_mentors->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_mentors->select();

    model_mentors->setHeaderData(0, Qt::Horizontal, "Uni ID");
    model_mentors->setHeaderData(1, Qt::Horizontal, "First Name");
    model_mentors->setHeaderData(2, Qt::Horizontal, "Last Name");
    model_mentors->setHeaderData(3, Qt::Horizontal, "Gender");
    model_mentors->setHeaderData(4, Qt::Horizontal, "Academic Level");
    model_mentors->setHeaderData(5, Qt::Horizontal, "College");
    model_mentors->setHeaderData(6, Qt::Horizontal, "Language");
    model_mentors->setHeaderData(7, Qt::Horizontal, "Training 1");
    model_mentors->setHeaderData(8, Qt::Horizontal, "Training 2");
    model_mentors->setHeaderData(9, Qt::Horizontal, "WWVP Card Num");
    model_mentors->setHeaderData(10, Qt::Horizontal, "Confirm");
    model_mentors->setHeaderData(11, Qt::Horizontal, "Role");

    ui->tableView_mentors->setModel(model_mentors);
    ui->tableView_mentors->hideColumn(11);
    ui->tableView_mentors->resizeColumnsToContents();

    // display mentees' table
    model_mentees = new QSqlTableModel(this,db);    // model_mentees is a private pointer defined in header file
    model_mentees->setTable("mentee");
    model_mentees->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_mentees->select();

    model_mentees->setHeaderData(0, Qt::Horizontal, "Uni ID");
    model_mentees->setHeaderData(1, Qt::Horizontal, "First Name");
    model_mentees->setHeaderData(2, Qt::Horizontal, "Last Name");
    model_mentees->setHeaderData(3, Qt::Horizontal, "Gender");
    model_mentees->setHeaderData(4, Qt::Horizontal, "Academic Level");
    model_mentees->setHeaderData(5, Qt::Horizontal, "College");
    model_mentees->setHeaderData(6, Qt::Horizontal, "Language");
    model_mentees->setHeaderData(7, Qt::Horizontal, "Consideration");
    model_mentees->setHeaderData(8, Qt::Horizontal, "Role");

    ui->tableView_mentees->setModel(model_mentees);
    ui->tableView_mentees->hideColumn(8);
    ui->tableView_mentees->resizeColumnsToContents();

    // switch to mentors' page
    ui->stack->setCurrentIndex(0);
    ui->actionManage_Mentors->setChecked(true);
    qDebug() << "Switch to Mentors Page";

}

MainWindow::~MainWindow()
{
    delete ui;
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

