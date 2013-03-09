#include "StdAfx.h"
#include "VirusDB.h"
#include "ParsePE.h"
#include "XmlFile.h"

CVirusDB::CVirusDB(void)
{

}

CVirusDB::~CVirusDB(void)
{
}


// 加载病毒库
//
BOOL CVirusDB::Load()
{
	//PVRECORD pVRecord;  // 用于存放病毒库中每个病毒记录的内容

	/////*------------------- eicar virus ------------------------*/

	//pVRecord = new VRECORD;  // 新建病毒记录
	//if(pVRecord == NULL)
	//{
	//	return FALSE;
	//}

	//pVRecord->dwVirusID = 1;  // 指定病毒的ID编号
	//pVRecord->dwSignCount = 3;  // 存放特征码的段数，也就是对于该病毒取了多少段特征码

	//VSIGNATURE aVSign1[3]=
	//{
	//	// 纯文件偏移，特征起始地址0，特征长度32，后面32个字节是具体特征值
	//	{
	//		BS_PHY_FILE, 0, 0, 32, BL_EQUAL, 
	//		0x58, 0x35, 0x4F, 0x21, 0x50, 0x25, 0x40, 0x41, 0x50, 0x5B, 0x34, 0x5C, 0x50, 0x5A, 0x58, 0x35, 
	//		0x34, 0x28, 0x50, 0x5E, 0x29, 0x37, 0x43, 0x43, 0x29, 0x37, 0x7D, 0x24, 0x45, 0x49, 0x43, 0x41, 
	//	},
	//	// 纯文件偏移，特征起始地址32，特征长度32，后面32个字节是具体特征值
	//	{
	//		BS_PHY_FILE, 0, 32, 32, BL_EQUAL,
	//		0x52, 0x2D, 0x53, 0x54, 0x41, 0x4E, 0x44, 0x41, 0x52, 0x44, 0x2D, 0x41, 0x4E, 0x54, 0x49, 0x56, 
	//		0x49, 0x52, 0x55, 0x53, 0x2D, 0x54, 0x45, 0x53, 0x54, 0x2D, 0x46, 0x49, 0x4C, 0x45, 0x21, 0x24, 
	//	},
	//	// 纯文件偏移，特征起始地址64，特征长度4，后面4个字节是具体特征值
	//	{
	//		BS_PHY_FILE, 0, 64, 4, BL_EQUAL,
	//		0x48, 0x2B, 0x48, 0x2A, 
	//	},
	//};

	//// 存放每段特征码的内容
	//for(DWORD i=0; i<pVRecord->dwSignCount; i++)
	//{
	//	PVSIGNATURE	pSign = new VSIGNATURE;
	//	*pSign = aVSign1[i];
	//	pVRecord->pVSign[i] = pSign;
	//}

	//// 向列表容器中加入此病毒记录
	//m_listVRecords.push_back(pVRecord);


	/////*--------------- notepad.exe as a fake virus -----------------------*/

	//pVRecord = new VRECORD;  // 新建病毒记录
	//if(pVRecord == NULL)
	//{
	//	return FALSE;
	//}

	//pVRecord->dwVirusID = 2;  // 指定病毒的ID编号
	//pVRecord->dwSignCount = 4;  // 存放特征码的段数，也就是对于该病毒取了多少段特征码

	//VSIGNATURE aVSign2[4]=
	//{
	//	// 纯文件偏移，特征起始地址256，特征长度32，后面32个字节是具体特征值
	//	{
	//		BS_PHY_FILE, 0, 256, 32, BL_EQUAL,
	//		0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9D, 0x73, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 
	//		0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 
	//	},
	//	// 纯文件偏移，特征起始地址1952，特征长度32，后面32个字节是具体特征值
	//	{
	//		BS_PHY_FILE, 0, 1952, 32, BL_EQUAL,
	//		0x4E, 0x00, 0x70, 0x00, 0x45, 0x00, 0x6E, 0x00, 0x63, 0x00, 0x6F, 0x00, 0x64, 0x00, 0x69, 0x00, 
	//		0x6E, 0x00, 0x67, 0x00, 0x44, 0x00, 0x69, 0x00, 0x61, 0x00, 0x6C, 0x00, 0x6F, 0x00, 0x67, 0x00, 
	//	},
	//	// 纯文件偏移，特征起始地址31776，特征长度32，后面32个字节是具体特征值
	//	{
	//		BS_PHY_FILE, 0, 31776, 32, BL_EQUAL,
	//		0x4E, 0x00, 0x6F, 0x00, 0x74, 0x00, 0x65, 0x00, 0x70, 0x00, 0x61, 0x00, 0x64, 0x00, 0x00, 0x00, 
	//		0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 
	//	},
	//	// 纯文件偏移，特征起始地址65168，特征长度32，后面32个字节是具体特征值
	//	{
	//		BS_PHY_FILE, 0, 65184, 32, BL_EQUAL,
	//		0x6E, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x6E, 0x00, 0x61, 0x00, 0x6C, 0x00, 0x4E, 0x00, 
	//		0x61, 0x00, 0x6D, 0x00, 0x65, 0x00, 0x00, 0x00, 0x4E, 0x00, 0x6F, 0x00, 0x74, 0x00, 0x65, 0x00, 
	//	}
	//};

	//// 存放每段特征码的内容
	//for(DWORD i=0; i<pVRecord->dwSignCount; i++)
	//{
	//	PVSIGNATURE	pSign = new VSIGNATURE;
	//	*pSign = aVSign2[i];
	//	pVRecord->pVSign[i] = pSign;
	//}

	//// 向列表容器中加入此病毒记录
	//m_listVRecords.push_back(pVRecord);


	/////*---------------------- CIH virus -----------------------*/

	//pVRecord = new VRECORD;  // 新建病毒记录
	//if(pVRecord == NULL)
	//{
	//	return FALSE;
	//}

	//pVRecord->dwVirusID = 3;  // 指定病毒的ID编号
	//pVRecord->dwSignCount = 3;  // 存放特征码的段数，也就是对于该病毒取了多少段特征码

	//VSIGNATURE aVSign3[3]=
	//{
	//	// 以BS_SUB_NT_HEADERS参考点偏移，特征起始地址-1，特征长度1，后面1个字节是具体特征值
	//	{
	//		BS_STRUCT_OFFSET, 1, -1, 1, BL_NOT_EQUAL, 
	//		0x00,
	//	},
	//	// 以BS_SUB_ENTRY_POINT参考点偏移，特征起始地址0，特征长度32，后面32个字节是具体特征值
	//	{
	//		BS_STRUCT_OFFSET, 2, 0, 32, BL_EQUAL, 
	//		0x55, 0x8D, 0x44, 0x24, 0xF8, 0x33, 0xDB, 0x64, 0x87, 0x03, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x5B, 
	//		0x8D, 0x4B, 0x42, 0x51, 0x50, 0x50, 0x0F, 0x01, 0x4C, 0x24, 0xFE, 0x5B, 0x83, 0xC3, 0x1C, 0xFA, 
	//	},
	//	// 以BS_SUB_ENTRY_POINT参考点偏移，特征起始地址137，特征长度27，后面27个字节是具体特征值
	//	{
	//		BS_STRUCT_OFFSET, 2, 137, 27, BL_EQUAL, 
	//		0xCD, 0x20, 0x53, 0x00, 0x01, 0x00, 0x83, 0xC4, 0x20, 0x97, 0x8D, 0x46, 0x9D, 0xCF, 0x8D, 0x87, 
	//		0xF7, 0xFC, 0xFF, 0xFF, 0x50, 0xCD, 0x20, 0x67, 0x00, 0x40, 0x00, 
	//	},
	//};

	//// 存放每段特征码的内容
	//for(DWORD i=0; i<pVRecord->dwSignCount; i++)
	//{
	//	PVSIGNATURE	pSign = new VSIGNATURE;
	//	*pSign = aVSign3[i];
	//	pVRecord->pVSign[i] = pSign;
	//}

	//// 向列表容器中加入此病毒记录
	//m_listVRecords.push_back(pVRecord);
   

   CString xmlFileName;  // xml文件路径变量
	GetModuleFileName(NULL, xmlFileName.GetBuffer(MAX_PATH), MAX_PATH);  // 获得当前应用程序路径
	xmlFileName.ReleaseBuffer();  // 一定要释放，否则无法拼接
	xmlFileName = xmlFileName.Left(xmlFileName.ReverseFind('\\'));  // 去掉当前应用程序名
	xmlFileName = xmlFileName + "\\bin\\DB.xml";  // 拼接成xml文件路径

	CXmlFile xmlFile;
	xmlFile.SetXmlFileName(xmlFileName);  // 设置xml文件路径
	xmlFile.LoadForPE(&m_listVRecords);  // 加载病毒库

	return TRUE;
}


// 卸载病毒库
//
BOOL CVirusDB::Unload()
{
	list<PVRECORD>::iterator iter = m_listVRecords.begin();  // 列表容器迭代器指向列表头

	// 遍历列表容器
	while(iter != m_listVRecords.end())
	{
		PVRECORD pVRecord = *iter;  // 列表容器中的一个病毒记录
		// 清空每一个病毒记录里的内容
		for(DWORD i=0; i<pVRecord->dwSignCount; i++)
		{
			delete pVRecord->pVSign[i];  // 删除指针所指对象
		}
		delete pVRecord;  // 删除指针所指对象

		iter++;  // 迭代器指向下一个列表元素
	}

	return TRUE;
}


// 将扫描对象在本病毒库中检测，返回病毒ID或0
DWORD CVirusDB::Search(CScanObject* pScanObj)
{
	CParsePE parserPE;  // PE分析器
	parserPE.BasicParse((CMemFileObject*)pScanObj);  // 判断是否PE文件

	BOOL bVirus = FALSE;  // 判断是否病毒标志
	list<PVRECORD>::iterator iter = m_listVRecords.begin();  // 列表容器迭代器指向列表头
	// 遍历列表容器
	while(iter != m_listVRecords.end())
	{
		PVRECORD pVRecord = *iter;  // 列表容器中的一个病毒记录
		bVirus = FALSE;  // 病毒判断标识

		// 比对一个病毒记录中的每段特征码
		for(DWORD i=0; i<pVRecord->dwSignCount; i++)
		{
			PVSIGNATURE pVSign = pVRecord->pVSign[i];  // 指向一段特征码

			// 判断此段特征码的类型
			switch(pVSign->eType)
			{
			// 若特征码类型为纯文件偏移
			case BS_PHY_FILE:  
				bVirus = pScanObj->Compare(pVSign->nOffset, pVSign->nSize, pVSign->Signature, pVSign->eLogicOp);
				break;
			// 若特征码类型为以某个结构为参考点偏移(目前只适用于PE文件)
			case BS_STRUCT_OFFSET:  
				// 判断是否属于内存文件
				if(pScanObj->GetObjType() != BO_MEM_FILE)
				{
					bVirus = FALSE;
					break;  // 跳出此段特征码
				}

				// 若是PE文件
				if(parserPE.GetFSPE().bPEFile == TRUE)
				{
					// 判断特征码子类型
					switch(pVSign->dwSubType)
					{
					case BS_SUB_NT_HEADERS:  // 以BS_SUB_NT_HEADERS参考点偏移
						bVirus = pScanObj->Compare(((LPBYTE)parserPE.GetFSPE().pNtHeaders)+pVSign->nOffset, 
							pVSign->nSize, pVSign->Signature, pVSign->eLogicOp);
						break;
					case BS_SUB_ENTRY_POINT:  // 以BS_SUB_ENTRY_POINT参考点偏移
						bVirus = pScanObj->Compare(((LPBYTE)parserPE.GetFSPE().pEntryPoint)+pVSign->nOffset, 
							pVSign->nSize, pVSign->Signature, pVSign->eLogicOp);
						break;
					default:
						bVirus = FALSE;  // 例如特征码子类型不正确
					}
				}
				// 不是PE文件
				else
				{
					bVirus = FALSE;
					break;  // 跳出此段特征码
				}
				break;
			default:
				bVirus = FALSE;  // 例如特征码类型不正确
			}

			// 有一段特征码不符合就跳出此病毒记录
			if(bVirus == FALSE)	
			{
				break;  // 跳出for循环
			}
		}

		// 若扫描对象匹配一个病毒记录中的所有特征码，就认为是此种病毒
		if(bVirus == TRUE)
		{
			return pVRecord->dwVirusID;  // 返回病毒ID
		}
		else
		{
			iter++;  // 迭代器指向下一个列表元素
		}
	}

	// 若未查出病毒，且为PE文件，则调用启发式扫描
	if(bVirus == FALSE && parserPE.GetFSPE().bPEFile == TRUE)
	{
		// 若是未知威胁
		if(parserPE.AdvancedParse() == TRUE)
		{
			return -2;  // 表示未知威胁
		}
	}
	
	return 0;  // 表示无病毒(至少此病毒库中没有与之匹配的特征码)
}