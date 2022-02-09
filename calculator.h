#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = 0);
    ~Calculator();

private:
    Ui::Calculator *ui;
    void calculate();
    void clearDisplay();
    void clearHistory();
    void clear();
    void SetTextToHistory(QString const&);

private slots :
    void numberReleased();
    void mathButtonReleased();
    void equalsButtonReleased();
    void powButtonReleased();
    void clearButtonReleased();
    void decimalPointButtonReleased();
};

#endif // CALCULATOR_H
