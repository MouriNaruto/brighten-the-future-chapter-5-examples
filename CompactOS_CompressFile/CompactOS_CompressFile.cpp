// CompactOS_CompressFile.cpp : 定义控制台应用程序的入口点。
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

	// 打开文件并获取文件句柄（传入选项照搬系统自带Compact命令行工具实现）
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
		// 指定过滤驱动版本，现阶段只有该选项
		WofCompactInfo.WofInfo.Version = WOF_CURRENT_VERSION;

		// 指定Provider版本，必须使用该选项
		WofCompactInfo.WofInfo.Provider = WOF_PROVIDER_FILE;

		// 指定Provider版本，现阶段只有该选项
		WofCompactInfo.FileProviderInfo.Version = FILE_PROVIDER_CURRENT_VERSION;

		// 指定选项，必须填0
		WofCompactInfo.FileProviderInfo.Flags = 0;

		// 指定压缩时使用的算法，选项如下
		// FILE_PROVIDER_COMPRESSION_XPRESS4K
		// FILE_PROVIDER_COMPRESSION_LZX
		// FILE_PROVIDER_COMPRESSION_XPRESS8K
		// FILE_PROVIDER_COMPRESSION_XPRESS16K
		WofCompactInfo.FileProviderInfo.Algorithm = FILE_PROVIDER_COMPRESSION_XPRESS4K;

		// 发送IO操作码以调用过滤驱动执行压缩操作
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
			// TODO: 操作成功的处理
		}
		else
		{
			// TODO: 操作失败的处理，通过GetLastError()获取错误码
		}

		// 关闭文件句柄
		CloseHandle(hFile);
	}

	return 0;
}


