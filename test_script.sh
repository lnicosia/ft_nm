#!/bin/bash

file=a.out
echo "$file diff:"
./ft_nm $file > ft_nm.txt
nm $file > nm.txt
diff ft_nm.txt nm.txt
rm ft_nm.txt nm.txt

file=ft_nm
echo "$file diff:"
./ft_nm $file > ft_nm.txt
nm $file > nm.txt
diff ft_nm.txt nm.txt
rm ft_nm.txt nm.txt

if [ $# = 1 ]
then
	file=$1
	echo "$file diff:"
	./ft_nm $file > ft_nm.txt
	nm $file > nm.txt
	diff ft_nm.txt nm.txt
	rm ft_nm.txt nm.txt
fi
