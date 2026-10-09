for %%G in (Q2R Q3A QUAKE2 SOF) do (
    if [%%G] NEQ [Q2R] msbuild .\msvc\rocketmod_qmm.vcxproj /p:Configuration=Release-%%G /p:Platform=x86
    if errorlevel 1 exit /b errorlevel
    msbuild .\msvc\rocketmod_qmm.vcxproj /p:Configuration=Release-%%G /p:Platform=x64
    if errorlevel 1 exit /b errorlevel
)
