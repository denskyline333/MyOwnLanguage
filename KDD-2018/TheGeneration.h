#pragma once
#include "stdafx.h"
#include "IdentfTable.h"
#include "LexTable.h"
#include "In.h"

void StartPoint();
void EndPoint();
void Generation(LT::LexTable &Lextable, In::StToken *tokens, IT::IdTable & idtable);
