#!/bin/sh
# Author  : Constantine Apostolou <conmarap@gmail.com>
# License : GNU GPL v3
#	    (c) Copyright 2012 Cinux
# Purpose : Script to build Cinux Core Utils
#-------------------------------------------------------

echo " * BUILD [Cinux Core Utils 0.1.9]"
sleep 1

#-------------------------------------------------------
# Build "tcpsniff"
#-------------------------------------------------------
cd raw_tcpsniff
gcc -o tcpsniff tcpsniff.c
echo "   --> tcpsniff"
mv tcpsniff bin/usr/bin || echo "   --> Compilation not successfull" 
zip -r tcpsniff bin
echo "   --> tcpsniff.zip"
cp tcpsniff.zip ../packs
rm tcpsniff.zip
cd ..
#-------------------------------------------------------

sleep 1
#-------------------------------------------------------
# Build "websid"
#-------------------------------------------------------
cd web_server
gcc -o websid websid.c
echo "   --> websid"
mv websid bin/usr/bin || echo "   --> Compilation not successfull"
zip -r websid bin
echo "   --> websid.zip"
cp websid.zip ../packs
rm websid.zip
cd ..
#-------------------------------------------------------

sleep 1
#-------------------------------------------------------
# Build "cintinser"
#-------------------------------------------------------
cd tiny_server
gcc -o cintinser main.c
echo "   --> cintinser"
mv cintinser bin/usr/bin || echo "   --> Compilation not successfull"
rm -r bin/usr/share/webroot
cp -r webroot bin/usr/share
zip -r cintinser bin
echo "   --> cintinser.zip"
cp cintinser.zip ../packs
rm cintinser.zip
cd ..
#-------------------------------------------------------

sleep 1
#-------------------------------------------------------
# Build "hostlookup"
#-------------------------------------------------------
cd host_lookup
gcc -o hstlk main.c
echo "   --> hstlk"
mv hstlk bin/usr/bin || echo "   --> Compilation not successfull"
zip -r hstlk bin
echo "   --> hstlk.zip"
cp hstlk.zip ../packs
rm hstlk.zip
cd ..
#-------------------------------------------------------

echo " * END_BUILD: Please make sure no errors occured, otherwise recompile"
