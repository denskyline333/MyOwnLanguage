#include "stdafx.h"
#include "TheGeneration.h"

ofstream of("Final.html");
void StartPoint()
{
	of << "<html>" << endl;
	of << "<head>" << endl;
	of << "<title>" << endl;
	of << "JavaScript KDD-2018" << endl;
	of << "</title>" << endl;
	of << "<body bgcolor=\"#AEB6BF\">" << endl;
	of << "<p>" << endl;
	of << "<font size=\"7\">" << endl;
	of << "<script>" << endl << endl;
}

void EndPoint()
{	
	of << "</script>" << endl;
	of << "</font>" << endl;
	of << "</p> " << endl;
	of << "</body>" << endl;
	of << "</head>" << endl;
	of << "</html>" << endl;
	of.close();
}


void Generation( LT::LexTable &Lextable, In::StToken *tokens, IT::IdTable & idtable)
{
	StartPoint();
	for (int i = 0; i < Lextable.size; i++)
	{
		switch (Lextable.table[i].lexema)
		{
		case LEX_PLUS:
		{
			of << " + ";
			break;
		}
		case LEX_MULTIPLY:
		{
			of << " * ";
			break;
		}
		case LEX_MINUS:
		{
			of << " - ";
			break;
		}
		case LEX_DIVISION:
		{
			of << " / ";
			break;
		}
		case LEX_EQUAL:
		{
			of << " = ";
			break;
		}
		case LEX_COMMA:
		{
			of << " , ";
			break;
		}
		case SPACE:
		{
			of << " ";
			break;
		}

		case LEX_RIGHTPARENT:
		{
			of << ")";
			break;
		}
		case LEX_LEFTPARENT:
		{
			of << "(";
			break;
		}
		case LEX_LEFTBRACE:
		{
			of << endl << "{ " << endl;
			break;
		}
		case LEX_RIGHTBRACE:
		{
			of << "}" << endl << endl;
			break;
		}
		case LEX_SEMICOLON:
		{
			of << Lextable.table[i].lexema<< endl;
			for (int j = 1; j < i; j++) 
			{
				if (Lextable.table[i - j].lexema == LEX_SEMICOLON)
				{
					break;
				}
				if (Lextable.table[i - j].lexema == LEX_ID && Lextable.table[i - j + 1].lexema == LEX_EQUAL )
				{
					of<< "if (parseInt(" << tokens[i - j].token << ") <0)" <<endl<< "{"<<endl<<tokens[i - j].token<< "=0;"<<endl<<"}"<<endl;
					break;
				}
			}
			break;
		}
		case LEX_EXIST:
		{		
			of << "var ";
			break;
		}
		case LEX_LITERAL: 
		{
			of << tokens[i].token;
			break;
		}
		case LEX_ID:
		{
			int b;
			b = Lextable.table[i].idxTI;
			if (Lextable.table[i - 2].lexema == LEX_WRITE)
			{
				if (idtable.table[b].iddatatype == IT::NUM)
					of << "Math.floor(" << idtable.table[b].id << ")";
				else
					of << idtable.table[b].id;
			}
			else if (idtable.table[b].iddatatype == IT::STR ||
				IT::NUM && Lextable.table[i - 2].lexema != LEX_WRITE)
			{
				of << idtable.table[b].id;
			}
			break;
		}
		case LEX_WRITE:
		{
			of << "document.write('<br />');" <<endl
				<<"document.write" ;
			break;
		}
		case LEX_LENGTH:
		{
			of << tokens[i + 2].token << ".length";
			i += 3;
			break;
		}
		case LEX_SUBSTR:
		{
			of << tokens[i + 2].token << ".substr(";
			i += 3;
			break;
		}
		case LEX_REPLACE:
		{
			of << tokens[i + 2].token << ".replace(";
			i += 3;
			break;
		}
		case STOP_POINT:
		{
			of << "."<<endl;
			break;
		}
		case LEX_FUNCTION:
		{
			of << "function ";
			break;
		}
		case LEX_RETURN:
		{
			of << "return ";
			break;
		}		
		}
	}
	EndPoint();
}
