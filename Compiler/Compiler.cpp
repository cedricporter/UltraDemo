// Compiler.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "Compiler.h"


// ���ǵ���������һ��ʾ��
COMPILER_API int nCompiler=0;

// ���ǵ���������һ��ʾ����
COMPILER_API int fnCompiler(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� Compiler.h
CCompiler::CCompiler()
{
	return;
}
