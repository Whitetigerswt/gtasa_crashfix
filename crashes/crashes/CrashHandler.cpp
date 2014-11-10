#include "CrashHandler.h"
#include <string>
#include "main.h"
#include "CLog.h"

std::string GetGTADirectory()
{
	// Create a new variable to hold the location to gta_sa.exe - but not the directory.
	char exe_path[MAX_PATH + 1];

	// Get the path of the gta_sa.exe and place it into our exe_path variable.
	GetModuleFileName(NULL, exe_path, sizeof(exe_path));

	// Change it to an std::string.
	std::string szExePath(exe_path);

	// Find the last instances of \, so we can remove everything after it and have our GTA directory.
	int i = szExePath.rfind("\\");

	// Create a new variable to hold the directory path after we've cut out the gta_sa.exe part.
	std::string DirectoryPath;

	// Put the directory path in our directory path variable.
	DirectoryPath = szExePath.substr(0, i);

	return DirectoryPath;
}

int CALLBACK CrashCallback(CR_CRASH_CALLBACK_INFO* pInfo)
{
	// The application has crashed!

	// Add additional log file.
	CLog log = CLog("exception_info.txt", false);
	log.Write("Exception Type: %d", pInfo->pExceptionInfo->exctype);

	if (pInfo->pExceptionInfo->exctype == CR_CPP_INVALID_PARAMETER)
	{
		log.Write("Function: %s", pInfo->pExceptionInfo->function);
		log.Write("Line: %d", pInfo->pExceptionInfo->line);
		log.Write("Expression: %s", pInfo->pExceptionInfo->expression);
		log.Write("File: %s", pInfo->pExceptionInfo->file);
	}

	if (pInfo->pExceptionInfo->exctype == CR_SEH_EXCEPTION)
	{
		log.Write("Code: %d", pInfo->pExceptionInfo->code);
	}

	if (pInfo->pExceptionInfo->exctype == CR_CPP_SIGFPE)
	{
		log.Write("fpe_subcode: %d", pInfo->pExceptionInfo->fpe_subcode);
	}

	if (pInfo->pExceptionInfo->pexcptrs != NULL)
	{

		log.Write("Exception at address: 0x%x", pInfo->pExceptionInfo->pexcptrs->ExceptionRecord->ExceptionAddress);
		log.Write("Exception Code: 0x%x || Exception Flags: %d", pInfo->pExceptionInfo->pexcptrs->ExceptionRecord->ExceptionCode, pInfo->pExceptionInfo->pexcptrs->ExceptionRecord->ExceptionFlags);
		log.Write("Exception Info 0: %d || Exception Info 1: %d", pInfo->pExceptionInfo->pexcptrs->ExceptionRecord->ExceptionInformation[0], pInfo->pExceptionInfo->pexcptrs->ExceptionRecord->ExceptionInformation[1]);
		log.Write("Exception Info 2: %d", pInfo->pExceptionInfo->pexcptrs->ExceptionRecord->ExceptionInformation[2]);
		log.Write("NumberParameters: %d", pInfo->pExceptionInfo->pexcptrs->ExceptionRecord->NumberParameters);

		log.Write("");
		log.Write("EAX: 0x%x || EBX: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Eax, pInfo->pExceptionInfo->pexcptrs->ContextRecord->Ebx);
		log.Write("ECX: 0x%x || EDX: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Ecx, pInfo->pExceptionInfo->pexcptrs->ContextRecord->Edx);
		log.Write("EDI: 0x%x || ESI: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Edi, pInfo->pExceptionInfo->pexcptrs->ContextRecord->Esi);
		log.Write("EBP: 0x%x || ESP: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Ebp, pInfo->pExceptionInfo->pexcptrs->ContextRecord->Esp);

		log.Write("");
		log.Write("EFlags: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->EFlags);
		log.Write("EIP: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Eip);

		log.Write("");
		log.Write("ContextFlags: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->ContextFlags);
		log.Write("Debug registers:");
		log.Write("Dr0: 0x%x || Dr1: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Dr0, pInfo->pExceptionInfo->pexcptrs->ContextRecord->Dr1);
		log.Write("Dr2: 0x%x || Dr3: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Dr2, pInfo->pExceptionInfo->pexcptrs->ContextRecord->Dr3);
		log.Write("Dr6: 0x%x || Dr7: 0x%x", pInfo->pExceptionInfo->pexcptrs->ContextRecord->Dr6, pInfo->pExceptionInfo->pexcptrs->ContextRecord->Dr7);

		log.Write("");
		log.Write("FloatSave: %f", pInfo->pExceptionInfo->pexcptrs->ContextRecord->FloatSave);
		log.Write("SegCs: %d || SegDs: %d", pInfo->pExceptionInfo->pexcptrs->ContextRecord->SegCs, pInfo->pExceptionInfo->pexcptrs->ContextRecord->SegDs);
		log.Write("SegEs: %d || SegFs: %d", pInfo->pExceptionInfo->pexcptrs->ContextRecord->SegEs, pInfo->pExceptionInfo->pexcptrs->ContextRecord->SegFs);
		log.Write("SegGs: %d || SegSs: %d", pInfo->pExceptionInfo->pexcptrs->ContextRecord->SegGs, pInfo->pExceptionInfo->pexcptrs->ContextRecord->SegSs);
	}

	char file[MAX_PATH];
	sprintf_s(file, sizeof(file), "%s\\exception_info.txt", GetGTADirectory());
	crAddFile2(file, "exception_info.txt", "Exception Info", CR_AF_MAKE_FILE_COPY);

	// Return CR_CB_DODEFAULT to generate error report
	return CR_CB_DODEFAULT;
}



void SetCrashCallback()
{
	// Add crash callback
	crSetCrashCallback(CrashCallback, NULL);
}

void CrashHandler()
{
	// Create new crash info.
	CR_INSTALL_INFO info;

	// Reset it's memory.
	memset(&info, 0, sizeof(CR_INSTALL_INFO));
	info.cb = sizeof(CR_INSTALL_INFO);

	info.pszAppName = "GTA San Andreas";

	char msg[30];
	sprintf_s(msg, sizeof(msg), "%0.2f", VERSION);
	info.pszAppVersion = msg;
	info.pszEmailSubject = "crashes asi crash report";
	info.pszEmailTo = "whitetigerswt@gmail.com";
	info.pszUrl = "http://sixtytiger.com/tiger/crashes/crashrpt.php";

	info.uPriorities[CR_HTTP] = 3;  // First try send report over HTTP 
	info.uPriorities[CR_SMTP] = 2;  // Second try send report over SMTP  
	info.uPriorities[CR_SMAPI] = 1; // Third try send report over Simple MAPI 

	// Install all available exception handlers
	info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;
	// Keep the reports in a queue incase the user doesn't send them immediately.
	info.dwFlags |= CR_INST_SEND_QUEUED_REPORTS;

	// Show the additional fields by default, this is the E-Mail and description of what they were doing before crash fields.
	info.dwFlags |= CR_INST_SHOW_ADDITIONAL_INFO_FIELDS;

	// Automatically attach the crash reporter to all newly created threads.
	info.dwFlags |= CR_INST_AUTO_THREAD_HANDLERS;

	// Define the Privacy Policy URL 
	info.pszPrivacyPolicyURL = "http://sixtytiger.com/tiger/crashes/privacypolicy.html";

	// Install crash reporting
	int nResult = crInstall(&info);
	if (nResult != 0)
	{
		// Crash reporter failed to initialize.
		return;
	}

	// Add the crash callback.
	SetCrashCallback();

	// Create a screenshot that we can investigate.
	crAddScreenshot2(CR_AS_MAIN_WINDOW | CR_AS_USE_JPEG_FORMAT, 90);
}
