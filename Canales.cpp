#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <sstream>


#include "Particula.h"
#include "Funciones_cuadratic_adhesion.h"
#include "set_particulas.h"
#include "vecinos.h"
#include "Sim_core.h"


using namespace std;

int main(){

  srand(44);
  vector<Particula> particulas;


  double d_eq = 12.0;
  double d_cut = 30;
  double f_rep = 30;
  double f_adh = 10;
  float eta = 3;

  default_random_engine generator;
  //uniform_real_distribution<float> distribution(-eta/2.0, eta/2.0);

//Definicion de parámetros. valores posibles para cada uno
  float inversion_prob_min = 3.0/1900;
  float prob_inversion_step;

  //vector <float> f_adhs = {0.75};
  //vector <string> f_adh_name = {"0.75"};
  vector <float> f_adhs = {0};
  vector <string> f_adh_name = {"0"};


  vector <float> d_cuts = {20,25,30,35};
  vector <string> d_cuts_name = {"20","25","30","35"};


//  vector <float> d_cuts = {20};
//  vector <string> d_cuts_name = {"20"};


  vector <float> d_eqs = {10,12,14,16,18,20};
  vector <string> d_eqs_name = {"10","12","14","16","18","20"};


//  vector <float> d_eqs = {16};
//  vector <string> d_eqs_name = {"16"};



//srand(144);

  float L = 650;   //Hacer mas preciso, sacado de experimentos, si no puro humo
  float t_max =  120;// 2 horas

  //Manejo de archivos. Donde y con qué nombre se guardarán
  string ruta = "./parameter_evaluation_final/" ;
  string comando = "mkdir -p "+ruta;
  system(comando.c_str());
  string nombre_salida;
  string nombre_salida_base;



  //Ingreso de valores experimentales: Las velocidades (Para estimar su distribución)
                                    // y los n de celula por canal (para la densidad)
  vector<float> velocidades_reales;
  velocidades_reales = cargar_dist("velocidades_aisaldas.txt");
  vector<vector<int> > densidades = cargar_n_cells("N_por_canal_vs_t.csv");

  //cout <<velocidades_reales.size()<<endl;


  float vel_max_esperada = *max_element(velocidades_reales.begin(),velocidades_reales.end());
  map<float,float> inv_cdf = get_inverted_cdf(velocidades_reales);
  //float uniform_dist_pick;
  for (int test_adh = 0; test_adh < f_adhs.size(); test_adh++){
    f_adh = f_adhs[test_adh];
    for (int test_d_eq = 0; test_d_eq < d_eqs.size(); test_d_eq++){
        d_eq = d_eqs[test_d_eq];
        for (int test_d_cut = 0; test_d_cut < d_cuts.size(); test_d_cut++){
          //vd_iniciales.clear();
          d_cut = d_cuts[test_d_cut];
          nombre_salida_base = ruta +"/Sim_";
          nombre_salida = nombre_salida_base+"dcut_"+d_cuts_name[test_d_cut]+"_deq_"+d_eqs_name[test_d_eq]+".txt";

          clean_start_file(nombre_salida);

          for (int canal = 0; canal < 12; canal++ ){
            vector <string> output;
            int canal_a_imitar = rand()%densidades.size();
            //cout << "Canal a imitar: " << canal_a_imitar<<endl;
            vector<int> n_por_tiempo = densidades[canal_a_imitar];

            //cout << "Por empezar la simulacion del canal: "<<canal <<" \n";

            output = simular(d_eq, d_cut, f_rep, f_adh, generator,eta, inversion_prob_min, velocidades_reales,t_max ,L,n_por_tiempo,inv_cdf);
            escribir_posiciones(nombre_salida,output,canal);
          }
        }
     }
   }

   cout << "Fin del programa";
return(0);
}
