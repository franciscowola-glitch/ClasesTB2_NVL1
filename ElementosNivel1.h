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
		return Rectangle(posicion.X + 30, posicion.Y + 20, ancho - 60, alto - 40);
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

