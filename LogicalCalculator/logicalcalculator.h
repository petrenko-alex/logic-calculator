#ifndef LOGICALCALCULATOR_H
#define LOGICALCALCULATOR_H

#include <QtWidgets/QMainWindow>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QStack>
#include "ui_logicalcalculator.h"

class LogicalCalculator : public QMainWindow
{
	Q_OBJECT

public:
	LogicalCalculator(QWidget *parent = 0);
	~LogicalCalculator();

private slots:
	/* Слоты кнопок калькулятора */
	void aButton();
	void bButton();
	void cButton();
	void dButton();
	void andButton();
	void orButton();
	void notButton();
	void implicationButton();
	void equivalenceButton();
	void lBracketButton();
	void rBracketButton();
	void resultButton();
	void clearButton();
	void delButton();

private:
	/* Получить список элементов выражения из строки выражения expr */
	QStringList getListOfExprElements(QString expr);

	/* Преобразовать инфиксное выражение в постфиксное */
	QStringList convertToPostfixExpr(QStringList infixExpr);

	/* Рассчитать постфиксное выражение expr при данных значениях операндов A,B,C,D */
	int calculateExpr(QStringList expr,int A,int B = 0,int C = 0,int D = 0);

	/* Создать и показать таблицу истинности */
	void createLogicalTable(QStringList expr);

	/* Соединение сигналов и слотов */
	void connections();

	/* Проверить, является ли элемент element операндом */
	bool isOperand(QString element);

	/* Проверить, является ли элемент element операцией */
	bool isOperation(QString element);

	/* Проверить, является ли элемент element круглой скобкой */
	bool isBracket(QString element);

	/* Получить арность операции operation */
	unsigned int getOperationsArity(QString operation);

	/* Получить приоритет операции operation */
	unsigned int priority(QString operation);

	/* Получить список операндов, использующихся в выражении */
	QStringList getOperands();

	/* Проверить, присутствует ли операнд operand в листе operandsList */
	bool isAlreadyInList(QStringList operandsList, QString operand);

	/* Создать заголовки колонок для таблицы истинности на основе использующихся в выражении операндов */
	void makeHeadersForTable(QTableWidget *table);

	/* Таблица истинности для 1 переменной */
	void tableForOneVar(QTableWidget *table, QStringList &expr);

	/* Таблица истинности для 2 переменных */
	void tableForTwoVar(QTableWidget *table, QStringList &expr);

	/* Таблица истинности для 3 переменных */
	void tableForThreeVar(QTableWidget *table, QStringList &expr);

	/* Таблица истинности для 4 переменных */
	void tableForFourVar(QTableWidget *table, QStringList &expr);

	/* Добавить данные в таблицу истинности */
	void contentForTable(QTableWidget *table, QStringList &expr);

	/* Проверить выражение на корректность расстаноки скобок */
	bool checkBrackets();

	/* Проверить корректность постфиксного выражения */
	void checkPostfixExpr(QStringList postfixExpr)  throw(QString&);

	/* Проверить корректность инфиксного выражения */
	void checkInfixExpr(QStringList infixExpr)  throw(QString&);

	Ui::LogicalCalculatorClass ui;

};

#endif // LOGICALCALCULATOR_H
