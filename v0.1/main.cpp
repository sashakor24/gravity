#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <cstdint>
#include <chrono>
#include <cmath>
#include <ctime>
#include "Point.hpp"
#include "Planet.hpp"
#include "const.hpp"

float computationTime = COMPUTATION_TIME;

class SpeedPanel
{
	static const uint8_t n = 8;
	uint8_t k = 0;
	Color colorActive = (Color) {173,255,47};
	Color colorPassive = (Color) {105,105,105};
	int speed[n];
	GLfloat points[n][6];
public:
	const float SIDE = 0.04;//0.03
	const float SPACE = SIDE / 10;
	const float LEFT = 0.01;
	const float TOP = 0.03;
	void getPoints(Point& a, Point& b, Point& c, int i)
	{
		if(i >= 0 && i < n)
		{
			a.set(points[i][0],points[i][1]);
			b.set(points[i][2],points[i][3]);
			c.set(points[i][4],points[i][5]);
		}
	}
	uint8_t getN()
	{
		return n;
	}
	float get(int i, int j)
	{
		if(i >= 0 && i < n && j >=0 && j < 6)
			return points[i][j];
		else return 0;
	}
	SpeedPanel()
	{
		speed[0] = 1;
		speed[1] = 2;
		speed[2] = 5;
		speed[3] = 10;
		speed[4] = 100;
		speed[5] = 1000;
		speed[6] = 10000;
		speed[7] = 100000;//I think here should be sqrt
		for (uint8_t i = 0; i < n; ++i)
		{
			points[i][0] = -1 + LEFT + i * (SIDE * sin(PI / 3) + SPACE);
			points[i][1] = 1 - TOP;
			points[i][2] = points[i][0];
			points[i][3] = points[i][1] - SIDE;
			points[i][4] = points[i][0] + SIDE * sin(PI / 3);// the vertex is extended by an insquare frame
			points[i][5] = points[i][1] - SIDE * cos(PI / 3);
		}
	}
	void SetK(uint8_t k)
	{
		if (k >= 0 && k < n)
			this->k = k;
	}
	void draw()
	{
		for (uint8_t i = 0; i < n; ++i)
		{
			GLubyte r,g,b;
			if (i <= k)
				colorActive.get(r,g,b);
			else
				colorPassive.get(r,g,b);
			glColor3ub(r,g,b);
			GLuint vertexBuffer;
			glGenBuffers(1,&vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER,sizeof(points[i]),points[i],GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void*)0);
			glDrawArrays(GL_TRIANGLES,0,3);
			glDisableVertexAttribArray(0);
		}
	}
	operator int()
	{
		return speed[k];
	}
};
SpeedPanel speed;
double kmPix = 3.5E4;
Point camera;
int N = 15;
int l = 0;
Planet* P;
void drawPlanet()
{
	for (int i = 0; i < l; ++i)
	{
		//check the object is in the frame
		float x,y;
		x = P[i].p.getX() - camera.getX();// coordinates relatively the camera
		y = P[i].p.getY() - camera.getY();
		if (2 * abs(x) - P[i].getR() < WIDTH * kmPix
			&& 2 * abs(y) - P[i].getR() < HEIGHT * kmPix)
		{
			GLfloat triangleFan[2 * (VERTEXES + 2)];
			int k = 0;//amount of vertexes
				triangleFan[2 * k] = 2 * (P[i].p.getX() - camera.getX()) / WIDTH / kmPix;//center
				triangleFan[2 * k + 1] = 2 * (P[i].p.getY() - camera.getY()) / HEIGHT / kmPix;
				++k;
				float angle = PI / 2;
				float delta = 2 * PI / VERTEXES;
				for (int j = 0; j <= VERTEXES; ++j)
				{
					Point directVector;
					if (j == VERTEXES)
						if(P[i].getR() / kmPix >= MIN_PIX)
							directVector.set(
							cos(angle) * P[i].getR(),
							sin(angle) * P[i].getR()
							);
						else
							directVector.set(
							cos(angle) * MIN_PIX * kmPix,
							sin(angle) * MIN_PIX * kmPix
							);
					else
						if(P[i].getR() / kmPix >= MIN_PIX)
							directVector.set(
							cos(angle + j * delta) * P[i].getR(),
							sin(angle + j * delta) * P[i].getR()
							);
						else
							directVector.set(
							cos(angle + j * delta) * MIN_PIX * kmPix,
							sin(angle + j * delta) * MIN_PIX * kmPix
							);
					triangleFan[2 * k] = 2 * (directVector.getX() + x) / WIDTH / kmPix;//center
					triangleFan[2 * k + 1] = 2 * (directVector.getY() + y) / HEIGHT / kmPix;
					++k;
				}
			GLubyte r,g,b,a;
			a = 255;
			if(P[i].getR() / kmPix < MIN_PIX)
				a = 127;
			P[i].c.get(r,g,b);
			glColor4ub(r,g,b,a);
			GLuint vertexBuffer;
			glGenBuffers(1,&vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER,sizeof(triangleFan),triangleFan,GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void*)0);
			glDrawArrays(GL_TRIANGLE_FAN,0,k);
			glDisableVertexAttribArray(0);
		}
	}
}
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	drawPlanet();
	speed.draw();
}
void resize(Planet* P, int& N)
{
	int newN = N + 5;
	Planet* newP = new Planet[newN];
	for (int i = 0; i < N; ++i)
		newP[i] = P[i];
	delete[] P;
	P = newP;
	N = newN;
}
void gravity()
{
	for (int i = 0; i < l; ++i)
		for (int j = i + 1; j < l; ++j)
		{
			Point dist = P[j].p - P[i].p;
			double F = G * P[i].getM() * P[j].getM() / pow(dist.len(),2);
			Point n = dist.getUnit();
			P[i].v += n * (F / P[i].getM()) * COMPUTATION_TIME;
			P[j].v -= n * (F / P[j].getM()) * COMPUTATION_TIME;
		}
}
void step()
{
	for (int i = 0; i < l; ++i)
		P[i].p += P[i].v * COMPUTATION_TIME;
		//velosity is measured in m/sec
		//computation time is measured in sec/computation
}
int OpenGLToSFML(float a, bool isHorisontal)
{
	if (isHorisontal)
	{
		a *= WIDTH / 2.f;
		a += WIDTH / 2.f;
	}
	else
	{
		a = -a;
		a *= HEIGHT / 2.f;
		a += HEIGHT / 2.f;
	}
	return (int)a;
}
float SFMLToOpenGL(int a, bool isHorisontal)
{
	float ans;
	if (isHorisontal)
	{
		ans = a - WIDTH / 2;
		ans /= WIDTH / 2.f;
	}
	else
	{
		a = -a;
		ans = a + HEIGHT / 2;
		ans /= HEIGHT / 2.f;
	}
	return ans;
}
double SFMLToMySystem(int a, bool isHorisontal)//??
{
	double ans;
	if (isHorisontal)
	{
		ans = a - WIDTH / 2;
		ans *= kmPix;
		ans += camera.getX();
	}
	else
	{
		a = -a;
		ans = a + HEIGHT / 2;
		ans *= kmPix;
		ans += camera.getY();
	}
	return ans;
}
inline double sqr(double x)
{
	return x * x;
}
int main()
{
	//std::ofstream f("output.txt");
	float SPLeftVertexX = speed.get(0, 0);
	float SPTopVertexY = speed.get(0, 1);
	float SPLowVertexY = speed.get(0, 3);
	float SPRightVertexX = speed.get(speed.getN() - 1, 4);
	P = new Planet[N];
	//P[0].set(1.989E30,6.9634E5,(Point){0,0},(Point){0,0},(Color){255,0,0});
	//P[1].set(3.285E23,2.4397E3,(Point){4.6E10,0},(Point){0,48000},(Color){128,128,128}); //2.4397E3 - R
	//P[2].set(4.867E24,2.4397E3,(Point){1.0802E11,0},(Point){0,35020},(Color){128,128,128}); //2.4397E3 - R
	//4.7362E4 - v
	//6.9634E5
	P[0].set(1.989E30,6.9634E5,(Point){0,0},(Point){0,-2E6},(Color){255,0,0});
	P[1].set(1.989E30,6.9634E5,(Point){1.4E7,0},(Point){0,2E6},(Color){255,255,255});
	l = 2;
	sf::Window window(sf::VideoMode(WIDTH,HEIGHT),"Solar System OpenGL");
	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(FPS);
	window.setActive(true);
	bool running = true;
	std::chrono::time_point<std::chrono::system_clock> tframe;
	std::chrono::time_point<std::chrono::system_clock> tcalc;
	std::chrono::time_point<std::chrono::system_clock> t;
	tframe = std::chrono::system_clock::now();
	tcalc = tframe;
	while(running)
	{
		sf::Event event;
		while(window.pollEvent(event))
			switch(event.type)
			{
				case sf::Event::Closed:
					running = false;
					break;
				case sf::Event::Resized:
					glViewport(0,0,event.size.width,event.size.height);
					break;
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left)// handling of choice of a planet or speed
					{
						sf::Vector2i mouse = sf::Mouse::getPosition(window);
						float mouseOGLX = SFMLToOpenGL(mouse.x,true);
						float mouseOGLY = SFMLToOpenGL(mouse.y,false);
						if (mouseOGLX >= SPLeftVertexX && mouseOGLX <= SPRightVertexX
						&& mouseOGLY >= SPLowVertexY && mouseOGLY <= SPTopVertexY)
						{
							mouseOGLX -= SPLeftVertexX;
							float unit = (SPRightVertexX - SPLeftVertexX) / speed.getN();
							speed.SetK(round(mouseOGLX / unit));
							computationTime = COMPUTATION_TIME / speed;
						}
						else
						{
							double mouseX = SFMLToMySystem(mouse.x,true);
							double mouseY = SFMLToMySystem(mouse.y,false);
							for(int i = 0; i < l; ++i)
							if(sqrt(sqr(mouseX - P[i].p.getX()) + sqr(mouseY - P[i].p.getY())) <= P[i].getR())
							{
							camera = P[i].p;//temporary
							break;
							}
						}
					}
					break;
			}
		//if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		t = std::chrono::system_clock::now();
		if (std::chrono::duration_cast<std::chrono::microseconds>(t - tcalc).count() / (float) 1000000 > computationTime)
		{
			gravity();
			step();
			tcalc = std::chrono::system_clock::now();
		}
		if (std::chrono::duration_cast<std::chrono::microseconds>(t - tframe).count() / (float) 1000000 > FRAME_TIME)
		{
			draw();
			window.display();
			tframe = std::chrono::system_clock::now();
			tcalc = tframe;
		}
	}
	return 0;
} 
