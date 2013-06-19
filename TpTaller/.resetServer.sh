./copiarExe.sh 2> /dev/null
port=$(sed -n "s/^.*serverPort:[ ]\([0-9][0-9]*\)/\1/p" ./configuration/server/serverLocalConfig.yaml)

while true; do 
	./TpTaller -sn
	result="$?"
	if [ "$result" -eq "2" ]; then 
		kill -9 $(lsof -n -i| grep "$port" | sed -n "s/^[a-Z]*[ ]\([0-9][0-9]*\)[ ].*$/\1/p" | grep -m 1 ".*") 2> /dev/null
	fi
	if [ "$result" -eq "5" ]; then
		echo "Error al bindear al puerto, reintentando en 5 segundos"
		sleep 1
		echo -n "."
		sleep 1
		echo -n "."
		sleep 1
		echo -n "."
		sleep 1
		echo -n "."
		sleep 1
		echo "."
		continue
	fi
	if [ "$result" -ne "2" ] && [ "$result" -ne "5" ]; then
		exit
	fi
	echo "Reiniciando el servidor ..."
done
