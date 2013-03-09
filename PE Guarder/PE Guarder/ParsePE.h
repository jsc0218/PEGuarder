#pragma once
#include "MemFileObject.h"

#define MAX_SECTIONS 64  // 最大块数
#define MAX_IMPORTS 64  // 最大导入表数


// 用于存储PE结构的主要信息
typedef struct tagFSPE
{
	BOOL bPEFile;  // 判断是否有效PE文件
	PIMAGE_DOS_HEADER pDosHeader;  // 指向DOS头的指针
	PIMAGE_NT_HEADERS pNtHeaders;  // 指向NT头的指针
	PIMAGE_FILE_HEADER pFileHeader;  // 指向文件头的指针
	PIMAGE_OPTIONAL_HEADER pOptionalHeader;  // 指向可选头的指针
	LPBYTE pEntryPoint;  // 指向程序入口点的指针  
	INT nSectionCount;  // 块数
	PIMAGE_SECTION_HEADER aSectionHeaders[MAX_SECTIONS];  // 块指针数组
	INT nImportCount;  // 导入表数
	PIMAGE_IMPORT_DESCRIPTOR aImportDescriptors[MAX_IMPORTS];  // 导入表指针数组
}FSPE, *PFSPE;


// 根据传入的CMemFileObject 对象参数，实现对PE文件的确定以及PE 格式的分析
class CParsePE
{
public:
	CParsePE(void);  // 构造函数
	~CParsePE(void);  // 析构函数

	BOOL BasicParse(IN CMemFileObject* pScanObj);  // 将一个内存中执行对象的PE结构进行分析
	BOOL AdvancedParse();  // 对PE文件进行启发式扫描
	BOOL StructureParse();  // 对PE文件结构进行分析，判断异常
	BOOL ImportTableParse();  // 对输入表进行分析(查看需调用的API)
	DWORD Rva2Offset(DWORD rva);  // 根据相对虚拟地址(rva)计算偏移地址(offset)
	FSPE GetFSPE();  // 得到PE信息

protected:
	FSPE m_PE;  // PE信息结构
};
