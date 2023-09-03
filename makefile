build:
	clang -g -Iinclude src/algo/*.c src/test.c -o ./test.exe

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
	src\test.c \
	src\algo\*.c \
	/link \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64" \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\ucrt\x64" \
	/LIBPATH:"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.35.32215\lib\x64" \
	user32.lib

run:
	.\test.exe

clean:
	del *.obj \
	del *.ilk \
	del *.pdb \
	del *.exe

codelldb:
	C:\Users\alem\.vscode\extensions\vadimcn.vscode-lldb-1.9.2\adapter\codelldb.exe --port 13000

build_linux:
	clang -g -Iinclude src/algo/*.c src/test.c -o ./test

run_linux:
	./test

debug_linux:
	lldb ./test

valgrind_linux:
	valgrind --leak-check=full --track-origins=yes ./test

clean_linux:
	rm test

