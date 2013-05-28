
if [ $# -ne 2 ]; then
	exit
fi

s_file=$1
d_file=$2

num=`grep -n playerLocations $s_file 2>/dev/null | cut -d ':' -f 1  >/dev/null 2>&1`
num=`echo $num+1 | bc   > /dev/null 2>&1`
sed "${num}s/^.*$/  - name: NAME_TAG/g" $s_file 2>/dev/null 1> $d_file 
