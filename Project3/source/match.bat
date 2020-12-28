@echo off
g++ chain_reaction.cpp board.cpp rules.cpp player.cpp algorithm_ST.cpp algorithm_TA.cpp
set /a red=0
set /a blue=0
set /a numofgame=30
IF NOT (%1)==() (set /a numofgame=%1)
for /L %%n in (1 1 %numofgame%) do (
a.exe > output
find "Red Player won the game" .\output>nul && set /a red+=1 && echo Red won
find "Blue Player won the game" .\output>nul && set /a blue+=1 && echo Blue won
)
echo %numofgame% games
echo Red wins %red%
echo Blue wins %blue%
del .\output
del .\a.exe
pause