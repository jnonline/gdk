
for /r %~f1 %%R in (.svn) do if exist "%%R" (rd /s /q "%%R")