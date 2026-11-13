#pragma once
#include "Nivel.h"
#include "Lobo.h"
#include "Padre.h"
#include "ElementosNivel1.h"

ref class NivelPadre : public Nivel {

private:

	// ELEMENTOS MOVIBLES
	Padre^ padre;
	List<Lobo^>^ lobos;

	// PINTURAS
	SolidBrush^ pincelOscuro = gcnew SolidBrush(Color::FromArgb(0, 0, 0, 15));

public:

	// CONSTRUCTORES Y DESTRUCTOR
	NivelPadre(GestorArchivos^ archivos) :Nivel(archivos) {	
		this->lobos = gcnew List<Lobo^>(20);
	}
	~NivelPadre() {
		delete pincelOscuro;
	}

	// SOBREESCRITURA DE METODOS
	virtual void CrearEntidades() override {

		this->fondo = gcnew Bitmap("Sprites//FondoNivel1.png"); // fondo

		this->padre = gcnew Padre(850, 80, 5);
		this->jugador = padre; // crear al padre y asignarlo como el jugador del nivel

		// CREAR ARBOLES
		this->AgregarObstaculo(gcnew ArbolNivel1( 200, 300, 1, 1, 80, 80, 3));
		this->AgregarObstaculo(gcnew ArbolNivel1( 800, 640, 1, 1, 80, 80, 3));
		this->AgregarObstaculo(gcnew ArbolNivel1( 300, 200, 1, 1, 80, 80, 3));
		this->AgregarObstaculo(gcnew ArbolNivel1( 100, 410, 1, 1, 80, 80, 3));
		this->AgregarObstaculo(gcnew ArbolNivel1( 970, 320, 1, 1, 80, 80, 3));
		this->AgregarObstaculo(gcnew ArbolNivel1(1070, 100, 1, 1, 80, 80, 3));

		// AGREGAR TODOS LOS LOBOS A LA LISTA LOBOS Y LUEGO A ENEMIGOS
		this->lobos->Add(gcnew Lobo(100, 100, 4));
		this->lobos->Add(gcnew Lobo(100, 550, 4));
		this->lobos->Add(gcnew Lobo(1210, 650,4));

		for (int i = 0; i < lobos->Count; i++) {
			this->AgregarEnemigo(lobos[i]);
		}

		// RESINA o ACEITE
		this->AgregarRecurso(gcnew Resina(500, 500, 50,50, 10));
		this->AgregarRecurso(gcnew Resina(20 , 400, 50,50, 10));
		this->AgregarRecurso(gcnew Resina(500, 500, 50,50, 10));




	}

	virtual void LogicaPropia(Teclado^ t) override {
		
		padre->GastarAceite(); // al padre se le gasta el aceite

		// asignar el radio de luz del padre a todos los lobos
		for (int i = 0; i < lobos->Count; i++) { 
			lobos[i]->SetRadioLuz(padre->GetRadioLuz());
		}


		

	}

	virtual void ConfigurarNarrativa() override {

	}

	virtual bool ObjetivoCumplido() override {
		return false;
	}

	virtual String^ GetTitulo() override {
		return "La noche en el bosque";
	}

	virtual void DibujarExtras(Graphics^ g) override {

		// Oscuridad de la noche y el aro de luz de la lampara
		int cx = jugador->GetX();
		int cy = jugador->GetY();
		int r = padre->GetRadioLuz();

		Region^ oscuro = gcnew Region(Rectangle(0,0,ANCHO_JUEGO,ALTO_JUEGO));
		GraphicsPath^ luz = gcnew GraphicsPath();
		luz->AddEllipse(cx-r+43, cy-r+45, r*2, r*2);
		oscuro->Exclude(luz);
		g->FillRegion(pincelOscuro, oscuro);



		delete luz;
		delete oscuro;
	}

	virtual void DibujarHUD(Graphics^ g) override {

		if (this->jugador == nullptr) { return; }

		Font^ letra1 = gcnew Font("Calibri", 18);
		RectangleF RectanguloHUD = RectangleF(10, 10, 340, 80);

		g->FillRectangle(fondoHUD, RectanguloHUD);
		g->DrawString(jugador->GetNombre(), letra1, Brushes::White, RectanguloHUD.X + 5,RectanguloHUD.Y);

		for (int i = 0; i < this->jugador->GetVidasMax(); i++) {
			
			if (i < this->jugador->GetVidas()) {
				g->FillEllipse(Brushes::Red, 20 + (28 * i), 48, 20, 20);
			}

			else {
				g->FillEllipse(Brushes::DarkGray, 20 + (28 * i), 48, 20, 20);
			}

		}

		g->DrawString("Puntaje: " + Convert::ToString(this->puntaje), letra1, Brushes::White, 190, 20);
		g->DrawString("Aceite: " + Convert::ToString(this->padre->GetAceite()), letra1, Brushes::White, 190, 45);

		delete letra1;

	} // Nombre del prota, vidas etc de acuerdo al nvl


	//____________________________________________________________________________
	
	virtual void AlTocarRecurso(Recurso^ r) override {
		
		// si es aceite, entonces el padre recarga 
		if (r->GetTipo() == TipoRecurso::Aceite) {
			padre->RecargarAceite(r->GetValor());
		}



		Nivel::AlTocarRecurso(r);
	}
	
	virtual void AlChocarConEnemigo(Enemigo^ e) override  {
		this->jugador->RecibirDanio(e->GetDanioContacto());
	} 	// Todo nivel puede tener un evento de chocar con enemigo, cada nivel decide que ocurre, el comportamiento general es que al chocar se reciba daño

	virtual void AlImpactarEnemigo(Proyectil^ p, Enemigo^ e)  override {
		e->RecibirDanio(p->GetDanio());
		p->Desactivar();
		if (!e->EstaVivo()) {
			this->puntaje += e->GetPuntos();
		}
	} 	// Proyectil de jugador a un enemigo e

	virtual void AlImpactarJugador(Proyectil^ p)  override {
		this->jugador->RecibirDanio(p->GetDanio());
		p->Desactivar();
	} 	// Proyectil DANIA a el jugador

	virtual void AlChocarConObstaculo(Obstaculo^ o)  override {

		this->jugador->DeshacerMovimiento();

	}	// ál Chocar con un obstaculo


};
