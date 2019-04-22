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
    pb_reset=ui->pb_reset;
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

    QRegExp regx_guige("([1-3]?\\*?[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9]?\\*?[1-9][0-9]?[0-9]?\\*[1-9][0-9]?\\+[1-9]?\\*?[1-9][0-9]?[0-9]?\\*([1-9][0-9]?)?)|"
                       "(4\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9]?[0-9]?\\*[1-9][0-9]?)|"
                       "(\\-[1-9][0-9][0-9]?\\*[1-9][0-9]?\\*[1-9][0-9]?)|"
                       "(\\-[1-9][0-9][0-9]?[0-9]?)|"
                       "(\\-[1-9][0-9][0-9]?[0-9]?\\*[1-9][0-9]?[0-9]?[0-9]?)");
    QValidator* vali_gg=new QRegExpValidator(regx_guige,this);
    le_guige->setValidator(vali_gg);
    le_guige->setPlaceholderText("2*100*10+2*80*8+2*60*6");

    connect(cb_tong,SIGNAL(stateChanged(int)),this,SLOT(method_cbt(int)));
    connect(cb_lv,SIGNAL(stateChanged(int)),this,SLOT(method_cbl(int)));
    connect(pb_calc,SIGNAL(clicked()),this,SLOT(method_calc()));
    connect(pb_clear,SIGNAL(clicked()),this,SLOT(method_clear()));
    connect(te_content,SIGNAL(textChanged()),this,SLOT(method_textedit_cursormove()));
    connect(pb_sumadd,SIGNAL(clicked()),this,SLOT(method_sumadd()));
    connect(pb_reset,SIGNAL(clicked()),this,SLOT(method_reset()));



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::method_clear()
{
    le_guige->clear();
    le_600->clear();
    le_800->clear();
    le_1000->clear();
    le_1200->clear();
    le_400->clear();

}

void MainWindow::method_calc()
{
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


    bool ggd_bool=s400!=0 || s600!=0 || s800!=0 || s1000!=0 || s1200!=0;

    //@@重新设计思路 用正则表达式匹配
    //@1 100*10
    QRegExp re_abc("[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@2 2*100*10
    QRegExp re_2abc("[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@3 100*10+100*10
    QRegExp re_abcn("[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@4 100*10+2*100*10
    QRegExp re_abc2n("[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@5 2*100*10+100*10
    QRegExp re_2abcn("[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@6 2*100*10+2*100*10
    QRegExp re_2abc2n("[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@7 100*10+100*10+50*5
    QRegExp re_abcnpe("[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@8 100*10+100*10+2*50*5
    QRegExp re_abcn2pe("[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@9 100*10+2*100*10+50*5
    QRegExp re_abc2npe("[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@10 100*10+2*100*10+2*50*5
    QRegExp re_abc2n2pe("[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@11 2*100*10+100*10+50*5
    QRegExp re_2abcnpe("[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@12 2*100*10+100*10+2*50*5
    QRegExp re_2abcn2pe("[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@13 2*100*10+2*100*10+50*5
    QRegExp re_2abc2npe("[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@14 2*100*10+2*100*10+2*50*5
    QRegExp re_2abc2n2pe("[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-3]\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?");

    //@15 4*100*10
    QRegExp re_4abcn("4\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@16 4*100*10+80*8
    QRegExp re_4abcnpe("4\\*[1-9][0-9][0-9]?\\*[1-9][0-9]?\\+[1-9][0-9][0-9]?\\*[1-9][0-9]?");
    //@17  pm         100*10*12  //p: 100*10 m:12 p代表排的规格，m代表数量
    QRegExp re_pm("\\-[1-9][0-9][0-9]?\\*[1-9][0-9]?\\*[1-9][0-9]?");

//    @18  price      p1000  单台柜子的价格
    QRegExp re_price("\\-[1-9][0-9][0-9]?[0-9]?");

//    @19  price*m    p800*10
    QRegExp re_pricem("\\-[1-9][0-9][0-9]?[0-9]?\\*[1-9][0-9]?[0-9]?[0-9]?");




    //排的单价
    int dj=le_dj->text().toInt();
    //排的规格
    QString txt=le_guige->text();

    QStringList sl_jiahao=txt.split("+");

    if(ggd_bool){


        //@@1  --------------------------------------------------------------------------
        if(re_abc.exactMatch(txt)){
//            qDebug()<<"reabc";

            QString pabc=sl_jiahao.at(0);

            QStringList sl_xinghao=pabc.split("*");
            int pabc_width=sl_xinghao.at(0).toInt();
            int pabc_deep=sl_xinghao.at(1).toInt();

            te_content->append(QString::number(pabc_width)+"*"+QString::number(pabc_deep));


            if(state=="t")
            {
                ui->statusBar->showMessage("ABC MODE | 单排 材质 铜");
                int T=0;
                T=pabc_width*pabc_deep*8.9*dj/1000;

                te_content->append(QString::number(T)+" T");

                QString TS="T*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";
                te_content->append(TS);

                int SUMTS=T*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));
            }

            if(state=="l")
            {
                ui->statusBar->showMessage("ABC MODE | 单排 材质 铝");
                int L=0;
                L=pabc_width*pabc_deep*2.7*dj/1000;
                te_content->append(QString::number(L)+" L");
                QString LS="L*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";
                te_content->append(LS);
                int SUMLS=L*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));

            }


        }


        //    @@2  -------------------------------------------------------
        if(re_2abc.exactMatch(txt))
        {
            QString pabc=sl_jiahao.at(0);

            QStringList sl_xinghao=pabc.split("*");
            int pabc_sl=sl_xinghao.at(0).toInt();
            int pabc_width=sl_xinghao.at(1).toInt();
            int pabc_deep=sl_xinghao.at(2).toInt();



            te_content->append(QString::number(pabc_sl)+"*"+QString::number(pabc_width)+"*"+QString::number(pabc_deep));

            if(state=="t"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABC MODE | 复排 材质 铜");
                int T=0;
                T=pabc_width*pabc_deep*8.9*dj/1000;

                te_content->append(QString::number(T)+" T");

                QString TS="T*"+QString::number(pabc_sl)+"*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";
                te_content->append(TS);
                int SUMTS=T*pabc_sl*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");

                list.append(QString::number(SUMTS));
            }

            if(state=="l"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABC MODE | 复排 材质 铝");
                int L=0;
                L=pabc_width*pabc_deep*2.7*dj/1000;
                te_content->append(QString::number(L)+" L");
                QString LS="L*"+QString::number(pabc_sl)+"*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";
                te_content->append(LS);
                int SUMLS=L*pabc_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");

                list.append(QString::number(SUMLS));

            }


        }

        //    @@3 ------------------------------------

        if(re_abcn.exactMatch(txt)){

            QString pabc=sl_jiahao.at(0);
            QString pn=sl_jiahao.at(1);

            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");

            int pabc_width=sl_xinghaoabc.at(0).toInt();
            int pabc_deep=sl_xinghaoabc.at(1).toInt();

            int pn_width=sl_xinghaon.at(0).toInt();
            int pn_deep=sl_xinghaon.at(1).toInt();

            te_content->append(QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_width)+"*"+QString::number(pn_deep));

            if(state=="t"){
                ui->statusBar->showMessage("ABCN MODE | 单排单零 材质 铜");
                int Tabc=0;
                int Tn=0;
                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn");

                QString TS="Tabc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));
            }

            if(state=="l"){
                ui->statusBar->showMessage("ABCN MODE | 单排单零 材质 铝");
                int Labc=0;
                int Ln=0;
                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln");

                QString TS="Labc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMLS=Labc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));

            }


        }



        //    @@4---------------------------------------------------

        if(re_abc2n.exactMatch(txt)){



            QString pabc=sl_jiahao.at(0);
            QString pn=sl_jiahao.at(1);

            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");

            int pabc_width=sl_xinghaoabc.at(0).toInt();
            int pabc_deep=sl_xinghaoabc.at(1).toInt();

            int pn_sl=sl_xinghaon.at(0).toInt();
            int pn_width=sl_xinghaon.at(1).toInt();
            int pn_deep=sl_xinghaon.at(2).toInt();



            te_content->append(QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_sl)+"*"+QString::number(pn_width)+"*"+QString::number(pn_deep));


            if(state=="t"){
                ui->statusBar->showMessage("ABC"+QString::number(pn_sl)+"N MODE | 单排复零 材质 铜");
                int Tabc=0;
                int Tn=0;
                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn");

                QString TS="Tabc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));
            }

            if(state=="l"){
                ui->statusBar->showMessage("ABC"+QString::number(pn_sl)+"N MODE | 单排复零 材质 铝");
                int Labc=0;
                int Ln=0;
                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln");

                QString TS="Labc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMLS=Labc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));

            }

        }



        //    @@5 ------------------------------------------------------
        if(re_2abcn.exactMatch(txt)){

            QString pabc=sl_jiahao.at(0);
            QString pn=sl_jiahao.at(1);

            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");

            int pabc_sl=sl_xinghaoabc.at(0).toInt();
            int pabc_width=sl_xinghaoabc.at(1).toInt();
            int pabc_deep=sl_xinghaoabc.at(2).toInt();


            int pn_width=sl_xinghaon.at(0).toInt();
            int pn_deep=sl_xinghaon.at(1).toInt();



            te_content->append(QString::number(pabc_sl)+"*"+QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_width)+"*"+QString::number(pn_deep));

            if(state=="t"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABCN MODE | 复排单零 材质 铜");
                int Tabc=0;
                int Tn=0;
                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn");

                QString TS="Tabc*"+QString::number(pabc_sl)+"*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*pabc_sl*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));
            }

            if(state=="l"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABCN MODE | 复排单零 材质 铝");
                int Labc=0;
                int Ln=0;
                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln");

                QString LS="Labc*"+QString::number(pabc_sl)+"*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(LS);
                int SUMLS=Labc*pabc_sl*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));

            }



        }

        //    @@6 ------------------------------------------------------

        if(re_2abc2n.exactMatch(txt)){

            QString pabc=sl_jiahao.at(0);
            QString pn=sl_jiahao.at(1);

            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");

            int pabc_sl=sl_xinghaoabc.at(0).toInt();
            int pabc_width=sl_xinghaoabc.at(1).toInt();
            int pabc_deep=sl_xinghaoabc.at(2).toInt();


            int pn_sl=sl_xinghaon.at(0).toInt();
            int pn_width=sl_xinghaon.at(1).toInt();
            int pn_deep=sl_xinghaon.at(2).toInt();


            te_content->append(QString::number(pabc_sl)+"*"+QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_sl)+"*"+QString::number(pn_width)+"*"+QString::number(pn_deep));

            if(state=="t"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABC"+QString::number(pn_sl)+"N MODE | 复排复零 材质 铜");
                int Tabc=0;
                int Tn=0;
                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn");

                QString TS="Tabc*"+QString::number(pabc_sl)+"*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*pabc_sl*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));
            }

            if(state=="l"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABC"+QString::number(pn_sl)+"N MODE | 复排复零 材质 铝");
                int Labc=0;
                int Ln=0;
                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Tabc "+QString::number(Ln)+" Tn");

                QString LS="Labc*"+QString::number(pabc_sl)+"*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(LS);
                int SUMLS=Labc*pabc_sl*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));

            }




        }


        //    @@7 ------------------------------------------------------
        if(re_abcnpe.exactMatch(txt)){

            pabc=sl_jiahao.at(0);
            pn=sl_jiahao.at(1);
            ppe=sl_jiahao.at(2);

            //以 *号为分割符，查看是排的数量  100*10
            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");
            QStringList sl_xinghaope=ppe.split("*");

            int pabc_width=sl_xinghaoabc.at(0).toInt();
            int pabc_deep=sl_xinghaoabc.at(1).toInt();

            //ABCNPE  单排单零
            int pn_width=sl_xinghaon.at(0).toInt();
            int pn_deep=sl_xinghaon.at(1).toInt();

            //ABCNPE  单排单零单地

            int ppe_width=sl_xinghaope.at(0).toInt();
            int ppe_deep=sl_xinghaope.at(1).toInt();


            te_content->append(QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_width)+"*"+QString::number(pn_deep)+"+"+QString::number(ppe_width)+"*"+QString::number(ppe_deep));

            if(state=="t"){
                ui->statusBar->showMessage("ABCNPE MODE | 单排单零单地 材质 铜");
                int Tabc=0;
                int Tn=0;
                int Tpe=0;

                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;
                Tpe=ppe_width*ppe_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn "+QString::number(Tpe)+" Tpe");

                QString TS="Tabc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Tpe*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tpe*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));


            }

            if(state=="l"){
                ui->statusBar->showMessage("ABCNPE MODE | 单排单零单地 材质 铝");
                int Labc=0;
                int Ln=0;
                int Lpe=0;

                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;
                Lpe=ppe_width*ppe_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln "+QString::number(Lpe)+" Lpe");

                QString LS="Labc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Lpe*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(LS);
                int SUMLS=Labc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Lpe*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));


            }



        }

        //    @@8 ------------------------------------------------------
        if(re_abcn2pe.exactMatch(txt)){


            pabc=sl_jiahao.at(0);
            pn=sl_jiahao.at(1);
            ppe=sl_jiahao.at(2);

            //以 *号为分割符，查看是排的数量  100*10
            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");
            QStringList sl_xinghaope=ppe.split("*");

            int pabc_width=sl_xinghaoabc.at(0).toInt();
            int pabc_deep=sl_xinghaoabc.at(1).toInt();

            //ABCNPE  单排单零
            int pn_width=sl_xinghaon.at(0).toInt();
            int pn_deep=sl_xinghaon.at(1).toInt();

            int ppe_sl=sl_xinghaope.at(0).toInt();
            int ppe_width=sl_xinghaope.at(1).toInt();
            int ppe_deep=sl_xinghaope.at(2).toInt();


            te_content->append(QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_width)+"*"+QString::number(pn_deep)+"+"+QString::number(ppe_sl)+"*"+QString::number(ppe_width)+"*"+QString::number(ppe_deep));

            if(state=="t"){
                ui->statusBar->showMessage("ABCN"+QString::number(ppe_sl)+"PE MODE | 单排单零复地 材质 铜");
                int Tabc=0;
                int Tn=0;
                int Tpe=0;

                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;
                Tpe=ppe_width*ppe_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn "+QString::number(Tpe)+" Tpe");

                QString TS="Tabc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Tpe*"+QString::number(ppe_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tpe*ppe_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));


            }

            if(state=="l"){
                ui->statusBar->showMessage("ABCN"+QString::number(ppe_sl)+"PE MODE | 单排单零复地 材质 铝");
                int Labc=0;
                int Ln=0;
                int Lpe=0;

                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;
                Lpe=ppe_width*ppe_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln "+QString::number(Lpe)+" Lpe");

                QString LS="Labc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Lpe*"+QString::number(ppe_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(LS);
                int SUMLS=Labc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Lpe*ppe_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));


            }








        }


        //    @@ 9------------------------------------------------------
        if(re_abc2npe.exactMatch(txt)){

            pabc=sl_jiahao.at(0);
            pn=sl_jiahao.at(1);
            ppe=sl_jiahao.at(2);

            //以 *号为分割符，查看是排的数量  100*10
            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");
            QStringList sl_xinghaope=ppe.split("*");

            int pabc_width=sl_xinghaoabc.at(0).toInt();
            int pabc_deep=sl_xinghaoabc.at(1).toInt();

            int pn_sl=sl_xinghaon.at(0).toInt();
            int pn_width=sl_xinghaon.at(1).toInt();
            int pn_deep=sl_xinghaon.at(2).toInt();


            //ABC2NPE  单排复零单地


            int ppe_width=sl_xinghaope.at(0).toInt();
            int ppe_deep=sl_xinghaope.at(1).toInt();


            te_content->append(QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_sl)+"*"+QString::number(pn_width)+"*"+QString::number(pn_deep)+"+"+QString::number(ppe_width)+"*"+QString::number(ppe_deep));

            if(state=="t"){
                ui->statusBar->showMessage("ABC"+QString::number(pn_sl)+"NPE MODE | 单排复零单地 材质 铜");
                int Tabc=0;
                int Tn=0;
                int Tpe=0;

                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;
                Tpe=ppe_width*ppe_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn "+QString::number(Tpe)+" Tpe");

                QString TS="Tabc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Tpe*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tpe*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));


            }

            if(state=="l"){
                ui->statusBar->showMessage("ABC"+QString::number(pn_sl)+"NPE MODE | 单排复零单地 材质 铝");
                int Labc=0;
                int Ln=0;
                int Lpe=0;

                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;
                Lpe=ppe_width*ppe_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln "+QString::number(Lpe)+" Lpe");

                QString LS="Labc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Lpe*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(LS);
                int SUMLS=Labc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Lpe*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));


            }

        }

        //    @@ 10------------------------------------------------------

        if(re_abc2n2pe.exactMatch(txt)){

            pabc=sl_jiahao.at(0);
            pn=sl_jiahao.at(1);
            ppe=sl_jiahao.at(2);

            //以 *号为分割符，查看是排的数量  100*10
            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");
            QStringList sl_xinghaope=ppe.split("*");

            int pabc_width=sl_xinghaoabc.at(0).toInt();
            int pabc_deep=sl_xinghaoabc.at(1).toInt();

            int pn_sl=sl_xinghaon.at(0).toInt();
            int pn_width=sl_xinghaon.at(1).toInt();
            int pn_deep=sl_xinghaon.at(2).toInt();


            int ppe_sl=sl_xinghaope.at(0).toInt();
            int ppe_width=sl_xinghaope.at(1).toInt();
            int ppe_deep=sl_xinghaope.at(2).toInt();


            te_content->append(QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_sl)+"*"+QString::number(pn_width)+"*"+QString::number(pn_deep)+"+"+QString::number(ppe_sl)+"*"+QString::number(ppe_width)+"*"+QString::number(ppe_deep));

            if(state=="t"){
                ui->statusBar->showMessage("ABC"+QString::number(pn_sl)+"N"+QString::number(ppe_sl)+"PE MODE | 单排复零复地 材质 铜");
                int Tabc=0;
                int Tn=0;
                int Tpe=0;

                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;
                Tpe=ppe_width*ppe_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn "+QString::number(Tpe)+" Tpe");

                QString TS="Tabc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Tpe*"+QString::number(ppe_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tpe*ppe_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));


            }

            if(state=="l"){
                ui->statusBar->showMessage("ABC"+QString::number(pn_sl)+"N"+QString::number(ppe_sl)+"PE MODE | 单排复零复地 材质 铝");
                int Labc=0;
                int Ln=0;
                int Lpe=0;

                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;
                Lpe=ppe_width*ppe_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln "+QString::number(Lpe)+" Lpe");

                QString LS="Labc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Lpe*"+QString::number(ppe_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(LS);
                int SUMLS=Labc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Lpe*ppe_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));


            }



        }

        //    @@ 11------------------------------------------------------


        if(re_2abcnpe.exactMatch(txt)){



            pabc=sl_jiahao.at(0);
            pn=sl_jiahao.at(1);
            ppe=sl_jiahao.at(2);

            //以 *号为分割符，查看是排的数量  100*10
            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");
            QStringList sl_xinghaope=ppe.split("*");

            int pabc_sl=sl_xinghaoabc.at(0).toInt();
            int pabc_width=sl_xinghaoabc.at(1).toInt();
            int pabc_deep=sl_xinghaoabc.at(2).toInt();



            int pn_width=sl_xinghaon.at(0).toInt();
            int pn_deep=sl_xinghaon.at(1).toInt();


            int ppe_width=sl_xinghaope.at(0).toInt();
            int ppe_deep=sl_xinghaope.at(1).toInt();


            te_content->append(QString::number(pabc_sl)+"*"+QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_width)+"*"+QString::number(pn_deep)+"+"+QString::number(ppe_width)+"*"+QString::number(ppe_deep));

            if(state=="t"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABCNPE MODE | 复排单零单地 材质 铜");
                int Tabc=0;
                int Tn=0;
                int Tpe=0;

                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;
                Tpe=ppe_width*ppe_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn "+QString::number(Tpe)+" Tpe");

                QString TS="Tabc*3*"+QString::number(pabc_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Tpe*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*pabc_sl*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tpe*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));


            }

            if(state=="l"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABCNPE MODE | 复排单零单地 材质 铝");
                int Labc=0;
                int Ln=0;
                int Lpe=0;

                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;
                Lpe=ppe_width*ppe_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln "+QString::number(Lpe)+" Lpe");

                QString LS="Labc*3*"+QString::number(pabc_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Lpe*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(LS);
                int SUMLS=Labc*3*pabc_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Lpe*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));


            }


        }




        //    @@ 12------------------------------------------------------

        if(re_2abcn2pe.exactMatch(txt)){


            pabc=sl_jiahao.at(0);
            pn=sl_jiahao.at(1);
            ppe=sl_jiahao.at(2);

            //以 *号为分割符，查看是排的数量  100*10
            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");
            QStringList sl_xinghaope=ppe.split("*");

            int pabc_sl=sl_xinghaoabc.at(0).toInt();
            int pabc_width=sl_xinghaoabc.at(1).toInt();
            int pabc_deep=sl_xinghaoabc.at(2).toInt();

            int pn_width=sl_xinghaon.at(0).toInt();
            int pn_deep=sl_xinghaon.at(1).toInt();




            int ppe_sl=sl_xinghaope.at(0).toInt();
            int ppe_width=sl_xinghaope.at(1).toInt();
            int ppe_deep=sl_xinghaope.at(2).toInt();


            te_content->append(QString::number(pabc_sl)+"*"+QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_width)+"*"+QString::number(pn_deep)+"+"+QString::number(ppe_sl)+"*"+QString::number(ppe_width)+"*"+QString::number(ppe_deep));

            if(state=="t"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABCN"+QString::number(ppe_sl)+"PE MODE | 复排单零复地 材质 铜");
                int Tabc=0;
                int Tn=0;
                int Tpe=0;

                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;
                Tpe=ppe_width*ppe_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn "+QString::number(Tpe)+" Tpe");

                QString TS="Tabc*3*"+QString::number(pabc_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Tpe*"+QString::number(ppe_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*pabc_sl*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tpe*ppe_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));


            }

            if(state=="l"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABCN"+QString::number(ppe_sl)+"PE MODE | 复排单零复地 材质 铝");
                int Labc=0;
                int Ln=0;
                int Lpe=0;

                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;
                Lpe=ppe_width*ppe_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln "+QString::number(Lpe)+" Lpe");

                QString LS="Labc*3*"+QString::number(pabc_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Lpe*"+QString::number(ppe_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(LS);
                int SUMLS=Labc*3*pabc_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Lpe*ppe_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));


            }




        }



        //    @@ 13------------------------------------------------------

        if(re_2abc2npe.exactMatch(txt)){
            pabc=sl_jiahao.at(0);
            pn=sl_jiahao.at(1);
            ppe=sl_jiahao.at(2);

            //以 *号为分割符，查看是排的数量  100*10
            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");
            QStringList sl_xinghaope=ppe.split("*");

            int pabc_sl=sl_xinghaoabc.at(0).toInt();
            int pabc_width=sl_xinghaoabc.at(1).toInt();
            int pabc_deep=sl_xinghaoabc.at(2).toInt();


            int pn_sl=sl_xinghaon.at(0).toInt();
            int pn_width=sl_xinghaon.at(1).toInt();
            int pn_deep=sl_xinghaon.at(2).toInt();





            //复零单地模式


            int ppe_width=sl_xinghaope.at(0).toInt();
            int ppe_deep=sl_xinghaope.at(1).toInt();


            te_content->append(QString::number(pabc_sl)+"*"+QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_sl)+"*"+QString::number(pn_width)+"*"+QString::number(pn_deep)+"+"+QString::number(ppe_width)+"*"+QString::number(ppe_deep));

            if(state=="t"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABC"+QString(pn_sl)+"NPE MODE | 复排复零单地 材质 铜");
                int Tabc=0;
                int Tn=0;
                int Tpe=0;

                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;
                Tpe=ppe_width*ppe_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn "+QString::number(Tpe)+" Tpe");

                QString TS="Tabc*3*"+QString::number(pabc_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Tpe*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*pabc_sl*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tpe*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));


            }

            if(state=="l"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABC"+QString(pn_sl)+"NPE MODE | 复排复零单地 材质 铝");
                int Labc=0;
                int Ln=0;
                int Lpe=0;

                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;
                Lpe=ppe_width*ppe_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln "+QString::number(Lpe)+" Lpe");

                QString LS="Labc*3*"+QString::number(pabc_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Lpe*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(LS);
                int SUMLS=Labc*3*pabc_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Lpe*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));


            }



        }


        //    @@ 14------------------------------------------------------

        if(re_2abc2n2pe.exactMatch(txt)){
            pabc=sl_jiahao.at(0);
            pn=sl_jiahao.at(1);
            ppe=sl_jiahao.at(2);

            //以 *号为分割符，查看是排的数量  100*10
            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");
            QStringList sl_xinghaope=ppe.split("*");

            int pabc_sl=sl_xinghaoabc.at(0).toInt();
            int pabc_width=sl_xinghaoabc.at(1).toInt();
            int pabc_deep=sl_xinghaoabc.at(2).toInt();


            int pn_sl=sl_xinghaon.at(0).toInt();
            int pn_width=sl_xinghaon.at(1).toInt();
            int pn_deep=sl_xinghaon.at(2).toInt();


            int ppe_sl=sl_xinghaope.at(0).toInt();
            int ppe_width=sl_xinghaope.at(1).toInt();
            int ppe_deep=sl_xinghaope.at(2).toInt();


            te_content->append(QString::number(pabc_sl)+"*"+QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(pn_sl)+"*"+QString::number(pn_width)+"*"+QString::number(pn_deep)+"+"+QString::number(ppe_sl)+"*"+QString::number(ppe_width)+"*"+QString::number(ppe_deep));

            if(state=="t"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABC"+QString::number(pn_sl)+"N"+QString::number(ppe_sl)+"PE MODE | 复排复零复地 材质 铜");
                int Tabc=0;
                int Tn=0;
                int Tpe=0;

                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;
                Tpe=ppe_width*ppe_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn "+QString::number(Tpe)+" Tpe");

                QString TS="Tabc*3*"+QString::number(pabc_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Tpe*"+QString::number(ppe_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*pabc_sl*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tpe*ppe_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));


            }

            if(state=="l"){
                ui->statusBar->showMessage(QString::number(pabc_sl)+"ABC"+QString::number(pn_sl)+"N"+QString::number(ppe_sl)+"PE MODE | 复排复零复地 材质 铝");
                int Labc=0;
                int Ln=0;
                int Lpe=0;

                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;
                Lpe=ppe_width*ppe_deep*2.7*dj/1000;

                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln "+QString::number(Lpe)+" Lpe");

                QString LS="Labc*3*"+QString::number(pabc_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*"+QString::number(pn_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Lpe*"+QString::number(ppe_sl)+"*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(LS);
                int SUMLS=Labc*3*pabc_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*pn_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Lpe*ppe_sl*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));


            }



        }

        //    @@ 15------------------------------------------------------
        //4*100*10
        if(re_4abcn.exactMatch(txt)){

            pabc=sl_jiahao.at(0);

            //    qDebug()<<pabc;



            //以 *号为分割符，查看是排的数量  100*10
            QStringList sl_xinghaoabc=pabc.split("*");


            int pabc_width=sl_xinghaoabc.at(1).toInt();
            int pabc_deep=sl_xinghaoabc.at(2).toInt();


            int pn_width=sl_xinghaoabc.at(1).toInt();
            int pn_deep=sl_xinghaoabc.at(2).toInt();



            te_content->append("4*"+QString::number(pabc_width)+"*"+QString::number(pabc_deep));

            if(state=="t"){
                ui->statusBar->showMessage("4ABCN MODE | 单排单零 材质 铜");
                int Tabc=0;
                int Tn=0;


                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;


                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn");

                QString TS="Tabc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")";

                te_content->append(TS);
                int SUMTS=Tabc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));


            }

            if(state=="l"){
                ui->statusBar->showMessage("4ABCN MODE | 单排单零 材质 铝");
                int Labc=0;
                int Ln=0;


                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;


                te_content->append(QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln");

                QString LS="Labc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")";

                te_content->append(LS);
                int SUMLS=Labc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));


            }


        }


        //    @@ 16------------------------------------------------------
        //abcnpe 4*100*10+80*8
        if(re_4abcnpe.exactMatch(txt)){

            pabc=sl_jiahao.at(0);
            pn=sl_jiahao.at(0);
            ppe=sl_jiahao.at(1);


            //以 *号为分割符，查看是排的数量  100*10
            QStringList sl_xinghaoabc=pabc.split("*");
            QStringList sl_xinghaon=pn.split("*");
            QStringList sl_xinghaope=ppe.split("*");

            int pabc_width=sl_xinghaoabc.at(1).toInt();
            int pabc_deep=sl_xinghaoabc.at(2).toInt();



            int pn_width=sl_xinghaon.at(1).toInt();
            int pn_deep=sl_xinghaon.at(2).toInt();


            int ppe_width=sl_xinghaope.at(0).toInt();
            int ppe_deep=sl_xinghaope.at(1).toInt();


            te_content->append("4*"+QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"+"+QString::number(ppe_width)+"*"+QString::number(ppe_deep));

            if(state=="t"){
                ui->statusBar->showMessage("4ABCNPE MODE | 单排单零单地 材质 铜");
                int Tabc=0;
                int Tn=0;
                int Tpe=0;

                Tabc=pabc_width*pabc_deep*8.9*dj/1000;
                Tn=pn_width*pn_deep*8.9*dj/1000;
                Tpe=ppe_width*ppe_deep*8.9*dj/1000;

                te_content->append(QString::number(Tabc)+" Tabc "+QString::number(Tn)+" Tn "+QString::number(Tpe)+" Tpe");

                QString TS="Tabc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Tn*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Tpe*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(TS);
                int SUMTS=Tabc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tn*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Tpe*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));


            }

            if(state=="l"){
                ui->statusBar->showMessage("4ABCNPE MODE | 单排单零单地 材质 铝");
                int Labc=0;
                int Ln=0;
                int Lpe=0;

                Labc=pabc_width*pabc_deep*2.7*dj/1000;
                Ln=pn_width*pn_deep*2.7*dj/1000;
                Lpe=ppe_width*ppe_deep*2.7*dj/1000;

                te_content->append("4*"+QString::number(Labc)+" Labc "+QString::number(Ln)+" Ln "+QString::number(Lpe)+" Lpe");

                QString LS="Labc*3*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+

                        ")+Ln*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+
                        ")+Lpe*(0.6*"+QString::number(s600)+"+0.8*"+QString::number(s800)+"+1*"+QString::number(s1000)+"+1.2*"+QString::number(s1200)+"+0.4*"+QString::number(s400)+")";

                te_content->append(LS);
                int SUMLS=Labc*3*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Ln*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200)+Lpe*(0.4*s400+0.6*s600+0.8*s800+1*s1000+1.2*s1200);
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));


            }



        }

 }//ggd_bool end
        //@  17------------------------------------------------------------
        //@17  pm  abc mode        p100*10*12  //p: 100*10 m:12 p代表排的规格，m代表数量
        if(re_pm.exactMatch(txt)){

            QString psabc=txt.mid(1,-1);
            qDebug()<<psabc;

            QStringList sl_xinghao=psabc.split("*");
            int pabc_width=sl_xinghao.at(0).toInt();
            int pabc_deep=sl_xinghao.at(1).toInt();
            int pabc_ms=sl_xinghao.at(2).toInt();

            te_content->append("- "+QString::number(pabc_width)+"*"+QString::number(pabc_deep)+"*"+QString::number(pabc_ms));


            if(state=="t")
            {
                ui->statusBar->showMessage("- ABC MODE | 分支排 材质 铜");
                int T=0;
                T=pabc_width*pabc_deep*8.9*dj/1000;

                te_content->append(QString::number(T)+" T");

                QString TS="T*"+QString::number(pabc_ms);
                te_content->append(TS);

                int SUMTS=T*pabc_ms;
                te_content->append("= "+QString::number(SUMTS)+"\n");
                list.append(QString::number(SUMTS));
            }

            if(state=="l")
            {
                ui->statusBar->showMessage("- ABC MODE | 分支排  材质 铝");
                int L=0;
                L=pabc_width*pabc_deep*2.7*dj/1000;
                te_content->append(QString::number(L)+" L");
                QString LS="L*"+QString::number(pabc_ms);
                te_content->append(LS);
                int SUMLS=L*pabc_ms;
                te_content->append("= "+QString::number(SUMLS)+"\n");
                list.append(QString::number(SUMLS));

            }

    }

        //    @18  price      p1000  单台柜子的价格
//            QRegExp re_price("\\-[1-9][0-9][0-9]?[0-9]?");
        if(re_price.exactMatch(txt)){

            QString price=txt.mid(1,-1);

            te_content->append("- "+price);
            ui->statusBar->showMessage("- PRICE MODE | 分支排");
            te_content->append("= "+price+"\n");
            list.append(price);



        }

        //    @19  price*m    p800*10
//            QRegExp re_pricem("\\-[1-9][0-9][0-9]?[0-9]?\\*[1-9][0-9]?[0-9]?[0-9]?");
        if(re_pricem.exactMatch(txt)){
            QString pr=txt.mid(1,-1);
            int p=pr.split("*").at(0).toInt();
            int m=pr.split("*").at(1).toInt();

            te_content->append("- "+QString::number(p)+" * "+QString::number(m));
            int SUMP=p*m;
            ui->statusBar->showMessage("- PRICE MODE | 分支排");
            te_content->append("= "+QString::number(SUMP)+"\n");
            list.append(QString::number(SUMP));

        }





}//end



void MainWindow::method_cbt(int s)
{
    if(s==Qt::Checked){
        cb_lv->setChecked(false);
        cb_tong->setChecked(true);
        state="t";
        le_dj->setText("60");

    }
    //    qDebug()<<"state "<<state;

}

void MainWindow::method_cbl(int s)
{
    if(s==Qt::Checked){
        cb_tong->setChecked(false);
        cb_lv->setChecked(true);
        state="l";
        le_dj->setText("23");
    }

    //    qDebug()<<"state "<<state;

}

void MainWindow::method_textedit_cursormove()
{
    te_content->moveCursor(QTextCursor::End);
}

void MainWindow::method_sumadd()
{

    int len=list.length();
    if(len>0){
        int sum=0;
        for (int i=0;i<len;i++){
            sum=sum+list.at(i).toInt();
        }
        te_content->append("sum ( "+list.join(",")+" )");
        te_content->append("sum ==========>> "+QString::number(sum)+"\n");
    }
}

void MainWindow::method_reset()
{
    te_content->clear();
    list.clear();
    method_clear();
}
