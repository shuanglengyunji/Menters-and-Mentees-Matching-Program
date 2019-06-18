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
    //db.setDatabaseName(db_path);
    db.setDatabaseName(db_demo_path);
    if (!db.open()) {
        qDebug() << "Cannot open database";
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"), QMessageBox::Cancel);
        return;
    }
    qDebug() << "Database Init Success";

    model = new QSqlTableModel(this,db);
    model->setTable("m_Mentors");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    ui->tableView_mentors->setModel(model);
    ui->tableView_mentors->resizeColumnsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionImport_Mentors_triggered()
{
    qDebug() << "Import Mentors data";
}

void MainWindow::on_actionImport_Mentees_triggered()
{
    qDebug() << "Import Mentees data";
}
