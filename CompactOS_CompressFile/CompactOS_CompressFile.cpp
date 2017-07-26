// CompactOS_CompressFile.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <Windows.h>

int main()
{
	// �����ļ��������
	HANDLE hFile = INVALID_HANDLE_VALUE;

	// ����һ���ṹ��������ṹ��ϲ������Է���ʹ�ò��������
	struct
	{
		WOF_EXTERNAL_INFO WofInfo;
		FILE_PROVIDER_EXTERNAL_INFO FileProviderInfo;
	} WofCompactInfo = { 0 };

	// ���ļ�����ȡ�ļ����������ѡ���հ�ϵͳ�Դ�Compact�����й���ʵ�֣�
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
		// ָ�����������汾���ֽ׶�ֻ�и�ѡ��
		WofCompactInfo.WofInfo.Version = WOF_CURRENT_VERSION;

		// ָ��Provider�汾������ʹ�ø�ѡ��
		WofCompactInfo.WofInfo.Provider = WOF_PROVIDER_FILE;

		// ָ��Provider�汾���ֽ׶�ֻ�и�ѡ��
		WofCompactInfo.FileProviderInfo.Version = FILE_PROVIDER_CURRENT_VERSION;

		// ָ��ѡ�������0
		WofCompactInfo.FileProviderInfo.Flags = 0;

		// ָ��ѹ��ʱʹ�õ��㷨��ѡ������
		// FILE_PROVIDER_COMPRESSION_XPRESS4K
		// FILE_PROVIDER_COMPRESSION_LZX
		// FILE_PROVIDER_COMPRESSION_XPRESS8K
		// FILE_PROVIDER_COMPRESSION_XPRESS16K
		WofCompactInfo.FileProviderInfo.Algorithm = FILE_PROVIDER_COMPRESSION_XPRESS4K;

		// ����IO�������Ե��ù�������ִ��ѹ������
		if (DeviceIoControl(
			hFile,
			FSCTL_SET_EXTERNAL_BACKING,
			&WofCompactInfo,
			sizeof(WofCompactInfo),
			nullptr,
			0,
			nullptr,
			nullptr))
		{
			// TODO: �����ɹ��Ĵ���
		}
		else
		{
			// TODO: ����ʧ�ܵĴ���ͨ��GetLastError()��ȡ������
		}

		// �ر��ļ����
		CloseHandle(hFile);
	}

	return 0;
}


