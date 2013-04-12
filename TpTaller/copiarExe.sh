cp ./Debug/TpTaller .
valgrind -v --leak-check=full --suppressions=./valgrind-rules.supp ./TpTaller 

