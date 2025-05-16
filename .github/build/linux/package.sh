#!/bin/sh
mkdir -p package
cd package
rm -f *
cp ../README.md ./
cp ../LICENSE ./

for x in Q3A; do
  cp ../bin/release-$x/x86/rocketmod_qmm_$x.so ./
  cp ../bin/release-$x/x86_64/rocketmod_qmm_x86_64_$x.so ./
done 

cd ..
