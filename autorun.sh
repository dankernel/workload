
#!/bin/sh
for loopdirectory in `ls -d data/*.csv`; do
  ./main $loopdirectory;
done 

