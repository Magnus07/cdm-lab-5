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
{   // 㬮�� ��室� � ४���?�
    if (variables == column)
        return;

    // ������ �㫨�?� ? �����箪
    bool isIt = false;
    for (int i = 1; i <= rows; i++)
    {
        QTableWidgetItem * item;
        if (!isIt)
            item = new QTableWidgetItem("0");
        else
            item = new QTableWidgetItem("1");
        if (i % (rows/(multiplier)) == 0) // ������ ��?�� �㫨�?� �� �����窨 � �������
            isIt = !isIt;
        // ��⠭����� �������
        ui->tableWidget->setItem(i-1, column, item);
    }
    // ������ �㭪�?�
    fillVariables(rows,column+1,multiplier*2, variables);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    // ����?����?� ⠡���? ���砩���
    if (index == 0)
        return;
    int rows = 4;
    QStringList headers = {"x","y"};
    if (index == 2){rows = 8;headers.append("z");}
    if (index == 3){rows = 16;headers.append("z");headers.append("t");}

    headers.append("f");

    // ���� ⠡����
    ui->tableWidget->clear();

    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(index + 2);

    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // �������� ���祭�� ��?����
    fillVariables(rows,0,2,index+1);

    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
}


// �㭪�?� ��� ����� ���� ����
// �਩��� ������� ��� ���. 1 - ����, 0 - ����
QString MainWindow::SDNF_SKNF(QString SDNF)
{
    QString output = "F = ";
    // ��室��� �� ������� �浪�
    for (int i = 0; i < ui->tableWidget->rowCount();i++)
    {   // �� � ���祭��, 瘟 �� ஧��鸞�
        if (ui->tableWidget->item(i,ui->tableWidget->columnCount()-1)->text() == SDNF)
        {   // �� ��। 樬 �����訢�� ��ࠧ
            if (output[output.size()-1] == ")")
                output.append(SDNF == "1" ? "&" : "*");
            // �?��ਢ�� �㦪� ��� ������ ��ࠧ�
            output.append("(");
            // ��室��� �� ����?� ������?
            for (int j =0; j < ui->tableWidget->columnCount()-1;j++)
            {   // ��६� � �㦪� ��?���
                output.append("(");
                // ������� �����⭮ �� ��?���, � 类� �����
                char sym = 'a' + j;
                // ����� ���祭�� �� ���?���� ��?����
                output.append(QString(QChar::fromLatin1(sym)));
                // �� �����?��� ������� ��?��� ?� �����祭��
                if (ui->tableWidget->item(i,j)->text() != SDNF)
                    output.append(QString(QChar::fromLatin1('\'')));
                output.append(")");
                // ����� �����?���� ���� �?� ��?�����
                if (j !=  ui->tableWidget->columnCount()-2 )
                    output.append(SDNF == "1"? "*" : "&");
            } // ���ਢ�� ��ࠧ
            output.append(")");
        }
    }
    return output;
}


// ��ॢ?ઠ �� ����⮭�?���
QString MainWindow::isLinear()
{
    // ��।������� ����� �冷�
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {   // ����� ����㯭�� �冷�
        for (int j = i+1; j < ui->tableWidget->rowCount(); j++)
        {   // �� ���祭�� �浪� "�" �?��� ���祭�� �浪� "�", ������� false(���� �� ����⮭��)
            if (ui->tableWidget->item(i,ui->tableWidget->columnCount() - 1)->text().toInt() > ui->tableWidget->item(j,ui->tableWidget->columnCount() - 1)->text().toInt())
                return "false";
        }
    }
    return "true";
}


// ��ॢ?ઠ �� ᠬ�������?���
QString MainWindow::selfDuality()
{   // ஡�� ��?���
    QString set;
    // ��।����󨬮�� ����� �冷�, ? ��ࠧ� ���� f*
    for (int i = 0; i < ui->tableWidget->rowCount();i++)
        set.push_front(ui->tableWidget->item(i,ui->tableWidget->columnCount()-1)->text() == "1" ? "0" : "1");
    // ��।������� ����� �冷�
    for (int i = 0; i < ui->tableWidget->rowCount();i++)
    {   // �� �� � ���� ��?��� �� �?������, ������� false
        if (ui->tableWidget->item(i,ui->tableWidget->columnCount()-1)->text() != set[i])
            return "false";
    }
    return "true";
}


QString MainWindow::Zhehalkin()
{   // �������� ��?��� १�����, ��ਬ�� ����
    QString SDNF = SDNF_SKNF("1");
    SDNF.replace("&", "+"); // ���?��� ���'��?� ��������� �� mod 2

    SDNF.replace("'", "+1"); // ���?��� �����祭��

    return SDNF;
}


// �㭪�?�, 猪 ���� १����
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


// �㭪�?�, 猪 ������������ �� ��?��? ���祭� ⠡���?
// ��ॢ?�������, � ����� ����� ���祭�, �� �? - ���� ���?�
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
