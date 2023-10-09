#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <stdio.h>
#include <algo/container_test.h>
#include <algo/sort_test.h>

typedef int (*test_fn)();

static
void test(const char* test_name, test_fn fn) {
        printf("[%s]\t", test_name);
        if(fn()) {
                printf("\033[1;31m FAIL");
        } else {
                printf("\033[1;32m SUCCESS");
        }
        printf("\033[0m \n");
}

static
void test_main() {
        test("test_Array_add", test_Array_add);
        test("test_Array_remove", test_Array_remove);
        test("test_Array_get", test_Array_get);
        test("test_Array_len", test_Array_len);
        test("test_Array_clear", test_Array_clear);

        test("test_List_add", test_List_add);
        test("test_List_remove", test_List_remove);
        test("test_List_head", test_List_head);
        test("test_List_tail", test_List_tail);

        test("test_Queue_push", test_Queue_push);
        test("test_Queue_pop", test_Queue_pop);

        test("test_Map_insert", test_Map_insert);
        test("test_Map_remove", test_Map_remove);
        test("test_Map_find", test_Map_find);

        test("test_HashMap_insert", test_HashMap_insert);
        test("test_HashMap_remove", test_HashMap_remove);
        test("test_HashMap_find", test_HashMap_find);

        test("test_Heap_push", test_Heap_push);
        test("test_Heap_pop", test_Heap_pop);

        test("test_BTree_insert", test_BTree_insert);
        test("test_BTree_find", test_BTree_find);

        test("test_BpTree_insert", test_BpTree_insert);
        test("test_BpTree_find", test_BpTree_find);

        test("test_Graph_print", test_Graph_print);
        test("test_Graph_bfs", test_Graph_bfs);
        test("test_Graph_dfs", test_Graph_dfs);
        test("test_Graph_mst", test_Graph_mst);
        test("test_Graph_path", test_Graph_path);

        test("test_quicksort", test_quicksort);
}

int main() {
#ifdef _DEBUG
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
#endif

        test_main();

#ifdef _DEBUG
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
#endif

        return 0;
}
