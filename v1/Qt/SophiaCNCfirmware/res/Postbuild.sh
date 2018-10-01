# Create archive
zip build/SrcOut.zip *.h *.cpp *.qrc *.pro *.user *.ui res/* include/*

# Copy to www folder
cp build/SrcOut.zip /var/www/html/cnc