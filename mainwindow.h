#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_tableWidget_cellChanged(int row, int column);

private:
    Ui::MainWindow *ui;

    void fillVariables(int rows, int column, int multiplier, int variables);

    void getResult(int variables, int rows, int columns);

    QString SDNF_SKNF(QString SDNF);

    QString isLinear();

    void formOutput();

    QString selfDuality();

    QString Zhehalkin();
};
#endif // MAINWINDOW_H
