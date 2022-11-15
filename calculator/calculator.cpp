#include "calculator.h"
#include "mybutton.h"

#include <QGridLayout>
#include <QToolButton>
#include <QDebug>
#include <QtMath>

Calculator::Calculator()
{
    m_display_up = new QLineEdit();
    m_display_down = new QLineEdit();
    m_sign = new QLabel();
    QGridLayout* mainLayout = new QGridLayout;

    mainLayout -> setSizeConstraint(QLayout::SetFixedSize);

    m_display_down -> setText("0");

    m_display_up -> setReadOnly(true);
    m_display_down -> setReadOnly(true);

    m_sign -> setAlignment(Qt::AlignRight);
    m_display_up -> setAlignment(Qt::AlignRight);
    m_display_down -> setAlignment(Qt::AlignRight);

    QFont font = m_display_up -> font();
    font.setPointSize(font.pointSize() + 8);
    m_display_up -> setFont(font);
    m_display_down -> setFont(font);
    m_sign -> setFont(font);

    for(int i = 0; i < 10; i++)
        m_digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));

    button *pointButton = createButton(".", SLOT(pointClicked()));
    button *changeSignButton = createButton(m_change_sign, SLOT(changeSignClicked()));

    button *backspaceButton = createButton("Backspace", SLOT(backspaceClicked()));
    button *clearButton = createButton("Clear", SLOT(clear()));
    button *clearAllButton = createButton("Clear All", SLOT(clearAll()));

    button *cosButton = createButton("cos", SLOT(unaryOperatorClicked()));
    button *sinButton = createButton("sin", SLOT(unaryOperatorClicked()));
    button *tgButton = createButton("tg", SLOT(unaryOperatorClicked()));
    button *ctgButton = createButton("ctg", SLOT(unaryOperatorClicked()));

    button *squareRootButton = createButton(m_squareRoot_sign, SLOT(unaryOperatorClicked()));
    button *powerButton = createButton(m_power_sign, SLOT(unaryOperatorClicked()));
    button *reciprocalButton = createButton(m_reciprocal_sign, SLOT(unaryOperatorClicked()));

    button *divisionButton = createButton(m_division_sign, SLOT(doubleOperatorClicked()));
    button *timesButton = createButton(m_times_sign, SLOT(doubleOperatorClicked()));
    button *minusButton = createButton(m_minus_sign, SLOT(doubleOperatorClicked()));
    button *plusButton = createButton(m_plus_sign, SLOT(doubleOperatorClicked()));

    button *equalButton = createButton("=", SLOT(equalClicked()));

    for(int i = 1; i < 10; i++) {
        int row = ((9 - i) / 3) + 4;
        int column = ((i - 1) % 3) + 1;
        mainLayout -> addWidget(m_digitButtons[i], row, column);
    }

    mainLayout -> addWidget(m_display_up, 0, 0, 1, 6);
    mainLayout -> addWidget(m_sign, 1, 5, 1, 1);
    mainLayout -> addWidget(m_display_down, 2, 0, 1, 6);
    mainLayout -> addWidget(backspaceButton, 3, 0, 1, 2);
    mainLayout -> addWidget(clearButton, 3, 2, 1, 2);
    mainLayout -> addWidget(clearAllButton, 3, 4, 1, 2);

    mainLayout -> addWidget(cosButton, 4, 0);
    mainLayout -> addWidget(sinButton, 5, 0);
    mainLayout -> addWidget(tgButton, 6, 0);
    mainLayout -> addWidget(ctgButton, 7, 0);

    mainLayout -> addWidget(m_digitButtons[0], 7, 1);
    mainLayout -> addWidget(pointButton, 7, 2);
    mainLayout -> addWidget(changeSignButton, 7, 3);

    mainLayout -> addWidget(divisionButton, 4, 4);
    mainLayout -> addWidget(timesButton, 5, 4);
    mainLayout -> addWidget(minusButton, 6, 4);
    mainLayout -> addWidget(plusButton, 7, 4);

    mainLayout -> addWidget(squareRootButton, 4, 5);
    mainLayout -> addWidget(powerButton, 5, 5);
    mainLayout -> addWidget(reciprocalButton, 6, 5);
    mainLayout -> addWidget(equalButton, 7, 5);

    setLayout(mainLayout);

    setWindowTitle("Calculator");
}

void Calculator::abortOperation(){
    m_display_up -> setText("###");
    m_display_down -> setText("###");
}

bool Calculator::calculate(double operand, const QString &operation)
{
    double temp_total = m_display_up -> text().toDouble();
    if(operation == m_plus_sign) {
        temp_total += operand;
    } else if(operation == m_minus_sign) {
        temp_total -= operand;
      }
      else if(operation == m_times_sign) {
        temp_total *= operand;
      }
      else if(operation == m_division_sign) {
        if(operand == 0.0)
            return false;
        temp_total /= operand;
      }

    m_display_up -> setText(QString::number(temp_total));
    return true;
}

button *Calculator::createButton(const QString &text, const char *member)
{
    button* btn = new button(text);
    connect(btn, SIGNAL(clicked()), this, member);
    return btn;
}

void Calculator::digitClicked() {
    button* btn = (button*) sender();
    int digit = btn -> text().toUInt();

    if(m_display_down -> text() == "0") {
        m_display_down -> clear();
        m_display_up -> clear();
     }

    m_display_down -> setText(m_display_down -> text() + QString::number(digit));
}

void Calculator::unaryOperatorClicked(){
    button* btn = (button*) sender();
    QString operation = btn -> text();

    double operand = m_display_down -> text().toDouble();

    double result = 0.0;

    if(operation == m_squareRoot_sign)
    {
        if(operand < 0.0) {
            abortOperation();
            return;
        }

        result = std::sqrt(operand);
    } else if(operation == m_power_sign)
        result = std::pow(operand, 2.0);
    else if(operation == m_reciprocal_sign){
        if(operand == 0.0) {
            abortOperation();
            return;
        }

        result = 1.0 / operand;
    } else if(operation == m_cos_sign) {
        result = qCos(operand);
    } else if(operation == m_sin_sign) {
        result = qSin(operand);
    } else if(operation == m_tg_sign) {
        result = qTan(operand);
    } else if(operation == m_ctg_sign) {
        result = 1 / qTan(operand);
    }

    m_display_down -> setText(QString::number(result));
}

void Calculator::doubleOperatorClicked(){
    button* btn = (button*) sender();
    QString operation = btn -> text();
    double operand = m_display_down -> text().toDouble();

    if(m_display_down -> text() == "0")
        return;
    m_sign -> setText(operation);

    if(m_display_down -> text() == "")
        return;
    m_display_down -> clear();

    if(!m_pending_operation.isEmpty()) {
        if(!calculate(operand, m_pending_operation)) {
            abortOperation();
            return;
        }
        m_pending_operation.clear();
    } else {
        m_display_up -> setText(QString::number(operand));
    }

    m_pending_operation = operation;
}

void Calculator::equalClicked(){
    double operand = m_display_down -> text().toDouble();

    if(!m_pending_operation.isEmpty()) {
        if(!calculate(operand, m_pending_operation)) {
            abortOperation();
            return;
        }
        m_pending_operation.clear();
    }

    m_display_down -> setText(m_display_up -> text());
    m_display_up -> clear();
    m_sign -> clear();
}

void Calculator::pointClicked(){
    if(!m_display_down -> text().contains('.')) {
        m_display_down -> setText(m_display_down -> text() + '.');
    }
}

void Calculator::changeSignClicked(){
    QString text = m_display_down -> text();
    double val = text.toDouble();

    if(val > 0.0)
        text.prepend("-");
    else if(val < 0.0)
        text.remove(0, 1);

    m_display_down -> setText(text);
}

void Calculator::backspaceClicked(){
    QString text = m_display_down -> text();
    text.chop(1);

    if(text.isEmpty()){
        text = "0";
    }
    m_display_down -> setText(text);
}

void Calculator::clear(){
    m_display_down -> setText("0");
}

void Calculator::clearAll(){
    m_display_down -> setText("0");
    m_display_up -> setText("0");
}



