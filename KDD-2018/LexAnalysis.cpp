#include "stdafx.h"	
#include "Graphs.h"
#include "FST.h"
#include "LexAnalysis.h"
#include "Error.h"

namespace Lex
{
	bool flag2 = false;
	bool flag1 = false;
	Graph graph[N_GRAPHS] =   // графы для конечных автоматов
	{
		{ LEX_LIB, FST::FST(GRAPH_MYLIB) },
		{ LEX_LITERAL, FST::FST(GRAPH_INTEGER_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_STRING_LITERAL) },
		{ LEX_SEPARATORS, FST::FST(GRAPH_SEPARATORS) },  
		{ LEX_DIVISION, FST::FST(GRAPH_FORWARD_SLASH) },
		{ LEX_EXIST, FST::FST(GRAPH_EXIST) },
		{ LEX_LENGTH, FST::FST(GRAPH_LENGTH) },
		{ LEX_SUBSTR, FST::FST(GRAPH_SUBSTR) },
		{ LEX_REPLACE, FST::FST(GRAPH_REPLACE) },
		{ LEX_MAIN, FST::FST(GRAPH_MAIN) },
		{ LEX_ID_TYPE_I, FST::FST(GRAPH_INTEGER) },
		{ LEX_ID_TYPE_S, FST::FST(GRAPH_STRING) },
		{ LEX_FUNCTION, FST::FST(GRAPH_FUNCTION) },
		{ LEX_RETURN, FST::FST(GRAPH_RETURN) },
		{ LEX_WRITE, FST::FST(GRAPH_WRITE) },
		{ LEX_ID, FST::FST(GRAPH_ID) },
	};
	LEX Begin_Lex(In::IN &InStruct, Log::LOG &log, Parm::PARM &parm) //начало Лексического анализа
	{
		LEX Tables;
		Tables.Lextable = LT::Create(LT_MAXSIZE);  // создание таблицы лексем
		Tables.IDtable = IT::Create(TI_MAXSIZE);  // создание таблицы идентификаторов
		bool isFunc = false;      // флаг на функцию
		bool isParam = false;     //флаг на параметр
		int count_parm = 0;
		int MainBody = NULL;
		int MyFunc = NULL;
		int LibBody = NULL;
		int funcend = NULL;
		char buffer[255] = "";
		char tempTokenFunc[255] = ""; //буфер для имени функции
		char tempTokenFunction[255] = "";   //буфер
		char tempToken[255] = "";    //буфер

		IT::IDDATATYPE funcType;   // тип функции
		IT::IDDATATYPE idType;   // тип идентификатора

		for (int i = 0; i < In::StToken::tokensSize; i++) // начинаем просмотр массива слов
		{
			for (int j = 0; j < N_GRAPHS; j++)    // ищем подходящий автомат для слова	
			{
				FST::FST fstex(InStruct.tokens[i].token, graph[j].graph);   // попытка разбора
				if (FST::execute(fstex)) // если разбор успешный то 
				{
					switch (graph[j].lexema)     // ищем подходящий выбор для лексемы
					{

					case LEX_MAIN:   // если главная ф-ция
					{
						MainBody++;   //увеличим с 0 до 1 , всё правильно , необходима ровно 1 гл ф-ция
						if (MainBody > 1)    // если найдено более 1-ой главной ф-ции
							Log::writeError(log.stream, Error::GetError(116, InStruct.tokens[i].line, NULL)); // делаем запись в журнал ошибок

						strcpy_s(tempTokenFunc, "");
						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line);   // запись в таблицу , с фиксацией номера строки 
						LT::Add(Tables.Lextable, entrylt);    // добавление лексемы в таблицу 
						break; //выход
					}
					case LEX_LIB:   // если стандартная библиотека
					{
						LibBody++;
						if (LibBody > 1)    // если найдено более 1-ой библиотеки
							Log::writeError(log.stream, Error::GetError(403, InStruct.tokens[i].line, NULL)); // делаем запись в журнал ошибок

						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line);// запись в таблицу , с фиксацией номера строки 
						LT::Add(Tables.Lextable, entrylt);// добавление лексемы в таблицу 
						break; //выход
					}



					case LEX_LENGTH: // лексема функции substr
					{
						MyFunc = 1;
						IT::Entry entryit("length", i, IT::NUM, IT::S); // создаём элемент ТИ с занесением всех собранных данных
						IT::Add(Tables.IDtable, entryit);						 // добавляем созданный элемент
						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // создание
																																		 // элемента таблицы лексем
						LT::Add(Tables.Lextable, entrylt); //занесение в таблицу лексем
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].count_parm = 1;
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[0] = 'q';
						break;
					}

					case LEX_SUBSTR: // лексема функции substr
					{
						MyFunc = 1;
						IT::Entry entryit("substr", i, IT::STR, IT::S); // создаём элемент ТИ с занесением всех собранных данных
						IT::Add(Tables.IDtable, entryit);						 // добавляем созданный элемент
						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // создание
																																		 // элемента таблицы лексем
						LT::Add(Tables.Lextable, entrylt); //занесение в таблицу лексем
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].count_parm = 3;
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[0] = 'q';
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[1] = 'n';
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[2] = 'n';
						break;
					}

					case LEX_REPLACE: // лексема функции replace
					{
						MyFunc = 1;
						IT::Entry entryit("replace", i, IT::STR, IT::S); // создаём элемент ТИ с занесением всех собранных данных
						IT::Add(Tables.IDtable, entryit);						 // добавляем созданный элемент
						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // создание
																																		 // элемента таблицы лексем
						LT::Add(Tables.Lextable, entrylt); //занесение в таблицу лексем
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].count_parm = 3;
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[0] = 'q';
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[1] = 'q';
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[2] = 'q';
						break;
					}

					case LEX_ID://идентификтор
					{						
						
						if (Tables.Lextable.table[i - 1].lexema != LEX_FUNCTION && IT::IsId(Tables.IDtable, InStruct.tokens[i].token) == -1)
						{
							strncpy(buffer, tempTokenFunc, ID_MAXSIZE - 1);
							strcat_s(buffer, InStruct.tokens[i].token);
							strncpy(InStruct.tokens[i].token, buffer, ID_MAXSIZE - 1);
						}
						strncpy_s(tempToken, InStruct.tokens[i].token, ID_MAXSIZE - 1); // копирование имени идентифкатора
						strncpy_s(InStruct.tokens[i].token, InStruct.tokens[i].token, ID_MAXSIZE - 1); //копирование имени
						if (IT::IsId(Tables.IDtable, InStruct.tokens[i].token) == -1) // если данный идентификатор не найден повторно 
						{
							if (Tables.Lextable.table[i - 1].lexema == LEX_FUNCTION) // если предыдущей лексемой была лексема функциии
							{
								strncpy_s(tempTokenFunction, tempToken, ID_MAXSIZE - 1); // копируем обозначение принадлежности к имени идентификатора
								strncpy_s(tempTokenFunc, tempToken, ID_MAXSIZE - 1);

								funcType = idType;     // указываем тип идентификатора
								if (Tables.Lextable.table[i - 2].lexema == LEX_ID_TYPE_I) idType = IT::NUM;
								if (Tables.Lextable.table[i - 2].lexema == LEX_ID_TYPE_S) idType = IT::STR;
								IT::Entry entryit(tempTokenFunction, i, idType, IT::F); // создаём элемент ТИ с занесением всех собранных данных
								IT::Add(Tables.IDtable, entryit); // добавляем созданный элемент
								LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // создание
																																				 // элемента таблицы лексем
								LT::Add(Tables.Lextable, entrylt); //занесение в таблицу лексем
								isParam = true; // наличие параметров
								if (InStruct.tokens[i + 2].token[0] == LEX_RIGHTPARENT) // если лексема через одну лексема )
									isParam = false; // параметры закончились
								isFunc = true;
								break;

							}
							else if (Tables.Lextable.table[i - 1].lexema == LEX_ID_TYPE_I) //если предыдущая лесема была лексемой целого типа данных 
							{
								if (Tables.Lextable.table[i - 2].lexema == LEX_EXIST)//если предыдущая лесема была exist
								{
									if (Tables.Lextable.table[i - 2].lexema == LEX_SEPARATORS)//если предыдущая лесема была /*+-(){}...
									{
										std::cout << Tables.Lextable.table[i - 2].lexema << std::endl;
										Log::writeError(log.stream, Error::GetError(108, InStruct.tokens[i].line, NULL)); // если не сработал инф ошибка о том что
										flag1 = true;																		  //  небыло предварительного определения
									}
								}
								if (isParam == false) // если идентификатор типа переменная не является параметром
								{
									IT::Entry entryit(InStruct.tokens[i].token, i, idType, IT::V); // создаём элемент табл инд
									IT::Add(Tables.IDtable, entryit); //заносим в табл инд
								}
								else if (isParam == true) // иначе если параметр
								{
									int qwe = i;
									while (Tables.IDtable.table[qwe].idtype != IT::F)
										qwe--;
									Tables.IDtable.table[qwe].parms[kol] = 'n';
									kol++;
									if (kol == INT_MAXSIZE_PARM)
									{
										Log::writeError(log.stream, Error::GetError(115, InStruct.tokens[i].line, NULL));
										flag1 = true;
									}
									IT::Entry entryit(InStruct.tokens[i].token, i, idType, IT::P);// создаём элемент табл инд
									IT::Add(Tables.IDtable, entryit);//заносим в табл инд
																	 //
									if (InStruct.tokens[i + 1].token[0] == LEX_RIGHTPARENT) // если следующая лексема лексема )
									{
										isParam = false; // параметры закончились
										int qwe = i;
										while (Tables.IDtable.table[qwe].idtype != IT::F)
											qwe--;
										Tables.IDtable.table[qwe].count_parm = kol;
										kol = 0;
									}
								}

								LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // создание элемента табл лекс
								LT::Add(Tables.Lextable, entrylt); // добавление в табл лекс
								break;
							}
							else if (Tables.Lextable.table[i - 1].lexema == LEX_ID_TYPE_S) //если предыдущая лесема была лексемой стокового типа данных
							{
								if (isParam == false) // если идентификатор типа переменная не является параметром
								{
									IT::Entry entryit(InStruct.tokens[i].token, i, idType, IT::V); // создаём элемент табл инд
									IT::Add(Tables.IDtable, entryit); //заносим в табл инд
								}
								else if (isParam == true) // иначе если параметр
								{
									int qwe = i;
									while (Tables.IDtable.table[qwe].idtype != IT::F)
										qwe--;
									Tables.IDtable.table[qwe].parms[kol] = 'n';
									kol++;
									if (kol == INT_MAXSIZE_PARM)
									{
										Log::writeError(log.stream, Error::GetError(103, InStruct.tokens[i].line, NULL));
										flag1 = true;
									}
									IT::Entry entryit(InStruct.tokens[i].token, i, idType, IT::P);// создаём элемент табл инд
									IT::Add(Tables.IDtable, entryit);//заносим в табл инд
																	 //
									if (InStruct.tokens[i + 1].token[0] == LEX_RIGHTPARENT) // если следующая лексема лексема )
									{
										isParam = false; // параметры закончились
										int qwe = i;
										while (Tables.IDtable.table[qwe].idtype != IT::F)
											qwe--;
										Tables.IDtable.table[qwe].count_parm = kol;
										kol = 0;
									}
								}

								LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // создание элемента табл лекс
								LT::Add(Tables.Lextable, entrylt); // добавление в табл лекс
								break;
							}
							Log::writeError(log.stream, Error::GetError(108, InStruct.tokens[i].line, NULL)); // если не сработал иф ошибка о том что
							flag1 = true;																		  //  небыло предварительного определения
						}
						// далее проверка на переопределение	
						else // в противном случае 
						{
							if (Tables.Lextable.table[i - 1].lexema == LEX_ID_TYPE_I || Tables.Lextable.table[i - 1].lexema == LEX_ID_TYPE_S || Tables.Lextable.table[i - 1].lexema == LEX_FUNCTION) // если предыдущим
																																																	 //был тип данных или лексема функции
							{
								Log::writeError(log.stream, Error::GetError(107, InStruct.tokens[i].line, NULL)); //выдаём ошибку о попытке переопределения
								flag1 = true;
							}
							LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); //создание элемента табл лексем
							LT::Add(Tables.Lextable, entrylt); // занесение элемента в таблицу лексем
							break;
						}
						break;
					}
					case LEX_LITERAL: // лексема литерала
					{
						switch (Tables.Lextable.table[i - 1].lexema) // проверим предыдущие лексемы
						{
						case LEX_EQUAL: // лексема =
						{
							IT::Entry entryit(LITERAL, i, graph[j].graph.type, IT::L); // создание элемента табл ид с пометкой литерал
							IT::Add(Tables.IDtable, entryit); //добавление в таблицу
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, InStruct.tokens[i].token); // установление значения литерала
							LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, Tables.IDtable.size - 1); // создание элемента табл лекс
							LT::Add(Tables.Lextable, entrylt); // добавление элемента в табл лекс
							if (Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token)].idtype == IT::F) // если предыдущая была лекс ф-ции
							{
								Log::writeError(log.stream, Error::GetError(115, InStruct.tokens[i].line, NULL)); // ошибка !!!  нельзя присваивать значение ф-ции
								flag1 = true;
							}
							if (Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token)].idtype == IT::V ||    //если перед = была переменная или параметр
								Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token)].idtype == IT::P)
							{
								IT::SetValue(Tables.IDtable, IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token), InStruct.tokens[i].token); // устанавливаем им их значение в поле табл идентификаторов
							}
							break;
						}

						// если предыдущие от литерала 
						case LEX_LEFTPARENT:  // лексема левой круглой скобочки
						case LEX_COMMA: //лексема запятой
						case LEX_WRITE:  // лексема оператора вывода
						case LEX_RETURN: //лексема оператора возвращения значения
						{

							IT::Entry entryit(LITERAL, i, graph[j].graph.type, IT::L); // создание элемента табл ид с пометкой литерал
							IT::Add(Tables.IDtable, entryit); //добавление в таблицу
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, InStruct.tokens[i].token); // установление значения литерала
							LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, Tables.IDtable.size - 1); // создание элемента табл лекс
							LT::Add(Tables.Lextable, entrylt); // добавление элемента в табл лекс
							break;
						}
						default:
							IT::Entry entryit(LITERAL, i, graph[j].graph.type, IT::L); // создание элемента табл ид с пометкой литерал
							IT::Add(Tables.IDtable, entryit); //добавление в таблицу
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, InStruct.tokens[i].token); // установление значения литерала
							LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, Tables.IDtable.size - 1); // создание элемента табл лекс
							LT::Add(Tables.Lextable, entrylt); // добавление элемента в табл лекс
						}
						break;
					}

					case LEX_SEPARATORS: // лексема сепараторов
					{
						if (Tables.Lextable.table[i - 1].lexema == LEX_SEPARATORS)
							Error::GetError(605, InStruct.tokens[i].line, NULL);
						switch (InStruct.tokens[i].token[0])
						{
						case LEX_SEMICOLON:
						{
							if (Tables.Lextable.table[i - 2].lexema == LEX_RETURN) // если  вернуться на 2 лекс назад и будет лексема return
							{
								int qwe = i; int qwe1 = i + 1;
								while (Tables.IDtable.table[qwe].idtype != IT::F)
									qwe--;
								if (Tables.IDtable.table[qwe].iddatatype != Tables.IDtable.table[Tables.Lextable.table[i - 1].idxTI].iddatatype)
								{
									Log::writeError(log.stream, Error::GetError(405, InStruct.tokens[i].line, NULL));  // возвращаемое значение не совпадает с типом функции
									flag1 = true;
								}
								kol = 0;
								IT::IDDATATYPE retType; // созданим воращаемое значение ф-ции
								if (Tables.Lextable.table[i - 1].lexema == LEX_LITERAL) // если предыдущая литерал
									retType = Tables.IDtable.table[Tables.IDtable.size - 1].iddatatype; // данный литерал и есть возвр ф-ции значение
							}
						}

						default:
						{
							LT::Entry entrylt(InStruct.tokens[i].token[0], InStruct.tokens[i].line); // сохдаём элемент табл лексем
							LT::Add(Tables.Lextable, entrylt); // занесение элемента в таблицу лексем
						}
						}
						break;
					}
					case LEX_ID_TYPE_S: // лексема типа данных string
					case LEX_ID_TYPE_I: // лексема типа данных integer
						idType = graph[j].graph.type; // заполнение поля типа данных
					default:
					{
						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line); // создание эл. табл лекс.
						LT::Add(Tables.Lextable, entrylt); // занесение в табл. лекс
						break;
					}
					}
					break;
				}
			}
		}
		if (!MainBody) // если небыло гл ф-ции
		{
			flag1 = true;
			Log::writeError(log.stream, Error::GetError(400)); // ошибка отсутствие главной ф-ции
		}
		if (LibBody == 0 && MyFunc == 1)
		{
			Log::writeError(log.stream, Error::GetError(402));
		}// не подключена библиотека
		for (int i = 0; i < Tables.Lextable.size; i++)
		{

			if (Tables.Lextable.table[i].lexema == LEX_LITERAL && InStruct.tokens[i].token[0] == char(48)
				&& Tables.Lextable.table[i - 1].lexema == LEX_DIVISION)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(408, InStruct.tokens[i + 1].line, NULL));
			}

			if (Tables.Lextable.table[i].lexema == LEX_EQUAL && Tables.Lextable.table[i - 1].lexema == LEX_COMMA)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			}
			if (Tables.Lextable.table[i].lexema == LEX_COMMA && Tables.Lextable.table[i - 1].lexema == LEX_EQUAL)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			}
			if (Tables.Lextable.table[i].lexema == LEX_SEMICOLON && Tables.Lextable.table[i - 1].lexema == LEX_SEMICOLON)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			}
			//if (Tables.Lextable.table[i].lexema == LEX_LEFTPARENT && Tables.Lextable.table[i - 1].lexema == LEX_LEFTPARENT)
			//{
			//	flag2 = true;
			//	Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			//}
			if (Tables.Lextable.table[i].lexema == LEX_COMMA && Tables.Lextable.table[i - 1].lexema == LEX_COMMA)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			}
			//if (Tables.Lextable.table[i].lexema == LEX_RIGHTPARENT && Tables.Lextable.table[i - 1].lexema == LEX_RIGHTPARENT)
			//{
			//	flag2 = true;
			//	Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			//}
			if (Tables.Lextable.table[i].lexema == LEX_PLUS && Tables.Lextable.table[i - 1].lexema == LEX_PLUS)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			}
			if (Tables.Lextable.table[i].lexema == LEX_MINUS && Tables.Lextable.table[i - 1].lexema == LEX_MINUS)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			}
			if (Tables.Lextable.table[i].lexema == LEX_MULTIPLY && Tables.Lextable.table[i - 1].lexema == LEX_MULTIPLY)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			}
			if (Tables.Lextable.table[i].lexema == LEX_DIVISION && Tables.Lextable.table[i - 1].lexema == LEX_DIVISION)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			}

		    if (Tables.Lextable.table[i - 2].lexema == LEX_EQUAL && Tables.Lextable.table[i - 3].lexema == LEX_EQUAL)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			}
			if (Tables.Lextable.table[i].lexema == LEX_RIGHTBRACE && Tables.Lextable.table[i - 1].lexema == LEX_RIGHTBRACE)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			}
			if (Tables.Lextable.table[i].lexema == LEX_LEFTBRACE && Tables.Lextable.table[i - 1].lexema == LEX_LEFTBRACE)
			{
				flag2 = true;
				Log::writeError(log.stream, Error::GetError(106, InStruct.tokens[i + 1].line, NULL));
			}
		}

		if (flag1 == false && flag2 == false)
		{
			std::cout << "Проект собран! "<<endl;
			
			for (int i = 0; i < Tables.Lextable.size; i++) // проверка выражний и не стандратных функций
			{

				if (Tables.Lextable.table[i].lexema == LEX_ID || Tables.Lextable.table[i].lexema == LEX_LITERAL)
				{
					if (Tables.IDtable.table[Tables.Lextable.table[i].idxTI].idtype == IT::F)
					{
						if (Tables.Lextable.table[i + 1].lexema != LEX_LEFTPARENT)
							Log::writeError(log.stream, Error::GetError(409, InStruct.tokens[i + 1].line, NULL));
					}
				}

				if (Tables.Lextable.table[i].lexema == LEX_EQUAL && Tables.Lextable.table[i - 1].lexema == LEX_EQUAL)
					flag2 = true;
				if (Tables.Lextable.table[i].lexema == LEX_EQUAL && Tables.Lextable.table[i - 1].lexema == LEX_ID && Tables.Lextable.table[i + 1].lexema != LEX_LEFTPARENT) // если мы словили равно, а до равно стоит идентификатор
				{
					if (Tables.Lextable.table[i + 1].lexema == LEX_MINUS)
						Log::writeError(log.stream, Error::GetError(118, InStruct.tokens[i + 1].line, NULL));
					if (Tables.IDtable.table[Tables.Lextable.table[i - 1].idxTI].iddatatype == IT::STR) // и этот идентификатор строкового типа 
					{
						if (Tables.IDtable.table[Tables.Lextable.table[i + 1].idxTI].iddatatype != IT::STR) // а после равно идентификатор не равный типу 
							Log::writeError(log.stream, Error::GetError(401, InStruct.tokens[i + 1].line, NULL));
						if (Tables.Lextable.table[i + 2].lexema != LEX_SEMICOLON && Tables.IDtable.table[Tables.Lextable.table[i + 1].idxTI].idtype != IT::S
							&& Tables.IDtable.table[Tables.Lextable.table[i + 1].idxTI].idtype != IT::F) // или же функция
							Log::writeError(log.stream, Error::GetError(404, InStruct.tokens[i + 2].line, NULL));
					}
					if (Tables.IDtable.table[Tables.Lextable.table[i - 1].idxTI].iddatatype == IT::NUM) // и этот идентификатор целочисленного типа 
					{					
						if (Tables.IDtable.table[Tables.Lextable.table[i + 1].idxTI].iddatatype != IT::NUM){//если следующий строкового типа, то ошибка 
							
					     Log::writeError(log.stream, Error::GetError(401, InStruct.tokens[i + 1].line, NULL));
							}
						

						int j = i, k = i;
						while (Tables.Lextable.table[j].lexema != LEX_SEMICOLON) //ищем ;
						{
							if (Tables.Lextable.table[j].lexema == LEX_RIGHTBRACE)//если раньше нашли закрывающуй конструкцию, то ошибка
								Log::writeError(log.stream, Error::GetError(119, InStruct.tokens[i + 1].line, NULL));
							j++;
						}
						bool param = false;
						if (Tables.Lextable.table[j].lexema == LEX_SEMICOLON) // если нашли ;
						{
							while (k != j) // запускаем цикл провеерки выражения
							{
							
								if (Tables.Lextable.table[k].lexema == LEX_RIGHTPARENT) // если это были параметры функции
									param = false;
								if (Tables.Lextable.table[k].lexema == LEX_ID || Tables.Lextable.table[k].lexema == LEX_LITERAL) // если это идентификатор
								{																
										if (Tables.IDtable.table[Tables.Lextable.table[k].idxTI].idtype == IT::F)
										{
											if (Tables.Lextable.table[k + 1].lexema == LEX_LEFTPARENT)
											{
												param = true;
											}
											else
											{
												Log::writeError(log.stream, Error::GetError(119, InStruct.tokens[i + 1].line, NULL));
											}
											
										}

									
								}
								k++;
							}
						}
					}
				}
			}
			bool flag = false; 
			int some_count = 0; 
			char some_buffer[INT_MAXSIZE_PARM];
			for (int i = 0; i < Tables.Lextable.size; i++)
			{

				if (Tables.Lextable.table[i].lexema == LEX_FUNCTION && flag == false && flag2 == false)
				{

					int j = i, funct = i;
					while (Tables.Lextable.table[j].lexema != LEX_RIGHTPARENT)
						j++;
					int count = 0;
					for (int p = i; p != j; p++)
					{
						if (Tables.Lextable.table[p].lexema == LEX_ID_TYPE_I)
						{
							Tables.IDtable.table[Tables.Lextable.table[funct + 1].idxTI].parms[count] = 'n';
							count++;
						}
						if (Tables.Lextable.table[p].lexema == LEX_ID_TYPE_S)
						{
							Tables.IDtable.table[Tables.Lextable.table[funct + 1].idxTI].parms[count] = 'q';
							count++;
						}
					}
				}
				if (Tables.Lextable.table[i].lexema == LEX_MAIN)
					flag = true;

				if (flag == true)
				{
					if (Tables.Lextable.table[i].lexema == LEX_ID || Tables.Lextable.table[i].lexema == LEX_LITERAL)
					{
						if (Tables.IDtable.table[Tables.Lextable.table[i].idxTI].idtype == IT::F)
						{
							int param = 0;
							if (Tables.Lextable.table[i + 1].lexema == LEX_LEFTPARENT)
							{
								int k = i + 1;
								while (Tables.Lextable.table[k].lexema != LEX_RIGHTPARENT)
								{
									if (Tables.Lextable.table[k].lexema == LEX_ID || Tables.Lextable.table[k].lexema == LEX_LITERAL)
									{
										if (Tables.IDtable.table[Tables.Lextable.table[k].idxTI].iddatatype == IT::STR)
											some_buffer[some_count] = 'q';
										if (Tables.IDtable.table[Tables.Lextable.table[k].idxTI].iddatatype == IT::NUM)
											some_buffer[some_count] = 'n';
										some_count++;
									}
									k++;
								}
								if (Tables.IDtable.table[Tables.Lextable.table[i].idxTI].count_parm < 0)
									Tables.IDtable.table[Tables.Lextable.table[i].idxTI].count_parm = 0;
								if (some_count != Tables.IDtable.table[Tables.Lextable.table[i].idxTI].count_parm)
								{
									Log::writeError(log.stream, Error::GetError(406, InStruct.tokens[i + 1].line, NULL));
								}
								for (int r = 0; r < some_count; r++)
									if (some_buffer[r] != Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[r])
									{
										Log::writeError(log.stream, Error::GetError(407, InStruct.tokens[i + 1].line, NULL));
									}
								some_count = 0;
							}
						}
					}
				}
			}
		}
		return Tables; // возвращаем ТИ ТЛ
	}
}