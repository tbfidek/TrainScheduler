#fisier folosit pentru compilarea serverului&clientului TCP iterativ

all:
	g++ serv1.cpp ./files/*.cpp ./Commands/*.cpp -I. -o s1 
	g++ cl1.cpp -o cl1 

clean:
	rm -f cl1 s1
