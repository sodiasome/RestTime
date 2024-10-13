#include "LoginDlg.h"
#include "ui_LoginDlg.h"

#include "DbCtrl.h"
#include <QMessageBox>
#include <QSettings>

LoginDlg::LoginDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);

    Init();

    connect(ui->btn_Ok,SIGNAL(Clicked()),this,SLOT(on_btn_Ok_clicked));

}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::Init()
{
    InitUi();

    DbCtrl::Init("data.db");
    DbCtrl::RunSql("create table if not exists Person (ID int primary key, Name varchar(20), Passwd varchar(20));");
    DbCtrl::RunSql("create table if not exists Holiday("
                   "ID int auto_increment primary key, "
                   "HDate varchar(20), "                    //生理期日期
                   "HWeight varchar(20),"                   //体重
                   "HSpend varchar(20),"                    //预计持续天数
                   "HFlow varchar(20), "                     //流量
                   "HPain varchar(20) "                    //疼痛等级
                   ");");
    DbCtrl::RunSql("insert into Person values(1, 'admin', '0602')");

    QString path = "./user.ini";
    m_config = new QSettings(path, QSettings::IniFormat);

    QString strCheckName = m_config->value(QString("config/") + "NameRemember").toString();
    if(strCheckName == "Yes")
    {
        QSqlQuery query;
        query.exec("select Name from Person");
        query.next();
        QString strNameT;
        strNameT = query.value(0).toString();

        ui->cb_RememberName->setCheckState(Qt::CheckState::Checked);
        ui->ed_Name->setText(strNameT);
    }
    QString strCheckPasswd = m_config->value(QString("config/") + "PasswdRemember").toString();
    if(strCheckPasswd == "Yes")
    {
        QSqlQuery query;
        query.exec("select Passwd from Person");
        query.next();
        QString strPasswdT;
        strPasswdT = query.value(0).toString();

        ui->cb_RememberPasswd->setCheckState(Qt::CheckState::Checked);
        ui->ed_Passwd->setText(strPasswdT);
    }
}
void LoginDlg::InitUi()
{
    QFont fonTitle("楷体", 18, 75);
    ui->lb_LoginTitle->setFont(fonTitle);
    ui->ed_Passwd->setEchoMode(QLineEdit::Password);

    //this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(251,102,102, 200), stop:1 rgba(20,196,188, 210));");
    ui->btn_Ok->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                    "border: none;border-radius:15px;}"
                                    "QPushButton:hover{background-color: rgb(22,218,208);}"
                                    "QPushButton:pressed{background-color: rgb(17,171,164);}");
    ui->btn_Reset->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                    "border: none;border-radius:15px;}"
                                    "QPushButton:hover{background-color: rgb(22,218,208);}"
                                    "QPushButton:pressed{background-color: rgb(17,171,164);}");

    ui->ed_Name->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                    "color: rgb(31,31,31);"		//字体颜色
                                    "padding-left:20px;"       //内边距-字体缩进
                                    "background-color: rgb(255, 255, 255);" //背景颜色
                                    "border:2px solid rgb(20,196,188);border-radius:15px;");//边框粗细-颜色-圆角设置

    ui->ed_Passwd->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                    "color: rgb(31,31,31);"		//字体颜色
                                    "padding-left:20px;"       //内边距-字体缩进
                                    "background-color: rgb(255, 255, 255);" //背景颜色
                                    "border:2px solid rgb(20,196,188);border-radius:15px;");//边框粗细-颜色-圆角设置
}

void LoginDlg::on_btn_Ok_clicked()
{
    QString strName = ui->ed_Name->text();
    if(strName.isEmpty())
    {
        QMessageBox::information(NULL, "提示", "用户名输入为空！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    QString strPasswd = ui->ed_Passwd->text();
    if(strName.isEmpty())
    {
        QMessageBox::information(NULL, "提示", "密码输入为空！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }

    QSqlQuery query;
    query.exec("select * from Person");
    QString strNameT,strPasswdT;
    while(query.next())
    {
        strNameT = query.value(1).toString();
        strPasswdT = query.value(2).toString();
        if(strName == strNameT && strPasswd == strPasswdT)
        {
            emit(sigLogin());
            emit(sigClose());
            return;
        }
    }
    QMessageBox::information(nullptr,"提示","账号或密码错误",QMessageBox::Ok,QMessageBox::Ok);
}

void LoginDlg::on_btn_Reset_clicked()
{
    QMessageBox::information(nullptr,"提示","暂时不提供账号重置功能",QMessageBox::Ok,QMessageBox::Ok);
}

void LoginDlg::on_cb_RememberPasswd_stateChanged(int arg1)
{
    m_config->beginGroup("config");
    if(arg1)
        m_config->setValue("PasswdRemember", "Yes");
    else
        m_config->setValue("PasswdRemember", "No");
    m_config->endGroup();
}

void LoginDlg::on_cb_RememberName_stateChanged(int arg1)
{
    m_config->beginGroup("config");
    if(arg1)
        m_config->setValue("NameRemember", "Yes");
    else
        m_config->setValue("NameRemember", "No");
    m_config->endGroup();
}
