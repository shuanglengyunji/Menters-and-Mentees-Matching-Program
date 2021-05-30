#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVersionNumber version(1, 2, 3);

    // app apth
    QString tmp_path = qApp->applicationDirPath() + "/tmp";
    QDir().mkdir(tmp_path);  //qDebug() << "tmp Path" << tmp_path;

    // init database

    // get database path and database demo path
    QString db_path = tmp_path + "/" + MY_DATA_BASE_NAME;
    // init database
    db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(":memory:");         //db.setDatabaseName(db_path);
    db.setDatabaseName(db_path);
    if (!db.open()) {
        qDebug() << "Cannot open database";
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"), QMessageBox::Cancel);
        return;
    }
    myMentorsTableModel(this, db).init_table();
    myMenteesTableModel(this, db).init_table();

    // switch to mentors' page
    ui->stack->setCurrentIndex(0);      // qDebug() << "Switch to Mentors Page";
    ui->actionManage->setChecked(true);
    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(false);
    ui->actionMentors_Grouping->setChecked(false);
    ui->actionMentees_Grouping->setChecked(false);
}

MainWindow::~MainWindow()
{
    // mentors
    delete model_mentors;

    // mentees
    delete model_mentees;

    // group
    delete model_group_mentors_grouped;
    delete model_group_mentors_to_be_grouped;

    // match
    delete model_match_mentors;
    delete model_match_mentees_matched;
    delete model_match_mentees_to_be_match;

    delete ui;
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

    load_match_mentees();
}

/*

// Drop all the tables
str = "DROP TABLE IF EXISTS 'mentor'";
query.exec(str);
str = "DROP TABLE IF EXISTS 'mentee'";
query.exec(str);

*/


