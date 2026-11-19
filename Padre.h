#pragma once
#include "Protagonista.h"

ref class Padre : public Protagonista {

private: // particular
	int aceite;
	int aceiteMax;
	int ticksGasto;
	int contadorAceite; 

public:
	Padre(int x, int y, int vidas) :
		Protagonista("Sprites//Padre.png", x, y, 4, 9, 110, 110, vidas,5)
	{
		this->ticksPorFrame = 2;
		this->margenHitbox = 15;

		this->aceiteMax = 100;
		this->aceite = aceiteMax;
		this->ticksGasto = 30; // tiempo para que el aceite se baje
		this->contadorAceite = 0;
		this->primerFrameMovimiento = 0;

	}
	~Padre() {}
	
	// Sobreescritura de metodos
	String^ GetNombre() override {
		return "Padre";
	}
	Proyectil^ Atacar() override {
		return nullptr;
	}

	// Metodos del aceite
	void GastarAceite() {

		if (aceite > 0) {
			contadorAceite++;
		}
		if (contadorAceite >= ticksGasto) {
			this->aceite -= 1;
			contadorAceite = 0;
		}

	}
	void ReducirAceite(int n) {
		this->aceite -= n;
	}
	void RecargarAceite(int cant) {
		if (aceite + cant >= aceiteMax) {
			aceite = aceiteMax;
		}
		else {
			aceite += cant;
		}

	}
	void AceiteAgotado() {
		this->aceite = 0;
	}


	// Getters aceite
	int GetRadioLuz() {
		int radioAceite = 40 + (int)(aceite * 1.5);
		return radioAceite;
	}
	int GetAceite() {
		return aceite;
	}
	int GetAceiteMax() {
		return aceiteMax;
	}
	void GastarPorGolpeDeHacha() {
		if (this->GetAceite() - 4 <= 0) {
			this->AceiteAgotado();
		}
		else {
			this->ReducirAceite(4);
		}
	}

	// Sobreescritura de hitbox
	virtual Rectangle GetHitbox() override {
		return Rectangle(posicion.X+35, posicion.Y+12, ancho-70, alto-25);
	}

	void BajarVida() {
		this->vidas -= 1;
	}

};
