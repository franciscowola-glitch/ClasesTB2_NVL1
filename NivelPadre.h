#pragma once
#include "Nivel.h"
#include "Lobo.h"
#include "Padre.h"
#include "ElementosNivel1.h"
#include "Efecto.h"

ref class NivelPadre : public Nivel {

private:

	// ELEMENTOS MOVIBLES
	Padre^ padre;

	// ALIADOS
	Aliado^ hansel;
	Aliado^ gretel;

	List<Lobo^>^ lobos;
	List<ArbolNivel1^>^ arboles; // lista de arboles para cortar
	List<Efecto^>^ efectos; // lista de efectos (hacha)


	// PINTURAS
	SolidBrush^ pincelOscuro = gcnew SolidBrush(Color::FromArgb(235, 0, 0, 15));
	SolidBrush^ pincelAzul = gcnew SolidBrush(Color::Blue);


	// MECANICAS DEL CULPA
	int confianzaMax = 100;
	int confianza = confianzaMax;
	int ticksBajarCulpa = 2;
	int contadorTicksCulpa = 0;

	// SALIDA MUNDO 1
	Rectangle salidaMundo1 = Rectangle(615,704,75,10);



private: // CORTE DE ARBOLES
	int lenaRecogida = 0;
	int lenaNecesaria = 5;

	int cooldownHacha = 10; // 10 ticks para que el hacha no sea espameable
	int COOLDOWN_HACHA = 10; // tiempo de espera hachazos

public:

	// CONSTRUCTORES Y DESTRUCTOR
	NivelPadre(GestorArchivos^ archivos) :Nivel(archivos) {	
		this->lobos =	gcnew List<Lobo^>(20);
		this->arboles = gcnew List<ArbolNivel1^>(20);
		this->efectos = gcnew List<Efecto^>(20);
	}
	~NivelPadre() {
		delete pincelOscuro;
	}

private:
	// ----------- PROPIOS DEL NIVEL ---------------------------

	void ActualizarLuzYLobos() {
		// asignar el radio de lux del padre a todos los lobos
		for (int i = 0; i < lobos->Count; i++) {
			lobos[i]->SetRadioLuz(padre->GetRadioLuz());
		}
	}
	void ActualizarConfianza() {
		// revisar si los ninios se elajaron lo suficiente del radio de luz
		if (contadorTicksCulpa > 0) { contadorTicksCulpa--; }

		bool hanselFuera = this->hansel->DistanciaA(padre) > padre->GetRadioLuz();
		bool gretelFuera = this->gretel->DistanciaA(padre) > padre->GetRadioLuz();

		if (hanselFuera || gretelFuera) {
			if (this->contadorTicksCulpa == 0) {
				confianza--;
				contadorTicksCulpa = ticksBajarCulpa;
			}
		}

		else {
			if (this->contadorTicksCulpa == 0) {
				if (confianza < confianzaMax) {
					confianza++;
				}
				contadorTicksCulpa = 25;
			}
		}

		if (this->confianza <= 0) {
			padre->BajarVida();
			confianza = 50;
			Decir("Hansel y Gretel: Papa, tenemos miedo, no nos dejes...");
		}
	}
	void ProcesarHacha(Teclado^ t) {
		// bajar el contador del cooldown, cada 20 podra golpear
		if (cooldownHacha > 0) {
			cooldownHacha--;
		};

		// buscar arboles, si uno esta a menos distancia que 90, entonces colision
		if (cooldownHacha == 0 && t->RecienPresionada(Keys::Space)) {
			for (int i = 0; i < arboles->Count; i++) {
				if (arboles[i]->DistanciaA(padre) < 90 && arboles[i]->GetActivo() == true) {

					arboles[i]->Golpear(); // se golpea el arbol y se baja un punto de resistencia

					this->padre->GastarPorGolpeDeHacha();


					switch (padre->GetFila())
					{

					case 0: { // abajo
						this->efectos->Add(gcnew Efecto("Sprites//Hacha.png", padre->GetCentroX() - 30, padre->GetCentroY() - 5, 4, 9, 70, 70, 1, padre->GetFila()));
						break;
					}
					case 1: { // izquierda
						this->efectos->Add(gcnew Efecto("Sprites//Hacha.png", padre->GetCentroX() - 50, padre->GetCentroY() - 25, 4, 9, 70, 70, 1, padre->GetFila()));
						break;
					}
					case 2: { // derecha
						this->efectos->Add(gcnew Efecto("Sprites//Hacha.png", padre->GetCentroX() - 18, padre->GetCentroY() - 25, 4, 9, 70, 70, 1, padre->GetFila()));
						break;
					}
					case 3: { // arriba
						this->efectos->Add(gcnew Efecto("Sprites//Hacha.png", padre->GetCentroX() - 32, padre->GetCentroY() - 75, 4, 9, 70, 70, 1, padre->GetFila()));
						break;
					}

					default:
						break;
					}

					this->cooldownHacha = this->COOLDOWN_HACHA;

					// si el arbol se murio, entonces agregar una lena y resina en tal posicion
					if (arboles[i]->GetActivo() == false) {
						int x = arboles[i]->GetX();
						int y = arboles[i]->GetY();
						int ancho = 90;
						int alto = 90;
						int valor = 3;

						this->AgregarRecurso(gcnew LenaArbol(x, y, ancho, alto, valor));
					}

					break; // salir , no es necesario revisar otros arboles.

				}
			}
		}

		// LIMPIAR HACHAS INACTIVAS
		for (int i = 0; i < efectos->Count; ) {
			efectos[i]->Actualizar(); // actualizar
			if (!efectos[i]->GetActivo()) {
				efectos->RemoveAt(i);
			}
			else {
				i++;
			}
		}
	}

	// FASE 2
	void CrearEntidadesFase2() {
		// FASE 2
		lobos->Add(gcnew Lobo(20, 500, 3));
		lobos->Add(gcnew Lobo(1000, 600, 3));
		for (int i = 0; i < lobos->Count; i++) {
			AgregarEnemigo(lobos[i]);
		}
		
		this->AgregarRecurso(gcnew Resina(100, 450, 50, 50, 20));
		this->AgregarRecurso(gcnew Resina(360, 200, 50, 50, 20));
		this->AgregarRecurso(gcnew Resina(899, 687, 50, 50, 20));



	}
	void CargarMundo2() {

		this->SetFondo("Sprites//FondoNivel1_2.png"); // cambiar fondo
		this->enemigos->Clear();
		this->obstaculos->Clear();
		this->recursos->Clear();
		this->proyectiles->Clear();
		this->lobos->Clear();
		this->arboles->Clear();
		this->efectos->Clear();

		// ACTUALIZAR SPWN de pesonajes
		padre->SetPosicion(973,69);
		hansel->SetPosicion(1039,73);
		gretel->SetPosicion(1101,72);

		CambiarFase(2);
		Decir("Padre: Ya cruzamos una buena parte del bosque, debemos mantenernos atentos...");
		
		// CREAR ENTIDADES NIVEL 2
		CrearEntidadesFase2();
		





	}


public:

	// SOBREESCRITURA DE METODOS
	virtual void CrearEntidades() override {

		this->fondo = gcnew Bitmap("Sprites//FondoNivel1_1.png"); // fondo

		this->padre = gcnew Padre(850, 80, 5);
		this->jugador = padre; // crear al padre y asignarlo como el jugador del nivel

		// CREAR ALIADO
		this->AgregarAliado(gcnew Aliado("Sprites//Hansel.png",850,50,4,9,64,80,4,50));
		this->AgregarAliado(gcnew Aliado("Sprites//Gretel.png", 900, 50, 4, 9, 64, 80, 3, 50));
		this->hansel = aliados[0];
		this->gretel = aliados[1];
		
		this->hansel->SetLider(padre);
		this->gretel->SetLider(hansel);


		// CREAR ARBOLES EN LA LISTA Y PASARLOS A LA LISTA DE OBSTACULOS
		this->arboles->Add(gcnew ArbolNivel1( 970, 320, 1, 1, 120, 120, 2));
		this->arboles->Add(gcnew ArbolNivel1(1070, 100, 1, 1, 120, 120, 2));
		this->arboles->Add(gcnew ArbolNivel1(275, 500, 1, 1, 120, 120, 2));
		this->arboles->Add(gcnew ArbolNivel1(550, 600, 1, 1, 130, 130, 2));
		this->arboles->Add(gcnew ArbolNivel1(620, 600, 1, 1, 130, 130, 2));
		this->arboles->Add(gcnew ArbolNivel1(754, 262, 1, 1, 130, 130, 2));
		this->arboles->Add(gcnew ArbolNivel1(800, 450, 1, 1, 130, 130, 2));



		for (int i = 0; i < arboles->Count; i++) {
			this->AgregarObstaculo(arboles[i]);
		}


		// AGREGAR TODOS LOS LOBOS A LA LISTA LOBOS Y LUEGO A ENEMIGOS
		this->lobos->Add(gcnew Lobo(100, 100, 4));
		this->lobos->Add(gcnew Lobo(1210, 650,4));
		for (int i = 0; i < lobos->Count; i++) {
			this->AgregarEnemigo(lobos[i]);
		}

		// RESINA o ACEITE
		this->AgregarRecurso(gcnew Resina(90, 500, 50,50, 15));
		this->AgregarRecurso(gcnew Resina(378 , 400, 50,50, 15));
		this->AgregarRecurso(gcnew Resina(950, 300, 50,50, 15));
		this->AgregarRecurso(gcnew Resina(500, 600, 50, 50, 15));
		

		// MONTON PIEDRAS
		this->AgregarObstaculo(gcnew MontonDePiedras(490, 0, 120, 120));
		this->AgregarObstaculo(gcnew MontonDePiedras(515,0,120, 120));
		this->AgregarObstaculo(gcnew MontonDePiedras(0, 270, 120, 120));
		this->AgregarObstaculo(gcnew MontonDePiedras(1185, 280, 120, 120));


	}

	virtual void LogicaPropia(Teclado^ t) override {

		// switch para cambiar fases
		switch (this->fase)
		{

		// fase 0
		case 0: {
			this->Decir("Madrastra: Hace hambre, tienes que dejar a tus hijos");
			CambiarFase(1);
			break;
		}
		
		// fase 1 BOSQUE FUERA DE LA CASA
		case 1: {

			// CARGAR MUNDO 2
			if (padre->GetHitbox().IntersectsWith(salidaMundo1)) {
				this->CargarMundo2();
				break;
			}

			// al padre se le gasta el aceite
			padre->GastarAceite();

			this->ActualizarLuzYLobos();
			this->ActualizarConfianza();
			this->ProcesarHacha(t);

			break;
		}

		// fase 2 SEGUNDA PATE DEL BOSQUE
		case 2: {


			// al padre se le gasta el aceite
			padre->GastarAceite();

			this->ActualizarLuzYLobos();
			this->ActualizarConfianza();
			this->ProcesarHacha(t);


			break;
		}


		default:
			break;
		}

	}

	virtual void ConfigurarNarrativa() override {
		this->Decir("El padre sale del hogar...");
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

		
		SolidBrush^ luzNoche = gcnew SolidBrush(Color::FromArgb(235, 0, 0, 15));
		Region^ oscuro = gcnew Region(Rectangle(0,0,ANCHO_JUEGO,ALTO_JUEGO));
		GraphicsPath^ luz = gcnew GraphicsPath();
		luz->AddEllipse(cx-r+43, cy-r+45, r*2, r*2);
		oscuro->Exclude(luz);
		g->FillRegion(pincelOscuro, oscuro);
		

		// DIBUJAR EFECTOS
		for (int i = 0; i < efectos->Count; i++) {
			efectos[i]->Dibujar(g);
		}

		// Hitbox para salidaMundo1
		if (fase == 1) {
			Pen^ pensitoSalidaMundo1 = gcnew Pen(Color::Yellow, 2);
			g->DrawRectangle(pensitoSalidaMundo1, salidaMundo1);
		}

	}

	virtual void DibujarHUD(Graphics^ g) override {

		if (this->jugador == nullptr) { return; }

		Font^ letra1 = gcnew Font("Calibri", 18);
		RectangleF RectanguloHUD = RectangleF(10, 10, 500, 75);
		SolidBrush^ bHUD = gcnew SolidBrush(Color::FromArgb(180,135,206,235));
		Pen^ pensito1 = gcnew Pen(Color::DarkSlateBlue,2);

		g->FillRectangle(bHUD, RectanguloHUD);
		g->DrawRectangle(pensito1, Rectangle(RectanguloHUD.X, RectanguloHUD.Y, RectanguloHUD.Width, RectanguloHUD.Height));
		g->DrawString(jugador->GetNombre(), letra1, Brushes::White, RectanguloHUD.X + 5,RectanguloHUD.Y);

		// DIBUJAR VIDAS
		for (int i = 0; i < this->jugador->GetVidasMax(); i++) {
			
			if (i < this->jugador->GetVidas()) {
				g->FillEllipse(Brushes::Red, 20 + (28 * i), 48, 20, 20);
			}

			else {
				g->FillEllipse(Brushes::DarkGray, 20 + (28 * i), 48, 20, 20);
			}

		}

		g->DrawString("Puntaje:  " + Convert::ToString(this->puntaje), letra1, Brushes::White, 160, 20);
		g->DrawString("Aceite:  " + Convert::ToString(this->padre->GetAceite()) + "%", letra1, Brushes::White, 160, 45);
		g->DrawString("Troncos:  " + lenaRecogida.ToString() + " / " + lenaNecesaria.ToString() , letra1, Brushes::White, 300, 20);
		g->DrawString("Confianza: " + confianza.ToString() + "%", letra1, Brushes::White, 300, 45);


		delete letra1;

	} // Nombre del prota, vidas etc de acuerdo al nvl



	//------------------------- EVENTOS ---------------------------------------
	
	virtual void AlTocarRecurso(Recurso^ r) override {
		
		// si es aceite, entonces el padre recarga 
		if (r->GetTipo() == TipoRecurso::Aceite) {
			padre->RecargarAceite(r->GetValor());
		}

		if (r->GetTipo() == TipoRecurso::Lena) {
			lenaRecogida++;
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
