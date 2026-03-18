echo off

path = %path%;D:\MATLAB\setup\waijung17_03a\utils\gnumake382-w32
path = %path%;D:\keil C\ARM\ARMCC\BIN

echo on
gnumake382-w32 -f test2.mk

