// CompactOS_UncompressFile.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <Windows.h>

int main()
{
	// �����ļ��������
	HANDLE hFile = INVALID_HANDLE_VALUE;

	// ���ļ�����ȡ�ļ������ʹ��ѡ��ο�ϵͳ�Դ�Compact�����й���ʵ�֣�
	hFile = CreateFileW(
		L"E:\\Tools\\TotalCMD\\TOTALCMD64.EXE",
		FILE_LIST_DIRECTORY | FILE_READ_ATTRIBUTES,
		FILE_SHARE_READ | FILE_SHARE_DELETE,
		nullptr,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN,
		nullptr);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		// ����IO�������Ե��ù�������ִ�н�ѹ������
		if (DeviceIoControl(
			hFile,
			FSCTL_DELETE_EXTERNAL_BACKING,
			nullptr,
			0,
			nullptr,
			0,
			nullptr,
			nullptr))
		{
			// �����ɹ�
		}

		// �ر��ļ����
		CloseHandle(hFile);
	}

	return 0;
}

