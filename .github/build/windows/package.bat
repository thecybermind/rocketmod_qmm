mkdir package
pushd package
del /q *
rem copy ..\README.md .\
rem copy ..\LICENSE .\
copy ..\bin\Release-Q2R\x64\rocketmod_qmm_x86_64_Q2R.dll .\
copy ..\bin\Release-Q3A\x86\rocketmod_qmm_Q3A.dll .\
copy ..\bin\Release-Q3A\x64\rocketmod_qmm_x86_64_Q3A.dll .\
copy ..\bin\Release-QUAKE2\x86\rocketmod_qmm_QUAKE2.dll .\
copy ..\bin\Release-QUAKE2\x64\rocketmod_qmm_x86_64_QUAKE2.dll .\
popd
