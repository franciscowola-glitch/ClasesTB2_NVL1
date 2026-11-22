#pragma once
#include "Obstaculo.h"
#include "Recurso.h"

ref class ArbolNivel1 : public Obstaculo {

public:
	ArbolNivel1(int x, int y, int filas, int columnas, int ancho, int alto, int resistencia) :
		Obstaculo("Sprites//Arbol.png", x, y, filas, columnas, ancho, alto, resistencia)
	{

	}
	~ArbolNivel1() {}


	// Sobreescritura de hitbox
	virtual Rectangle GetHitbox() override {
		return Rectangle(posicion.X + 40, posicion.Y + 30, ancho - 80, alto - 60);
	}


};

ref class Resina : public Recurso {

public:
	Resina(int x, int y, int ancho, int alto, int valor) : Recurso("Sprites//Resina.png", x, y, 1, 1, ancho, alto, TipoRecurso::Aceite, valor)
	{
	}
	~Resina() {}

	// Sobreescritura de hitbox
	virtual Rectangle GetHitbox() override {
		return Rectangle(posicion.X+10, posicion.Y+10, ancho-20, alto-20);
	}


};

ref class LenaArbol : public Recurso {

public:

	LenaArbol(int x, int y, int ancho, int alto, int valor) : Recurso("Sprites//Tronco.png", x, y, 1, 1, ancho, alto, TipoRecurso::Lena, valor)
	{
	}
	~LenaArbol() {}

	// Sobreescritura de hitbox
	virtual Rectangle GetHitbox() override {
		return Rectangle(posicion.X + 40, posicion.Y + 35, ancho - 80, alto - 70);
	}

};

ref class MontonDePiedras : public Obstaculo {

public:
	MontonDePiedras(int x, int y, int ancho, int alto) :
		Obstaculo("Sprites//MontonPiedras.png", x, y, 1, 1, ancho, alto, 1000)
	{

	}
	~MontonDePiedras() {}


	// Sobreescritura de hitbox
	virtual Rectangle GetHitbox() override {
		return Rectangle(posicion.X + 35, posicion.Y + 20, ancho - 70, alto - 60);
	}


};

ref class PozoDeAgua : public Obstaculo {

public:
	PozoDeAgua(int x, int y, int ancho, int alto) :
		Obstaculo("Sprites//PozoDeAgua.png", x, y, 1, 1, ancho, alto, 0)
	{

	}
	~PozoDeAgua() {}


	// Sobreescritura de hitbox
	virtual Rectangle GetHitbox() override {
		return Rectangle(posicion.X + 75, posicion.Y + 70, ancho - 150, alto - 100);
	}


};

ref class Puerta : public Obstaculo {

private:
	bool abriendo;
	bool abierta;
	int contadorTicks;

public:
	Puerta(int x, int y) :
		Obstaculo("Sprites//Puerta.png", x, y, 1, 5, 200, 152, 0)
	{
		abriendo = false;
		abierta = false;
		contadorTicks = 0;
	}
	~Puerta(){

	}

	void Abrir() {
		this->abriendo = true;
	}

	void Actualizar() override {
		contadorTicks++;
		if (abriendo && contadorTicks > 6) {
			frameX++;
			contadorTicks = 0;
		}
		// ultima fase de apertura 
		if (frameX == 4) {
			abriendo = false;
			abierta = true;
		}
	}

	// Sobreescritura de hitbox
	virtual Rectangle GetHitbox() override {
		return Rectangle(posicion.X + ancho * 10 / 100,
			posicion.Y,
			ancho * 80 / 100,
			alto - 75);
	}


	void SetAbierta() {
		frameX = 4;
		abierta = true;
	}
	bool GetAbierta() {
		return this->abierta;
	}


};
