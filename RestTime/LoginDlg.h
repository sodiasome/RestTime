#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QWidget
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = nullptr);
    ~LoginDlg();
    void Init();
    void InitUi();
    QSettings *m_config;

signals:
    void sigLogin();
    void sigClose();

private slots:
    void on_btn_Ok_clicked();
    void on_btn_Reset_clicked();
    void on_cb_RememberPasswd_stateChanged(int arg1);

    void on_cb_RememberName_stateChanged(int arg1);

private:
    Ui::LoginDlg *ui;
};

#endif // LOGINDLG_H
