#include "MainDlg.h"
#include "ui_dialog.h"
#include "DbCtrl.h"

#include <QDebug>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QImageReader>
#include <QFileDialog>

//#define PAIN_IMG "C:\\Users\\sodia\\Desktop\\Untitled.png"
#define PAIN_IMG "img\\Untitled.png"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    Init();

    connect(&m_loginDlg,SIGNAL(sigLogin()),this,SLOT(show()));
    connect(&m_loginDlg,SIGNAL(sigClose()),&m_loginDlg,SLOT(close()));
    connect(ui->listLevel1, SIGNAL(currentTextChanged(const QString &)), this, SLOT(OnTextChanged(const QString &)));
}
void Dialog::Init()
{
    m_loginDlg.show();
    m_nPainLevel = 0;

    m_iniFile = new QSettings("config.ini", QSettings::IniFormat);

    InitUi();

    ui->edCurr->setText(ui->caleCurr->selectedDate().toString("yyyy年MM月dd日"));
}
void Dialog::InitUi()
{
    //图片裁剪
    QImageReader imgReader;
    imgReader.setFileName(PAIN_IMG);
    QSize imgSize = imgReader.size();
    imgSize.scale(imgSize.width(),imgSize.height(),Qt::IgnoreAspectRatio);
    imgReader.setScaledSize(imgSize);
    QImage imgTarget = imgReader.read();
    for (int i=0;i<6;++i)
    {

        m_imgPain[i] = imgTarget.copy(i*210,0,200,270);

        //QString strTarget = QString("%1.jpg").arg(i);
        //if(m_imgPain[i].save(strTarget))
        //    qDebug() <<strTarget<<"ok";
        //else
        //    qDebug() <<strTarget<<"failed";
    }


    ui->listLevel1->clear();
    QSqlQuery query;
    query.exec("select * from Holiday;");
    while(query.next())
    {
        QString strValue = query.value(1).toString();
        ui->listLevel1->addItem(new QListWidgetItem(QIcon(":/png/"),strValue));
    }

    //疼痛显示
    QPixmap pixmap = QPixmap::fromImage(m_imgPain[m_nPainLevel]);
    int with = ui->lbPaintPic->width();
    int height = ui->lbPaintPic->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    ui->lbPaintPic->setPixmap(fitpixmap);

    //头像
    QString strUser = m_iniFile->value("config/UserPic").toString();
    if(strUser.isEmpty())
        strUser = PAIN_IMG;
    QImage img;
    img.load(strUser);
    QPixmap pixmapUser = QPixmap::fromImage(img);
    with = ui->lbInfoUser->width();
    height = ui->lbInfoUser->height();
    QPixmap fitpixmapUser = pixmapUser.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    ui->lbInfoUser->setPixmap(fitpixmapUser);
}
Dialog::~Dialog()
{
    delete ui;
}


void Dialog::OnTextChanged(const QString strText)
{
    QString strCurrPain;
    QString strCurrWeight;
    QString strCurrSpend;
    QString strCurrFlow;

    QString strSql = QString("select * from Holiday where HDate='%1';").arg(strText);

    QSqlQuery query;
    query.exec(strSql);
    while(query.next())
    {
        strCurrWeight = query.value(2).toString();
        strCurrSpend = query.value(3).toString();
        strCurrFlow = query.value(4).toString();
        strCurrPain = query.value(5).toString();
    }

    //痛经
    QPixmap pixmap = QPixmap::fromImage(m_imgPain[strCurrPain.toInt()]);
    int with = ui->lbPaintPic->width();
    int height = ui->lbPaintPic->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    ui->lbInfoPaintPic->setPixmap(fitpixmap);

    ui->lbInfoWeight->setText(strCurrWeight);
    ui->pbFlow->setValue(strCurrFlow.toInt());
}

void Dialog::on_caleCurr_clicked(const QDate &date)
{
    ui->edCurr->setText(ui->caleCurr->selectedDate().toString("yyyy年MM月dd日"));
}

void Dialog::on_btnNoteDown_clicked()
{
    QString strCurrDate = ui->edCurr->text();       //日期
    QString strCurrWeight = ui->edWeight->text();   //体重
    QString strCurrSpend = ui->edSpend->text();     //持续天数
    QString strCurrFlow = ui->lbFlow->text();       //流量
    //ID
    QString strID = ui->caleCurr->selectedDate().toString("yyyyMMdd");


    QString strSql = QString("insert into Holiday(ID,HDate,HWeight,HSpend,HFlow,HPain) values('%1', '%2','%3','%4','%5','%6');")
            .arg(strID).arg(strCurrDate).arg(strCurrWeight).arg(strCurrSpend).arg(strCurrFlow).arg(m_nPainLevel);
    DbCtrl::RunSql(strSql);

    InitUi();
}

void Dialog::on_btnDelete_clicked()
{
    QString strCurr = ui->edCurr->text();
    QString strSql = QString("delete from Holiday where HDate = '%1';").arg(strCurr);
    DbCtrl::RunSql(strSql);

    InitUi();
}

void Dialog::on_btnChoosePain_clicked()
{
    ++m_nPainLevel;
    if(m_nPainLevel > 5)
        m_nPainLevel = 0;

    QPixmap pixmap = QPixmap::fromImage(m_imgPain[m_nPainLevel]);
    int with = ui->lbPaintPic->width();
    int height = ui->lbPaintPic->height();
    QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    ui->lbPaintPic->setPixmap(fitpixmap);
}

void Dialog::on_btnGetFile_clicked()
{
    QString strFilePath;
    QImage image;
    strFilePath = QFileDialog::getOpenFileName(this,
                                            "Please choose an image file",
                                            "",
                                            "Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");

    if( strFilePath != "" && image.load(strFilePath) )
    {
      QPixmap pixmap = QPixmap::fromImage(image);
      int with = ui->lbInfoUser->width();
      int height = ui->lbInfoUser->height();
      QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
      ui->lbInfoUser->setPixmap(fitpixmap);
    }
    m_iniFile->setValue( "config/UserPic",  strFilePath);
}

void Dialog::on_sliderFlow_valueChanged(int value)
{
    QString strFlow = QString("%1").arg(value);
    ui->lbFlow->setText(strFlow);
}
