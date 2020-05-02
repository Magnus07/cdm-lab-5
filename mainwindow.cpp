#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillVariables(int rows, int column, int multiplier, int variables)
{   // умова виходу з рекурс?ї
    if (variables == column)
        return;

    // ознака нулик?в ? одиничок
    bool isIt = false;
    for (int i = 1; i <= rows; i++)
    {
        QTableWidgetItem * item;
        if (!isIt)
            item = new QTableWidgetItem("0");
        else
            item = new QTableWidgetItem("1");
        if (i % (rows/(multiplier)) == 0) // ознака зм?ни нулик?в на одинички чи навпаки
            isIt = !isIt;
        // встановлюємо елемент
        ui->tableWidget->setItem(i-1, column, item);
    }
    // виклик функц?ї
    fillVariables(rows,column+1,multiplier*2, variables);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    // Конф?гурац?я таблиц? звичайної
    if (index == 0)
        return;
    int rows = 4;
    QStringList headers = {"x","y"};
    if (index == 2){rows = 8;headers.append("z");}
    if (index == 3){rows = 16;headers.append("z");headers.append("t");}

    headers.append("f");

    // очищаємо таблицю
    ui->tableWidget->clear();

    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(index + 2);

    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // заповнюємо значення зм?нних
    fillVariables(rows,0,2,index+1);

    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
}
