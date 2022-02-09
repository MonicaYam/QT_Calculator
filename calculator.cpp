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

    // Connect signals and slots
    connect(ui->Button_Add, SIGNAL(released()), this,
            SLOT(mathButtonReleased()));
    connect(ui->Button_Subtract, SIGNAL(released()), this,
            SLOT(mathButtonReleased()));
    connect(ui->Button_Multiply, SIGNAL(released()), this,
            SLOT(mathButtonReleased()));
    connect(ui->Button_Divide, SIGNAL(released()), this,
            SLOT(mathButtonReleased()));
    connect(ui->Button_Equals, SIGNAL(released()), this,
            SLOT(equalsButtonReleased()));
    connect(ui->Button_Pow, SIGNAL(released()), this,
            SLOT(powButtonReleased()));
    connect(ui->Button_Clear, SIGNAL(released()), this,
            SLOT(clearButtonReleased()));
    connect(ui->Button_DecimalPoint, SIGNAL(released()), this,
            SLOT(decimalPointButtonReleased()));
}

Calculator::~Calculator()
{
    delete ui;
}


//SLOT functions ----

//user released number button
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
    QString newNum = displayVal + buttonVal;
    ui->Display->setText(QString::number(newNum.toDouble(), 'g', 16));
    ui->History->setText(historyVal + buttonVal);

}

//user released +,-,/,* button
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

    // get symbol value from the button
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

    // put current display value into result
    result =  ui->Display->text().toDouble();
    //set text on hitory
    SetTextToHistory(ui->History->text() + trigger);
    // Clear display when user enter number at next
    clearDisplayTrigger=true;

}

//user released = button
void Calculator::equalsButtonReleased(){
    QString historyVal = ui->History->text();
    
    //if user already pressed =, then do nothing
    if(historyVal.right(1)!="="){
        double answer = 0.0;

        // get display value
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

//user released x2 button
void Calculator::powButtonReleased(){
    //get display value
    double num = ui->Display->text().toDouble();
    
    //calculate pow
    double powNumber = pow(num, 2.0);
    
    //set text
    ui->History->setText("pow("+QString::number(num)+")");
    ui->Display->setText(QString::number(powNumber));

}

//user released c button
void Calculator::clearButtonReleased(){
    //clear everything
    clear();
    result=0;
}

//user released . button
void Calculator::decimalPointButtonReleased(){
    //set text
    ui->History->setText(ui->History->text()+'.');
    ui->Display->setText(ui->Display->text()+'.');
}


//functions ----

void Calculator::calculate(){
    double answer = 0.0;
    //get display value
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
    
    //clear display and history
    clear();
    //set answer as a text on history and display
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
