# Make file 
all:
	gcc main.c -o main
	gcc lib/api.c -o api -lcurl

main:
	gcc main.c -o main
api:
	gcc lib/api.c -o api -lcurl
