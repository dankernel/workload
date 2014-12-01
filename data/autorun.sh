
#!/bin/sh
for loopdirectory in `ls -d ./*`; do
  basename=$(basename $loopdirectory) 
  basename=${basename%.*csv.gz}
  gzip -d -c $basename.csv.gz > $basename.csv
done 

