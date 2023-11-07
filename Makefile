comp1: cg.c expr.c gen.c interp.c main.c scan.c tree.c
	cc -g $^ -o $@

compn: cgn.c expr.c gen.c interp.c main.c scan.c tree.c
	cc -g $^ -o $@

clean:
	rm -rf comp1 compn *.o *.s out

test: comp1
	./comp1 input01
	cc -o out out.s
	./out
	./comp1 input02
	cc -o out out.s
	./out
