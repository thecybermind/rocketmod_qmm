msbuild .\msvc\rocketmod_qmm.vcxproj /p:Configuration=Release-Q2R /p:Platform=x64
if errorlevel 1 exit /b errorlevel
msbuild .\msvc\rocketmod_qmm.vcxproj /p:Configuration=Release-Q3A /p:Platform=x86
if errorlevel 1 exit /b errorlevel
msbuild .\msvc\rocketmod_qmm.vcxproj /p:Configuration=Release-Q3A /p:Platform=x64
if errorlevel 1 exit /b errorlevel
msbuild .\msvc\rocketmod_qmm.vcxproj /p:Configuration=Release-QUAKE2 /p:Platform=x86
if errorlevel 1 exit /b errorlevel
msbuild .\msvc\rocketmod_qmm.vcxproj /p:Configuration=Release-QUAKE2 /p:Platform=x64
if errorlevel 1 exit /b errorlevel
