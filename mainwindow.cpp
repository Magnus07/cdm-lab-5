#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

using namespace std;


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


// функц?я для пошуку СДНФ СКНФ
// Приймає одиницю або нуль. 1 - СДНФ, 0 - СКНФ
QString MainWindow::SDNF_SKNF(QString SDNF)
{
    QString output = "F = ";
    // проходимо по кожному рядку
    for (int i = 0; i < ui->tableWidget->rowCount();i++)
    {   // якщо це значення, яке ми розглядаємо
        if (ui->tableWidget->item(i,ui->tableWidget->columnCount()-1)->text() == SDNF)
        {   // якщо перед цим завершився вираз
            if (output[output.size()-1] == ")")
                output.append(SDNF == "1" ? "&" : "*");
            // в?дкриваємо дужки для нового виразу
            output.append("(");
            // проходимо по кожн?й колонц?
            for (int j =0; j < ui->tableWidget->columnCount()-1;j++)
            {   // беремо у дужки зм?нну
                output.append("(");
                // визначаємо конкретно ту зм?нну, з якою працюємо
                char sym = 'a' + j;
                // додаємо значення до вих?дної зм?нної
                output.append(QString(QChar::fromLatin1(sym)));
                // якщо необх?дно подавати зм?нну ?з запереченням
                if (ui->tableWidget->item(i,j)->text() != SDNF)
                    output.append(QString(QChar::fromLatin1('\'')));
                output.append(")");
                // додаємо необх?дний знак м?ж зм?нними
                if (j !=  ui->tableWidget->columnCount()-2 )
                    output.append(SDNF == "1"? "*" : "&");
            } // закриваємо вираз
            output.append(")");
        }
    }
    return output;
}


// перев?рка на монотонн?сть
QString MainWindow::isLinear()
{
    // передивляємося кожен рядок
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {   // кожен наступний рядок
        for (int j = i+1; j < ui->tableWidget->rowCount(); j++)
        {   // якщо значення рядка "а" б?льше значення рядка "б", повертаємо false(вона не монотонна)
            if (ui->tableWidget->item(i,ui->tableWidget->columnCount() - 1)->text().toInt() > ui->tableWidget->item(j,ui->tableWidget->columnCount() - 1)->text().toInt())
                return "false";
        }
    }
    return "true";
}


// перев?рка на самодвоїст?сть
QString MainWindow::selfDuality()
{   // робоча зм?нна
    QString set;
    // передивляєимося кожен рядок, ? одразу формуємо f*
    for (int i = 0; i < ui->tableWidget->rowCount();i++)
        set.push_front(ui->tableWidget->item(i,ui->tableWidget->columnCount()-1)->text() == "1" ? "0" : "1");
    // передивляємося кожен рядок
    for (int i = 0; i < ui->tableWidget->rowCount();i++)
    {   // якщо хоча б одна зм?нна не сп?впадає, повертаємо false
        if (ui->tableWidget->item(i,ui->tableWidget->columnCount()-1)->text() != set[i])
            return "false";
    }
    return "true";
}


QString MainWindow::Zhehalkin()
{   // оголошуємо зм?нну результату, отримуємо СДНФ
    QString SDNF = SDNF_SKNF("1");
    SDNF.replace("&", "+"); // зам?няємо кон'юкц?ї додаванням за mod 2

    SDNF.replace("'", "+1"); // зам?нюємо заперечення

    return SDNF;
}


// функц?я, яка формує результат
void MainWindow::formOutput()
{
    QString sdnf = "PDNF: " + SDNF_SKNF("1") + "\n";
    QString sknf = "PCNF: " + SDNF_SKNF("0") + "\n";
    QString Linear = "Is Monotone: " + isLinear() + "\n";
    QString zero = ui->tableWidget->item(0,ui->tableWidget->columnCount()-1)->text() == "0" ? "true\n" : "false\n";
    QString SavesZero = "Saves zero: " + zero;
    QString one = ui->tableWidget->item(ui->tableWidget->rowCount()-1,ui->tableWidget->columnCount()-1)->text() == "1" ? "true\n" : "false\n";
    QString SavesOne = "Saves one: " + one;
    QString selfDual = "Self duality: " + selfDuality() + "\n";
    QString ZhehalkinP = "Zhehalkin: " + Zhehalkin();
    ui->textEdit->setText(sdnf + sknf + Linear + SavesZero + SavesOne + selfDual + ZhehalkinP);
}


// функц?я, яка викликається при зм?нн? значень таблиц?
// перев?ряється, чи немає пустих значень, якщо н? - формуємо вив?д
void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++)
        {
            QTableWidgetItem * item = ui->tableWidget->item(i,j);
            if (item == NULL)
                return;
        }
    }
    formOutput();
}
