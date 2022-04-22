#include "detect_leaks.hpp"

void start_detecting() {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// Send all reports to STDOUT
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

	// Set the debug-heap flag so that freed blocks are kept on the
	// linked list, to catch any inadvertent use of freed memory
	SET_CRT_DEBUG_FIELD(_CRTDBG_DELAY_FREE_MEM_DF);

	// Set the debug-heap flag so that memory leaks are reported when the process terminates.
	SET_CRT_DEBUG_FIELD(_CRTDBG_LEAK_CHECK_DF);
}
