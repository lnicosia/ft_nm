#!/bin/bash

OPT="$@"

echo "$file diff:"
./ft_nm $OPT > ft_nm.txt
nm $OPT > nm.txt
diff ft_nm.txt nm.txt
