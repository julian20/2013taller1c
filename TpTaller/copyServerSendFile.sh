
s_file=$1
d_file=$2

num=`grep -n playerLocations $s_file | cut -d ':' -f 1`
num=`echo $num+1 | bc`
sed "${num}s/^.*$/  - name: NAME_TAG/g" $s_file > $d_file
