mkdir src
mkdir build

#rm -R src/*
##rm -R build/*

cd src

rm SrcOut.zip
wget -N pc/cnc/SrcOut.zip

unzip -o SrcOut.zip

cd ../build

/opt/Qt5.10/bin/qmake ../src/SophiaCNCfirmware.pro

make -j4

./SophiaCNCfirmware

