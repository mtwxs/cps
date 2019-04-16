#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include<QDebug>
#include <QTextEdit>
#include <QCheckBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QPushButton* pb_calc;
    QPushButton* pb_clear;
    QLineEdit* le_guige;
    QTextEdit* te_content;
    QLineEdit* le_dj;
    QCheckBox* cb_tong;
    QCheckBox* cb_lv;
    QString state;
    QLineEdit* le_400;
    QLineEdit* le_600;
    QLineEdit* le_800;
    QLineEdit* le_1000;
    QLineEdit* le_1200;

    QString pnum;
    QString pabc;
    QString pn;
    QString ppe;
    QString pabc_width;
    QString pabc_deep;
    QString pn_width;
    QString pn_deep;
    QString ppe_width;
    QString ppe_deep;

private slots:
    void method_clear();
    void method_calc();
    void method_cbt(int);
    void method_cbl(int);

};

#endif // MAINWINDOW_H
