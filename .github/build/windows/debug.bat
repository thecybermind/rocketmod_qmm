for %%x in (Q3A) do (
    msbuild .\msvc\rocketmod_qmm.vcxproj /p:Configuration=Debug-%%x /p:Platform=x86
    msbuild .\msvc\rocketmod_qmm.vcxproj /p:Configuration=Debug-%%x /p:Platform=x64
)
