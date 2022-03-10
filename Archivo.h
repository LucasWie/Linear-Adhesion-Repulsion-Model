#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>

//#include "Particula.h"
using namespace std;


/*
void set_cantidad(string nombre_salida, int n,float pasos){
  ofstream salida (nombre_salida);
  //cout << pasos <<endl;
  //salida << n <<"," << pasos << endl;
  salida.close();

}
*/

//Necesaria paara obtener la distribucion real de velocidades
vector<float> cargar_dist(string filename){
  ifstream archi;
  cout << filename <<endl;
  archi.open(filename.c_str());
  if (archi.is_open()){
    cout << "si abrio el archivo"<<endl;
  }
  else{
    cout << "NO se abriò el archivo"<<endl;
  }
  vector<float> values;
  float v;
  while(archi >> v){
    //cout << v<<endl;
    values.push_back(v);

  }

  return(values);
}

vector<vector<int> > cargar_n_cells(string filename){
  ifstream archi;
  archi.open(filename);
  vector<vector<int> > cantidades;
  vector<int> canal;
  int n;
  string line;
  //stringstream ss;
  for (line; getline(archi, line); ) {
    canal.clear();
    stringstream ss(line);
    while(ss >> n){
        canal.push_back(n);
    }
    cantidades.push_back(canal);

  }
  return cantidades;
}



void  escribir_posiciones(string nombre_salida, vector<Particula> particulas,float tiempo_acumulado, int canal){
  //string ruta = "/home/lucas/Escritorio/Doctorado/Curso_simulacion//";
  nombre_salida = nombre_salida;//ruta + ...
  ofstream salida (nombre_salida,ios::app);

  int N = particulas.size();
  //salida << tiempo_acumulado << endl;
  for (int i = 0; i < N; i++){
    salida << particulas[i].get_id() << ",";

    salida << particulas[i].get_pos_x() << ",";
    salida << particulas[i].get_vel_x()<< ",";
    salida << tiempo_acumulado <<",";
    salida << canal;
    salida << "\n";

  }
  salida.close();
}


void clean_start_file(string nombre_salida){
  ofstream salida (nombre_salida,ios::out);
  salida <<"ID,Position X,Velocidad,Time,Canal" <<endl;
  salida.close();

}


void escribir_posiciones(string nombre_salida, vector<string> output, int canal){
   ofstream salida (nombre_salida,ios::app);
   for (int i = 0; i < output.size(); i++){
     salida << output[i] << "," << canal << "\n";
   }
}
