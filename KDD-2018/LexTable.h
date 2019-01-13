#pragma once
#include "stdafx.h"
//спецификаци€ таблицы лексем
#define	LEXEMA_FIXSIZE   1	        //размер лексемы
#define	LT_MAXSIZE		 4096	    //максимальное количество строк в таблице лексем
#define	LT_TI_NULLDX	 0xffffffff	//нет элемента таблицы идентификаторов
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

namespace LT	//таблица лексем
{
	struct Entry                       //структура дл€ содержимого табл.лексем
	{
		char lexema;					//лексема
		int sn;							//номер строки в исходном тексте
		int idxTI;						//индекс в таблице идентификаторов или LT_TI_NULLIDX

		Entry();
		Entry(char lexema, int snn, int idxti = LT_TI_NULLDX);
	};

	struct LexTable						//экземпл€р таблицы лексем
	{
		int maxsize;					//Ємкость таблицы лексем
		int size;						//текущий размер таблицы лексем
		Entry* table;					//массив (Entity)строк таблицы лексем
	};

	LexTable Create(int size);		            //Ємкость < LT_MAXSIZE
	void Add(LexTable &lextable, Entry entry);	//экземпл€р таблицы лексем, строка таблицы лексем
}