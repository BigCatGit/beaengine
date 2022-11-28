/*
 * BeaEngine 4 - x86 & x86-64 disassembler library
 *
 * Copyright 2006-2020, BeatriX
 * File coded by BeatriX
 *
 * This file is part of BeaEngine.
 *
 *    BeaEngine is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    BeaEngine is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with BeaEngine.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "BeaEngine/BeaEngine.h"
#include "Includes/protos.h"
#include "Includes/internal_datas.h"
#include "Includes/instr_set/Data_opcode.h"
#include "Includes/instr_set/instructions_list.c"
#include "Includes/instr_set/opcodes_Grp1.c"
#include "Includes/instr_set/opcodes_Grp2.c"
#include "Includes/instr_set/opcodes_Grp3.c"
#include "Includes/instr_set/opcodes_Grp4.c"
#include "Includes/instr_set/opcodes_Grp5.c"
#include "Includes/instr_set/opcodes_Grp6.c"
#include "Includes/instr_set/opcodes_Grp7.c"
#include "Includes/instr_set/opcodes_Grp8.c"
#include "Includes/instr_set/opcodes_Grp9.c"
#include "Includes/instr_set/opcodes_Grp12.c"
#include "Includes/instr_set/opcodes_Grp13.c"
#include "Includes/instr_set/opcodes_Grp14.c"
#include "Includes/instr_set/opcodes_Grp15.c"
#include "Includes/instr_set/opcodes_Grp16.c"
#include "Includes/instr_set/opcodes_Grp17.c"
#include "Includes/instr_set/opcodes_prefixes.c"
#include "Includes/Routines_ModRM.c"
#include "Includes/Routines_Disasm.c"
#include "Includes/BeaEngineVersion.c"

void BeaEngine(void){return;}

int __bea_callspec__ DisasmToString(char* codes, int len, bool isX64, unsigned __int64 option, unsigned __int64 virtualAddr, char* outBuff, int outBuffLen)
{
	DISASM Disasm_Info;
	char* end_offset = (char*)codes + len;
	(void)memset(&Disasm_Info, 0, sizeof(DISASM));
	Disasm_Info.EIP = (UInt64)codes;
	if (virtualAddr > 0) {
		Disasm_Info.VirtualAddr = virtualAddr;
	}
	Disasm_Info.Archi = isX64 ? 0 : 1;               // 1 = 表示反汇编32位 / 0 = 表示反汇编64位
	if (option)
	{
		Disasm_Info.Options = PrefixedNumeral;           // 默认语法格式 MasmSyntax, PrefixedNumeral: 为前缀加0x方式显示常量, 我们要行这种
	}
	else {
		Disasm_Info.Options = MasmSyntax;
	}
	int count = 0;

	while (!Disasm_Info.Error)
	{
		if (count >= outBuffLen - 2) {
			return 0;
		}
		Disasm_Info.SecurityBlock = (UInt64)end_offset - Disasm_Info.EIP;
		if (Disasm_Info.SecurityBlock <= 0)
			break;
		len = Disasm(&Disasm_Info);
		switch (Disasm_Info.Error)
		{
		case OUT_OF_BLOCK:
			break;
		case UNKNOWN_OPCODE:
			Disasm_Info.EIP += 1;
			Disasm_Info.Error = 0;
			if (virtualAddr > 0) {
				Disasm_Info.VirtualAddr += 1;
			}
			break;
		default:
			if (virtualAddr > 0) {
				char tmp[128] = { 0 };
				//printf("%.16llx > %s\n", Disasm_Info.VirtualAddr, &Disasm_Info.CompleteInstr);
				sprintf(tmp, "%.16llx > %s\r\n", Disasm_Info.VirtualAddr, &Disasm_Info.CompleteInstr);
				strcat(outBuff, tmp);
			}
			else {
				//printf("%s \n", &Disasm_Info.CompleteInstr);
				strcat(outBuff, Disasm_Info.CompleteInstr);
			}
			strcat(outBuff, "\r\n");
			count += strlen(Disasm_Info.CompleteInstr);
			Disasm_Info.EIP += len;
		}
	}
	return count;
}
