#pragma once

#include "stdafx.h"
#include "IdentfTable.h"
#include "LexTable.h"
#include "In.h"
#include "Log.h"
#include "FST.h"

static int kol;

namespace Lex
{
	struct LEX
	{
		LT::LexTable Lextable;
		IT::IdTable	IDtable;
	};

	struct Graph
	{
		char lexema;
		FST::FST graph;
	};

	LEX Begin_Lex(In::IN &InStruct, Log::LOG &log, Parm::PARM &parm);
}
