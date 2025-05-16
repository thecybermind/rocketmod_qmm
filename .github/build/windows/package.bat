mkdir package
pushd package
del /q *
rem copy ..\README.md .\
rem copy ..\LICENSE .\

for %%x in (
        Q3A
       ) do (
         copy ..\bin\Release-%%x\x86\rocketmod_qmm_%%x.dll .\
         copy ..\bin\Release-%%x\x64\rocketmod_qmm_x86_64_%%x.dll .\         
       )        
popd
