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

    // mentors' table
    model_mentors = new QSqlTableModel(this,db);    // model_mentors is a private pointer defined in header file
    model_mentors->setTable("mentor");
    model_mentors->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_mentors->select();

    ui->tableView_mentors->setModel(model_mentors);
    ui->tableView_mentors->resizeColumnsToContents();

    // mentees' table
    model_mentees = new QSqlTableModel(this,db);    // model_mentees is a private pointer defined in header file
    model_mentees->setTable("mentee");
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


    QSqlQuery q("", db);

    QFile file(file_path);
    QTextStream ts (&file);
    int fs=1;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
                if(fs) {
                    QByteArray line=file.readLine();
                    fs=0;
                }
                while(!ts.atEnd())
                {
                    QString linedata="insert into mentor(uid, first_name, last_name, academic_level, college, languages, train_1, train_2, wwvp_card, is_confirm, role, gender) \n values (";
                    QStringList line=ts.readLine().split(',');
                    for(int i=0;i<line.length();i++){
                       linedata.append("\'");
                       QString temps=line.at(i);
                       temps.remove("\"");
                       linedata.append(temps);
                       linedata.append("\'");
                       linedata.append(",");
                    }
                    linedata.append("\'Mentor\', \'Other\')");
                    q.exec(linedata);
                }
                file.close();
    }
    else {
        QMessageBox::warning(this, tr("Unable to open file"), tr("An error occurred while "
                                                               "opening file: ") + db.lastError().text());
    }

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

    QSqlQuery q("", db);

    QFile file(file_path);
    QTextStream ts (&file);
    int fs=1;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
                if(fs) {
                    QByteArray line=file.readLine();
                    fs=0;
                }
                while(!ts.atEnd())
                {
                    QString linedata="insert into mentee(uid, first_name, last_name, languages, academic_level, college, consideration, role, gender) values(";
                    QStringList line=ts.readLine().split(',');
                    for(int i=0;i<line.length();i++){
                       linedata.append("'");
                       QString temps=line.at(i);
                       temps.remove("\"");
                       temps.remove("ANU ");
                       if(temps.contains("Arts")) temps="CASS";
                       else if(temps.contains("Law")) temps="COL";
                       else if(temps.contains("Asia")) temps="CAP";
                       else if(temps.contains("Business")) temps="CBE";
                       else if(temps.contains("Engineering")) temps="CECS";
                       else if(temps.contains("Health")) temps="CHM";
                       else if(temps.contains("Science")) temps="COS";
                       linedata.append(temps);
                       linedata.append("\'");
                       linedata.append(",");
                    }
                    linedata.append("\'Mentee\',\'Other\')");
                    qDebug()<<linedata<<endl;
                    q.exec(linedata);
                }
                file.close();
    }
    else {
        QMessageBox::warning(this, tr("Unable to open file"), tr("An error occurred while "
                                                               "opening file: ") + db.lastError().text());
    }

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

// mentors

void MainWindow::on_lineEdit_mentors_search_editingFinished()
{
    QString str = ui->lineEdit_mentors_search->text();
    if(str.isEmpty())
    {
        model_mentors->setFilter("");
    }
    else
    {
        QString argument = "uid LIKE '%" + str + "%'"   \
                + " OR " + "first_name LIKE '%" + str + "%'"    \
                + " OR " + "last_name LIKE '%" + str + "%'";
        qDebug() << argument;
        model_mentors->setFilter(argument);
    }
}

void MainWindow::on_pushButton_mentors_add_clicked()
{
    int row = ui->tableView_mentors->selectionModel()->currentIndex().row();
    model_mentors->insertRow(row);
    qDebug() << "Insert Row";
}

void MainWindow::on_pushButton_mentors_delete_clicked()
{
    int row = ui->tableView_mentors->selectionModel()->currentIndex().row();
    model_mentors->removeRow(row);
    model_mentors->submit();
    qDebug() << "Delete Row";
}

void MainWindow::on_pushButton_mentors_revert_clicked()
{

}

void MainWindow::on_pushButton_mentors_submit_clicked()
{

}

// mentees

void MainWindow::on_lineEdit_mentees_search_editingFinished()
{
    QString str = ui->lineEdit_mentees_search->text();
    if(str.isEmpty())
    {
        model_mentees->setFilter("");
    }
    else
    {
        QString argument = "uid LIKE '%" + str + "%'"   \
                + " OR " + "first_name LIKE '%" + str + "%'"    \
                + " OR " + "last_name LIKE '%" + str + "%'";
        qDebug() << argument;
        model_mentees->setFilter(argument);
    }
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

