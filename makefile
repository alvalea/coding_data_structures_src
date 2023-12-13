build: build_llvm

build_llvm:
	clang -g -Iinclude src/*.c test/*.c cmd/testbin/main.c -o ./testbin.exe

build_win:
	"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" && \
	cl /Zi /EHsc /D_DEBUG /MDd /Fetestbin.exe /Iinclude src\*.c test\*.c cmd\testbin\main.c

run:
	.\testbin.exe

clean:
	del *.obj \
	del *.ilk \
	del *.pdb \
	del *.exe

build_linux:
	clang -Iinclude src/*.c test/*.c cmd/testbin/main.c -o ./testbin

build_debug_linux:
	clang -g -Iinclude src/*.c test/*.c cmd/testbin/main.c -o ./testbin

run_linux:
	./testbin

debug_linux:
	lldb ./testbin

valgrind_linux:
	valgrind --leak-check=full --track-origins=yes ./testbin

clean_linux:
	rm testbin

