 /*
 *  GRAFO.CPP - Plantilla para la implementaci�n de la clase GRAFOS
 *  Alumno: Alberto Antonio Hernandez Hernandez
 *  Autores : Antonio Sedeno Noda, Sergio Alonso
 *  Cursos   : 2021-2022
 */

#include "grafo.h"

void GRAFO :: destroy() {
	for (unsigned i=0; i< n; i++)
    {
		LS[i].clear();
		// A[i].clear();  limpia la matriz de adyancencia no pedida aun 
		if (dirigido == 1)
        {
            LP[i].clear();
        };
	}
	LS.clear();
	LP.clear();
	// A.clear();

}

void GRAFO :: build (char nombrefichero[85], int &errorapertura) {
    ElementoLista     dummy;
	ifstream textfile;
	textfile.open(nombrefichero);
	if (textfile.is_open()) {
		unsigned i, j, k;
		// leemos por conversion implicita el numero de nodos, arcos y el atributo dirigido
		textfile >> (unsigned &) n >> (unsigned &) m >> (unsigned &) dirigido;
		// los nodos internamente se numeran desde 0 a n-1
		// creamos las n listas de sucesores
		LS.resize(n);
        // leemos los m arcos
		for (k=0;k<m;k++) {
			textfile >> (unsigned &) i  >> (unsigned &) j >> (int &) dummy.c;
            if (Es_dirigido() == 1) {    //si el grafo es dirigido tiene sucesores y predecesores
                LP.resize(n);
                dummy.j = j - 1;
                LS[i - 1].push_back(dummy);     // metemos el valor en la lista de sucesores
                dummy.j = i - 1;
                LP[j - 1].push_back(dummy);     // metemos el valor en la lista de predecesores
            } else if (Es_dirigido() == 0) {     //si el grafo es no dirigido solo tiene lista de adyacencia (LS)
                dummy.j = i - 1;
                LS[j - 1].push_back(dummy);     // insertamos el valor en la lista de adyacencia 
                if (i != j) {
                    dummy.j = j - 1;
                    LS[i - 1].push_back(dummy);     // verificamos que no es un bucle, y si no lo es lo introducimos nuevamente
                }
            }
        }
    } else {
        errorapertura = 1;      // si no se cumple ninguno de estos casos, error de lectura
    }
}

GRAFO::~GRAFO() {
	destroy();
}

GRAFO::GRAFO(char nombrefichero[85], int &errorapertura) {
	build (nombrefichero, errorapertura);
}

void GRAFO:: actualizar (char nombrefichero[85], int &errorapertura) {
    //Limpiamos la memoria dinamica asumida en la carga previa, como el destructor
    destroy();
    //Leemos del fichero y actualizamos G con nuevas LS y, en su caso, LP
    build(nombrefichero, errorapertura);
}

unsigned GRAFO::Es_dirigido() {
    return dirigido;   //devuelve si un grafo es dirigido "1" o no "0" 
}

void GRAFO::Info_Grafo() {
    if (dirigido == 1) {
        cout << "El grafo es dirigido" << endl;
        cout << "   Nodos: " << n << " | Arcos: " << m << endl;
    } else {
        cout << "El grafo es NO dirigido" << endl;
        cout << "Nodos: " << n << " | Aristas: " << m << endl;
    }
}

void Mostrar_Lista(vector<LA_nodo> L) {
    for (int i = 0; i < L.size(); i++) {
        cout << "Nodo [" << i + 1 << "]: ";     //imprime el numero del nodo
        for (int j = 0; j < L[i].size();j++) {
            cout << L[i][j].j + 1 << ":(" << L[i][j].c << ") ";     //Imprime las aristas/arcos de una en una con sus (costes)
        }
        if (!L[i].size()) {      // si no hay arista entre dos nodos devuelve "vacio"
            cout << "Vacio";
        }
        cout << endl;
    }
}

void GRAFO :: Mostrar_Listas (int l) {
    if (l == 0 || l == 1) {
        Mostrar_Lista(LS);  //imprime la lista de sucesores(0)/adyacencia(1)
    } else if (l == -1){
        Mostrar_Lista(LP);  //imprime lista de predecesores(-1)
    }
}

// void GRAFO::Mostrar_Matriz() //Muestra la matriz de adyacencia, tanto los nodos adyacentes como sus costes
// {

// }

void GRAFO::dfs_num(unsigned i, vector<LA_nodo>  L, vector<bool> &visitado, vector<unsigned> &prenum, unsigned &prenum_ind, vector<unsigned> &postnum, unsigned &postnum_ind) //Recorrido en profundidad recursivo con recorridos enum y postnum
{
	visitado[i] = true;
    prenum[prenum_ind++]=i;     //asignamos el orden de visita prenum que corresponde el nodo i
    for (unsigned j=0;j<L[i].size();j++)
             if (!visitado[L[i][j].j])
                {
                dfs_num(L[i][j].j, L, visitado, prenum, prenum_ind, postnum, postnum_ind);
                };
    postnum[postnum_ind++]=i;   //asignamos el orden de visita posnum que corresponde al nodo i
}


void GRAFO::RecorridoProfundidad()
{
    unsigned num_nod;           
    vector<bool> visitado;      // creamos el vector para guardar los visitados
    vector<unsigned> prenum;    // creamos el vector para guardar los numeros anteriores 
    unsigned prenum_ind = 0;    
    vector<unsigned> postnum;   // creamos el vector para guardar los numeros posteriores
    unsigned postnum_ind = 0;

    visitado.resize(n);         // le cambiamos el tamaño a los vectores para guardar los n nodos
    prenum.resize(n);
    postnum.resize(n);

    cout << "Elija el nodo de partida? [1-" << n << "]: ";
    cin >> num_nod;        // introducimos el nodo de partida 

    dfs_num(num_nod-1, LS, visitado, prenum, prenum_ind, postnum, postnum_ind);     // llamamos a la funcion dfs


    cout << "Orden de visita de los nodos en preorden" << endl;
    for (int i = 0; i < prenum.size(); ++i) {   //recorremos el vector de la prenumeracion 
      if (i < prenum.size()-1)  {
         cout << "[" << prenum[i]+1 << "]";      //imprimimos el recorrido 
      } 
      if (i < prenum.size() - 2) {              //mientras se imprima el recorrido se imprime una flecha, menos en la ultima iteración
          cout << " -> ";
      }
    }
    cout << endl;
    cout << "Orden de visita de los nodos en postorden" << endl;
    for (int i = 0; i < postnum.size(); ++i) {      //recorremos el vector de la postnumeracion 
      if (i < postnum.size()-1) {                  
         cout << "[" << postnum[i]+1 << "]";    //imprimimos el recorrido 
      } 
      if (i < postnum.size() -2) {
          cout << " -> ";
      }
    }
   cout << endl;
}


void GRAFO::bfs_num(	unsigned i, //nodo desde el que realizamos el recorrido en amplitud
				vector<LA_nodo>  L, //lista que recorremos, LS o LP; por defecto LS
				vector<unsigned> &pred, //vector de predecesores en el recorrido
				vector<unsigned> &d) //vector de distancias a nodo i+1
//Recorrido en amplitud con la construcci�n de pred y d: usamos la cola
{
    vector<bool> visitado; //creamos e iniciamos el vector visitado
    visitado.resize(n, false);  
    visitado[i-1] = true;
 
    pred.resize(n, 0); //creamos e inicializamos pred y d
    d.resize(n, 0);
    pred[i-1] = i;
    d[i-1] = 0;

    queue<unsigned> cola; //creamos e inicializamos la cola
    cola.push(i-1);//iniciamos el recorrido desde el nodo i+1
 
    while (!cola.empty()) //al menos entra una vez al visitar el nodo i+1 y contin�a hasta que la cola se vac�e
    {  
        unsigned k = cola.front(); //cogemos el nodo k+1 de la cola
        cola.pop(); //lo sacamos de la cola
        //Hacemos el recorrido sobre L desde el nodo k+1
        for (unsigned j = 0; j < L[k].size() ; j++)
            //Recorremos todos los nodos u adyacentes al nodo k+1
            {
                if(visitado[L[k][j].j] == false) {  //Si el nodo u no esta visitado
                    visitado[L[k][j].j] = true;     //Lo visitamos
                    cola.push(L[k][j].j);           //Lo metemos en la cola
                    pred[L[k][j].j] = k+1;          //le asignamos el predecesor
                    d[L[k][j].j] = d[k]+1;          //le calculamos su etiqueta distancia
                }
            };
        //Hemos terminado pues la cola esta vacia
    };
}

void GRAFO::RecorridoAmplitud() //Construye un recorrido en amplitud desde un nodo inicial
{
    unsigned i;                 //creamos la variable nodo dado
    unsigned n = LS.size();     //asignamos a n el valor del tamaño de la lista de sucesores
    vector<unsigned> pred;      //creamos el vector de los predecesores
    vector<unsigned> d;         //creamos el vector distancia de aristas 
    
    int distancia = i;          //inicializamos la distancia al nodo inicial
    int max = 0;                //inicializamso el valor maximo a 0
    int maxpred = 0;            //hacemos lo mismo con el predecesor maximo


    cout<<"Elija nodo de partida? [1-"<< n <<"]: ";
    cin>>i;
    cout<<"Nodo Inicial: "<<i<<endl;

    bfs_num(i, LS, pred, d);    //le pasamos los datos a la funcion bfs, que realiza el recorrido 

    for(int r = 0; r < d.size(); r++) {     //vamos recorriendo el vector d de las distancias de aristas 
        if(d[r] > max) {                    //para saber la distancia máxima
            max = d[r];                     //vamos actualizando la variable max
        }
    }
    cout<<"\nNodos segun distancia al nodo inicial en numero de aristas";
    cout<<"\nDistancia 0 aristas : "<<i;            //el propio nodo de inicio está siempre a 0 de distancia
    for(int j = 1; j <= max; j++) {
        cout<<"\nDistancia "<<j<<" aristas : ";     
        bool primero = true;
        for(unsigned k = 0; k < d.size(); k++) {    //recorremos el vector de distancias 
            if(d[k] == j) {                         
                if(primero) {
                    primero = !primero;             //saltamos el propio nodo inicial
                } else {
                    cout<<" : ";
                }
                cout<<k+1;                          //imprimimos los nodos a una distancia de j unidades de i (si lo hay)
            }
        }

    }
    cout<<"\n\nRamas de conexion en el recorrido";
   
    for (unsigned int l = 1; l < pred.size(); l++) {        //recorremos la lista de predecesores 
        for (unsigned int h = 0; h < d.size(); h++) {       //recorremos la lista de distancia
            if (d[h] == l && pred[h] != 0) {                
                vector<unsigned int> v;                     
                unsigned int temp = h+1;
                v.push_back(temp);                          //metemos los nodos recorridos en profundidas en el vector v
                while (pred[(temp-1)] > i) {                //mientras el nodo sea menor que el i dado
                    temp = pred[(temp-1)];
                    v.push_back(temp);                      //lo metemos en el vector v (temporalmente)
                }
                cout <<"\n"<< i ;                           //imprimimos el comienzo del recorrido, es decir el nodo i
                for (unsigned int x{static_cast<unsigned int>(v.size())}; x > 0; --x ) {
                    cout << " - " << v[x-1];               // imprimimos los nodos del recorrido, obtenidos en el vector temporal v
                }
            }
        }
    }
    cout<<"\n";
}

void GRAFO::Kruskal() {
    assert(!Es_dirigido());     //kruskal se aplica a grafos no dirigidos
    vector<AristaPesada> Aristas, T;
    Aristas.resize(m);
    unsigned k = 0;
    for (unsigned i = 0; i < n; i++) {                
        for (unsigned j = 0; j < LS[i].size(); j++) {
            if (i < LS[i][j].j) {
                Aristas[k].extremo1 = i;        //introducimos el nodo origen en el vector de Aristas
                Aristas[k].extremo2 = LS[i][j].j;   //introducimos el nodo destino en el vector de Aristas
                Aristas[k++].peso = LS[i][j].c;     //int
            }
        }
    };
    vector <unsigned> Raiz;     //creamos el vector raiz para guardar las comp. conexas
    Raiz.resize(n);
    for (unsigned q = 0; q < n; q++) {
        Raiz[q]= q;              //metemos la componentes conexas iniciales del grafo
    };
    
    ///////

    int minimo;         //auxiliar minimo para guardar la arista de coste minimo
    int head {0};       //comienzo del vector desordenado, y va avanzando a medida que ordena las aristas por su peso
    int aux_z;          //variable auxiliar de z
    int kill;           //iguala las componentes conexas compatibles
    int contador{0};    //contador de aristas del arbol generador  
    int peso{0};        //peso del arbol MST
    AristaPesada dummie;    //dummie que contiene la arista de menor peso
    while (T.size() < n - 1) {
        minimo = maxint;        //inicializamos el minimo a un valor enorme
        for (int z = head; z < m; ++z) {
            if (Aristas[z].peso < minimo) {
                minimo = Aristas[z].peso;      //vamos actualizando el coste minimo hallado
                aux_z = z;      //guardamos el indice de la arista en un auxiliar
            }
        }
        dummie = Aristas[aux_z];    //metemos en el dummie dicha arista
        Aristas[aux_z] = Aristas[head];     
        Aristas[head] = dummie;     //movemos la arista de menor coste a la cabeza
        head++;       //incrementamos la cabeza para apuntar a la siguiente arista
        if (Raiz[dummie.extremo1] != Raiz[dummie.extremo2]) {
            T.push_back(dummie);
            peso += dummie.peso;      //sumamos el peso de la arista al peso total del arbol
            ++contador;               //incrementamos el contador de aristas
            cout << "Arista numero " << contador << " (" << dummie.extremo1+1 << ", " 
                                     << dummie.extremo2+1 << "), con peso " << dummie.peso << endl; 
            kill = dummie.extremo1;       //componente conexa a eliminar  
            for (int l = 0; l < n; ++l) {       //recorremos todos los nodos para saber su comp. conexa
                if (Raiz[l] == kill) {          //si encontramos un nodo con dicha com. conexa 
                    Raiz[l] = Raiz[dummie.extremo2];   //actualizamos la componente conexa respecto a la nueva 
                }
            }
        }
    }
    cout << "El peso del arbol generador de minimo coste es: " << peso << endl << endl;
}

void GRAFO::MostrarCamino(unsigned s, unsigned i, vector<unsigned> pred)
{
    if (i != s)
    {
        MostrarCamino(s,pred[i],pred);
        cout << pred[i]+1 << " - ";
    }
}

void GRAFO::Dijkstra() {
    assert(Es_dirigido());
    vector<bool> PermanentementeEtiquetado;
    vector<int> d;
    vector<unsigned> pred;
    int min;
    unsigned s, candidato;
    //Inicialmente no hay ningun nodo permanentemente etiquetado
    PermanentementeEtiquetado.resize(n,false);
    //Inicialmente todas las etiquetas distancias son infinito
    d.resize(n,maxint);
    //Inicialmente el pred es null
    pred.resize(n,UERROR);
    //Solicitamos al usuario nodo origen
    cout << endl;
    cout << "Caminos minimos: Dijkstra" << endl;
    cout << "Nodo de partida? [1-"<< n << "]: ";
    cin >> (unsigned &) s;
    //La etiqueta distancia del nodo origen es 0, y es su propio pred
    d[--s]=0; pred[s]=s;
    do
    {
        min = maxint;          //iniciamos el valor minimo a un numero muy grande

        // - Buscamos un nodo candidato a ser permanentemente etiquedado: aquel de
        //   entre los no permanentemente etiquetados, es decir, en el almacén con menor
        //   etiqueta distancia no infinita.
        for(int i = 0; i < n; ++i) {
            if(d[i] < min && PermanentementeEtiquetado[i] == false && d[i] < maxint){
                candidato = i;      //valoramos los candidatos, si hayamos uno minimo, lo guardamos
                min = d[i];        // guardamos su distancia en min
            }
        }

        // - Si existe ese candidato, lo etiquetamos permanentemente
        if (PermanentementeEtiquetado[candidato] == false ) {
            PermanentementeEtiquetado[candidato] = true;
        }

        // usamos los arcos de la lista de sucesores para buscar atajos.
        for (int i = 0; i < LS[candidato].size(); ++i) {
            // para todo j sucesor de candidato
            // si la distancia de j es mayor a la de candidato + coste de candidato a j
            // j es distinto de candidato y no esta permanentemente etiquetado
            if(d[LS[candidato][i].j] > (d[candidato] + LS[candidato][i].c) && LS[candidato][i].j != candidato && 
               PermanentementeEtiquetado[LS[candidato][i].j] == false) {
                   d[LS[candidato][i].j] = d[candidato] + LS[candidato][i].c;  //actualizamos la distancia (distancia mas coste) 
                   pred[LS[candidato][i].j] = candidato;      // el predecesor de j pasar a ser el candidato
            }
        }
    }
    while (min < maxint);      // - Esto lo hacemos mientras haya candidatos
    cout << "Soluciones:" << endl;

    //En esta parte del código, mostramos los caminos mínimos, si los hay
    for (int i = 0; i < n; ++i){   
        if (i != s) { 
            if (PermanentementeEtiquetado[i] == true) {     //si hay camino minimo (ha sido perma. etiqu.), lo mostramos
                cout << "El camino desde el nodo " << s + 1 << " hasta el nodo " << i + 1 << " es : ";
                MostrarCamino(s, i, pred);
                cout << i + 1 << " de longitud " << d[i] << endl;
            }   else {      // si no, mostramos que no hay camino
                cout << "No hay camino desde el nodo " << s + 1 << " al nodo " << i + 1 << endl;
            }
        }
    }
}
