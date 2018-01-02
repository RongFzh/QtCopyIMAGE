#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "qlogging.h"
#include "qlistview.h"
#include "qtreeview.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar_copy->setVisible(false);
    m_strl_SrcFolders.clear();
    m_strl_SrcFolders.append(ui->lineEdit_src->text());
    m_str_DstFolder = ui->lineEdit_dst->text();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_src_clicked()
{
    // refer to : http://bbs.csdn.net/topics/390972596?page=1
    QFileDialog* dlg_src = new QFileDialog(this);
    dlg_src->setFileMode(QFileDialog::Directory);
    dlg_src->setOption(QFileDialog::DontUseNativeDialog, true);

    QListView *lv_src = dlg_src->findChild<QListView*>("listView");
    if (lv_src) {
        lv_src->setSelectionMode(QAbstractItemView::MultiSelection);
    }
    QTreeView *tv_src = dlg_src->findChild<QTreeView*>();
    if (tv_src) {
        tv_src->setSelectionMode(QAbstractItemView::MultiSelection);
    }
    dlg_src->setFilter(QDir::Dirs);

    dlg_src->exec();
    m_strl_SrcFolders = dlg_src->selectedFiles();
    if(!m_strl_SrcFolders.isEmpty())
    {
        ui->lineEdit_src->setText(m_strl_SrcFolders[0]);
    }
}

void MainWindow::on_pushButton_dst_clicked()
{
    m_str_DstFolder =
            QFileDialog::getExistingDirectory(NULL,
                                              "Choose Dst Folder",
                                              "E:\\",
                                              QFileDialog::ShowDirsOnly);
    if(!m_str_DstFolder.isEmpty())
    {
        ui->lineEdit_dst->setText(m_str_DstFolder);
    }

}

void MainWindow::on_pushButton_run_clicked()
{
    // 将有关控件disable
    ui->pushButton_src->setDisabled(true);
    ui->pushButton_dst->setDisabled(true);
    ui->pushButton_run->setDisabled(true);
    ui->checkBox_copy_static->setDisabled(true);
    ui->checkBox_copy_fluoro->setDisabled(true);
    ui->checkBox_copy_report->setDisabled(true);

    // 显示进度条
    ui->progressBar_copy->setVisible(true);
    ui->progressBar_copy->setValue(0);

    // 拷贝的种类
    m_check_cpy_static = ui->checkBox_copy_static->checkState();
    m_check_cpy_fluoro = ui->checkBox_copy_fluoro->checkState();
    m_check_cpy_report = ui->checkBox_copy_report->checkState();

    // 将空的, 名字和源文件夹相同的文件夹建在目标文件夹下.
    // 遍历每一个目标文件夹, 将相应项拷贝到目标文件夹下对应的文件夹中.
    // 使得源文件夹的目录树和目标文件夹下的目录树一模一样.
    for(int i = 0;i<m_strl_SrcFolders.length();i++)
    {
        QString str_CurrFolder = m_strl_SrcFolders[i];
        CopyFilesRecur(str_CurrFolder, m_str_DstFolder);
        ui->progressBar_copy->setValue((i+1)/m_strl_SrcFolders.length()*100);
    }

    // 拷贝完成后将有关的控件enable
    ui->pushButton_src->setEnabled(true);
    ui->pushButton_dst->setEnabled(true);
    ui->pushButton_run->setEnabled(true);
    ui->checkBox_copy_static->setEnabled(true);
    ui->checkBox_copy_fluoro->setEnabled(true);
    ui->checkBox_copy_report->setEnabled(true);

}
int MainWindow::CopyFilesRecur(QString str_src, QString str_dst)
{
    QFileInfo file_Info(str_src);
    if(file_Info.isDir())
    {
        bool b_cpy = true;
        if(str_src.endsWith("RAW")
                && m_check_cpy_static != Qt::Checked)
            b_cpy = false;
        else if(str_src.contains("RAW_")
                && m_check_cpy_fluoro != Qt::Checked)
            b_cpy = false;
        else if(str_src.endsWith("REPORT")
                && m_check_cpy_report != Qt::Checked)
            b_cpy = false;
        if(b_cpy)
        {
            // 建立文件夹
            QStringList strl_SrcSplit = str_src.split("/");
            QString str_FolderName = strl_SrcSplit[strl_SrcSplit.length()-1];
            QString str_SubDst = str_dst+"/"+str_FolderName;
            m_dir.mkdir(str_SubDst);
            QDir dir_SubSrc(str_src);
            QStringList strl_SubSrcFiles = dir_SubSrc.entryList();
            // 遍历子文件夹
            for(int i = 0; i<strl_SubSrcFiles.length(); i++)
            {
                QString str_CurFile = strl_SubSrcFiles[i];
                if(!str_CurFile.endsWith('.'))
                {
                    CopyFilesRecur(str_src+"/"+str_CurFile, str_SubDst);
                }
            }
        }
    }
    // 是文件, 直接拷贝
    else if(file_Info.isFile())
    {
        QStringList str_FileName = str_src.split("/");
        QFile::copy( str_src, str_dst + "/" + str_FileName[str_FileName.length()-1] );
    }//end of if(file_Info.isDir())

    return 0;
}

void MainWindow::on_pushButton_stop_clicked()
{
    // 将有关的控件enable
    ui->pushButton_src->setEnabled(true);
    ui->pushButton_dst->setEnabled(true);
    ui->pushButton_run->setEnabled(true);
    ui->checkBox_copy_static->setEnabled(true);
    ui->checkBox_copy_fluoro->setEnabled(true);
    ui->checkBox_copy_report->setEnabled(true);

}
