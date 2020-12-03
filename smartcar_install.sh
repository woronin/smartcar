#!/bin/bash  
#
# 03/12/2020 woronin 
# sudo apt-get install libqt5-dev, qt5-dev-tools, gcc4.8-c++, geany , gedit, libQtGui.so.4 , LibQtXml.so.4, qt5-webkit-devel
#
#
echo "This is a shell script install program smartcar"  
mkdir $HOME/.config/smartcar
cp smart.ini skript.kum logo_umki.png  $HOME/.config/smartcar/.
echo "/usr/local/bin/smartcar -b">smartcar_b
chmod 777 smartcar_b
sudo cp smartcar smartcar_b /usr/local/bin/.
sudo cp Smartcar.desktop /usr/share/applications/.
echo "done"
/usr/local/bin/smartcar_b
