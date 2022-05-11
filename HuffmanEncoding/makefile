encode:
	gcc main.c -o main -fsanitize=address; ./main encode test.txt encode.txt


decode:
	gcc main.c -o main -fsanitize=address; ./main decode encode.txt decode.txt
