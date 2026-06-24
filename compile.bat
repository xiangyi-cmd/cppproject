@echo off
chcp 65001 >nul
set PATH=C:\mingw64\bin;%PATH%
echo Compiling C++ Hotel Booking and Room Management System...
g++ -std=c++17 -Iinclude src/Room.cpp src/HotelManager.cpp src/main.cpp -o hotel_system.exe
if %ERRORLEVEL% EQU 0 (
    echo Compilation Succeeded! Run hotel_system.exe to start the application.
) else (
    echo Compilation Failed!
)
pause
