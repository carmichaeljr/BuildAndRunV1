@ECHO off
setLocal

SET buildAndRunPath="C:\Users\JackCarmichael\Documents\Programming\C Programms\BuildAndRun\Version1"
SET buildAndRunAbsPath="C:\Users\JackCarmichael\Documents\Programming\C Programms\BuildAndRun\Version1\BuildAndRun.exe"
cd %buildAndRunPath%
%buildAndRunAbsPath% %1 %2

endLocal
exit