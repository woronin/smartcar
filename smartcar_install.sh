#!/bin/bash  
#
# 07/11/2017 woronin 
# sudo apt-get install libqt4-dev, qt4-dev-tools, g++, geany
#
echo "This is a shell script install program smartcar"  
mkdir $HOME/.config/smartcar
cp smart.ini skript.kum logo_umki.png  $HOME/.config/smartcar/.
echo "/usr/local/bin/smartcar -b">smartcar_b
chmod 777 smartcar_b
sudo cp smartcar smartcar_b /usr/local/bin/.
sudo cp smartcar.desktop /usr/share/applications/.
echo "done"
/usr/local/bin/smartcar_b
