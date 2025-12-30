for %%x in (Q2R Q3A QUAKE2) do (
    msbuild .\msvc\rocketmod_qmm.vcxproj /p:Configuration=Release-%%x /p:Platform=x86
    msbuild .\msvc\rocketmod_qmm.vcxproj /p:Configuration=Release-%%x /p:Platform=x64
)
