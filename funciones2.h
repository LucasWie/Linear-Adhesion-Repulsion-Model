#include <random>
#include <algorithm>
#include <map>

#include "Archivo.h"

using namespace std;


void calcular_vel(vector<Particula> &particulas, vector < vector<int> > vecinos, double r_eq, double r_cut, double f_rep, double f_adh,float eta){
  for (int i = 0; i<particulas.size(); i++){
    float vel = particulas[i].get_velocidad_deseada();
    float vec_forces = 0;
    for (int id_vec = 0; id_vec < vecinos[i].size(); id_vec++){
      double dist = abs(particulas[vecinos[i][id_vec]].get_pos_x()-particulas[i].get_pos_x());
      //cout << dist << endl;
      if(dist < r_cut) {
        if (dist < r_eq){
          //cout << "la particula: " << i<<" esta muy cerca de  " << id_vec << " asi que se repele hacia:" <<  endl;
          int e_i = (particulas[vecinos[i][id_vec]].get_pos_x()-particulas[i].get_pos_x())/dist;//esto deberìa ser 1 o -1
          //cout << "E_i:"<<e_i <<endl;
          vec_forces = vec_forces + (-1)*e_i*(f_rep*(abs(dist-r_eq)/(r_eq)));

          if (particulas[i].get_velocidad_deseada() * particulas[vecinos[i][id_vec]].get_velocidad_deseada() < 0
              && !(particulas[i].was_inverted()) && !(particulas[vecinos[i][id_vec]].was_inverted()) ){

            //"Estan chocando con velocidades opuestas, uno debe ceder"
            if (float(rand())/RAND_MAX < 0.5){  //Igual probabilidad para ambos

              particulas[i].invert_velocidad_deseada();
            }
            else{
              particulas[vecinos[i][id_vec]].invert_velocidad_deseada();
            }
            particulas[i].set_invertion_true();
            particulas[vecinos[i][id_vec]].set_invertion_true();
          }

        }
        else{
          int e_i = (particulas[vecinos[i][id_vec]].get_pos_x()-particulas[i].get_pos_x())/dist;//esto deberìa ser 1 o -1
          vec_forces = vec_forces + e_i*(f_adh*( abs(dist-r_eq) / (r_cut-r_eq)));
        }
      }

    }
    float epsilon =  -(eta/2.0) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(eta)));

    particulas[i].set_vel_x(vel + vec_forces + epsilon);
  }
}


void mover(vector<Particula> &particulas,float dt, float L, float prob){
  int higher_id = 0;
  vector<int> to_eliminate;
  for (int i=0; i<particulas.size();i++){
    particulas[i].reset_invertion();
    //if (abs(particulas[i].get_vel_x() > 50)){cout << "Va Muy rápido " << endl;}
    particulas[i].set_pos_x(particulas[i].get_pos_x()+dt*particulas[i].get_vel_x());
    if (float(rand())/RAND_MAX < prob){
      particulas[i].invert_velocidad_deseada();
    }
    if (particulas[i].get_pos_x() < 0  || (particulas[i].get_pos_x() > L) )  {
      particulas[i].set_out_limit();
      //cout << i << " Debe ser eliminada"<<endl;
    }

  }

/*
  for (int i=0; i<particulas.size();i++){
    if (particulas[i].is_out()){
      remove(particulas
    }

  }
*/
  particulas.erase(std::remove_if(particulas.begin(),
                                particulas.end(),
                                [](Particula & x){return x.is_out()==true;}),
                 particulas.end());


 for (int i=0; i<particulas.size();i++){
   if (particulas[i].is_out()){
     cout << "no se elimino como debe ser"<<endl;
   }
}


/*
  particulas.erase(
    remove_if(particulas.begin(), particulas.end(), is_out_of_limit),
    particulas.end());
*/
}



int add_particle(vector<Particula> &particulas, int higher_id,float vd, float L){

  Particula nueva;
  //cout << "valor recibido:"<<vd<<endl;

  for (int i=0; i<particulas.size();i++){
    if (higher_id < particulas[i].get_id()){
      higher_id = particulas[i].get_id();
    }
  }
  nueva.set_id(higher_id+1);

  nueva.set_vel_x(0);

  if (rand() < RAND_MAX/2.0){
      int i = 0;
      for (i;i < particulas.size();i++){
        if (particulas[i].get_pos_x() < 12){

          break;
        }
      }
      if (i == particulas.size()){
        nueva.set_velocidad_deseada(vd);
        nueva.set_pos_x(0);
        particulas.push_back(nueva);

      }
  }
  else{
    int j = 0;
    for (j;j < particulas.size();j++){
      if (particulas[j].get_pos_x() > L-12){
        break;
      }
    }
    if (j == particulas.size()){
      nueva.set_velocidad_deseada(-1*vd);
      nueva.set_pos_x(L);
      particulas.push_back(nueva);

    }
  }
  //cout << "nueva: " << nueva.get_velocidad_deseada()<<endl;

return (higher_id+1);
}



map<float,float> get_inverted_cdf(vector<float> dist_original){

  map<float,float> inverted_cdf;

  //Crear CDF
  int n = dist_original.size();
  sort(dist_original.begin(),dist_original.end());

  for (int i=0;i<n;i++){
    inverted_cdf.insert( pair<float,float>(float(i)/n,dist_original[i]) );

  }
  return (inverted_cdf);
}


float valor_segun_inverted_cdf(float uniform, map<float,float> inverted_cdf){

  float new_val;
  float low = inverted_cdf.lower_bound(uniform)->first;
  float high = inverted_cdf.upper_bound(uniform)->first;
  new_val = (inverted_cdf[low] + inverted_cdf[high])/2.0;
  return new_val;


}
