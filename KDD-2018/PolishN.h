#pragma once
#include "stdafx.h"
#include "LexAnalysis.h"
#include "FST.h"

struct PN
{
	LT::Entry result[LT_MAXSIZE]; // ������ ��������������� ��������� � �������� �������
	int size = 0;
	enum { LH = '(', RH = ')', PLUS = '+', MINUS = '-', MULT = '*', DIVIS = '/', EQUAL = '=' };
	int prior(char operation); // ��������� ��������
	int search(int number, Lex::LEX *tables); // ����� ������ ��������� � ������� ������
	bool CreatePolishNotation(Lex::LEX *tables);
};
