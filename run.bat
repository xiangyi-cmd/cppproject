@echo off
chcp 65001 >nul
set PATH=C:\mingw64\bin;%PATH%
if not exist hotel_system.exe (
    echo Executable not found. Compiling first...
    g++ -std=c++17 -Iinclude src/Room.cpp src/HotelManager.cpp src/main.cpp -o hotel_system.exe
)
if exist hotel_system.exe (
    hotel_system.exe
) else (
    echo Failed to compile and run.
    pause
)
