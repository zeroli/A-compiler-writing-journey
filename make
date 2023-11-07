set -x
gcc -o parser -g main.c scan.c expr.c tree.c interp.c
gcc -o parser2 -g main.c scan.c expr2.c tree.c interp.c
