#pragma once
#include "Enemigo.h"

ref class Lobo : public Enemigo {

private:
	int radioLuz; // cuanto va ser el radio en el que se espanta
	int margenAcecho = 40; // margen de distancia para el lobo

public:
	Lobo(int x, int y, int vel) : Enemigo("Sprites//Lobo.png", x, y, 4, 5, 96, 96, 1, vel, 1, 10) 
	{
		this->primerFrameMovimiento = 0;
		this->ticksPorFrame = 4;
		this->margenHitbox = 22;
		this->radioLuz = 0;
	}
	~Lobo(){}

	// Setter de luz
	void SetRadioLuz(int r) {
		radioLuz = r;
	}

	// Comportamiento propio
	void Comportamiento() override {
		
		if (objetivo == nullptr) { return; }
		
		int distancia = (int)this->DistanciaA(this->objetivo); // distancia al objetivo
	
		if (distancia < radioLuz) { // si distancia < radio -> alejarse 
			this->AlejarseDe(objetivo->GetCentroX(), objetivo->GetCentroY());
		}

		else if (distancia >= radioLuz && distancia <= radioLuz + margenAcecho) {
			this->Mover(0, 0); // si esta mayor a radio luz y menor al radio mas acecho -> quieto
		}
	
		else if (distancia > radioLuz + margenAcecho) {
			this->MoverHacia(objetivo->GetCentroX(),objetivo->GetCentroY());
		}

	}


};
