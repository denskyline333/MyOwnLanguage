#pragma once
#include "stdafx.h"
//������������ ������� ������
#define	LEXEMA_FIXSIZE   1	        //������ �������
#define	LT_MAXSIZE		 4096	    //������������ ���������� ����� � ������� ������
#define	LT_TI_NULLDX	 0xffffffff	//��� �������� ������� ���������������
#define LEX_SEPARATORS	 'S'
#define	LEX_ID			 'i'			
#define	LEX_LITERAL		 'l'	
#define LEX_LIB			 's'
#define	LEX_FUNCTION     'f'			
#define	LEX_MAIN		 'a'			
#define	LEX_RETURN		 'r'			
#define	LEX_WRITE		 'w'	
#define LEX_EXIST        'e'
#define	LEX_ID_TYPE_I    'n'
#define	LEX_ID_TYPE_S    'q'
#define LEX_LIBRARY      'm'
#define	LEX_SEMICOLON	 ';'			
#define	LEX_COMMA		 ','			
#define	LEX_LEFTBRACE	 '{'			
#define	LEX_RIGHTBRACE	 '}'			
#define	LEX_LEFTPARENT	 '('			
#define	LEX_RIGHTPARENT	 ')'			
#define	LEX_PLUS		 '+'	
#define	LEX_MINUS		 '-'
#define	LEX_MULTIPLY 	 '*'			
#define	LEX_DIVISION	 '/'	
#define SPACE            ' '
#define STOP_POINT       '.'
#define LEX_LENGTH		 'u'
#define LEX_SUBSTR		 'b'			
#define LEX_REPLACE      'y'
#define	LEX_EQUAL		 '='			
#define	LEX_OPERATION	 'v'
#define	INTEGER			 "integer"		
#define	STRING			 "string"
#define	LITERAL			 "literal"

namespace LT	//������� ������
{
	struct Entry                       //��������� ��� ����������� ����.������
	{
		char lexema;					//�������
		int sn;							//����� ������ � �������� ������
		int idxTI;						//������ � ������� ��������������� ��� LT_TI_NULLIDX

		Entry();
		Entry(char lexema, int snn, int idxti = LT_TI_NULLDX);
	};

	struct LexTable						//��������� ������� ������
	{
		int maxsize;					//������� ������� ������
		int size;						//������� ������ ������� ������
		Entry* table;					//������ (Entity)����� ������� ������
	};

	LexTable Create(int size);		            //������� < LT_MAXSIZE
	void Add(LexTable &lextable, Entry entry);	//��������� ������� ������, ������ ������� ������
}