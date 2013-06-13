./copiarExe.sh 2> /dev/null
port=$(sed -n "s/^.*serverPort:[ ]\([0-9][0-9]*\)/\1/p" ./configuration/server/serverLocalConfig.yaml)

while true; do 
	./TpTaller -sn
	if [ "$?" -eq "2" ]; then 
		kill -9 $(lsof -n -i| grep "$port" | sed -n "s/^[a-Z]*[ ]\([0-9][0-9]*\)[ ].*$/\1/p" | grep -m 1 ".*") 2> /dev/null
	else
		exit
	fi
	echo "Reiniciando el servidor ..."
	sleep 5
done
