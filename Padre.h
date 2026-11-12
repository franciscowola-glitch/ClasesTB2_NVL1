#pragma once
#include "Protagonista.h"

ref class Padre : public Protagonista {

private:
	int aceite;
	int aceiteMax;
	int ticksGasto;
	int contadorAceite;

public:
	Padre(int x, int y, int vidas) :
		Protagonista("Sprites//Padre.png", x, y, 4, 9, 85, 85, vidas, 4)
	{
		this->ticksPorFrame = 4;
		this->margenHitbox = 15;

		this->aceiteMax = 100;
		this->aceite = aceiteMax;
		this->ticksGasto = 35;
		this->contadorAceite = 0;

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
	void RecargarAceite(int cant) {
		if (aceite + cant >= aceiteMax) {
			aceite = aceiteMax;
		}
		else {
			aceite += cant;
		}

	}
	
	// Getters aceite
	int GetRadioLuz() {
		int radioAceite = 60 + (int)(aceite * 1.5);
		return radioAceite;
	}
	int GetAceite() {
		return aceite;
	}
	int GetAceiteMax() {
		return aceiteMax;
	}



};
