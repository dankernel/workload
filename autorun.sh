
#!/bin/sh
for loopdirectory in `ls -d data/*`; do
  ./main $loopdirectory;
done 

