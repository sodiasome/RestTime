#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "LoginDlg.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;

public:
    LoginDlg    m_loginDlg;
    QImage      m_imgPain[6];       //疼痛感等级图片
    int         m_nPainLevel;       //疼痛等级
    QSettings * m_iniFile;          //保存配置
    void Init();
    void InitUi();

private slots:
    void OnTextChanged(const QString strText);
    void on_caleCurr_clicked(const QDate &date);
    void on_btnNoteDown_clicked();
    void on_btnDelete_clicked();
    void on_btnChoosePain_clicked();
    void on_btnGetFile_clicked();
    void on_sliderFlow_valueChanged(int value);
};
#endif // DIALOG_H
