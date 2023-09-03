build:
	clang -g -Iinclude src/*.c test/*.c cmd/testbin/main.c -o ./testbin.exe

build_win:
	cl \
	/Zi \
	/EHsc \
	/D_DEBUG \
	/MDd \
	/Fetest.exe \
	/I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\winrt" \
	/I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um" \
	/I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt" \
	/I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared" \
	/I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.35.32215\include" \
	/Iinclude \
	src\*.c \
	test\*.c \
	cmd\testbin\main.c \
	/link \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64" \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\ucrt\x64" \
	/LIBPATH:"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.35.32215\lib\x64" \
	user32.lib

run:
	.\testbin.exe

clean:
	del *.obj \
	del *.ilk \
	del *.pdb \
	del *.exe

codelldb:
	C:\Users\alem\.vscode\extensions\vadimcn.vscode-lldb-1.9.2\adapter\codelldb.exe --port 13000

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

