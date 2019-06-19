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

    // mentors' table
    model_mentors = new QSqlTableModel(this,db);
    model_mentors->setTable("m_Mentors");
    model_mentors->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_mentors->select();

    ui->tableView_mentors->setModel(model_mentors);
    ui->tableView_mentors->resizeColumnsToContents();

    // mentees' table
    model_mentees = new QSqlTableModel(this,db);
    model_mentees->setTable("m_Mentees");
    model_mentees->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_mentees->select();

    ui->tableView_mentees->setModel(model_mentees);
    ui->tableView_mentees->resizeColumnsToContents();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionImport_Mentors_triggered()
{
    qDebug() << "Import Mentors data";

    QString file_path;
    file_path = QFileDialog::getOpenFileName(this,tr("Import Mentors Data"),"",tr("Data file (*.csv)"));
    if(file_path.isEmpty())
    {
        qDebug() << "Open failed";
        return;
    }
    qDebug() << "Mentors file Path:" << file_path;

}

void MainWindow::on_actionImport_Mentees_triggered()
{
    qDebug() << "Import Mentees data";

    QString file_path;
    file_path = QFileDialog::getOpenFileName(this,tr("Import Mentees Data"),"",tr("Data file (*.csv)"));
    if(file_path.isEmpty())
    {
        qDebug() << "Open failed";
        return;
    }
    qDebug() << "Mentees file Path:" << file_path;

}

void MainWindow::on_actionManage_Mentors_triggered()
{
    ui->stack->setCurrentIndex(0);
    qDebug() << "Switch to Mentors Page";
}

void MainWindow::on_actionManage_Mentees_triggered()
{
    ui->stack->setCurrentIndex(1);
    qDebug() << "Switch to Mentees Page";
}

void MainWindow::on_actionManage_Matching_triggered()
{
    ui->stack->setCurrentIndex(2);
    qDebug() << "Switch to Matching Page";
}

void MainWindow::on_pushButton_mentors_add_clicked()
{

}

void MainWindow::on_pushButton_mentors_delete_clicked()
{

}

void MainWindow::on_pushButton_mentors_revert_clicked()
{

}

void MainWindow::on_pushButton_mentors_submit_clicked()
{

}

void MainWindow::on_pushButton_mentees_add_clicked()
{

}

void MainWindow::on_pushButton_mentees_delete_clicked()
{

}

void MainWindow::on_pushButton_mentees_revert_clicked()
{

}

void MainWindow::on_pushButton_mentees_submit_clicked()
{

}


