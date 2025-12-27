#!/bin/sh
mkdir -p package
cd package
rm -f *
cp ../README.md ./
cp ../LICENSE ./

for f in Q3A; do
  cp ../bin/release-$f/x86/rocketmod_qmm_$f.so ./
  cp ../bin/release-$f/x86_64/rocketmod_qmm_x86_64_$f.so ./
done

cd ..
