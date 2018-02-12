#!/bin/bash
sudo apt install libasio-dev autoconf automake libtool curl make g++ unzip

bufver=3.4.0
wget https://github.com/google/protobuf/releases/download/v$bufver/protobuf-cpp-$bufver.tar.gz 
tar -xzf protobuf-cpp-$bufver.tar.gz
cd protobuf-$bufver
./autogen.sh
./configure
make 
make check 
sudo make install
sudo ldconfig 

cd ..

wget https://github.com/krpc/krpc/releases/download/v0.4.4/krpc-cpp-0.4.4.zip
unzip krpc-cpp-0.4.4.zip
cd krpc-cpp-0.4.4
./configure
make 
sudo make install
sudo ldconfig

cd ..

rm protobuf-cpp-$bufver.tar.gz
rm -r protobuf-cpp-$bufver
rm rm krpc-cpp-0.4.4.zip
rm -r krpc-cpp-0.4.4

