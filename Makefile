SHELL := /bin/bash
.ONESHELL : 
out/zenko_at_work.mp4 : zenko_at_work.jpg out/one_minute.wav
	ffmpeg -loop 1 -i zenko_at_work.jpg -i out/one_minute.wav -c:v libx264 -tune stillimage -c:a aac -b:a 192k -pix_fmt yuv420p -shortest $@

out/one_minute.wav : out/main one_hour.wav | out
	cd out
	./main ../one_hour.wav

out/main : main.c | out
	gcc -o $@ main.c -O3 -march=native -mtune=native -lsndfile

out : 
	mkdir out
