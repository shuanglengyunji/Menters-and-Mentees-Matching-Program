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
    myMentorsTableModel(this, db).create_table();
    myMenteesTableModel(this, db).create_table();

    // switch to mentors' page
    ui->stack->setCurrentIndex(0);      // qDebug() << "Switch to Mentors Page";
    ui->actionManage->setChecked(true);
    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(false);
    ui->actionMentees_Grouping->setChecked(false);
}

MainWindow::~MainWindow()
{
    // mentors
    delete model_mentors;

    // mentees
    delete model_mentees;

    // match
    delete model_match_mentors;
    delete model_match_mentees_matched;
    delete model_match_mentees_to_be_match;

    delete ui;
}

void MainWindow::table_header_menu(QTableView * view)
{
    view->horizontalHeader()->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(view->horizontalHeader(), &QHeaderView::customContextMenuRequested, this, [this, view](QPoint pos) {
        int idx = view->horizontalHeader()->logicalIndexAt(pos);
        QMenu contextMenu(this);
        QAction hide_section(QString("Hide %1").arg(view->model()->headerData(idx, Qt::Orientation::Horizontal, Qt::DisplayRole).toString()), this);
        connect(&hide_section, &QAction::triggered, this, [&](){
            view->horizontalHeader()->hideSection(idx);
        });
        contextMenu.addAction(&hide_section);
        QAction show_all("Show all", this);
        connect(&show_all, &QAction::triggered, this, [&](){
            for (int i=0; i<view->horizontalHeader()->count(); i++) {
                view->horizontalHeader()->showSection(i);
            }
        });
        if (view->horizontalHeader()->sectionsHidden()) {
            contextMenu.addAction(&show_all);
        }
        contextMenu.exec(QCursor::pos());
    });
}

void MainWindow::on_actionManage_triggered()
{
    ui->stack->setCurrentIndex(0);      // qDebug() << "Switch to Mentees Grouping Page";

    ui->actionManage->setChecked(true);
    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(false);
    ui->actionMentees_Grouping->setChecked(false);
}

void MainWindow::on_actionMentors_Editing_triggered()
{
    ui->stack->setCurrentIndex(1);      // qDebug() << "Switch to Mentors Page";

    ui->actionManage->setChecked(false);
    ui->actionMentors_Editing->setChecked(true);
    ui->actionMentees_Editing->setChecked(false);
    ui->actionMentees_Grouping->setChecked(false);

    load_mentors();
}

void MainWindow::on_actionMentees_Editing_triggered()
{
    ui->stack->setCurrentIndex(2);      // qDebug() << "Switch to Mentees Page";

    ui->actionManage->setChecked(false);
    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(true);
    ui->actionMentees_Grouping->setChecked(false);

    load_mentees();
}

void MainWindow::on_actionMentees_Grouping_triggered()
{
    ui->stack->setCurrentIndex(3);      // qDebug() << "Switch to Mentees Grouping Page";

    ui->actionManage->setChecked(false);
    ui->actionMentors_Editing->setChecked(false);
    ui->actionMentees_Editing->setChecked(false);
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


