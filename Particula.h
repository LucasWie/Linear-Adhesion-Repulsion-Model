#include <iostream>
#include <list>
#include <math.h>
#include <vector>

using namespace std;

class Particula{
  private:
    int id;
    double pos_x = 0;
    double pos_y = 0;
    double vel_x = 0;
    double vel_y = 0;

    double velocidad_deseada = 0;
    bool inverted_flag = false;
    bool out_of_limits = false;

  public:
    Particula(){
      int id;
      double pos_x = 0;
//      double pos_y = 0;
      double vel_x = 0;
//      double vel_y = 0;
      double velocidad_deseada = 0;
    }
    Particula(const Particula& a_copiar);
    bool operator==(const Particula& a_comparar)const ;
    void set_id(int nuevo_id){id = nuevo_id;}
    void set_velocidad_deseada(double v){velocidad_deseada = v;}
    void invert_velocidad_deseada(){
      velocidad_deseada = -1*velocidad_deseada;
      inverted_flag = true;
    }
    void set_out_limit(){out_of_limits=true;}
    bool is_out() {return out_of_limits;}

    bool was_inverted(){return inverted_flag;}
    void set_invertion_true(){inverted_flag = true;}
    void reset_invertion(){inverted_flag = false; }
    void set_pos_x(double x) {pos_x = x;}
  //  void set_pos_y(double y) {pos_y = y;}
  //  void set_vel_y(double dv) {vel_y = dv;}
    void set_vel_x(double mv) {vel_x = mv;}
    int get_id()const {return id;}
    double get_pos_x()const {return pos_x;}
    //double get_pos_y()const {return pos_y;}
    //double get_vel_y()const {return vel_y;}
    double get_vel_x()const {return vel_x;}
    double get_velocidad_deseada()const {return velocidad_deseada;}
};

bool is_out_of_limit(Particula p){return p.is_out();}



Particula::Particula (const Particula& a_copiar){
  id = a_copiar.id;
  pos_x = a_copiar.pos_x;
//  pos_y = a_copiar.pos_y;
  vel_x = a_copiar.vel_x;
//  vel_y = a_copiar.vel_y;
//  radio = a_copiar.radio;
  velocidad_deseada = a_copiar.velocidad_deseada;

}
bool Particula::operator==(const Particula& a_comparar) const{
  if(this->id == a_comparar.id)
   return(1);
  else{return(0);}
}
