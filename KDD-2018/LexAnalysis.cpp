#include "stdafx.h"	
#include "Graphs.h"
#include "FST.h"
#include "LexAnalysis.h"
#include "Error.h"

namespace Lex
{
	bool flag2 = false;
	bool flag1 = false;
	Graph graph[N_GRAPHS] =   // ����� ��� �������� ���������
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
	LEX Begin_Lex(In::IN &InStruct, Log::LOG &log, Parm::PARM &parm) //������ ������������ �������
	{
		LEX Tables;
		Tables.Lextable = LT::Create(LT_MAXSIZE);  // �������� ������� ������
		Tables.IDtable = IT::Create(TI_MAXSIZE);  // �������� ������� ���������������
		bool isFunc = false;      // ���� �� �������
		bool isParam = false;     //���� �� ��������
		int count_parm = 0;
		int MainBody = NULL;
		int MyFunc = NULL;
		int LibBody = NULL;
		int funcend = NULL;
		char buffer[255] = "";
		char tempTokenFunc[255] = ""; //����� ��� ����� �������
		char tempTokenFunction[255] = "";   //�����
		char tempToken[255] = "";    //�����

		IT::IDDATATYPE funcType;   // ��� �������
		IT::IDDATATYPE idType;   // ��� ��������������

		for (int i = 0; i < In::StToken::tokensSize; i++) // �������� �������� ������� ����
		{
			for (int j = 0; j < N_GRAPHS; j++)    // ���� ���������� ������� ��� �����	
			{
				FST::FST fstex(InStruct.tokens[i].token, graph[j].graph);   // ������� �������
				if (FST::execute(fstex)) // ���� ������ �������� �� 
				{
					switch (graph[j].lexema)     // ���� ���������� ����� ��� �������
					{

					case LEX_MAIN:   // ���� ������� �-���
					{
						MainBody++;   //�������� � 0 �� 1 , �� ��������� , ���������� ����� 1 �� �-���
						if (MainBody > 1)    // ���� ������� ����� 1-�� ������� �-���
							Log::writeError(log.stream, Error::GetError(116, InStruct.tokens[i].line, NULL)); // ������ ������ � ������ ������

						strcpy_s(tempTokenFunc, "");
						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line);   // ������ � ������� , � ��������� ������ ������ 
						LT::Add(Tables.Lextable, entrylt);    // ���������� ������� � ������� 
						break; //�����
					}
					case LEX_LIB:   // ���� ����������� ����������
					{
						LibBody++;
						if (LibBody > 1)    // ���� ������� ����� 1-�� ����������
							Log::writeError(log.stream, Error::GetError(403, InStruct.tokens[i].line, NULL)); // ������ ������ � ������ ������

						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line);// ������ � ������� , � ��������� ������ ������ 
						LT::Add(Tables.Lextable, entrylt);// ���������� ������� � ������� 
						break; //�����
					}



					case LEX_LENGTH: // ������� ������� substr
					{
						MyFunc = 1;
						IT::Entry entryit("length", i, IT::NUM, IT::S); // ������ ������� �� � ���������� ���� ��������� ������
						IT::Add(Tables.IDtable, entryit);						 // ��������� ��������� �������
						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // ��������
																																		 // �������� ������� ������
						LT::Add(Tables.Lextable, entrylt); //��������� � ������� ������
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].count_parm = 1;
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[0] = 'q';
						break;
					}

					case LEX_SUBSTR: // ������� ������� substr
					{
						MyFunc = 1;
						IT::Entry entryit("substr", i, IT::STR, IT::S); // ������ ������� �� � ���������� ���� ��������� ������
						IT::Add(Tables.IDtable, entryit);						 // ��������� ��������� �������
						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // ��������
																																		 // �������� ������� ������
						LT::Add(Tables.Lextable, entrylt); //��������� � ������� ������
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].count_parm = 3;
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[0] = 'q';
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[1] = 'n';
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[2] = 'n';
						break;
					}

					case LEX_REPLACE: // ������� ������� replace
					{
						MyFunc = 1;
						IT::Entry entryit("replace", i, IT::STR, IT::S); // ������ ������� �� � ���������� ���� ��������� ������
						IT::Add(Tables.IDtable, entryit);						 // ��������� ��������� �������
						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // ��������
																																		 // �������� ������� ������
						LT::Add(Tables.Lextable, entrylt); //��������� � ������� ������
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].count_parm = 3;
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[0] = 'q';
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[1] = 'q';
						Tables.IDtable.table[Tables.Lextable.table[i].idxTI].parms[2] = 'q';
						break;
					}

					case LEX_ID://������������
					{						
						
						if (Tables.Lextable.table[i - 1].lexema != LEX_FUNCTION && IT::IsId(Tables.IDtable, InStruct.tokens[i].token) == -1)
						{
							strncpy(buffer, tempTokenFunc, ID_MAXSIZE - 1);
							strcat_s(buffer, InStruct.tokens[i].token);
							strncpy(InStruct.tokens[i].token, buffer, ID_MAXSIZE - 1);
						}
						strncpy_s(tempToken, InStruct.tokens[i].token, ID_MAXSIZE - 1); // ����������� ����� �������������
						strncpy_s(InStruct.tokens[i].token, InStruct.tokens[i].token, ID_MAXSIZE - 1); //����������� �����
						if (IT::IsId(Tables.IDtable, InStruct.tokens[i].token) == -1) // ���� ������ ������������� �� ������ �������� 
						{
							if (Tables.Lextable.table[i - 1].lexema == LEX_FUNCTION) // ���� ���������� �������� ���� ������� ��������
							{
								strncpy_s(tempTokenFunction, tempToken, ID_MAXSIZE - 1); // �������� ����������� �������������� � ����� ��������������
								strncpy_s(tempTokenFunc, tempToken, ID_MAXSIZE - 1);

								funcType = idType;     // ��������� ��� ��������������
								if (Tables.Lextable.table[i - 2].lexema == LEX_ID_TYPE_I) idType = IT::NUM;
								if (Tables.Lextable.table[i - 2].lexema == LEX_ID_TYPE_S) idType = IT::STR;
								IT::Entry entryit(tempTokenFunction, i, idType, IT::F); // ������ ������� �� � ���������� ���� ��������� ������
								IT::Add(Tables.IDtable, entryit); // ��������� ��������� �������
								LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // ��������
																																				 // �������� ������� ������
								LT::Add(Tables.Lextable, entrylt); //��������� � ������� ������
								isParam = true; // ������� ����������
								if (InStruct.tokens[i + 2].token[0] == LEX_RIGHTPARENT) // ���� ������� ����� ���� ������� )
									isParam = false; // ��������� �����������
								isFunc = true;
								break;

							}
							else if (Tables.Lextable.table[i - 1].lexema == LEX_ID_TYPE_I) //���� ���������� ������ ���� �������� ������ ���� ������ 
							{
								if (Tables.Lextable.table[i - 2].lexema == LEX_EXIST)//���� ���������� ������ ���� exist
								{
									if (Tables.Lextable.table[i - 2].lexema == LEX_SEPARATORS)//���� ���������� ������ ���� /*+-(){}...
									{
										std::cout << Tables.Lextable.table[i - 2].lexema << std::endl;
										Log::writeError(log.stream, Error::GetError(108, InStruct.tokens[i].line, NULL)); // ���� �� �������� ��� ������ � ��� ���
										flag1 = true;																		  //  ������ ���������������� �����������
									}
								}
								if (isParam == false) // ���� ������������� ���� ���������� �� �������� ����������
								{
									IT::Entry entryit(InStruct.tokens[i].token, i, idType, IT::V); // ������ ������� ���� ���
									IT::Add(Tables.IDtable, entryit); //������� � ���� ���
								}
								else if (isParam == true) // ����� ���� ��������
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
									IT::Entry entryit(InStruct.tokens[i].token, i, idType, IT::P);// ������ ������� ���� ���
									IT::Add(Tables.IDtable, entryit);//������� � ���� ���
																	 //
									if (InStruct.tokens[i + 1].token[0] == LEX_RIGHTPARENT) // ���� ��������� ������� ������� )
									{
										isParam = false; // ��������� �����������
										int qwe = i;
										while (Tables.IDtable.table[qwe].idtype != IT::F)
											qwe--;
										Tables.IDtable.table[qwe].count_parm = kol;
										kol = 0;
									}
								}

								LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // �������� �������� ���� ����
								LT::Add(Tables.Lextable, entrylt); // ���������� � ���� ����
								break;
							}
							else if (Tables.Lextable.table[i - 1].lexema == LEX_ID_TYPE_S) //���� ���������� ������ ���� �������� ��������� ���� ������
							{
								if (isParam == false) // ���� ������������� ���� ���������� �� �������� ����������
								{
									IT::Entry entryit(InStruct.tokens[i].token, i, idType, IT::V); // ������ ������� ���� ���
									IT::Add(Tables.IDtable, entryit); //������� � ���� ���
								}
								else if (isParam == true) // ����� ���� ��������
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
									IT::Entry entryit(InStruct.tokens[i].token, i, idType, IT::P);// ������ ������� ���� ���
									IT::Add(Tables.IDtable, entryit);//������� � ���� ���
																	 //
									if (InStruct.tokens[i + 1].token[0] == LEX_RIGHTPARENT) // ���� ��������� ������� ������� )
									{
										isParam = false; // ��������� �����������
										int qwe = i;
										while (Tables.IDtable.table[qwe].idtype != IT::F)
											qwe--;
										Tables.IDtable.table[qwe].count_parm = kol;
										kol = 0;
									}
								}

								LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); // �������� �������� ���� ����
								LT::Add(Tables.Lextable, entrylt); // ���������� � ���� ����
								break;
							}
							Log::writeError(log.stream, Error::GetError(108, InStruct.tokens[i].line, NULL)); // ���� �� �������� �� ������ � ��� ���
							flag1 = true;																		  //  ������ ���������������� �����������
						}
						// ����� �������� �� ���������������	
						else // � ��������� ������ 
						{
							if (Tables.Lextable.table[i - 1].lexema == LEX_ID_TYPE_I || Tables.Lextable.table[i - 1].lexema == LEX_ID_TYPE_S || Tables.Lextable.table[i - 1].lexema == LEX_FUNCTION) // ���� ����������
																																																	 //��� ��� ������ ��� ������� �������
							{
								Log::writeError(log.stream, Error::GetError(107, InStruct.tokens[i].line, NULL)); //����� ������ � ������� ���������������
								flag1 = true;
							}
							LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, IT::IsId(Tables.IDtable, InStruct.tokens[i].token)); //�������� �������� ���� ������
							LT::Add(Tables.Lextable, entrylt); // ��������� �������� � ������� ������
							break;
						}
						break;
					}
					case LEX_LITERAL: // ������� ��������
					{
						switch (Tables.Lextable.table[i - 1].lexema) // �������� ���������� �������
						{
						case LEX_EQUAL: // ������� =
						{
							IT::Entry entryit(LITERAL, i, graph[j].graph.type, IT::L); // �������� �������� ���� �� � �������� �������
							IT::Add(Tables.IDtable, entryit); //���������� � �������
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, InStruct.tokens[i].token); // ������������ �������� ��������
							LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, Tables.IDtable.size - 1); // �������� �������� ���� ����
							LT::Add(Tables.Lextable, entrylt); // ���������� �������� � ���� ����
							if (Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token)].idtype == IT::F) // ���� ���������� ���� ���� �-���
							{
								Log::writeError(log.stream, Error::GetError(115, InStruct.tokens[i].line, NULL)); // ������ !!!  ������ ����������� �������� �-���
								flag1 = true;
							}
							if (Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token)].idtype == IT::V ||    //���� ����� = ���� ���������� ��� ��������
								Tables.IDtable.table[IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token)].idtype == IT::P)
							{
								IT::SetValue(Tables.IDtable, IT::IsId(Tables.IDtable, InStruct.tokens[i - 2].token), InStruct.tokens[i].token); // ������������� �� �� �������� � ���� ���� ���������������
							}
							break;
						}

						// ���� ���������� �� �������� 
						case LEX_LEFTPARENT:  // ������� ����� ������� ��������
						case LEX_COMMA: //������� �������
						case LEX_WRITE:  // ������� ��������� ������
						case LEX_RETURN: //������� ��������� ����������� ��������
						{

							IT::Entry entryit(LITERAL, i, graph[j].graph.type, IT::L); // �������� �������� ���� �� � �������� �������
							IT::Add(Tables.IDtable, entryit); //���������� � �������
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, InStruct.tokens[i].token); // ������������ �������� ��������
							LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, Tables.IDtable.size - 1); // �������� �������� ���� ����
							LT::Add(Tables.Lextable, entrylt); // ���������� �������� � ���� ����
							break;
						}
						default:
							IT::Entry entryit(LITERAL, i, graph[j].graph.type, IT::L); // �������� �������� ���� �� � �������� �������
							IT::Add(Tables.IDtable, entryit); //���������� � �������
							IT::SetValue(Tables.IDtable, Tables.IDtable.size - 1, InStruct.tokens[i].token); // ������������ �������� ��������
							LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line, Tables.IDtable.size - 1); // �������� �������� ���� ����
							LT::Add(Tables.Lextable, entrylt); // ���������� �������� � ���� ����
						}
						break;
					}

					case LEX_SEPARATORS: // ������� �����������
					{
						if (Tables.Lextable.table[i - 1].lexema == LEX_SEPARATORS)
							Error::GetError(605, InStruct.tokens[i].line, NULL);
						switch (InStruct.tokens[i].token[0])
						{
						case LEX_SEMICOLON:
						{
							if (Tables.Lextable.table[i - 2].lexema == LEX_RETURN) // ����  ��������� �� 2 ���� ����� � ����� ������� return
							{
								int qwe = i; int qwe1 = i + 1;
								while (Tables.IDtable.table[qwe].idtype != IT::F)
									qwe--;
								if (Tables.IDtable.table[qwe].iddatatype != Tables.IDtable.table[Tables.Lextable.table[i - 1].idxTI].iddatatype)
								{
									Log::writeError(log.stream, Error::GetError(405, InStruct.tokens[i].line, NULL));  // ������������ �������� �� ��������� � ����� �������
									flag1 = true;
								}
								kol = 0;
								IT::IDDATATYPE retType; // �������� ���������� �������� �-���
								if (Tables.Lextable.table[i - 1].lexema == LEX_LITERAL) // ���� ���������� �������
									retType = Tables.IDtable.table[Tables.IDtable.size - 1].iddatatype; // ������ ������� � ���� ����� �-��� ��������
							}
						}

						default:
						{
							LT::Entry entrylt(InStruct.tokens[i].token[0], InStruct.tokens[i].line); // ������ ������� ���� ������
							LT::Add(Tables.Lextable, entrylt); // ��������� �������� � ������� ������
						}
						}
						break;
					}
					case LEX_ID_TYPE_S: // ������� ���� ������ string
					case LEX_ID_TYPE_I: // ������� ���� ������ integer
						idType = graph[j].graph.type; // ���������� ���� ���� ������
					default:
					{
						LT::Entry entrylt(graph[j].lexema, InStruct.tokens[i].line); // �������� ��. ���� ����.
						LT::Add(Tables.Lextable, entrylt); // ��������� � ����. ����
						break;
					}
					}
					break;
				}
			}
		}
		if (!MainBody) // ���� ������ �� �-���
		{
			flag1 = true;
			Log::writeError(log.stream, Error::GetError(400)); // ������ ���������� ������� �-���
		}
		if (LibBody == 0 && MyFunc == 1)
		{
			Log::writeError(log.stream, Error::GetError(402));
		}// �� ���������� ����������
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
			std::cout << "������ ������! "<<endl;
			
			for (int i = 0; i < Tables.Lextable.size; i++) // �������� �������� � �� ����������� �������
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
				if (Tables.Lextable.table[i].lexema == LEX_EQUAL && Tables.Lextable.table[i - 1].lexema == LEX_ID && Tables.Lextable.table[i + 1].lexema != LEX_LEFTPARENT) // ���� �� ������� �����, � �� ����� ����� �������������
				{
					if (Tables.Lextable.table[i + 1].lexema == LEX_MINUS)
						Log::writeError(log.stream, Error::GetError(118, InStruct.tokens[i + 1].line, NULL));
					if (Tables.IDtable.table[Tables.Lextable.table[i - 1].idxTI].iddatatype == IT::STR) // � ���� ������������� ���������� ���� 
					{
						if (Tables.IDtable.table[Tables.Lextable.table[i + 1].idxTI].iddatatype != IT::STR) // � ����� ����� ������������� �� ������ ���� 
							Log::writeError(log.stream, Error::GetError(401, InStruct.tokens[i + 1].line, NULL));
						if (Tables.Lextable.table[i + 2].lexema != LEX_SEMICOLON && Tables.IDtable.table[Tables.Lextable.table[i + 1].idxTI].idtype != IT::S
							&& Tables.IDtable.table[Tables.Lextable.table[i + 1].idxTI].idtype != IT::F) // ��� �� �������
							Log::writeError(log.stream, Error::GetError(404, InStruct.tokens[i + 2].line, NULL));
					}
					if (Tables.IDtable.table[Tables.Lextable.table[i - 1].idxTI].iddatatype == IT::NUM) // � ���� ������������� �������������� ���� 
					{					
						if (Tables.IDtable.table[Tables.Lextable.table[i + 1].idxTI].iddatatype != IT::NUM){//���� ��������� ���������� ����, �� ������ 
							
					     Log::writeError(log.stream, Error::GetError(401, InStruct.tokens[i + 1].line, NULL));
							}
						

						int j = i, k = i;
						while (Tables.Lextable.table[j].lexema != LEX_SEMICOLON) //���� ;
						{
							if (Tables.Lextable.table[j].lexema == LEX_RIGHTBRACE)//���� ������ ����� ����������� �����������, �� ������
								Log::writeError(log.stream, Error::GetError(119, InStruct.tokens[i + 1].line, NULL));
							j++;
						}
						bool param = false;
						if (Tables.Lextable.table[j].lexema == LEX_SEMICOLON) // ���� ����� ;
						{
							while (k != j) // ��������� ���� ��������� ���������
							{
							
								if (Tables.Lextable.table[k].lexema == LEX_RIGHTPARENT) // ���� ��� ���� ��������� �������
									param = false;
								if (Tables.Lextable.table[k].lexema == LEX_ID || Tables.Lextable.table[k].lexema == LEX_LITERAL) // ���� ��� �������������
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
		return Tables; // ���������� �� ��
	}
}