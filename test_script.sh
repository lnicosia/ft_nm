#!/bin/bash

OPT="-arovpn --format=posix"

file=a.out
echo "$file diff:"
./ft_nm $file $OPT > ft_nm.txt
nm $file $OPT > nm.txt
diff ft_nm.txt nm.txt
#rm ft_nm.txt nm.txt

file=ft_nm
echo "$file diff:"
./ft_nm $file $OPT > ft_nm.txt
nm $file $OPT > nm.txt
diff ft_nm.txt nm.txt
#rm ft_nm.txt nm.txt

for arg in $@
do
	file=$arg
	echo "$file diff:"
	./ft_nm $file $OPT > ft_nm.txt
	nm $file $OPT > nm.txt
	diff ft_nm.txt nm.txt
	#rm ft_nm.txt nm.txt
done
