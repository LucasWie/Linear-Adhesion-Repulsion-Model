#include <vector>
//#include <ctime>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <time.h>

using namespace std;

vector <Particula> ubicar_particulas(int N,float x_max,float min_dist,vector<float> v_des){
  //srand(time(NULL));

  double distancia;
  int i = 0;
  int j;
  vector <Particula> particulas;
  Particula aux;

  //int N = 200;
  while (i < N){
      float nuevo_x = (rand()/float(RAND_MAX))*x_max;
      //cout << i << endl;

    for (j=0; j<i; j++){//ver si no se superpone con otra particula ya puesta.
      distancia = abs(particulas[j].get_pos_x() - nuevo_x);
      if (distancia < min_dist){//hay superposicion, buscar nueva posicion
        break;
      }
    }
    if (j == i){ //si recorrí todas las particulas previamente ubicadas sin colisiones, paso a la siguente
      //cout<<"i vale: " <<i<<endl;
      aux.set_id(i);
      aux.set_pos_x(nuevo_x);
      aux.set_vel_x(0);
      //int v_des = distribution(generator);

    //  aux.set_vel_y(0);
      if (rand() < RAND_MAX/2.0){
        aux.set_velocidad_deseada(v_des[i]);
      }
      else{
        aux.set_velocidad_deseada(-1*v_des[i]);
      }
      particulas.push_back(aux);
      i++;
      }
  }

  return particulas;
}



int add_particle(vector<Particula> &particulas, int higher_id,float vd, float L){

  Particula nueva;


  nueva.set_id(higher_id + 1);
  nueva.set_vel_x(0);
  if (rand() < RAND_MAX/2.0){ // rand 50/50 para ver si entre por izquierda o derecha

        nueva.set_velocidad_deseada(vd);
        nueva.set_pos_x(0);
        particulas.push_back(nueva);
  }
  else{

      nueva.set_velocidad_deseada(-1*vd);
      nueva.set_pos_x(L);
      particulas.push_back(nueva);
    }

return (higher_id+1);
}


void erase_bordering_particle(vector<Particula> &particulas, float L){// en realidad solo la marca para ser eliminada en el calculo siguiente, no la elimina realmente
  int id_erase = particulas[0].get_id();
  float min_dist_border = (particulas[0].get_pos_x() < L-particulas[0].get_pos_x()) ? particulas[0].get_pos_x() : L-particulas[0].get_pos_x();
  float dist_border;
  for (int i=1; i<particulas.size();i++){
    dist_border = (particulas[i].get_pos_x() < L-particulas[i].get_pos_x()) ? particulas[i].get_pos_x() : L-particulas[i].get_pos_x();
    if (dist_border < min_dist_border){
      min_dist_border = dist_border;
      id_erase = particulas[i].get_id();
    }
  }
  for (int i = 0; i<particulas.size();i++){
    if (particulas[i].get_id() == id_erase){
      particulas[i].set_out_limit();
      break;
    }
  }
}
