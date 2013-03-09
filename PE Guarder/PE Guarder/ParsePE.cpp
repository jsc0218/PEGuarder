#include "StdAfx.h"
#include "ParsePE.h"

// 构造函数
//
CParsePE::CParsePE(void)
{
	m_PE.bPEFile = FALSE;
	m_PE.pDosHeader = NULL;
	m_PE.pNtHeaders = NULL;
	m_PE.pFileHeader = NULL;
	m_PE.pOptionalHeader = NULL;
	m_PE.pEntryPoint = NULL;
	m_PE.nSectionCount = 0;
	m_PE.nImportCount = 0;
}


// 析构函数
//
CParsePE::~CParsePE(void)
{
	m_PE.bPEFile = FALSE;
	m_PE.pDosHeader = NULL;
	m_PE.pNtHeaders = NULL;
	m_PE.pFileHeader = NULL;
	m_PE.pOptionalHeader = NULL;
	m_PE.pEntryPoint = NULL;
	m_PE.nSectionCount = 0;
	m_PE.nImportCount = 0;
}


// 将一个内存中执行对象的PE结构进行分析
//
BOOL CParsePE::BasicParse(CMemFileObject* pScanObj)
{
	// 判断内存文件是否已打开
	if(pScanObj->IsOpened() == FALSE)
	{
		return FALSE;
	}

	DWORD dwObjSize = pScanObj->GetObjectSize();  // 扫描对象大小

	// 排除特例
	if(dwObjSize < sizeof(PIMAGE_DOS_HEADER))	
	{
		m_PE.bPEFile = FALSE;
		return FALSE;
	}

	m_PE.pDosHeader	= (PIMAGE_DOS_HEADER)pScanObj->GetBuffer();  // 返回内存指针

	// 检查"MZ"标志
	if(m_PE.pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		m_PE.bPEFile = FALSE;
		return FALSE;  // 不是PE文件
	}

	// 排除特例(dosx.exe)
	if((DWORD)m_PE.pDosHeader->e_lfanew > dwObjSize)
	{
		m_PE.bPEFile = FALSE;
		return FALSE;
	}

	// 基址+偏移量=NT头位置
	m_PE.pNtHeaders	= (PIMAGE_NT_HEADERS)
		(pScanObj->GetBuffer() + m_PE.pDosHeader->e_lfanew);

	// 检查"PE"标志
	if(m_PE.pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		m_PE.bPEFile = FALSE;
		return FALSE;  // 不是PE文件
	}
	else
	{
		m_PE.bPEFile = TRUE;  // 是PE文件
	}

	m_PE.pFileHeader = &(m_PE.pNtHeaders->FileHeader);  // 文件头指针取值
	m_PE.pOptionalHeader = &(m_PE.pNtHeaders->OptionalHeader);  // 可选头指针取值

	// 得到指向程序入口点的指针
	m_PE.pEntryPoint = pScanObj->GetBuffer() +  // 内存文件映射地址(基址)
		m_PE.pOptionalHeader->AddressOfEntryPoint;  // 程序入口RVA地址
	// 排除特例
	if(m_PE.pEntryPoint > (dwObjSize + pScanObj->GetBuffer()))
	{
		return FALSE;
	}

	// 得到块表中第一块的指针
	PIMAGE_SECTION_HEADER	pSectionHeader = (PIMAGE_SECTION_HEADER)((LPBYTE)m_PE.pNtHeaders + sizeof(IMAGE_NT_HEADERS));

	m_PE.nSectionCount = m_PE.pFileHeader->NumberOfSections;  // 得到块数
	// 填充块指针数组
	for(int i=0; i<m_PE.nSectionCount; i++)
	{
		m_PE.aSectionHeaders[i] = pSectionHeader;
		pSectionHeader++;
	}

	// 判断是否有导入表
	if(m_PE.pOptionalHeader->DataDirectory[1].VirtualAddress == 0)
	{
		m_PE.nImportCount = 0;  // 导入表数
	}
	else
	{
		// 得到第一个输入表
		PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(pScanObj->GetBuffer() +  // 内存文件映射地址(基址) 
			Rva2Offset(m_PE.pOptionalHeader->DataDirectory[1].VirtualAddress));  // 输入表rva转offset

		int j =0;
		// 填充导入表指针数组
		for(j=0; pImportDescriptor->Characteristics!=0; j++,pImportDescriptor++)
		{
			m_PE.aImportDescriptors[j] = pImportDescriptor;
		}
		m_PE.nImportCount = j;  // 导入表数
	}

	return TRUE;
}


// 对PE文件进行启发式扫描
//
BOOL CParsePE::AdvancedParse()
{
	// 从结构和导入表两个角度分析PE文件
	if(m_PE.nImportCount != 0 && StructureParse() == TRUE)
	{
		if(ImportTableParse() == TRUE)
		{
			return TRUE;  // 最终认定为未知病毒
		}
		else
		{
			return FALSE;  // 最终认定为非未知病毒
		}
	}
	else
	{
		return FALSE;  // 最终认定为非未知病毒
	}
}


// 对PE文件结构进行分析，判断异常
//
BOOL CParsePE::StructureParse()
{
	FLOAT unVirus = 0;  // 未知病毒的累加值
	FLOAT criteriaValue = 90;  // 从结构异常判断是否未知病毒的界限
	FLOAT addOfEryPotValue = 40;  // AddressOfEntryPoint权值
	FLOAT checkSumValue = 20;  // CheckSum权值
	FLOAT sectionValue = 40;  // SizeOfRawData，PointerToRawData权值

	/*------------- Optional Header --------------*/

	// AddressOfEntryPoint
	if(m_PE.pOptionalHeader->AddressOfEntryPoint < m_PE.aSectionHeaders[0]->VirtualAddress)
	{
		unVirus += addOfEryPotValue;
	}

	// CheckSum
	if(m_PE.pOptionalHeader->CheckSum == 0)
	{
		unVirus += checkSumValue;
	}

	/*------------- Section Table ----------------*/

	// 遍历块表
	for(int i=0; i<m_PE.nSectionCount; i++)
	{
		// SizeOfRawData，PointerToRawData
		if(m_PE.aSectionHeaders[i]->SizeOfRawData == 0 && m_PE.aSectionHeaders[i]->PointerToRawData == 0)
		{
			unVirus += sectionValue/m_PE.nSectionCount;
		}
	}

	// 未知病毒的累加值超过界限则认为结构异常，否则结构正常
	if(unVirus >= criteriaValue)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// 对输入表进行分析(查看需调用的API)
//
BOOL CParsePE::ImportTableParse()
{
	PIMAGE_THUNK_DATA pThunkData;
	LPBYTE pMapping = (LPBYTE)m_PE.pDosHeader;  // 映射基址
	CString dllName;  // dll名
	CString funName;  // dll中的函数名
	list<CString> listFunName;  // 存放dll中函数名的列表

	// 遍历所有输入表，得到所有dll名
	for(int i=0; i<m_PE.nImportCount; i++)
	{
		dllName = pMapping + Rva2Offset(m_PE.aImportDescriptors[i]->Name);  // dll名

		// 若OriginalFirstThunk的值为0，系统根据FirstThunk的值找到指向函数名的地址串
		if(m_PE.aImportDescriptors[i]->OriginalFirstThunk == 0)
		{
			pThunkData = (PIMAGE_THUNK_DATA)(pMapping + Rva2Offset(m_PE.aImportDescriptors[i]->FirstThunk));
		}
		else
		{
			pThunkData = (PIMAGE_THUNK_DATA)(pMapping + Rva2Offset(m_PE.aImportDescriptors[i]->OriginalFirstThunk));
		}
		
		// 遍历IMAGE_THUNK_DATA数组，得到该dll中的函数名
		while(pThunkData->u1.Ordinal != 0)
		{
			// 如果pThunkData->u1.Ordinal的最高二进位为1，那么函数是由序数引入的
			if(pThunkData->u1.Ordinal & IMAGE_ORDINAL_FLAG32)
			{
				funName.Format("%x",pThunkData->u1.Ordinal & 0x0FFFF);
				listFunName.push_back(funName);
			}
			// 如果元素值的最高二进位为0，就可将该值作为RVA转入IMAGE_IMPORT_BY_NAME结构
			else
			{
				funName = pMapping + Rva2Offset(pThunkData->u1.Ordinal)+2;  // 加2是因为有Hint
				listFunName.push_back(funName);
			}

			pThunkData++;
		}
	}

	BOOL bFindFirstFile = FALSE;  // 判断FindFirstFile(A)函数是否使用
	BOOL bFindNextFile = FALSE;  // 判断FindNextFile(A)函数是否使用
	list<CString>::iterator iter = listFunName.begin();  // 列表容器迭代器指向列表头
	// 遍历列表容器
	while(iter != listFunName.end())
	{
		// 判断FindFirstFile(A)函数是否使用
		if(*iter == "FindFirstFile" || *iter == "FindFirstFileA")
		{
			bFindFirstFile = TRUE;
			if(bFindNextFile == TRUE)
			{
				break;  // 跳出while循环
			}
		}

		// 判断FindNextFile(A)函数是否使用
		if(*iter == "FindNextFile" || *iter == "FindNextFileA")
		{
			bFindNextFile = TRUE;
			if(bFindFirstFile == TRUE)
			{
				break;  // 跳出while循环
			}
		}

		iter++;  // 迭代器指向下一个列表元素
	}
	listFunName.clear();  // 清空列表元素

	// 若FindFirstFile(A)函数和FindNextFile(A)函数都使用，则从导入表角度认为是未知病毒，否则不是
	if(bFindFirstFile == TRUE && bFindNextFile == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// 根据相对虚拟地址(rva)计算偏移地址(offset)
//
DWORD CParsePE::Rva2Offset(DWORD rva)
{
	// 默认第一块地址最低，rva不落在任何块内
	if(rva<m_PE.aSectionHeaders[0]->VirtualAddress && rva>=0)
	{
		return rva;
	}

	// rva落在某一块内
	for(int i=0; i<m_PE.nSectionCount; i++)
	{
		if(m_PE.aSectionHeaders[i]->VirtualAddress <= rva &&
			rva < m_PE.aSectionHeaders[i]->VirtualAddress + m_PE.aSectionHeaders[i]->SizeOfRawData)
		{
			return m_PE.aSectionHeaders[i]->PointerToRawData + rva - m_PE.aSectionHeaders[i]->VirtualAddress;
		}
	}

	return 0;
}


// 得到PE信息
//
FSPE CParsePE::GetFSPE()
{
	return m_PE;
}