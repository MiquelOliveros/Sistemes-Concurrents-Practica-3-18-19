#!/bin/bash 
#################################################################
#   Codi Font													#
#	Practica 1 Sistemes Concurrents i Paralels					#
#	Alumnes:													#
#		Joan Palau Oncins		- 47692135W						#
#		Miquel Oliveros Muelas	- 0024201						#
################################################################# 

#S'executara aquest script cada vegada que es vulgui realitzar un test
#La comanda periniciar elservidor esposarà manualment
#Previ a llençar la comanda s'ha de modificar el servidor per a que al cap de 100 clients tancats es tanqui
#modificant el loop while true
FTP_PATH="../.."
for i in {1..100}
do
	cd example_dir
	$FTP_PATH/siftp localhost 77777 < commands1.txt &
	cd ..
	pids[${i}]=$! 
done

for pid in ${pids[*]}
do
	wait $pid
done
exit 1