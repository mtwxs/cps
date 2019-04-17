#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("cps 计算排 abc-n-pe");
    le_guige=ui->le_pai_guige;
    ui->le_p_danjai->setText("60");

    pb_calc=ui->pb_calc;
    pb_clear=ui->pb_clear;
    pb_sumadd=ui->pb_sumadd;
    le_dj=ui->le_p_danjai;

    cb_tong=ui->cb_tong;
    cb_lv=ui->cb_lv;
    cb_tong->setChecked(true);
    cb_lv->setChecked(false);
    state="t";
     te_content=ui->te_content;
     le_400=ui->le_400;
     le_600=ui->le_600;
     le_800=ui->le_800;
     le_1000=ui->le_1000;
     le_1200=ui->le_1200;

     le_400->setPlaceholderText("0");
     le_600->setPlaceholderText("0");
     le_800->setPlaceholderText("0");
     le_1000->setPlaceholderText("0");
     le_1200->setPlaceholderText("0");

     QRegExp regx("[1-9][0-9]{0,4}");
     QValidator *vali=new QRegExpValidator(regx,this);
     le_800->setValidator(vali);
     le_600->setValidator(vali);
     le_400->setValidator(vali);
     le_1000->setValidator(vali);
     le_1200->setValidator(vali);
     le_dj->setValidator(vali);

     QRegExp regx_guige("[1-3]?\\*?[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9]?\\*?[1-9][0-9]?[0-9]?\\*[1-9][0-9]?\\+[1-9]?\\*?[1-9][0-9]?[0-9]?\\*[1-9][0-9]?");
     QValidator* vali_gg=new QRegExpValidator(regx_guige,this);
     le_guige->setValidator(vali_gg);
     le_guige->setPlaceholderText("2*100*10+2*80*8+2*60*6 排数*排宽*排厚+排数*排宽*排厚+排数*排宽*排厚 ");

    connect(cb_tong,SIGNAL(stateChanged(int)),this,SLOT(method_cbt(int)));
    connect(cb_lv,SIGNAL(stateChanged(int)),this,SLOT(method_cbl(int)));
    connect(pb_calc,SIGNAL(clicked()),this,SLOT(method_calc()));
    connect(pb_clear,SIGNAL(clicked()),this,SLOT(method_clear()));
    connect(te_content,SIGNAL(textChanged()),this,SLOT(method_textedit_cursormove()));
    connect(pb_sumadd,SIGNAL(clicked()),this,SLOT(method_sumadd()));



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::method_clear()
{
   le_guige->clear();

}

void MainWindow::method_calc()
{
    //qDebug()<<le_guige->text()<<"ll";
    QString djs=le_dj->text();
    int dj=djs.toInt();
    QString txt=le_guige->text();
    QStringList sl_jiahao=txt.split("+");



    //配电柜的数量
    int s400=0;
    int s600=0;
    int s800=0;
    int s1000=0;
    int s1200=0;

    if(le_400->text()==""){
        s400=0;
    }else{
        s400=le_400->text().toInt();
    }

    if(le_600->text()==""){
        s600=0;

    }else{
        s600=le_600->text().toInt();
    }

    if(le_800->text()==""){
        s800=0;

    }else{
        s800=le_800->text().toInt();
    }

    if(le_1000->text()==""){
        s1000=0;

    }else{
        s1000=le_1000->text().toInt();
    }

    if(le_1200->text()==""){
        s1200=0;

    }else{
        s1200=le_1200->text().toInt();
    }

//判断有几个加号，同时也需要判断柜子的数量不能为0，如果为0则没有什么意义。

    if(sl_jiahao.length()>0 && (s400!=0 || s600!=0 || s800!=0 || s1000!=0 || s1200!=0)){
        //只有A B C三项时的情况
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        if(sl_jiahao.length()==1){
            pabc=sl_jiahao.at(0);
            //以 *号为分割符，查看是排的数量  100*10
            QStringList sl_xinghao=pabc.split("*");
           // qDebug()<<sls.length()<<sls.at(0);

            //单排模式
            if(sl_xinghao.length()==2){
                pabc_width=sl_xinghao.at(0);
                pabc_deep=sl_xinghao.at(1);
                te_content->append(pabc_width+"*"+pabc_deep);

                int piabc_width=pabc_width.toInt();
                int piabc_deep=pabc_deep.toInt();


                if(state=="t"){
                    int T=0;
                    T=piabc_width*piabc_deep*8.9*dj/1000;

                    te_content->append(QString::number(T)+" T");

                    QString TS="T*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                    te_content->append(TS);
                    int SUMTS=T*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                    te_content->append("= "+QString::number(SUMTS)+"\n");
                    list.append(QString::number(SUMTS));
                }

                if(state=="l"){
                    int L=0;
                    L=piabc_width*piabc_deep*2.7*dj/1000;
                    te_content->append(QString::number(L)+" L");
                    QString LS="L*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";
                    te_content->append(LS);
                    int SUMLS=L*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                    te_content->append("= "+QString::number(SUMLS)+"\n");
                    list.append(QString::number(SUMLS));

                }
            }

            //双排以上 2*100*10 3*100*10
            if(sl_xinghao.length()==3){
                 qDebug()<<"test";
                int pai_shu=sl_xinghao.at(0).toInt();
                int pai_width=sl_xinghao.at(1).toInt();
                int pai_deep=sl_xinghao.at(2).toInt();
                qDebug()<<pai_shu <<" "<<pai_width<<"88 "<<pai_deep;
                te_content->append(QString::number(pai_shu)+"*"+QString::number(pai_width)+"*"+QString::number(pai_deep));

                if(state=="t"){
                    int T=0;
                    T=pai_width*pai_deep*8.9*dj/1000;

                te_content->append(QString::number(T)+" T");

                QString TS="T*"+QString::number(pai_shu)+"*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";
                te_content->append(TS);
                int SUMTS=T*pai_shu*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");

                list.append(QString::number(SUMTS));
                }

                if(state=="l"){
                    int L=0;
                    L=pai_width*pai_deep*2.7*dj/1000;
                    te_content->append(QString::number(L)+" L");
                    QString LS="L*"+QString::number(pai_shu)+"*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";
                    te_content->append(LS);
                    int SUMLS=L*pai_shu*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                    te_content->append("= "+QString::number(SUMLS)+"\n");

                    list.append(QString::number(SUMLS));

                }


            }
            qDebug()<<list;
//////////////////////////////////////////////////////////////////////////////////////////////////////


        }
        //A B C N
        if(sl_jiahao.length()==2){





        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        //A B C N PE
        if(sl_jiahao.length()==3){

        }
    }


}

void MainWindow::method_cbt(int s)
{
    if(s==Qt::Checked){
        cb_lv->setChecked(false);
        cb_tong->setChecked(true);
        state="t";

    }
    qDebug()<<"state "<<state;

}

void MainWindow::method_cbl(int s)
{
    if(s==Qt::Checked){
        cb_tong->setChecked(false);
        cb_lv->setChecked(true);
        state="l";
    }

    qDebug()<<"state "<<state;

}

void MainWindow::method_textedit_cursormove()
{
    te_content->moveCursor(QTextCursor::End);
}

void MainWindow::method_sumadd()
{

    int len=list.length();
    int sum=0;
    for (int i=0;i<len;i++){
        sum=sum+list.at(i).toInt();
    }
   te_content->append("sum( "+list.join(",")+" )");
   te_content->append("sum ==========>> "+QString::number(sum));
}
