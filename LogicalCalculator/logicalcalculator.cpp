#include "logicalcalculator.h"

LogicalCalculator::LogicalCalculator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connections();
}

LogicalCalculator::~LogicalCalculator()
{

}

void LogicalCalculator::aButton()
{
	QString currentScreenText = ui.screen->text();
	currentScreenText += "A";
	ui.screen->setText(currentScreenText);
}

void LogicalCalculator::bButton()
{
	QString currentScreenText = ui.screen->text();
	currentScreenText += "B";
	ui.screen->setText(currentScreenText);
}

void LogicalCalculator::cButton()
{
	QString currentScreenText = ui.screen->text();
	currentScreenText += "C";
	ui.screen->setText(currentScreenText);
}

void LogicalCalculator::dButton()
{
	QString currentScreenText = ui.screen->text();
	currentScreenText += "D";
	ui.screen->setText(currentScreenText);
}

void LogicalCalculator::andButton()
{
	QString currentScreenText = ui.screen->text();
	currentScreenText += "&";
	ui.screen->setText(currentScreenText);
}

void LogicalCalculator::orButton()
{
	QString currentScreenText = ui.screen->text();
	currentScreenText += "|";
	ui.screen->setText(currentScreenText);
}

void LogicalCalculator::notButton()
{
	QString currentScreenText = ui.screen->text();
	currentScreenText += "!";
	ui.screen->setText(currentScreenText);
}

void LogicalCalculator::implicationButton()
{
	QString currentScreenText = ui.screen->text();
	currentScreenText += "=>";
	ui.screen->setText(currentScreenText);
}

void LogicalCalculator::equivalenceButton()
{
	QString currentScreenText = ui.screen->text();
	currentScreenText += "<=>";
	ui.screen->setText(currentScreenText);
}

void LogicalCalculator::lBracketButton()
{
	QString currentScreenText = ui.screen->text();
	currentScreenText += "(";
	ui.screen->setText(currentScreenText);
}

void LogicalCalculator::rBracketButton()
{
	QString currentScreenText = ui.screen->text();
	currentScreenText += ")";
	ui.screen->setText(currentScreenText);
}

void LogicalCalculator::resultButton()
{
	QString expression = ui.screen->text();

	/* Проверка на пустую строку */
	if (expression.isEmpty())
	{
		QMessageBox::warning(this, "Ошибка ввода", "Логическое выражение не задано.");
		return;
	}
	/* Проверка на скобки */
	if (!checkBrackets())
	{
		QMessageBox::warning(this, "Ошибка ввода", "Скобки расставлены не верно.");
		return;
	}

	QStringList exprList = getListOfExprElements(expression);

	/* Проверка инфиксного выражения на подряд идущие операнды */
	try
	{
		checkInfixExpr(exprList);
	}
	catch (QString &errorString)
	{
		QMessageBox::warning(this, "Ошибка ввода", errorString);
		return;
	}

	QStringList postfixExpr = convertToPostfixExpr(exprList);

	/* Проверка постфиксного выражения */
	try
	{
		checkPostfixExpr(postfixExpr);
	}
	catch (QString &errorString)
	{
		QMessageBox::warning(this, "Ошибка ввода", errorString);
		return;
	}
	
	/* Построение таблицы истинности */
	createLogicalTable(postfixExpr);
}

void LogicalCalculator::clearButton()
{
	ui.screen->setText("");
}

void LogicalCalculator::delButton()
{
	QString currentScreenText = ui.screen->text();
	int size = currentScreenText.size();


	if (size > 2 && currentScreenText[size - 1] == '>' && currentScreenText[size - 2] == '=' && currentScreenText[size - 3] == '<')
	/* Удаление "<=>" */
	{
		currentScreenText.remove(size - 3, 3);
	}
	else if (size > 1 && currentScreenText[size - 1] == '>' && currentScreenText[size - 2] == '=')
	/* Удаление "=>" */
	{
		currentScreenText.remove(size - 2, 2);
	}
	else
	/* Удаление односимвольных элементов */
	{
		currentScreenText.remove(size - 1, 1);
	}

	ui.screen->setText(currentScreenText);
}

QStringList LogicalCalculator::getListOfExprElements(QString expr)
{
	QStringList result;
	int size = expr.size();

	for (int i = 0; i < size; ++i)
	{
		if (i < (size - 1) && expr[i] == '=' && expr[i + 1] == '>')
		{
			result << "=>";
			++i;
		}
		else if (i < (size - 2) && expr[i] == '<' && expr[i+1] == '=' && expr[i+2] == '>')
		{
			result << "<=>";
			i += 2;
		}
		else
		{
			result << QString(expr[i]);
		}
	}
	return result;
}

QStringList LogicalCalculator::convertToPostfixExpr(QStringList infixExpr)
{
	QStack<QString> stack;
	QStringList result;
	int size = infixExpr.size();

	for (auto i : infixExpr)
	{
		if (isOperand(i))
		{
			/* Если встретился операнд, сразу помещаем в результирующий список */
			result << i;
		}
		else if (isOperation(i))
		{
			/* Если встретилась операция */
			
			while (!stack.empty() && stack.top() != "(" && priority(stack.top()) > priority(i))
			{
				result << stack.pop();
			}

			/* Операцию в стек */
			stack.push(i);
		}
		else if (isBracket(i))
		{
			/* Если встретилась скобка */
			if (i == "(")
			{
				stack.push(i);
			}
			else
			{
				QString tmp = stack.pop();

				while (tmp != "(")
				{
					result << tmp;
					tmp = stack.pop();
				}
			}
		}
	}

	/* Оставшиеся элементы в результирующий список */
	while (!stack.empty())
	{
		result << stack.pop();
	}

	return result;
}

int LogicalCalculator::calculateExpr(QStringList expr, int A, int B /*=0*/, int C /*=0*/, int D /*=0*/)
{
	int result = 0;
	QStack<int> stack;

	/* Проходим по выражению */
	for (auto i : expr)
	{
		if (isOperand(i))
		{
			/* Если встретился операнд, помещаем его значение в стек */
			if (i == "A")
				stack.push(A);
			else if (i == "B")
				stack.push(B);
			else if (i == "C")
				stack.push(C);
			else if (i == "D")
				stack.push(D);
		}
		else
		{
			/* Если встретилась операция, берем правый операнд из стека и анализируем тип операции */
			int rightOperand = stack.pop();

			if (i == "!")
			{
				/* Если унарная операция, проводим ее над правым операндом и результат в стек */
				stack.push(!rightOperand);
			}
			else
			{
				/* Если бинарная операция, берем левый операнд из стека ... */
				int leftOperand = stack.pop();

				/* ... проводим операцию ... */
				if (i == "|")
				{
					result = leftOperand | rightOperand;
				}
				else if (i == "=>")
				{
					result = !leftOperand | rightOperand;
				}
				else if (i == "&")
				{
					result = leftOperand & rightOperand;
				}
				else if (i == "<=>")
				{
					result = leftOperand == rightOperand;
				}
				/* ... результат кладем в стек */
				stack.push(result);
			}
		}
	}
	return stack.pop();
}

void LogicalCalculator::createLogicalTable(QStringList expr)
{
	/* Виджет таблицы */
	QTableWidget *logicalTable = new QTableWidget();
	logicalTable->verticalHeader()->hide();
	int columns = getOperands().size();
	logicalTable->setColumnCount(columns + 1);

	/* Логическая таблица */
	contentForTable(logicalTable, expr);

	/* Заголовки для колонок */
	makeHeadersForTable(logicalTable);
	
	/* Положение таблицы */
	logicalTable->show();
	int x = this->x() + this->width();
	int y = this->y();
	logicalTable->move(x, y);
}

void LogicalCalculator::connections()
{
	connect(ui.a, SIGNAL(clicked(bool)), SLOT(aButton()));
	connect(ui.b, SIGNAL(clicked(bool)), SLOT(bButton()));
	connect(ui.c, SIGNAL(clicked(bool)), SLOT(cButton()));
	connect(ui.d, SIGNAL(clicked(bool)), SLOT(dButton()));
	connect(ui.and, SIGNAL(clicked(bool)), SLOT(andButton()));
	connect(ui.or, SIGNAL(clicked(bool)), SLOT(orButton()));
	connect(ui.not, SIGNAL(clicked(bool)), SLOT(notButton()));
	connect(ui.implication, SIGNAL(clicked(bool)), SLOT(implicationButton()));
	connect(ui.equivalence, SIGNAL(clicked(bool)), SLOT(equivalenceButton()));
	connect(ui.lBracket, SIGNAL(clicked(bool)), SLOT(lBracketButton()));
	connect(ui.rBracket, SIGNAL(clicked(bool)), SLOT(rBracketButton()));
	connect(ui.del, SIGNAL(clicked(bool)), SLOT(delButton()));
	connect(ui.clear, SIGNAL(clicked(bool)), SLOT(clearButton()));
	connect(ui.result, SIGNAL(clicked(bool)), SLOT(resultButton()));
}

bool LogicalCalculator::isOperand(QString element)
{
	return element == "A" || element == "B" || element == "C" || element == "D";
}

bool LogicalCalculator::isOperation(QString element)
{
	return element == "&" || element == "|" || element == "!" || element == "=>" || element == "<=>";
}

bool LogicalCalculator::isBracket(QString element)
{
	return element == "(" || element == ")";
}

unsigned int LogicalCalculator::getOperationsArity(QString operation)
{
	if (operation == "!")
		return 1;
	else
		return 2;
}

unsigned int LogicalCalculator::priority(QString operation)
{
	if (operation == "!")
		return 5;
	else if (operation == "&")
		return 4;
	else if (operation == "|")
		return 3;
	else if (operation == "->")
		return 2;
	else if (operation == "<=>")
		return 1;
}

QStringList LogicalCalculator::getOperands()
{
	QStringList result;
	QString input = ui.screen->text();

	for (auto i : input)
	{
		if (isOperand(i) && !isAlreadyInList (result,i))
		{
			result << i;
		}
	}
	return result;
}

bool LogicalCalculator::isAlreadyInList(QStringList operandsList,QString operand)
{
	for (auto i : operandsList)
	{
		if (i == operand)
			return true;
	}
	return false;
}

void LogicalCalculator::makeHeadersForTable(QTableWidget *table)
{
	QStringList columns = getOperands();

	int j = 0;
	for (auto i : columns)
	{
		QTableWidgetItem *header = new QTableWidgetItem(QString(i));
		table->setHorizontalHeaderItem(j, header);
		++j;
	}
	QTableWidgetItem *header = new QTableWidgetItem("Result");
	table->setHorizontalHeaderItem(j, header);

	table->horizontalHeader()->setDefaultSectionSize(40);
	table->horizontalHeader()->setStretchLastSection(true);
}

void LogicalCalculator::tableForOneVar(QTableWidget *table,QStringList &expr)
{
	int row = 0;

	table->setRowCount(2);

	for (int i = 0; i <= 1; ++i)
	{
		int result = calculateExpr(expr, i);

		QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(i));
		table->setItem(row, 0, item1);

		QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(result));
		table->setItem(row, 1, item2);
		++row;
	}
	table->resize(80, 85);
}

void LogicalCalculator::tableForTwoVar(QTableWidget *table, QStringList &expr)
{
	int row = 0;
	table->setRowCount(4);

	for (int i = 0;  i <= 1; ++i)
	{
		for (int j = 0; j <= 1; ++j)
		{
			int result = calculateExpr(expr, i, j);

			QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(i));
			table->setItem(row, 0, item1);

			QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(j));
			table->setItem(row, 1, item2);

			QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(result));
			table->setItem(row, 2, item3);

			++row;
		}
	}
	table->resize(120, 145);
}

void LogicalCalculator::tableForThreeVar(QTableWidget *table, QStringList &expr)
{
	int row = 0;
	table->setRowCount(8);

	for (int i = 0; i <= 1; ++i)
	{
		for (int j = 0; j <= 1; ++j)
		{
			for (int k = 0; k <= 1; ++k)
			{
				int result = calculateExpr(expr, i, j, k);

				QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(i));
				table->setItem(row, 0, item1);

				QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(j));
				table->setItem(row, 1, item2);

				QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(k));
				table->setItem(row, 2, item3);

				QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(result));
				table->setItem(row, 3, item4);

				++row;
			}
		}
	}
	table->resize(170, 265);
	
}

void LogicalCalculator::tableForFourVar(QTableWidget *table, QStringList &expr)
{
	int row = 0;
	table->setRowCount(16);

	for (int i = 0; i <= 1; ++i)
	{
		for (int j = 0; j <= 1; ++j)
		{
			for (int k = 0; k <= 1; ++k)
			{
				for (int l = 0; l <= 1; ++l)
				{
					int result = calculateExpr(expr, i, j, k, l);

					QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(i));
					table->setItem(row, 0, item1);

					QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(j));
					table->setItem(row, 1, item2);

					QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(k));
					table->setItem(row, 2, item3);

					QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(l));
					table->setItem(row, 3, item4);

					QTableWidgetItem *item5 = new QTableWidgetItem(QString::number(result));
					table->setItem(row, 4, item5);

					++row;
				}
				
			}
		}
	}
	table->resize(220, 505);
}

void LogicalCalculator::contentForTable(QTableWidget *table, QStringList &expr)
{
	QStringList operands = getOperands();
	int numberOfOperands = operands.size();

	switch (numberOfOperands)
	{
		case 1:
			tableForOneVar(table, expr);
			break;
		case 2:
			tableForTwoVar(table, expr);
			break;
		case 3:
			tableForThreeVar(table, expr);
			break;
		case 4:
			tableForFourVar(table, expr);
			break;
		default:
			break;
	}
}

bool LogicalCalculator::checkBrackets()
{
	QString expression = ui.screen->text();
	int bracketsCounter = 0;
	for (auto i : expression)
	{
		if (i == '(')
			++bracketsCounter;
		else if (i == ')')
			--bracketsCounter;
	}

	return bracketsCounter == 0;
}

void LogicalCalculator::checkPostfixExpr(QStringList postfixExpr) throw(QString&)
{
	int operandsCounter = 0;
	int operandsCount = 0;
	int operationsCounter = 0;
	bool operandUsed = false;

	for (auto i : postfixExpr)
	{
		if (isOperand(i))
		{
			++operandsCounter;
			++operandsCount;
			operandUsed = true;
		}
		else if (isOperation(i) && operandsCounter >= getOperationsArity (i))
		{
			++operationsCounter;
			operandsCounter -= (getOperationsArity(i) - 1);
		}
		else if (isOperation(i) && operandsCounter < getOperationsArity(i))
		{
			QString errorString = "Недостаточно операндов для операций.";
			throw errorString;
		}
	}

	int difference = operandsCount - operationsCounter;
	if (!operandUsed)
	{
		QString errorString = "В выражении не используются операнды.";
		throw errorString;
	}
	else if (difference > 1)
	{
		QString errorString = "Не достаточно операций для всех операндов.";
		throw errorString;
	}
}

void LogicalCalculator::checkInfixExpr(QStringList infixExpr) throw(QString&)
{
	int size = infixExpr.size();
	QString errorString = "Не достаточно операций для всех операндов.";

	for (int i = 0; i < size; ++i)
	{
		if (i > 0 && isOperand(infixExpr[i]) && isOperand(infixExpr[i - 1]))
		{
			throw errorString;
		}
		else if (i > 1 && isOperand(infixExpr[i]) && infixExpr[i - 1] == "!" && isOperand(infixExpr[i - 2]))
		{
			throw errorString;
		}
	}
}
