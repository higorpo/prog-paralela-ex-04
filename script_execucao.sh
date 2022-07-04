gcc -o reproduz_texto reproduz_texto.c -pthread

rm saida.txt
for ((j=1; j <= 6; j++))
do
	echo "Executando $j..."
	./reproduz_texto 10000000 >> saida.txt
    sleep 0.2
done

rm ./reproduz_texto