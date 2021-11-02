#!/bin/bash

file=a.out
echo "$file diff:"
./ft_nm $file > ft_nm.txt
nm $file > nm.txt
diff ft_nm.txt nm.txt
#rm ft_nm.txt nm.txt

file=ft_nm
echo "$file diff:"
./ft_nm $file > ft_nm.txt
nm $file > nm.txt
diff ft_nm.txt nm.txt
#rm ft_nm.txt nm.txt

for arg in $@
do
	file=$arg
	echo "$file diff:"
	./ft_nm $file > ft_nm.txt
	nm $file > nm.txt
	diff ft_nm.txt nm.txt
	#rm ft_nm.txt nm.txt
done
