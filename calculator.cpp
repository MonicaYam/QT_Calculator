#include "calculator.h"
#include "ui_calculator.h"
#include <cmath>

double result = 0.0;
bool divideTrigger = false;
bool multiplyTrigger = false;
bool addTrigger = false;
bool subtractTrigger = false;

bool clearTrigger = false;
bool clearDisplayTrigger = false;

Calculator::Calculator(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(result));
    QPushButton *numberButtons[10];
    for(int i = 0; i < 10; ++i){
        QString buttonName = "Button_" + QString::number(i);
        numberButtons[i] = Calculator::findChild<QPushButton *>(buttonName);
        connect(numberButtons[i], SIGNAL(released()), this, SLOT(numberReleased()));
    }

    // Connect signals and slots for math buttons
    connect(ui->Button_Add, SIGNAL(released()), this,
            SLOT(mathButtonReleased()));
    connect(ui->Button_Subtract, SIGNAL(released()), this,
            SLOT(mathButtonReleased()));
    connect(ui->Button_Multiply, SIGNAL(released()), this,
            SLOT(mathButtonReleased()));
    connect(ui->Button_Divide, SIGNAL(released()), this,
            SLOT(mathButtonReleased()));

    // Connect equals button
    connect(ui->Button_Equals, SIGNAL(released()), this,
            SLOT(equalsButtonReleased()));

    // Connect Pow button
    connect(ui->Button_Pow, SIGNAL(released()), this,
            SLOT(powButtonReleased()));

    // Connect Clear button
    connect(ui->Button_Clear, SIGNAL(released()), this,
            SLOT(clearButtonReleased()));

    // Connect Clear button
    connect(ui->Button_DecimalPoint, SIGNAL(released()), this,
            SLOT(decimalPointButtonReleased()));
}

Calculator::~Calculator()
{
    delete ui;
}


//SLOT functions ----

void Calculator::numberReleased(){
    if(clearTrigger){
        clear();
        clearTrigger=false;
    }else if(clearDisplayTrigger){
        clearDisplay();
        clearDisplayTrigger=false;
    }
    //get history and display value
    QString historyVal = ui->History->text();
    QString displayVal = ui->Display->text();
    QPushButton *button = (QPushButton *)sender();
    QString buttonVal = button->text();
    QString newVal = displayVal + buttonVal;
    ui->Display->setText(QString::number(newVal.toDouble(), 'g', 16));
    ui->History->setText(historyVal + buttonVal);

}


void Calculator::mathButtonReleased(){
    if(ui->History->text().count("=")==1){
        clearHistory();
        SetTextToHistory(ui->Display->text());
        clearTrigger=false;
    }

    //need to calculate first
    if(addTrigger||subtractTrigger||divideTrigger||multiplyTrigger){
        calculate();
    }

    //get a pointer of pressed button
    QPushButton *button = (QPushButton *)sender();

    // Get math symbol on the button
    QString buttonVal = button->text();

    char trigger;
    if(QString::compare(buttonVal, "+", Qt::CaseInsensitive) == 0){
        addTrigger=true;
        trigger='+';
    } else if(QString::compare(buttonVal, "-", Qt::CaseInsensitive) == 0) {
        subtractTrigger=true;
        trigger='-';
    } else if(QString::compare(buttonVal, "*", Qt::CaseInsensitive) == 0){
        multiplyTrigger=true;
        trigger='*';

    } else{
        divideTrigger=true;
        trigger='/';
    }

    // Store current value in Display
    result =  ui->Display->text().toDouble();
    //set text on hitory
    SetTextToHistory(ui->History->text() + trigger);
    // Clear display when user enter number at next
    clearDisplayTrigger=true;

}

void Calculator::equalsButtonReleased(){
    QString historyVal = ui->History->text();
    if(historyVal.right(1)!="="){
        double answer = 0.0;

        // Get value in display
        double displayVal = ui->Display->text().toDouble();

        if(addTrigger){
           answer = result + displayVal;
           addTrigger=false;
        } else if(subtractTrigger){
            answer = result -  displayVal;
            subtractTrigger=false;
        } else if(multiplyTrigger){
            answer = result *  displayVal;
            multiplyTrigger=false;
        } else {
            answer = result /  displayVal;
            divideTrigger=false;
        }

        //set answer and add equals on history
        ui->Display->setText(QString::number(answer));
        ui->History->setText(historyVal + "=");

        //clear trigger for display and history
        clearTrigger = true;

    }
}

void Calculator::powButtonReleased(){
    double num = ui->Display->text().toDouble();
    double powNumber = pow(num, 2.0);
    ui->History->setText("pow("+QString::number(num)+")");
    ui->Display->setText(QString::number(powNumber));

}

void Calculator::clearButtonReleased(){
    clear();
    result=0;
}


void Calculator::decimalPointButtonReleased(){
    ui->History->setText(ui->History->text()+'.');
    ui->Display->setText(ui->Display->text()+'.');
}


//functions ----

void Calculator::calculate(){
    double answer = 0.0;
    double displayVal = ui->Display->text().toDouble();
    if(addTrigger){
        answer = result + displayVal;
        addTrigger=false;
    }else if(subtractTrigger){
        answer = result - displayVal;
        subtractTrigger=false;
    }else if(divideTrigger){
        answer = result / displayVal;
        divideTrigger=false;
    }else{
        answer = result * displayVal;
        multiplyTrigger=false;
    }
    clear();
    ui->History->setText(QString::number(answer));
    ui->Display->setText(QString::number(answer));
}

void Calculator::SetTextToHistory(QString const& text){
    ui->History->setText(text);
}

void Calculator::clear(){
    ui->History->setText("");
    ui->Display->setText("0");
}

void Calculator::clearDisplay(){
    ui->Display->setText("");
}

void Calculator::clearHistory(){
    ui->History->setText("");
}
