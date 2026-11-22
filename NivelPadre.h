#pragma once
#include "Nivel.h"
#include "Lobo.h"
#include "Padre.h"
#include "ElementosNivel1.h"
#include "Efecto.h"
#include "Npc.h"

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
	Font^ calibri13 = gcnew Font("Calibri", 13, FontStyle::Bold);
	Font^ letra1 = gcnew Font("Calibri", 18);
	SolidBrush^ bHUD = gcnew SolidBrush(Color::FromArgb(180, 0, 0, 0));
	Pen^ pensito1 = gcnew Pen(Color::DarkSlateBlue, 3);
	Pen^ pensitoSalidaMundo1 = gcnew Pen(Color::Yellow, 2);

	// MECANICAS DEL CULPA
	int confianzaMax = 100;
	int confianza = confianzaMax;
	int ticksBajarCulpa = 2;
	int contadorTicksCulpa = 0;

	// MECANICAS DE AGUA -> LA CUBETA
	PozoDeAgua^ pozo;
	int progresoAgua; // se sumara durante los ticks de mantenido la E
	Efecto^ cubeta = nullptr;



	// RECTANGULOS  DE SALIDA
	Rectangle salidaMundo1 = Rectangle(615, 704, 75, 10);
	Rectangle entradaCueva = Rectangle(89, 80, 100, 10);
	Rectangle salidaCueva = Rectangle(605, 40, 100, 10);


	// BOOL MUNDO
	bool primeraVezMundo2 = true;

	// ABRIR LA PUERTA FINAL
	Puerta^ puerta = nullptr;
	int ticksParaAbrir = -1; // el -1 es que aun no abre nada
	bool puertaAbierta = false;

	// ZONA FINAL PARTE FINAL:
	Rectangle zonaFinal = Rectangle(465, 50, 100, 20);


private: // ------------------- EL LENADOR de leña ----------------------------

	// NPC barra ALIADO DEL NIVEL
	Npc^ lenador; int estadoLenador; // 0 no ha habido ninguna colision, 1 con pedido hecho, 2 entregao
	bool tieneLlave = false; // ---------------------------------------------------------------------------> TIENE LA LLAVE
	bool llenandoAgua = false;

	// A RECOLECTAR:
	int lenaARecoger = 3;
	int aguaARecoger = 1;
	int huesoARecoger = 1;

private: // RECURSOS RECOGIDOS o INVENTARIO
	int lenaRecogida = 0;
	int aguaRecogida = 0;
	int huesoRecogido = 0; // WESO

	int cooldownHacha = 10; // 10 ticks para que el hacha no sea espameable
	int COOLDOWN_HACHA = 10; // tiempo de espera hachazos

	void HablarConLenador() {
		switch (estadoLenador)
		{

		case 0:
			Decir("Padre: Caballero, sabe donde esta la salida de aqui al otro lado del bosque?");
			Decir("Desconocido: Con que quieres la llave del sendero eh?");
			Decir("Padre: Que quiere a cambio de ella?");
			Decir("Desconocido: Necesito 3 troncos, 1 cubeta de agua y 1 hueso...");
			Decir("Padre: Para que?");
			Decir("Desconocido: Busca los objetos y te dare lo que quieres.");
			estadoLenador = 1; // para q no se repita.
			break;

		case 1:

			// si ya tiene todo restarle a los elementos de su inventario
			if (lenaRecogida >= lenaARecoger && aguaRecogida >= aguaARecoger && huesoRecogido >= huesoARecoger) {
				this->lenaRecogida = this->lenaRecogida - lenaARecoger;
				this->aguaRecogida = this->aguaRecogida - aguaARecoger;
				this->huesoRecogido = this->huesoRecogido - huesoARecoger;

				Decir("Desconocido: Mmmm, pense que no lo lograrias...");
				Decir("Desconocido: Ten tu llave...");
				Decir("Desconocido: Ese sendero es muy peligroso y denso, no veo porque entrar ahi");
				this->tieneLlave = true; // DA LA LLAVE
				this->estadoLenador = 2;

			}

			// si le falta alguno
			else {
				String^ faltaLena = (lenaARecoger - lenaRecogida).ToString();
				String^ faltaAgua = (aguaARecoger - aguaRecogida).ToString();
				String^ faltaHueso = (huesoARecoger - huesoRecogido).ToString();

				String^ txt = "Aun me faltan " + faltaLena + " tronco/s" + ", " + faltaAgua + " cubeta/s de agua" +
					" y " + faltaHueso + " hueso/s";
				Decir("Desconocido: " + txt);
			}




			break;

		case 2:
			// no hace nada
			break;


		default:
			break;
		}
	}




public:

	// CONSTRUCTORES Y DESTRUCTOR
	NivelPadre(GestorArchivos^ archivos) :Nivel(archivos) {
		this->lobos = gcnew List<Lobo^>(20);
		this->arboles = gcnew List<ArbolNivel1^>(20);
		this->efectos = gcnew List<Efecto^>(20);
	}
	~NivelPadre() {

		delete pincelOscuro;
		delete pincelAzul;
		delete bHUD;
		delete calibri13;
		delete letra1;
		delete pensito1;
		delete pensitoSalidaMundo1;
	}




private:
	// ----------- PROPIOS DEL NIVEL ---------------------------

	void ActualizarLuzYLobos() {
		// asignar el radio de lux del padre a todos los lobos

		if (fase == 3) {
			for (int i = 0; i < lobos->Count; i++) {
				lobos[i]->SetRadioLuz(0);
			}
		}
		else {
			for (int i = 0; i < lobos->Count; i++) {
				lobos[i]->SetRadioLuz(padre->GetRadioLuz());
			}
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
	void VaciarListas() {
		this->enemigos->Clear();
		this->obstaculos->Clear();
		this->recursos->Clear();
		this->proyectiles->Clear();
		this->lobos->Clear();
		this->arboles->Clear();
		this->efectos->Clear();
	}

	// ------------- FASE 2: AGUA Y LENADOR (TRONCOS) -----------------------
	void CrearEntidadesFase2() {
		// FASE 2

		// el npc es un obstaculo, no es atravezzable.
		lenador = gcnew Npc("Sprites//Lenador.png", 80, 305, 1, 6, 90, 110);
		this->AgregarObstaculo(lenador); // agrearlo a la lista de obstaculos

		// crear el pozo de agua y agregarlo a los "obstaculos" del nivel
		pozo = gcnew PozoDeAgua(1090, 415, 200, 200);
		this->AgregarObstaculo(pozo);

		// crear la puerta y agregarla a los obstaculos
		puerta = gcnew Puerta(415, 2);
		this->AgregarObstaculo(puerta);
		if (puertaAbierta) {
			puerta->SetAbierta();
		}

		lobos->Add(gcnew Lobo(20, 500, 3));
		lobos->Add(gcnew Lobo(1000, 600, 3));
		for (int i = 0; i < lobos->Count; i++) {
			AgregarEnemigo(lobos[i]);
		}

		this->arboles->Add(gcnew ArbolNivel1(623, 287, 1, 1, 120, 120, 1));
		this->arboles->Add(gcnew ArbolNivel1(888, 312, 1, 1, 120, 120, 2));
		this->arboles->Add(gcnew ArbolNivel1(526, 438, 1, 1, 120, 120, 1));
		for (int i = 0; i < arboles->Count; i++) {
			AgregarObstaculo(arboles[i]);
		}

		// RESINA o ACEITE
		if (primeraVezMundo2) {


			this->AgregarRecurso(gcnew Resina(90, 500, 50, 50, 20));
			this->AgregarRecurso(gcnew Resina(378, 400, 50, 50, 20));
			this->AgregarRecurso(gcnew Resina(950, 300, 50, 50, 20));
			this->AgregarRecurso(gcnew Resina(541, 380, 50, 50, 20));
			this->AgregarRecurso(gcnew Resina(200, 600, 70, 70, 25));
			this->AgregarRecurso(gcnew Resina(779, 580, 70, 70, 25));
			this->AgregarRecurso(gcnew Resina(1135, 370, 70, 70, 25));


		}

	}
	void CargarMundo2(int x, int y) {

		this->SetFondo("Sprites//FondoNivel1_2.png"); // cambiar fondo
		this->VaciarListas();

		// ACTUALIZAR SPWN de pesonajes
		padre->SetPosicion(x, y);
		hansel->SetPosicion(x + 70, y);
		gretel->SetPosicion(x - 70, y);

		CambiarFase(2);
		if (primeraVezMundo2) {
			Decir("Padre: Ya cruzamos una buena parte del bosque, debemos mantenernos atentos...");
		}
		// CREAR ENTIDADES NIVEL 2
		CrearEntidadesFase2();

		primeraVezMundo2 = false;
	}

	// --------- FASE 3: CUEVA --------------------
	void CargarCueva() {

		// Listas y fondo
		this->SetFondo("Sprites//FondoNivel1_3.png"); // cambiar fondo
		this->VaciarListas();

		// POSICIONES
		padre->SetPosicion(620, 130);
		hansel->SetPosicion(620 - hansel->GetAncho(), 130);
		gretel->SetPosicion(620 + gretel->GetAncho(), 130);

		// LOBOS
		lobos->Add(gcnew Lobo(119, 110, 3));
		lobos->Add(gcnew Lobo(276, 74, 2));
		lobos->Add(gcnew Lobo(960, 78, 3));
		lobos->Add(gcnew Lobo(1246, 144, 2));
		for (int i = 0; i < lobos->Count; i++) {
			AgregarEnemigo(lobos[i]);
		}

		// HUESO RANDOM
		AgregarRecurso(gcnew Recurso("Sprites//Hueso.png", 125 + rand() % (1138 - 125 + 1), 364 + rand() % (515 - 364 + 1), Size(60, 60), TipoRecurso::Hueso, 1));
		this->AgregarRecurso(gcnew Resina(200, 450, 70, 70, 25));
		this->AgregarRecurso(gcnew Resina(900, 580, 70, 70, 25));
		this->AgregarRecurso(gcnew Resina(500, 200, 70, 70, 25));


		CambiarFase(3);
		Decir("Hansel y Gretel: Padre cuidado, huele a lobos por aqui!");
		Decir("CUIDADO: LOS LOBOS EN LA CUEVA NO TENDRAN MIEDO A TU LAMPARA !!");


	}

	// ------- FASE 4: FINAL ---------------
	void CargarFinal() {

		SetFondo("Sprites//FondoNivel1_4.png");
		
		// vaciar
		VaciarListas();

		// posiciones
		int x = 200;
		int y = 200;
		padre->SetPosicion(x,y);
		hansel->SetPosicion(x+hansel->GetAncho(), y);
		gretel->SetPosicion(x+hansel->GetAncho() + gretel->GetAncho(), y);

		// ya no siguen al lider
		hansel->SetLider(nullptr);
		gretel->SetLider(nullptr);


		// sumar el puntaje del aceite y confianza
		puntaje += (padre->GetAceite() + confianza);

		CambiarFase(4);

		Decir("-> La puerta se ha cerrado, es muy antigua...");
		Decir("Hansel: Papa, a donde vas ahora, donde estamos?");
		Decir("Padre: Voy por mas troncos, esperenme aqui");
		Decir("Padre: (Perdonenme hijos mios...)");



	}




public:

	// SOBREESCRITURA DE METODOS
	virtual void CrearEntidades() override {

		this->fondo = gcnew Bitmap("Sprites//FondoNivel1_1.png"); // fondo

		this->padre = gcnew Padre(850, 80, 5);
		this->jugador = padre; // crear al padre y asignarlo como el jugador del nivel

		// CREAR ALIADO
		this->AgregarAliado(gcnew Aliado("Sprites//Hansel.png", 850, 50, 4, 9, 64, 80, 4, 50));
		this->AgregarAliado(gcnew Aliado("Sprites//Gretel.png", 900, 50, 4, 9, 64, 80, 4, 50));
		this->hansel = aliados[0];
		this->gretel = aliados[1];

		this->hansel->SetLider(padre);
		this->gretel->SetLider(hansel);


		// CREAR ARBOLES EN LA LISTA Y PASARLOS A LA LISTA DE OBSTACULOS
		this->arboles->Add(gcnew ArbolNivel1(970, 320, 1, 1, 120, 120, 2));
		this->arboles->Add(gcnew ArbolNivel1(1070, 100, 1, 1, 120, 120, 2));
		this->arboles->Add(gcnew ArbolNivel1(550, 600, 1, 1, 130, 130, 2));
		this->arboles->Add(gcnew ArbolNivel1(620, 600, 1, 1, 130, 130, 2));
		this->arboles->Add(gcnew ArbolNivel1(754, 262, 1, 1, 130, 130, 2));
		this->arboles->Add(gcnew ArbolNivel1(800, 450, 1, 1, 130, 130, 2));



		for (int i = 0; i < arboles->Count; i++) {
			this->AgregarObstaculo(arboles[i]);
		}


		// AGREGAR TODOS LOS LOBOS A LA LISTA LOBOS Y LUEGO A ENEMIGOS
		this->lobos->Add(gcnew Lobo(100, 100, 4));
		this->lobos->Add(gcnew Lobo(1210, 650, 4));
		for (int i = 0; i < lobos->Count; i++) {
			this->AgregarEnemigo(lobos[i]);
		}

		// RESINA o ACEITE
		this->AgregarRecurso(gcnew Resina(90, 500, 50, 50, 20));
		this->AgregarRecurso(gcnew Resina(378, 400, 50, 50, 20));
		this->AgregarRecurso(gcnew Resina(950, 300, 50, 50, 20));
		this->AgregarRecurso(gcnew Resina(500, 600, 50, 50, 20));


		// MONTON PIEDRAS
		this->AgregarObstaculo(gcnew MontonDePiedras(505, 0, 120, 120));
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
				this->CargarMundo2(973, 69);
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

			// ACTUALIZAR TODOS LOS LOBOS Y HACHA
			this->ActualizarLuzYLobos();
			this->ActualizarConfianza();
			this->ProcesarHacha(t);

			// EVENTO CON EL NPC si la distancia es menor, se trigerea el evento
			if (padre->DistanciaA(lenador) < 80 && t->RecienPresionada(Keys::E)) {
				this->HablarConLenador();
			}

			// LLENAR AGUA
			bool llenando = padre->DistanciaA(pozo) < 80 && aguaRecogida < aguaARecoger && t->Mantenida(Keys::E) && estadoLenador < 2;
			if (llenando) {
				if (cubeta == nullptr || !cubeta->GetActivo()) {
					cubeta = gcnew Efecto("Sprites//Cubeta.png", padre->GetX() + 45, padre->GetY() + 50, 1, 8, 60, 60, 2, 0);
				}

				llenandoAgua = true;
				cubeta->Actualizar();
				progresoAgua++;
				padre->GastarAceite(); // se gasta mas agua
				padre->DeshacerMovimiento();

				if (this->progresoAgua >= 150) {
					this->aguaRecogida++;

					cubeta = nullptr;

					llenandoAgua = false;
					Decir("->Se ha recogido una cubeta de agua...");
					Decir("->Cuidado con que se acabe el aceite.");
				}

			}
			else {
				progresoAgua = 0;
				cubeta = nullptr;
				llenandoAgua = false;
			}


			// ENTRADA A LA CUEVA o FASE 33
			if (padre->GetHitbox().IntersectsWith(entradaCueva)) {
				this->CargarCueva();
				break;
			}

			// PRESIOANR PARA ABRIR LA PUERTA
			if (padre->DistanciaA(puerta) < 100 && !puerta->GetAbierta() && t->RecienPresionada(Keys::E)) {

				// sin llave -> mensaje
				if (!tieneLlave) {
					Decir("Padre: Esta cerrada, necesito una llave");
				}

				// con llave
				else if (tieneLlave && ticksParaAbrir == -1) {
					efectos->Add(gcnew Efecto("Sprites//Llave.png", padre->GetX() + 30, padre->GetY() - 10, 1, 5, 25, 40, 3, 0));
					ticksParaAbrir = 25;
				}

			}

			//ticks de puerta
			if (ticksParaAbrir > 0) {
				ticksParaAbrir--;
			}
			if (ticksParaAbrir == 0) {
				puerta->Abrir();
				this->puertaAbierta = true;
				ticksParaAbrir = -1;
			}



			// SALIDA DEL NIVEL
			if (puerta->GetAbierta() == true && padre->GetHitbox().IntersectsWith(zonaFinal)) {
				this->CargarFinal();
				break;
			}



			break;
		}

			  // fase 3 CUEVA 
		case 3: {

			// SALIDA
			if (padre->GetHitbox().IntersectsWith(salidaCueva)) {
				this->CargarMundo2(94, 89);
				break;
			}

			padre->GastarAceite(); // bajar aceites
			ActualizarLuzYLobos(); // asignar luz 
			ActualizarConfianza(); // confianza de hansel y gretel 

			break;

		}

		case 4:
			break;

		default: {
			break;
		}

		}

	}

	virtual void ConfigurarNarrativa() override {
		this->Decir("El padre sale del hogar...");
		

		// pregunta reflexiva del nivel
		
		pregunta = gcnew PreguntaReflexiva(

			"El desconocido le advirtió al Padre que el sendero era peligroso, pero el abrio la puerta igual. Que evidencia dicha decision?",
			
			gcnew cli::array<String^>
				{
				"Que no escuchó bien la advertencia",
				"Que prefirio obedecer a la madrastra antes que proteger a sus hijos",
				"Que estaba seguro de que los lobos no los atacarian",
				"Que queria demostrar que era valiente"}, 1, 2);
	}

	virtual bool ObjetivoCumplido() override {
		return fase == 4 && !HayDialogo();
	}

	virtual String^ GetTitulo() override {
		return "La noche en el bosque";
	}

	virtual void DibujarExtras(Graphics^ g) override {

		// Oscuridad de la noche y el aro de luz de la lampara
		int cx = jugador->GetX();
		int cy = jugador->GetY();
		int r = padre->GetRadioLuz();

		Region^ oscuro = gcnew Region(Rectangle(0, 0, ANCHO_JUEGO, ALTO_JUEGO));
		GraphicsPath^ luz = gcnew GraphicsPath();
		luz->AddEllipse(cx - r + 43, cy - r + 45, r * 2, r * 2);

		// DIBUJAR EFECTOS
		for (int i = 0; i < efectos->Count; i++) {
			efectos[i]->Dibujar(g);
		}

		// Hitbox para SALIDAS O ENTRADAS
		if (fase == 1) {
			g->DrawRectangle(pensitoSalidaMundo1, salidaMundo1);
		}
		else if (fase == 2) {
			g->DrawRectangle(pensitoSalidaMundo1, entradaCueva);
			g->DrawRectangle(pensitoSalidaMundo1, zonaFinal);
		}
		else if (fase == 3) {
			g->DrawRectangle(pensitoSalidaMundo1, salidaCueva);
		}

		// NPC si cerca y no hay dialogos
		if (fase == 2) {
			if (padre->DistanciaA(lenador) < 80 && !HayDialogo() && estadoLenador < 2) {
				SizeF tsize = g->MeasureString("[E] Hablar", calibri13);
				g->FillRectangle(Brushes::White, Rectangle(lenador->GetX(), lenador->GetY(), tsize.Width, tsize.Height));
				g->DrawString("[E] Hablar", calibri13, Brushes::Black, lenador->GetX(), lenador->GetY());
			}
		}

		oscuro->Exclude(luz);

		// SI FASE 2 CREAR AL ARO ALRDEDEDOR DEL LENADOR
		if (fase == 2) {
			GraphicsPath^ luzLenador = gcnew GraphicsPath();
			luzLenador->AddEllipse(lenador->GetCentroX() - 55, lenador->GetCentroY() - 50, 100, 100);
			oscuro->Exclude(luzLenador);
			delete luzLenador;
		}
		if (fase == 2) {
			if (padre->DistanciaA(pozo) < 80 && aguaRecogida < aguaARecoger && estadoLenador < 2) {
				SizeF tsize = g->MeasureString("[E] Llenar", calibri13);
				g->FillRectangle(Brushes::White, Rectangle(padre->GetX(), padre->GetY(), tsize.Width, tsize.Height));
				g->DrawString("[E] Llenar", calibri13, Brushes::Black, padre->GetX(), padre->GetY());
			}
			if (llenandoAgua) {
				SizeF tsize = g->MeasureString("[E] Llenar", calibri13);
				g->FillRectangle(Brushes::Cyan, Rectangle(padre->GetX(), padre->GetY(), progresoAgua / 2, tsize.Height));
			}
			if (cubeta != nullptr) {
				cubeta->Dibujar(g);
			}
		}

		// [E] ABrir de la puerta
		if (fase == 2 && padre->DistanciaA(puerta) < 100) {

			if (tieneLlave) {
				SizeF tsize = g->MeasureString("[E] Abrir", calibri13);
				g->FillRectangle(Brushes::White, Rectangle(puerta->GetX(), puerta->GetY(), tsize.Width, tsize.Height));
				g->DrawString("[E] Abrir", calibri13, Brushes::Black, puerta->GetX(), puerta->GetY());
			}
			else if (!tieneLlave) {
				SizeF tsize = g->MeasureString("Cerrada", calibri13);
				g->FillRectangle(Brushes::White, Rectangle(puerta->GetX(), puerta->GetY(), tsize.Width, tsize.Height));
				g->DrawString("Cerrada", calibri13, Brushes::Black, puerta->GetX(), puerta->GetY());
			}

		}

		g->FillRegion(pincelOscuro, oscuro);

		delete oscuro;
		delete luz;


	}

	virtual void DibujarHUD(Graphics^ g) override {

		if (this->jugador == nullptr) { return; }


		RectangleF RectanguloHUD = RectangleF(ANCHO_JUEGO, 0, ANCHO_PANEL, ALTO_JUEGO);

		g->FillRectangle(bHUD, RectanguloHUD);
		g->DrawRectangle(pensito1, Rectangle(RectanguloHUD.X, RectanguloHUD.Y, RectanguloHUD.Width, RectanguloHUD.Height));
		g->DrawString(jugador->GetNombre(), letra1, Brushes::White, RectanguloHUD.X + 5, RectanguloHUD.Y);

		int px = ANCHO_JUEGO + 15, y = 50;   // y = 50: debajo del nombre

		// VIDAS
		for (int i = 0; i < jugador->GetVidasMax(); i++)
			g->FillEllipse(i < jugador->GetVidas() ? Brushes::Red : Brushes::DarkGray, px + 28 * i, y, 20, 20);

		// los texts del hud
		y += 40;
		g->DrawString("Puntaje: " + puntaje.ToString(), letra1, Brushes::White, px, y);  y += 30;
		g->DrawString("Aceite: " + padre->GetAceite().ToString() + "%", letra1, Brushes::White, px, y);  y += 30;
		g->DrawString("Confianza: " + confianza.ToString() + "%", letra1, Brushes::White, px, y);  y += 30;
		g->DrawString("Troncos: " + lenaRecogida.ToString() + " / " + lenaARecoger.ToString(), letra1, Brushes::White, px, y);  y += 30;
		g->DrawString("Agua: " + aguaRecogida.ToString() + " / " + aguaARecoger.ToString(), letra1, Brushes::White, px, y);  y += 30;
		g->DrawString("Hueso: " + huesoRecogido.ToString() + " / " + huesoARecoger.ToString(), letra1, Brushes::White, px, y);


	}



	//------------------------- EVENTOS ---------------------------------------

	virtual void AlTocarRecurso(Recurso^ r) override {

		// si es aceite, entonces el padre recarga 
		if (r->GetTipo() == TipoRecurso::Aceite) {
			padre->RecargarAceite(r->GetValor());
		}

		else if (r->GetTipo() == TipoRecurso::Lena) {
			lenaRecogida++;
		}

		else if (r->GetTipo() == TipoRecurso::Hueso) {
			huesoRecogido++;
		}

		Nivel::AlTocarRecurso(r);
	}

	virtual void AlChocarConEnemigo(Enemigo^ e) override {
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

		if (o == this->puerta && puerta->GetAbierta()) {
			return;
		}
		else {
			this->jugador->DeshacerMovimiento();
		}
	}	// ál Chocar con un obstaculo



};
