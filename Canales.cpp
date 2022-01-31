#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <unistd.h>
#include <map>


#include "Particula.h"
#include "funciones2.h"
#include "set_particulas.h"
#include "Cell_index.h"


using namespace std;

int main(){
  vector<Particula> particulas;
  //double velocidad_deseada = 5.0;
  double d_eq = 12.0;
  double d_cut=30;
  double f_rep = 30;
  double f_adh = 0.75;
  float eta=7;
  float inversion_prob_min = 3.0/1900;
  float prob_inversion_step;




  //vector <float> f_adhs = {0.2, 0.4, 0.6};
  //vector <string> f_adh_name = {"0.2","0.4","0.6"};

  vector <float> f_adhs = {0.75};
  vector <string> f_adh_name = {"0.75"};

  vector <float> d_cuts = {20,25,30,35,40};
  vector <string> d_cuts_name = {"20","25","30","35","40"};

  vector <float> d_eqs = {12,14,16,18,20};
  vector <string> d_eqs_name = {"12","14","16","18","20"};

  //vector <float> d_cuts = {30};
  //vector <string> d_cuts_name = {"30"};


  srand(44);
//srand(144);
  int N_max = 5;
  int n_ini = 3;
  float L = 700;
  float t_max =  360;
  string ruta = "./parameter_evaluation_final/" ;
  string comando = "mkdir -p "+ruta;
  system(comando.c_str());
  string nombre_salida;
  string nombre_salida_base;

  vector<float> vd_iniciales;


  vector<float> velocidades_reales;

  velocidades_reales = cargar_dist("velocidades_aisaldas.txt");

  cout <<velocidades_reales.size()<<endl;

  float vel_max_esperada = *max_element(velocidades_reales.begin(),velocidades_reales.end());

  map<float,float> inv_cdf = get_inverted_cdf(velocidades_reales);
  float uniform_dist_pick;

  for (int test_adh = 0; test_adh < 1; test_adh++){
    f_adh = f_adhs[test_adh];

  for (int test_d_eq = 0; test_d_eq < 5; test_d_eq++){
      d_eq = d_eqs[test_d_eq];


  for (int test_d_cut = 0; test_d_cut < d_cuts.size(); test_d_cut++){
      vd_iniciales.clear();
      d_cut = d_cuts[test_d_cut];

    //for (int test_v=0;test_v < 5; test_v++){
    //  velocidad_deseada = v_deseadas[test_v];
      float dt = 0.05 * (d_cut/vel_max_esperada);
      prob_inversion_step = inversion_prob_min*dt;
      //cout << "dt "<< dt <<endl;
      vector<vector<int> > vecinos;
      nombre_salida_base = ruta +"/t_cells_";
      //nombre_salida = nombre_salida_base+"eta_"+etas_name[test_eta]+"_vel_"+v_names[test_v]+".txt";
      //nombre_salida = nombre_salida_base+"dcut_"+d_cuts_name[test_d_cut]+"fadh_"+f_adh_name[test_adh]+".txt";
      nombre_salida = nombre_salida_base+"dcut_"+d_cuts_name[test_d_cut]+"d_eq_"+d_eqs_name[test_d_eq]+".txt";

      set_cantidad(nombre_salida, n_ini,t_max/dt);
      int last_id;
      for (int canal = 0; canal < 20; canal++ ){
        for (int part_ini=0;part_ini<n_ini;part_ini++){
              uniform_dist_pick = float(rand())/RAND_MAX;
              vd_iniciales.push_back(valor_segun_inverted_cdf(uniform_dist_pick, inv_cdf));
        }
          particulas = ubicar_particulas(n_ini,L, d_eq,vd_iniciales);//tengo las particulas en sus posiciones iniciales
          float t = 0;
          last_id = n_ini-1;
          //escribir_posiciones(nombre_salida_base +".txt",particulas,0);

          escribir_posiciones(nombre_salida,particulas,0,canal);
          for (int i = 1; i<(t_max/dt); i++){//for del tiempo
            t = t + dt;
            //cout << "Tiempo: " << t << endl;
            vecinos = calcular_vecinos(particulas,d_cut,L);
            calcular_vel(particulas, vecinos, d_eq, d_cut, f_rep, f_adh, eta);
            mover(particulas, dt, L, prob_inversion_step);


            if(t >= round(t)  && (t - round(t) < dt)){
              escribir_posiciones(nombre_salida,particulas,(t),canal);
              if (float(rand())/RAND_MAX < float((N_max-particulas.size()))/N_max) {
                uniform_dist_pick = float(rand())/RAND_MAX;
                float vd = valor_segun_inverted_cdf(uniform_dist_pick, inv_cdf);
                cout << vd << endl;
                last_id = add_particle(particulas,last_id,vd, L);
              }
            }
          }

        }
        cout<<"Fin del sistema "<<endl;
      }
    }
  }

return(0);
}
