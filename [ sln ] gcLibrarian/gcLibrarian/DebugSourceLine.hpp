#pragma once

//================================================================================
// gcFileDog:	(WCHAR) WHO >> (string) ToWHOM 
//================================================================================
//{
//	WCHAR* pWHO = WHO;
//	register int lengthWHO = 0;
//	do
//	{
//		++pWHO;
//		++lengthWHO;
//	} while (*pWHO != NULL);
//	pWHO = WHO;
//
//	char* conv = new char[lengthWHO + 1];
//	char* pConv = conv;
//	{
//		for (register uint i = 0; i < (uint)(lengthWHO * sizeof(*pWHO)); ++i)
//		{
//			if (i % 2 == 0)
//				*pConv++ = (char)*pWHO++;
//		}
//
//		*pConv = NULL;
//		string ToWHOM = string(conv);
//		ssDOG << ToWHOM;
//	}
//	delete[] conv;
//
//
//	//============================================================================
//	// End Point
//	//============================================================================
//	CString toPUSH(to_cstring(ssDOG.str()));
//	tFileDog_STACK.push_back(toPUSH);
//}