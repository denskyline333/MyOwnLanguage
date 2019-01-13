#include "stdafx.h"
#include "In.h"
#include "LexAnalysis.h"
#include "Log.h"
#include "Error.h"
#include "Parm.h"
#include "IdentfTable.h"
#include "LexTable.h"
#include "FST.h"
#include "TheGeneration.h"
#include "MFST.h"
#include "Greibach.h"
#include "PolishN.h"

int _tmain(int argc, wchar_t *argv[])
{
	setlocale(LC_CTYPE, "Russian");
	Log::LOG log;//оператор разрешения области видимости
	try
	{

		Parm::PARM parm = Parm::getparm(argc, argv); // параметры
		log = Log::getstream(parm.log); // открытие потока
		Log::writeLog(log); // заполнение журнала   дата время
		Log::writeParm(log, parm); // печать в журнал используемых параметров 
		In::IN in = In::getin(log.stream, parm.in);  // запусл контроля входных символов
		in.tokens = In::parsOfToken(in.tokens, in.text, in.code);   // формирование из считанного файла слов для лекс анализа.
		Lex::LEX tables = Lex::Begin_Lex(in, log, parm);  // запуск лексического анализа
		MFST::Mfst mfst(tables, GRB::getGreibach()); //инициализация для синтаксического анализатора

		if (Log::Error) // если нет ошибок
		{
			Log::writeIn(log, in); //  печать  информаци о считывании из файла
			Log::writeLexTable(log, tables.Lextable);  // печать таблицы лексем
			Log::writeIDtable(log, tables.IDtable); // печать таблицы идентификаторов
			if (parm.ITtable == true)
			{
				writeIDTableConsole(log, tables.IDtable); // печать таблицы идентификаторов в консоль
			}
			if (parm.LTtable == true)
			{
				Log::writeLexTableConsole(log, tables.Lextable);  // печать таблицы лексем в консоль
			}
			Log::writeIntermediateCode(log, tables.Lextable); // печать промежуточного кода
		}
		if (Log::Error)
		{
			MFST_TRACE_START(log); // шапка
			mfst.start(log); //старт синтаксического нализа
			mfst.savededucation(); //сохранить правила вывода
			mfst.printrules(log); // печать дерева разбора
		}

		if (Log::flaw == false)
		{
			Generation(tables.Lextable, in.tokens,  tables.IDtable);// запуск генерации и формирование файла с кодом JS
			PN polishNatation;
			if (polishNatation.CreatePolishNotation(&tables))
			{
				*log.stream << endl << endl << "Polish Notation";
				Log::writeIntermediateCode(log, tables.Lextable);
			}
		}

		Log::Close(log); // закрытие потока

	}
	catch (Error::ERROR e)
	{
		Log::writeError(log.stream, e); // ошибки

	}
}