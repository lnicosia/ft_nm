#!/bin/bash

#OPT="a --size"
OPT="a --size"

file=a.out
ft_nm_output=ft_nm-$file.txt
echo "$file diff:"
./ft_nm $file -$OPT > $ft_nm_output
nm_output=nm-$file.txt
nm $file -$OPT > $nm_output
diff $ft_nm_output $nm_output
#rm ft_nm.txt nm.txt

file=ft_nm
ft_nm_output=ft_nm-$file.txt
echo "$file diff:"
./ft_nm $file -$OPT > $ft_nm_output
nm_output=nm-$file.txt
nm $file -$OPT > $nm_output
diff $ft_nm_output $nm_output
#rm ft_nm.txt nm.txt

for arg in $@
do
	file=$arg
	echo "$file diff:"
	./ft_nm $file -$OPT > ft_nm.txt
	nm $file -$OPT > nm.txt
	diff ft_nm.txt nm.txt
	#rm ft_nm.txt nm.txt
done
