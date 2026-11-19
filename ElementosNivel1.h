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
