
if [ $# -ne 2 ]; then
	exit
fi

s_file=$1
d_file=$2

num=`grep -n playerLocations $s_file 2>/dev/null | cut -d ':' -f 1  2>/dev/null`
num=`echo $num+1 | bc   2> /dev/null`
sed "${num}s/^.*$/  - name: NAME_TAG/g" $s_file 2>/dev/null > $d_file 
