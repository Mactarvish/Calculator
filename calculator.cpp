#include "calculator.h"
#include <QRegExp>
#include <QMap>

Calculator::Calculator()
{
    isValid = NORMAL;
    signStack = new QStack<QString>;
    calculateQueue = new QQueue<QString>;

    // 设定符号的优先级
    priority.insert("+", 1);
    priority.insert("-", 1);
    priority.insert("*", 2);
    priority.insert("/", 2);
    priority.insert("(", 3);
}

Calculator::~Calculator()
{
    delete signStack;
    delete calculateQueue;
}

const QString& Calculator::InitString(QString &exp)  // 8+*8/0.00000001
{
    exp.remove(' ');
    exp.replace("（", "(");
    exp.replace("）", ")");

    exp.replace("(", "(0");
    exp.replace("+-", "-");
    exp.replace("-+", "-");
    QRegExp consequentMinus("\\-{2,}");
    int currentPos = 0;
    while ((currentPos = consequentMinus.indexIn(exp)) != -1)
    {
        // 奇数（非1）个减号，替换成一个减号
        if (consequentMinus.matchedLength() % 2 == 1)
        {
            exp.replace(currentPos, consequentMinus.matchedLength(), "-");
        }
        // 否则替换成一个加号
        else
        {
            exp.replace(currentPos, consequentMinus.matchedLength(), "+");
        }
        currentPos++;
    }
    exp.replace("--", "+");
    exp.replace(QRegExp("\\++"), "+");

    QRegExp reg("(\\*|\\/)(\\+|\\-)");
    reg.indexIn(exp);
    exp.replace(reg, reg.cap(1) + "(0" + reg.cap(2) + "1)" + reg.cap(1));

    exp.insert(0, "0");

    qDebug() << exp;
    return exp;
}

void Calculator::InitString_V2(QString &exp)
{
    int i = 0;
    exp.replace("（", "(");
    exp.replace("）", ")");
    if (exp.at(0) == '(')
    {
        exp.insert(0, "+");
    }
    exp.insert(0, "0");

    if (!exp.at(exp.length() - 1).isDigit() && exp.at(exp.length() - 1) != ')')
    {
        isValid = SYNERROR;
        return;
    }
    while (i < exp.length())
    {
        switch (exp.at(i).unicode())
        {
        // 空格
        case 32:
        {
            exp.remove(i, 1);
            break;
        }
        // +
        case 43:
        {
            bool motified = false;
            bool proSign = false;
            // 该符号前是*或者/，标志位置1
            if (exp.at(i - 1) == '*' || exp.at(i - 1) == '/')
            {
                proSign = true;
            }
            while (exp.at(i + 1) == '+')
            {
                exp.remove(i + 1, 1);
                motified = true;
            }
            // 如果该符号前是*或者/，当前的+也要删除
            if (proSign)
            {
                exp.remove(i, 1);
                break;
            }
            if (exp.at(i + 1) == '-')
            {
                exp.remove(i, 1);
                motified = true;
            }
            if (!motified)
            {
                i++;
            }
            break;
        }
        // -
        case 45:
        {
            bool motified = false;
            bool proSign = false;
            // 该符号前是*或者/，标志位置1
            if (exp.at(i - 1) == '*' || exp.at(i - 1) == '/')
            {
                proSign = true;
            }

            if (exp.at(i + 1) == '-')
            {
                exp.remove(i + 1, 1);
                exp.replace(i, 1, "+");
                motified = true;

            }
            while (exp.at(i + 1) == '+')
            {
                exp.remove(i + 1, 1);
                motified = true;
            }
            if (proSign)
            {
                exp.remove(i, 1);
            }

            if (!motified)
            {
                i++;
            }
            break;
        }
        // *
        case 42:
        {
            bool motified = false;
            if (exp.at(i + 1) == '+')
            {
                exp.replace(i + 1, 1, "(0+1)*");
                i += 7;
                motified = true;
            }
            if (exp.at(i + 1) == '-')
            {
                exp.replace(i + 1, 1, "(0-1)*");
                i += 7;
                motified = true;
            }
            if (!motified)
            {
                i++;
            }
            break;
        }
        // /
        case 47:
        {
            bool motified = false;
            if (exp.at(i + 1) == '+')
            {
                exp.replace(i + 1, 1, "(0+1)/");
                i += 7;
                motified = true;
            }
            if (exp.at(i + 1) == '-')
            {
                exp.replace(i + 1, 1, "(0-1)/");
                i += 7;
                motified = true;
            }
            if (!motified)
            {
                i++;
            }
            break;
        }
        // 左括号
        case 40:
        {
            bool motified = false;
            if (exp.at(i - 1).isNumber())
            {
                exp.insert(i, "*");
                i += 2;
                motified = true;
            }
            if (!motified)
            {
                i++;
            }
            break;
        }
        // 小数点
        case 46:
        {
            bool motified = false;
            if (!exp.at(i - 1).isDigit())
            {
                exp.insert(i, "0");
                i += 2;
                motified = true;
            }
            if (!motified)
            {
                i++;
            }
            break;
        }
        default:
        {
            if (!exp.at(i).isDigit() && exp.at(i) != ')')
            {
                qDebug() << "ERROR : invalid sign:" << exp.at(i);
                isValid = SYNERROR;
                return;
            }
            i++;
            break;
        }
        }
    }

    qDebug() << exp;
    //return exp;
}

double Calculator::Parse(QString exp)
{
    isValid = NORMAL;
    InitString_V2(exp);
    if (isValid != NORMAL)
    {
        return 0;
    }
    Split(exp);
    return HandleSuffixExp();
}

Calculator::ERROR Calculator::ExpIsValid() const
{
    return isValid;
}

bool Calculator::Split(const QString &exp)
{
/*
 * 对表达式进行分解，分解为后缀表达式并存入calculateStack中
 */

    QRegExp reg("[^0-9\\.]{1}");
    int beginPos = 0;
    int currentPos = -1;

    while ((currentPos = reg.indexIn(exp, currentPos + 1)) != -1)
    {
        // 该字段是数字，直接压入计算栈
        if (currentPos - beginPos != 0)
        {
            QString numField = exp.mid(beginPos, currentPos - beginPos);
            calculateQueue->enqueue(numField);
        }
        // 该字段是符号，进行符号处理
        const QString sign = exp.at(currentPos);

        HandleSign(sign);
        //qDebug() << "sign: " << sign;

        beginPos = currentPos + 1;
    }
    // 如果这时beginPos小于原表达式的长度，即还有未处理的字段，那么该字段必然是数字，直接压入计算栈
    if (beginPos < exp.length())
    {
        calculateQueue->enqueue(exp.mid(beginPos));
    }

    // 表达式已结束，清空符号栈并压入计算栈
    while (!signStack->isEmpty())
    {
        calculateQueue->enqueue(signStack->pop());
    }

    return true;
}

void Calculator::HandleSign(const QString &sign)
{
    // 如果该符号是')'，弹出'('上面所有的符号并压入计算栈
    if (sign == ")")
    {
        while (signStack->top() != "(")
        {
            calculateQueue->enqueue(signStack->pop());
        }
        // 弹出'('，丢弃
        signStack->pop();
        return;
    }

    if (signStack->isEmpty())
    {
        signStack->push(sign);
        return;
    }
    // 如果该符号不是')'而且优先级小于等于栈顶符号的优先级，那么弹出栈顶元素并压入计算栈
    // 如果该符号是'('，直接压入栈，什么也不做
    while (sign != ")" && !signStack->isEmpty() && sign != "(" && signStack->top() != "(" &&
           priority[sign] <= priority[signStack->top()])
    {
        calculateQueue->enqueue(signStack->pop());
    }
    // 该符号优先级已经大于栈顶元素，压入栈
    signStack->push(sign);
}

float Calculator::HandleSuffixExp()
{
    /*
     * 处理calculateStack中的后缀表达式
     */
    QStack<float> temp;
    QString queueStr;
    bool ok = false;
    while (!calculateQueue->isEmpty())
    {
        queueStr = calculateQueue->dequeue();
        float num = queueStr.toFloat(&ok);
        if (ok)
        {
            temp.push(num);
        }
        else
        {
            // 表达式有效性检查
            if (temp.isEmpty())
            {
                isValid = SYNERROR;
                return 0;
            }
            float operand_Right = temp.pop();
            if (temp.isEmpty())
            {
                isValid = SYNERROR;
                return 0;
            }
            float operand_Left = temp.pop();
            float result = 0;

            switch (queueStr.at(0).unicode())
            {
            // +
            case 43:
            {
                result = operand_Left + operand_Right;
                break;
            }
            // -
            case 45:
            {
                result = operand_Left - operand_Right;
                break;
            }
            // *
            case 42:
            {
                result = operand_Left * operand_Right;
                break;
            }
            // /
            case 47:
            {
                if (operand_Right <= 0.00000001 && operand_Right >= -0.00000001)
                {
                    isValid = MATHERROR;
                    return 0;
                }
                result = operand_Left / operand_Right;
                break;
            }
            default:
            {
                qDebug() << "ERROR : invalid sign";
                break;
            }
            }
            temp.push(result);
        }
    }
    float result = temp.pop();
    qDebug() << result;

    return result;
}

