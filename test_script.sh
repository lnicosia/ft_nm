#!/bin/bash

OPT=""

./ft_nm $OPT > ft_nm.txt
echo "$?" > ft_nm-ret.txt
nm $OPT > nm.txt
echo "$?" > nm-ret.txt
echo "Output diff:"
diff ft_nm.txt nm.txt
echo "Return diff:"
diff ft_nm-ret.txt nm-ret.txt
