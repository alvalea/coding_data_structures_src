#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <stdio.h>

int main()
{
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

  _CrtMemState sOld;
  _CrtMemState sNew;
  _CrtMemState sDiff;
  _CrtMemCheckpoint(&sOld); //take a snapshot


  _CrtMemCheckpoint(&sNew); //take a snapshot
  if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
  {
      printf("-----------_CrtMemDumpStatistics ---------\n");
      _CrtMemDumpStatistics(&sDiff);
      printf("-----------_CrtMemDumpAllObjectsSince ---------\n");
      _CrtMemDumpAllObjectsSince(&sOld);
      printf("-----------_CrtDumpMemoryLeaks ---------\n");
      _CrtDumpMemoryLeaks();
  }
}
