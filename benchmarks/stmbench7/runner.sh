for x in {16..44}
do
	echo $x
	./code/sb7_tt -r false -s m -d 1000 -w r -t false -m false -n 64 -o ${x}p100 | head -n 100 | tail -n 1
done
