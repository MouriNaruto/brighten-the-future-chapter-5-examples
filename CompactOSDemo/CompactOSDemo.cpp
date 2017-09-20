// CompactOSDemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <Windows.h>

#include <iostream>
#include <string>

std::wstring GetMessageByID(DWORD MessageID)
{
	std::wstring MessageString;
	LPWSTR pBuffer = nullptr;

	if (FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr,
		MessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(&pBuffer),
		0,
		nullptr))
	{
		MessageString = std::wstring(pBuffer, wcslen(pBuffer));

		LocalFree(pBuffer);
	}

	return MessageString;
}

int main()
{
	// 设置stl库的全局语言为系统当前语言以避免乱码
	std::locale::global(std::locale(""));

	std::wcout << L"请输入文件路径：";

	std::wstring FileName;
	std::wcin >> FileName;

	// 打开文件并获取文件句柄（使用选项参考系统自带Compact命令行工具实现）
	HANDLE hFile = CreateFileW(
		FileName.c_str(),
		FILE_LIST_DIRECTORY | FILE_READ_ATTRIBUTES,
		FILE_SHARE_READ | FILE_SHARE_DELETE,
		nullptr,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN,
		nullptr);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		// 定义一个结构体把两个结构体合并起来以方便使用并定义变量
		struct
		{
			WOF_EXTERNAL_INFO WofInfo;
			FILE_PROVIDER_EXTERNAL_INFO FileProviderInfo;
		} WofCompactInfo = { 0 };

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
			std::wcout << L"压缩算法：";

			switch (WofCompactInfo.FileProviderInfo.Algorithm)
			{
			case FILE_PROVIDER_COMPRESSION_XPRESS4K:
				std::wcout << L"XPRESS4K";
				break;
			case FILE_PROVIDER_COMPRESSION_LZX:
				std::wcout << L"LZX";
				break;
			case FILE_PROVIDER_COMPRESSION_XPRESS8K:
				std::wcout << L"XPRESS8K";
				break;
			case FILE_PROVIDER_COMPRESSION_XPRESS16K:
				std::wcout << L"XPRESS16K";
				break;
			default:
				std::wcout << L"未知";
				break;
			}

			std::wcout << std::endl;

			wchar_t Option = L'\0';
			std::wcout << L"请问是否解压缩（y/n）：";
			std::wcin >> Option;
			if (Option == L'y')
			{
				// 发送IO操作码以调用过滤驱动执行解压缩操作
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
					std::wcout << L"文件解压缩成功" << std::endl;
				}
				else
				{
					std::wcout << L"DeviceIoControl FSCTL_DELETE_EXTERNAL_BACKING 调用出错，文件压缩失败" << std::endl;
				}
			}

		}
		else
		{
			std::wcout << L"DeviceIoControl FSCTL_GET_EXTERNAL_BACKING 调用出错，文件可能未经CompactOS压缩或系统不是Windows 10且没有安装WofAdk驱动" << std::endl;

			wchar_t Option = L'\0';
			std::wcout << L"请问是否使用LZX压缩算法压缩（y/n）：";
			std::wcin >> Option;
			if (Option == L'y')
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
						std::wcout << L"文件压缩成功，压缩算法：";

						switch (WofCompactInfo.FileProviderInfo.Algorithm)
						{
						case FILE_PROVIDER_COMPRESSION_XPRESS4K:
							std::wcout << L"XPRESS4K";
							break;
						case FILE_PROVIDER_COMPRESSION_LZX:
							std::wcout << L"LZX";
							break;
						case FILE_PROVIDER_COMPRESSION_XPRESS8K:
							std::wcout << L"XPRESS8K";
							break;
						case FILE_PROVIDER_COMPRESSION_XPRESS16K:
							std::wcout << L"XPRESS16K";
							break;
						default:
							std::wcout << L"未知";
							break;
						}

						std::wcout << std::endl;
					}
					else
					{
						std::wcout << L"DeviceIoControl FSCTL_GET_EXTERNAL_BACKING 调用出错" << std::endl;
					}
				}
				else
				{
					std::wcout << L"DeviceIoControl FSCTL_SET_EXTERNAL_BACKING 调用出错，文件压缩失败" << std::endl;
				}
			}
		}

		CloseHandle(hFile);
	}
	else
	{
		std::wcout << L"CreateFileW 调用出错：" << GetMessageByID(GetLastError());
	}

	return 0;
}

