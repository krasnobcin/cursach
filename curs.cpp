#include <cairo.h>
#include<iostream>
#include <cairo-svg.h>
#include<string>
#include <math.h>
#include<cmath>
#include<map>
#include<stdlib.h>
#include<fstream>
#include<iomanip>

#define pi 3.1415

void line(double,double,double,double,cairo_t*); // функция рисующая линию
void put_a_marks(cairo_t*); // размечаем график в масштабе
void draw_graph_x(cairo_t*); // рисуем график для координаты
void draw_graph_v(cairo_t*);//рисуем график для скорости
void START();//Приветсвие пользователя и приглашение к вводу
void CALC();//Проверка коректности ввода и предварительные вычисления
void get_res_x();//Получение таблицы и графика для координаты 
void get_res_v();//Получение таблицы и график для скорости
template <typename T>
void read(T&);//Проверка правильности ввода переменной


double g = 9.81,m,l,A,n,ro,v,period;
double alpha,omega0,omega,radius;
const double w_size = 1000;
const double h_size = 1000;
const double marks_size = 2;
double kx; // сколько пикселей занимает единичный отрезок по оси ox;
double ky; // по оси oy
double maxarg;//максимальное значение аргумента
double maxvalue;//максимальное значение функции


int main(void)
{
   START();
   CALC();
   while(A/l > 0.5 || radius/l>0.01)
   {
      std::cout<<"Данная модель работает только с малыми колебаниями. Пожалуйста, введите корректные данные (длина маятника должна быть хотя бы в два раза больше максимального отколнения и радуиус шара должен быть хотя бы в 100 раз меньше длины маятника)\n";
      START();
      CALC();
   }
   get_res_x();
   get_res_v();

 return 0;
}

void line(double x1,double y1,double x2,double y2, cairo_t *cr)
{
   cairo_move_to(cr,x1,y1);
   cairo_line_to(cr,x2,y2);
   cairo_stroke(cr); 
}
void put_a_marks(cairo_t* cr)
{
   double tmp = round(maxvalue/(h_size/10)*100000)/100000;
   ky = 5/tmp; 
   double m = maxvalue;
   for (int i = 0;i<h_size;i+=5)
   {
      std::string t = std::to_string((int)h_size/2-i);
      if (t == "0") continue;
      m-=tmp * (i!=0);
      m = round(m*10000)/10000;
      t = std::to_string(m);
      if ( m == 0) continue;
      line(double(50-marks_size),double(i),double(50+marks_size),double(i),cr);
      cairo_move_to(cr,50-marks_size-6,i+0.5);
      char const *str = t.c_str(); 
      cairo_show_text(cr,str);
   }
   tmp  = round(maxarg/(w_size/10)*10000)/10000;
   kx =5/tmp;
   m = 0;
   for (int i = 55;i<w_size;i+=5)
   {
      m+=tmp;
      m = round(m*10000)/10000;
      line(double(i),double(h_size/2-marks_size),double(i),double(h_size/2+marks_size),cr);
      cairo_move_to(cr,i-3,h_size/2-marks_size+6);
      std::string t = std::to_string(m);
      t.erase(t.begin()+7,t.end());
      if (t == "0") continue;
      char const *str = t.c_str(); 
      cairo_show_text(cr,str);
   }
   cairo_move_to(cr,47,h_size/2+1);
   cairo_show_text(cr,"0");
}
void draw_graph_x(cairo_t *cr)
{
   std::ofstream fout("coordinate.txt");
   fout<<std::fixed<<std::setprecision(5)<<"t"<<'\t'<<"x"<<std::endl;
   double x1 = 0;
   double y1 = 0;
   double i = 0;
   while(i<=2*maxarg)
   {
      double y;
      if (omega0>alpha)
         y = A*exp(-alpha*i)*cos(omega*i);
      if (omega0==alpha)
         y = A*exp(-alpha*i);
      if (omega0<alpha)
         y = (A*exp(-alpha*i))/(2*omega)*((omega+alpha)*exp(omega*i)+(omega-alpha)*exp(-omega*i));
      line(50+x1*kx,h_size/2-y1*ky,50+i*kx,h_size/2-y*ky,cr);
      x1 = i;
      y1 = y;
      fout<<std::fixed<<std::setprecision(5)<<i<<" "<<y<<std::endl;
      i+=maxarg/1000;
   }
   fout.close();
}
void draw_graph_v(cairo_t *cr)
{
   std::ofstream fout("velocity.txt");
   fout<<std::fixed<<std::setprecision(5)<<"t"<<'\t'<<"v"<<std::endl;
   double x1 = 0;
   double y1 = 0;
   double i = 0;
   while(i<=2*maxarg)
   {
      double b = 6 * pi*radius*n;
      double E0 = m*omega*omega*A*A/2;
      double x = A*exp(-alpha*i)*cos(omega*i);
      double deltax = A*exp(-alpha*(i+maxarg/1000))*cos(omega*(i+maxarg/1000))-A*exp(-alpha*i)*cos(omega*i);
      double y;
      if (omega0>alpha)
         y = sqrt((2*E0*exp(-2*alpha*i)-omega*omega*x*x*m)/m)*(abs(deltax)/deltax);
      if (omega0==alpha)
         y = -alpha*A*exp(-alpha*i);
      if (omega0<alpha)
         y = omega0*omega0/omega*A*exp(-alpha*i)*sinh(omega*i);
      line(50+x1*kx,h_size/2-y1*ky,50+i*kx,h_size/2-y*ky,cr);
      x1 = i;
      y1 = y;
      fout<<std::fixed<<std::setprecision(5)<<i<<" "<<y<<std::endl;
      i+=maxarg/1000;
   }
   fout.close();
}
template <typename T>
void read(T& x)
{
   while(!scanf("%lf",&x) || x<=0)
   {
      while(getchar()!='\n');
      std::cout<<"Введено некорректное значение. Повторите ввод"<<std::endl;
   }
}
void START()
{
   std::cout<<"Программа моделирования поведения математического маятника в вязкой среде."<<std::endl;
   std::cout<<"Введите массу шара (в килограммах)"<<std::endl;
   read(m);
   std::cout<<"Введите длину маятника (в метрах)"<<std::endl;
   read(l);
   std::cout<<"Введите начальное отклонение маятника (в метрах)"<<std::endl;
   while(!scanf("%lf",&A))
   {
      while(getchar()!='\n');
      std::cout<<"Введено некорректное значение. Повторите ввод"<<std::endl;
   }
   std::cout<<"Введите вязкость среды (в Паскаль * метр)"<<std::endl;
   while(!scanf("%lf",&n))
   {
      while(getchar()!='\n');
      std::cout<<"Введено некорректное значение. Повторите ввод"<<std::endl;
   }
   std::cout<<"Введите плотность материала шара (в килограммах на метр кубический)"<<std::endl;
   read(ro);
}
void CALC()
{
   omega0 = sqrt(g/l);
   alpha = (3.0*pi*n/m)*pow(3.0*m/(4*pi*ro),1.0/3);
   radius = pow(3.0*m/(4.0*pi*ro),1.0/3);
   if (omega0 <= alpha)
   {
      omega = sqrt(alpha*alpha-omega0*omega0);
      period = 2*pi/omega;
   }
   else
   {
      omega = sqrt(omega0*omega0-alpha*alpha);
      period = 2*pi/omega;
   }
   maxarg = period*7;
}
void get_res_x()
{
   maxvalue = abs(A);
   cairo_surface_t *surface;
   cairo_t *cr;
   surface = cairo_svg_surface_create("coordinate.svg",w_size,h_size);
   cr = cairo_create(surface);
   cairo_set_source_rgb(cr,255,255,255);
   cairo_rectangle(cr,0,0,w_size,h_size);
   cairo_fill(cr);
   cairo_set_source_rgb(cr,0,0,0);
   cairo_set_line_width(cr,0.5);
   cairo_select_font_face(cr,"Sanc",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size(cr,1);
   line(50,h_size/2,w_size,h_size/2,cr);
   line(50,0,50,h_size,cr);
   put_a_marks(cr);
   draw_graph_x(cr);
   cairo_surface_destroy(surface);
   cairo_destroy(cr);

}
void get_res_v()
{
   maxvalue = abs(A*omega0);
   cairo_surface_t *surface;
   cairo_t *cr;
   surface = cairo_svg_surface_create("velocity.svg",w_size,h_size);
   cr = cairo_create(surface);
   cairo_set_source_rgb(cr,255,255,255);
   cairo_rectangle(cr,0,0,w_size,h_size);
   cairo_fill(cr);
   cairo_set_source_rgb(cr,0,0,0);
   cairo_set_line_width(cr,0.5);
   cairo_select_font_face(cr,"Sanc",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
   cairo_set_font_size(cr,1);
   line(50,h_size/2,w_size,h_size/2,cr);
   line(50,0,50,h_size,cr);
   put_a_marks(cr);
   draw_graph_v(cr);
   cairo_surface_destroy(surface);
   cairo_destroy(cr);
}