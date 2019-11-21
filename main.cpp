#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include <fstream>
#include <ctime>
#include <iostream> //для отладки
double t_u=100;
//относительно реального времени в секундах(для вывода на экран)
int S_pix=100;//сколько занимает в пикселях единица измерения расстояния(для вывода на экран)
const double S_u=1; //единица измерения расстояния в а.е.
const double t=0.01;//1=1 месяц; 1/30=1 день; 2=2 месяца и т.д.(для определения точности вычислений)
const double G=(7.9715E-7*pow(t,2))/pow(S_u,3);
const int N=4;//количество планет
const int Width=1920;//ширина экрана в пикселях
const int Height=1080;//высота экрана в пикселях
int Sun_x=Width/2;
int Sun_y=Height/2;
class Planet{
private:
    class direction{
    public:
        double x,y=0;
    };
    direction v;
    double m,r,x,y=0;
public:
    sf::CircleShape shape;
    Planet(double n_m, double n_r, double n_x, double n_y, double vx, double vy,uint8_t red, uint8_t g, uint8_t b){
        m=n_m;  r=n_r;  x=n_x;
        y=n_y;  v.x=vx*t; v.y=vy*t;
        shape.setRadius(r*S_pix);
        shape.setFillColor(sf::Color(red,g,b));
        shape.setPosition(sf::Vector2f(x*S_pix+Sun_x,y*S_pix+Sun_y));
    }
    Planet(){
        m=0;    r=0;    x=0;
        y=0;    v.x=0;  v.y=0;
        shape.setRadius(0.f);
        shape.setFillColor(sf::Color(0,0,0));
        shape.setPosition(sf::Vector2f(0,0));
    }
    void Get_coordinates(double &px,double &py){
        px=x; py=y;
    }
    double Get_m(){
        return m;
    }
    double Get_r(){
        return r;
    }
    void Sum(double F, double px, double py){
        double dx=px-x;
        double dy=py-y;
        double lb=sqrt(pow(dx,2)+pow(dy,2));//расстояние м/д двумя телами
        double div=F/lb;//отношение из подобия треугольников
        double lx=div*dx;
        double ly=div*dy;
        v.x+=lx/m;
        v.y+=ly/m;
    }
    void Step(){
        x+=v.x;
        y+=v.y;
        shape.setPosition(sf::Vector2f(x*S_pix+Sun_x,y*S_pix+Sun_y));
    }
};

//double Force(double m1, double m2, double l){
//   return (G*m1*m2)/pow(l,2);
//}

int main(){
    //0.0000631 p[1]
    Planet p[N];
    p[0]=Planet(333000,4.6792E-2,0,0,0,0,255,255,0);//4.6792E-3 радиус
    p[1]=Planet(5.5274E-2,4.6792E-2,0.4667,0,0,-0.698550,255,0,0);//1.631E-5 радиус
    p[2]=Planet(0.815,4.6792E-2,0.723,0,0,-0.631764,218,165,32);//4.0454E-5
    p[3]=Planet(1,4.6792E-2,1.01671388,0,0,-0.527072,0,250,154);//4.2635E-5
    sf::RenderWindow window(sf::VideoMode(Width, Height), "Solar Sistem");
    window.setFramerateLimit(60);
    sf::Texture spacetexture;
    spacetexture.loadFromFile("../images/space.jpg");
    sf::Sprite spacesprite(spacetexture);
    float t1=(clock()/(double)CLOCKS_PER_SEC)*10000;
    int flag=0;//установка камеры на планету/звезду(0-солнце,1-меркурий и т.д. до 9)
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type==sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelScrolled){
                if(event.mouseWheelScroll.delta==1.f){
                    if (S_pix>100)
                        S_pix-=100;
                } else S_pix+=100;
                for(int i=0;i<N;i++){
                    p[i].shape.setRadius(p[i].Get_r()*S_pix);
                }
            }
            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button==sf::Mouse::Left){//замедление времени
                    t_u*=2;
                }
                if(event.mouseButton.button==sf::Mouse::Right){//ускорение времени
                    t_u/=2;
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)||sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            Sun_y-=50;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)||sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            Sun_x-=50;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)||sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            Sun_y+=50;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)||sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            Sun_x+=50;
        //переключение камеры между планетами
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0))
            flag=0;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))&& N>0)
            flag=1;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))&& N>1)
            flag=2;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))&& N>2)
            flag=3;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))&& N>3)
            flag=4;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))&& N>4)
            flag=5;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))&& N>5)
            flag=6;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7))&& N>6)
            flag=7;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))&& N>7)
            flag=8;
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)||sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9))&& N>8)
            flag=9;
        double xc=0;
        double yc=0;
        p[flag].Get_coordinates(xc,yc);
        Sun_x=(Width/2)-(xc*S_pix);
        Sun_y=(Height/2)-(yc*S_pix);
    float t2=(clock()/(double)CLOCKS_PER_SEC)*10000;
    if (abs(t2-t1)>=t_u){
        t1=t2;
        for (int i=0;i<N;i++){
            double Gm=G*p[i].Get_m();
            double xi=0;
            double yi=0;
            p[i].Get_coordinates(xi,yi);
            for (int j=0;j<N;j++){
                if (i!=j){
                    double xj=0;
                    double yj=0;
                    p[j].Get_coordinates(xj,yj);
                    double F=Gm*p[j].Get_m();
                    F/=pow(sqrt(pow(xj-xi,2)+pow(yj-yi,2)),2);
                    p[i].Sum(F,xj,yj);//в будущем можно добавить буфер
                    //std::cout<<sqrt(pow(xj-xi,2)+pow(yj-yi,2))<<std::endl;
                }
            }
            p[i].Step();
        }
    }
    window.clear();
    window.draw(spacesprite);
    for (int i=0;i<N;i++){
        window.draw(p[i].shape);
    }
    window.display();
    }
    return 0;
}
