all:
	g++ Shi_Tomasi_nona.cpp `pkg-config opencv --libs --cflags opencv` -o Shi_Tomasi_nona
	g++ Shi_Tomasi.cpp `pkg-config opencv --libs --cflags opencv` -o Shi_Tomasi

Shi_Tomasi:
	g++ Shi_Tomasi.cpp `pkg-config opencv --libs --cflags opencv` -o Shi_Tomasi

Shi_Tomasi_nonea:
	g++ Shi_Tomasi_nona.cpp `pkg-config opencv --libs --cflags opencv` -o Shi_Tomasi_nona

clean:
	rm Shi_Tomasi
	rm Shi_Tomasi_nona
