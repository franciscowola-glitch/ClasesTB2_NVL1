#pragma once
#include "Obstaculo.h"

ref class Npc : public Obstaculo {

public:
	Npc(String^ ruta, int x, int y, int filas, int columnas, int ancho, int alto) : 
		Obstaculo (ruta,x,y,filas,columnas,ancho,alto,0)
	{
		this->primerFrameMovimiento = 0;
		this->ticksPorFrame = 3;
	}
	~Npc(){}

	void Actualizar() override {

		animando = true;
		Animar();


	}



};
