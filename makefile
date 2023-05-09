build: build_test

build_main:
	cl \
	/Zi \
	/EHsc \
	/W4 \
	/D_DEBUG \
	/MDd \
	/Femain.exe \
	/I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\winrt" \
	/I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\um" \
	/I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\ucrt" \
	/I"C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared" \
	/I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.35.32215\include" \
	/Iinclude \
	src\main.c \
	src\algo\*.c \
	/link \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64" \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\ucrt\x64" \
	/LIBPATH:"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.35.32215\lib\x64" \
	user32.lib

run_main:
	.\main.exe

build_test:
	cl \
	/Zi \
	/EHsc \
	/W4 \
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

run_test:
	.\test.exe

clean:
	del *.obj \
	del *.ilk \
	del *.pdb \
	del *.exe

build_test_linux:
	gcc -Wall -Iinclude src/algo/*.c src/test.c -o ./test

run_test_linux:
	./test

gdb_test_linux:
	gdb ./test

valgrind_test_linux:
	valgrind --leak-check=full --track-origins=yes ./test

clean_linux:
	rm test

