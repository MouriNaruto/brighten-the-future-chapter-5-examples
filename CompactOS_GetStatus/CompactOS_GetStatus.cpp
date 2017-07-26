// CompactOS_GetStatus.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <Windows.h>

int main()
{
	// 定义文件句柄变量
	HANDLE hFile = INVALID_HANDLE_VALUE;

	// 定义一个结构体把两个结构体合并起来以方便使用并定义变量
	struct
	{
		WOF_EXTERNAL_INFO WofInfo;
		FILE_PROVIDER_EXTERNAL_INFO FileProviderInfo;
	} WofCompactInfo = { 0 };

	// 打开文件并获取文件句柄（使用选项参考系统自带Compact命令行工具实现）
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
		// 发送IO操作码以调用过滤驱动获取压缩状态
		if (DeviceIoControl(
			hFile,
			FSCTL_GET_EXTERNAL_BACKING,
			nullptr,
			0,
			&WofCompactInfo,
			sizeof(WofCompactInfo),
			nullptr,
			nullptr))
		{
			// 操作成功
		}

		// 关闭文件句柄
		CloseHandle(hFile);
	}

	return 0;
}

