

// Añade el estado actual del sistema al vector de string de salida
void format_to_output(vector<Particula> &particulas, vector<string> &output, float time) {
      string aux;
      for (int i=0; i<particulas.size(); i++) {
        aux = to_string(particulas[i].get_id()) + ", "
            + to_string(particulas[i].get_pos_x()) + ", "
            + to_string(particulas[i].get_vel_x()) + ", "
            + to_string(time) ;
        output.push_back(aux);
      }
}



vector<string> simular(double d_eq, double d_cut, double f_rep, double f_adh,default_random_engine &generator,float eta, float inversion_prob_min, vector<float>v_real, float t_max, float L, vector<int> densidad, map<float,float> inv_cdf){

  // Buscar el dt correcto se necesita la velocidad y la distancia a la que se relacionan
  float vel_max_esperada = *max_element(v_real.begin(),v_real.end());
  float dt = 0.05 * (d_cut/vel_max_esperada);
  float prob_inversion_step = inversion_prob_min*dt;
  vector<string> output;
  //algunos vectores necesarios
  vector<float> vd_iniciales;
  vector<vector<int> > vecinos;

  float t = 0;

  int n_ini = densidad[0];
  int last_id = n_ini-1;

  //esto quedará para ser arreglado a futuro, definiendo la densidad de alguna manera


//inicialzar velocidades segun las velocidades reales (usando inverted cdf)
  for (int part_ini=0;part_ini<n_ini;part_ini++){
    float uniform_dist_pick = float(rand())/RAND_MAX;
    vd_iniciales.push_back(valor_segun_inverted_cdf(uniform_dist_pick, inv_cdf));
  }

  //ubicar en x (randomly)
  vector<Particula> particulas;
  particulas = ubicar_particulas(n_ini,L, d_eq,vd_iniciales);//tengo las particulas en sus posiciones iniciales
  float densidad_deseada;
  int pos_densidad_deseada = 0;
  int modificador = 1;
  for (int i = 1; i<(t_max/dt); i++){//for del tiempo
        t = t + dt;
        vecinos = calcular_vecinos(particulas,d_cut,L);
        calcular_vel(particulas, vecinos, d_eq, d_cut, f_rep, f_adh, generator,eta);
        mover(particulas, dt, L, prob_inversion_step);

        if(t >= round(t)  && (t - round(t) < dt)){
          //cout << "guardar estado al tiempo: "<< t <<endl;
          format_to_output(particulas, output, t);
          if (pos_densidad_deseada==densidad.size()-1){modificador=-1;}
          if (pos_densidad_deseada==0){modificador=1;}
          pos_densidad_deseada = pos_densidad_deseada+modificador;

          //cout<< "pos dens deseada: "<<pos_densidad_deseada << endl;
          densidad_deseada = densidad[pos_densidad_deseada];
          //cout << "dens deseada: " << densidad_deseada<<endl;

          if (particulas.size()  < densidad_deseada ){
              //cout << "Agrendo particula porque ya toca \n";
              float uniform_dist_pick = float(rand())/RAND_MAX;
              float vd = valor_segun_inverted_cdf(uniform_dist_pick, inv_cdf);
              //cout << vd << endl;
              last_id = add_particle(particulas,last_id,vd, L);
        //    }
          }
          else {
            if (particulas.size()  > densidad_deseada ) {
              erase_bordering_particle(particulas,L);
            }
          }
       }
    }
    return output;

}
