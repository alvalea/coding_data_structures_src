build:
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
	src\*.c \
	src\algo\*.c \
	/link \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\um\x64" \
	/LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22621.0\ucrt\x64" \
	/LIBPATH:"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.35.32215\lib\x64" \
	user32.lib

run:
	.\main.exe

clean:
	del *.obj \
	del *.ilk \
	del *.pdb \
	del *.exe
