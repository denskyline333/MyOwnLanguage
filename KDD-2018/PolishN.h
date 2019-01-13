#pragma once
#include "stdafx.h"
#include "LexAnalysis.h"
#include "FST.h"

struct PN
{
	LT::Entry result[LT_MAXSIZE]; // ’ранит преобразованное выражение в польской нотации
	int size = 0;
	enum { LH = '(', RH = ')', PLUS = '+', MINUS = '-', MULT = '*', DIVIS = '/', EQUAL = '=' };
	int prior(char operation); // ѕриоритет операции
	int search(int number, Lex::LEX *tables); // ѕоиск начала выражени€ в таблице лексем
	bool CreatePolishNotation(Lex::LEX *tables);
};
