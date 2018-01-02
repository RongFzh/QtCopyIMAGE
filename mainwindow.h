#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_src_clicked();

    void on_pushButton_dst_clicked();

    void on_pushButton_run_clicked();

    void on_pushButton_stop_clicked();

private:
    int CopyFilesRecur(QString str_src, QString str_dst);
    Ui::MainWindow *ui;
    QStringList m_strl_SrcFolders;
    QString     m_str_DstFolder;
    Qt::CheckState m_check_cpy_static;
    Qt::CheckState m_check_cpy_fluoro;
    Qt::CheckState m_check_cpy_report;

    QDir m_dir;
};

#endif // MAINWINDOW_H
