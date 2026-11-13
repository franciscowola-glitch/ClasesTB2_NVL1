#pragma once
#include "Enemigo.h"

ref class Lobo : public Enemigo {

private:
	int radioLuz; // cuanto va ser el radio en el que se espanta
	int margenAcecho = 30; // margen de distancia para el lobo

public:
	Lobo(int x, int y, int vel) : Enemigo("Sprites//Lobo.png", x, y, 4, 5, 110, 110, 1, vel, 1, 10) 
	{
		this->primerFrameMovimiento = 0;
		this->ticksPorFrame = 2;
		this->margenHitbox = 20;
		this->radioLuz = 0;
	}
	~Lobo(){}

	// Setter de luz
	void SetRadioLuz(int r) {
		radioLuz = r;
	}

	// Comportamiento propio
	void Comportamiento() override  {
		
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

	// Sobreescritura de hitbox
	virtual Rectangle GetHitbox() override {
		return Rectangle(posicion.X+8, posicion.Y + 30, ancho-30, alto - 60);
	}

};
