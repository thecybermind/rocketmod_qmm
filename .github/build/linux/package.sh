#!/bin/sh
mkdir -p package
cd package
rm -f *
cp ../README.md ./
cp ../LICENSE ./
cp ../bin/release-Q3A/x86/rocketmod_qmm_Q3A.so ./
cp ../bin/release-Q3A/x86_64/rocketmod_qmm_x86_64_Q3A.so ./
cp ../bin/release-QUAKE2/x86/rocketmod_qmm_QUAKE2.so ./
cp ../bin/release-QUAKE2/x86_64/rocketmod_qmm_x86_64_QUAKE2.so ./
cd ..
