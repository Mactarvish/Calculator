#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QString>
#include <QStack>
#include <QQueue>
#include <QStringList>
#include <QDebug>

class Calculator
{
public:
    enum ERROR{SYNERROR, MATHERROR, NORMAL};
    Calculator();
    ~Calculator();

    double Parse(QString exp);
    ERROR ExpIsValid() const;

private:
    const QString& InitString(QString& exp);
    void InitString_V2(QString& exp);
    bool Split(const QString& exp);
    void HandleSign(const QString& sign);
    float HandleSuffixExp();

    QStack<QString>* signStack;
    QQueue<QString>* calculateQueue;
    QMap<QString, int> priority;
    QString expression;
    ERROR isValid;
    //QStringList expBlock;
};

#endif // CALCULATOR_H
