@echo off
set OLDWD=%CD%
set DISDIR=%~dp0
cd %DISDIR%bin
jewelminer.exe
cd %OLDWD%
