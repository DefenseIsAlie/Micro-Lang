
entry: compiler

compiler: main.cpp specification.l grammar.ypp
	bison -d -t grammar.ypp -o grammar.cpp
	flex --outfile=lexxer.cpp specification.l
	g++ -O3 main.cpp grammar.cpp lexxer.cpp -o compiler


build/parser.cpp: src/grammar.y
	mkdir -p build/
	bison -d src/grammar.y -o $@

build/scanner.cpp: src/specification.l
	mkdir -p build/
	flex --outfile=$@ $^

test:
	./compiler $(F) > test.txt
	./a.out test.txt

unzip:
	rm -rf inputs/
	rm -rf outputs/
	unzip inputs.zip
	unzip outputs.zip

clean:
	rm -rf inputs/
	rm -rf outputs/
	rm -rf bin/
	rm -rf build/
	rm -rf grammar.cpp grammar.hpp lexxer.cpp Simulator test.elf *.out compiler

dev:
	@echo Josyula V N Taraka Abhishek
	@echo 200010021@iitdh.ac.in