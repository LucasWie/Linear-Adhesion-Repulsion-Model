#include <stdlib.h>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>

//#include "nodo.h"

using namespace std;

class Grafo{
private:
    float alto = 0;
    float ancho = 0;
    int N = 0;
    map <int, Nodo> nodos;
    vector <vector <bool> > m_ad;
    vector <vector <float> >mat_dis;
public:
  Grafo(){
    alto = 0;
    ancho = 0;
    N = 0;
  }

  Grafo(float L,float W,int N_nodos,int N_canales){
        N = N_nodos;
        alto = L;
        ancho = W;
        Nodo aux;

        m_ad.resize(N);
        mat_dis.resize(N);

        for (int i=0;i<N;i++){
          m_ad[i].resize(N);
          mat_dis[i].resize(N);
        }
        int nodos_por_canal = N_nodos/N_canales;
        float distancia_inter_canales = alto/N_canales;
        float distancia_intra_canales = ancho/nodos_por_canal;
        int id = 0;
        for (int i = 0;  i < N_canales; i++){
          cout << "empezando canal " << i <<endl;
          for(int j = 0; j < nodos_por_canal; j++){
            cout <<"i: " << i << " j: " << j<<endl;
            aux.set_id(id);
            aux.set_posx(j*distancia_intra_canales);
            aux.set_posy(i*distancia_inter_canales);
            cout<<"nodo en :" <<aux.get_posx() <<" , "<< aux.get_posy()<<endl;
            nodos.insert({id,aux});
            cout << "Se han insertado "<<id <<" Nodos\n";
            cout << "Un nodo es... X: " << nodos[id].get_posx() << " Y: "  << nodos[id].get_posy()<<endl ;

            if (j > 0 && id < m_ad.size()){
              m_ad[id][id-1] = 1;//conecta nodos consecutivos de un "Canal"
              m_ad[id-1][id] = 1;
              nodos[id].set_grado(nodos[id].get_grado()+1);
              nodos[id-1].set_grado(nodos[id-1].get_grado()+1);
            }
            id++;
          }

        }
        cout<< "Se creo la estructura"<<endl;
  }
  map<int,Nodo> ver_nodos(){return(nodos);}

  int get_nodo_conectado(int id_nodo){
    //cout << "Eligiendo entre los nodos conectados a "<<id_nodo << "\n";
    //cout <<"Grado: " << nodos[id_nodo].get_grado()<<endl;
    if(nodos[id_nodo].get_grado() > 0){
      //cout << nodos[id_nodo].get_grado()<<endl;
      int elegido = 1 + rand()%(nodos[id_nodo].get_grado());//elegido es cual de los vecinos de id_nodo será
      int i = 0;
      int positivos = 0;
      //cout<<"elegido es el :" << elegido<<"esimo vecido de "<<id_nodo<< endl;
      for (i=0;i<m_ad[id_nodo].size();i++){
        if (m_ad[id_nodo][i]){
          positivos++;
        }
        if (positivos == elegido){
          break;

        }

      }
      return i;

    }
    else{
      cout << "Esto se re rompio";
      exit(4);
      return (-1);
    }
  }

  int get_nodo_conectado_db(int id_nodo,float probabilidad, int origen){ //db:double_back
    int devuelto;
    if (nodos[id_nodo].get_grado() > 1){
      if(origen == id_nodo-1){
        //aca juega la probabilidad:
        if (rand()%100 < probabilidad*100){devuelto=origen;}
        else{devuelto=id_nodo+1;}

      }
      else{
        if (rand()%100 < probabilidad*100){devuelto=origen;}
        else{devuelto=id_nodo-1;}
      }

    }


    else{
    devuelto = origen;
    }

    return devuelto;
 }





  vector <vector <bool> > get_matriz(){return(m_ad);}

  int get_n_nodos(){return N;}

  Nodo* get_nodo(int id){return &(nodos[id]);}

  void set_pos_nodos(int numero_de_nodos, string path_nodos){

    ifstream archi_nodos;
    archi_nodos.open(path_nodos);
    //cout << nodos[2].get_posx()<<" "<<nodos[2].get_posy() <<endl;
    N = numero_de_nodos;
    string id1, x1, y1;
    int id;
    Nodo aux;
    m_ad.resize(N);
    for (int i=0;i<N;i++){
      m_ad[i].resize(N);
      for (int j=0;j<N;j++){
        m_ad[i][j]=0;
      }
    }
    for (int i = 0; i < numero_de_nodos; i++){
      archi_nodos >> id1 >> x1 >> y1;

      nodos[i].set_id( stoi(id1));
      nodos[i].set_posx( stof(x1) );
      nodos[i].set_posy( stof(y1) );
      //cout<<" Grado: " <<nodos[i].get_grado()<<endl;
    }
    archi_nodos.close();

  }

  void set_arcos(int N_arcos, int N_nodos, string path_arcos){
    ifstream archi_mad;
    archi_mad.open(path_arcos);
    string aux;
    N = N_nodos;
    for (int i=0; i<N; i++){
      nodos[i].set_grado(0);
    }
    //archi_mad >> aux;//hay que ponerlo para que lea la linea vacía
    //cout << "Primer aux leida "<< aux;
    for (int i=0;i<N_nodos;i++){ //es la cantidad de lineas
      for (int j=0; j<N_nodos;j++){ // j es la cantidad de valores que tiene la linea "i"
        archi_mad >> aux;
        //cout << aux << endl;
        if (aux == "1"){
          //cout <<"Los nodos "<<i <<" e " <<j<<" estan conectados\n";
          m_ad[i][j] = (aux == "1");
          nodos[i].set_grado(nodos[i].get_grado()+1);
          //nodos[j].set_grado(nodos[j].get_grado()+1);
        }
        else{
          //cout <<"Los nodos "<<i <<" e " <<j<<" NO estan conectados\n";
          m_ad[i][j] = (aux == "1");
          m_ad[j][i] = (aux == "1");
        }

      }
    }
    for (int i=0;i<N_nodos;i++){
      cout << "El nodo "<<i<< " tiene grado: "<<nodos[i].get_grado()<<endl;
    }

    archi_mad.close();
  }

};
