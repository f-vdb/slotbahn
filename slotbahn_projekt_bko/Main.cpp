#ifdef SFML_STATIC
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")  
#endif // SFML_STATIC


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>


const std::string BKO_LOGO("BKOLogo_80px.png");
const size_t WIDTH = 1300;                  // Fensterbreite
const size_t HEIGHT = 750;					// Fensterhöhe
const size_t PIXEL_LOGO = 80;
const size_t MARGIN_LOGO = 5;				// Abstand - Logo - Bildschirmrand
const size_t MARGIN_TOP_QUESTION = 100;		// Abstand - Bildschirmrand oben - Fragentext
const size_t MARGIN_LEFT_QUESTION = 50;		// Abstadn - Bildschirmrand lings - Fragentext

const std::string HS_FILENAME("hs.txt");
const std::string FOR_FILENAME("for.txt");
const std::string FHR_FILENAME("fhr.txt");
const std::string AHR_FILENAME("ahr.txt");

class Frage {
public:
	std::string frage_;
	std::string richtigeAntwort_;
	std::string falscheAntwortA_;
	std::string falscheAntwortB_;

	Frage(std::string f, std::string r, std::string fA, std::string fB) :
		frage_(f), richtigeAntwort_(r), falscheAntwortA_(fA), falscheAntwortB_(fB) { }

	std::string getFrage() const { return frage_; }
	std::string getRichtigeAntwort() const { return richtigeAntwort_; }
	std::string getFalscheAntwortA() const { return falscheAntwortA_; }
	std::string getFalscheAntwortB() const { return falscheAntwortB_; }

	void printDebug() const {
		std::cout << frage_ << " " << richtigeAntwort_ << " "
		<< falscheAntwortA_ << " " << falscheAntwortB_ << "\n";
	}


};

enum State {START, HS, FOR, FHR, AHR, STOP, RESTART};

std::vector<Frage> hsAlleFragen;
std::vector<Frage> forAlleFragen;
std::vector<Frage> fhrAlleFragen;
std::vector<Frage> ahrAlleFragen;

size_t getRandomNumber() {
	return rand() % 3;  // zufallszahl zwischen 0 und 2 
}

bool leseDateiInVector(const std::string& filename, std::vector<Frage>& vec)
{
	std::ifstream ifs;
	ifs.open(filename, std::ifstream::in);
	if (!ifs)
	{
		std::cout << "konnte datei nicht lesen";
		ifs.close();
		ifs.clear();
		return false;
	}
	std::string line;
	std::string frage, richtigeAntwort;
	std::string falscheAntwortA, falscheAntwortB;
	while (std::getline(ifs, line))
	{
		std::stringstream iss(line);
		std::getline(iss, frage, '|');
		std::getline(iss, richtigeAntwort, '|');
		std::getline(iss, falscheAntwortA, '|');
		std::getline(iss, falscheAntwortB, '|');
		Frage f(frage, richtigeAntwort,
			falscheAntwortA, falscheAntwortB);
		vec.push_back(f);
	}
	return true;
}




int main()
{
	srand(time(NULL));
	if (!leseDateiInVector(HS_FILENAME, hsAlleFragen))
		std::cout << "Fehler: konnte Datei " << HS_FILENAME << " nicht lesen.\n";
	if(!leseDateiInVector(FOR_FILENAME, forAlleFragen))
		std::cout << "Fehler: konnte Datei " << FOR_FILENAME << " nicht lesen.\n";
	if(!leseDateiInVector(FHR_FILENAME, fhrAlleFragen))
		std::cout << "Fehler: konnte Datei " << FHR_FILENAME << " nicht lesen.\n";
	if(!leseDateiInVector(AHR_FILENAME, ahrAlleFragen))
		std::cout << "Fehler: konnte Datei " << AHR_FILENAME << " nicht lesen.\n";
	
	
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Slotbahn - Projekt - Berufskolleg Opladen");
	sf::Sprite logoSprite;
	sf::Texture logoTexture;
	if (!logoTexture.loadFromFile(BKO_LOGO))
		std::cout << "Konnte Datei " << BKO_LOGO << " nicht laden\n";
	logoSprite.setTexture(logoTexture);
	logoSprite.setPosition(WIDTH - PIXEL_LOGO - MARGIN_LOGO, MARGIN_LOGO);
	sf::Font font;
	if (!font.loadFromFile("sansation.ttf"))
		std::cout << "coud not load font...\n";
	sf::Text frage;
	sf::Text antwortA;
	sf::Text antwortB;
	sf::Text antwortC;
	sf::Text state;

	frage.setFont(font);
	antwortA.setFont(font);
	antwortB.setFont(font);
	antwortC.setFont(font);
	state.setFont(font);

	std::string str("Welche Farbe hat Blut?");
	std::vector<std::string> antworten{ "rot", "grün", "blau" };
	
	std::string s = str;
	s += "\n\n\n";
	

	frage.setColor(sf::Color::Blue);
	frage.setCharacterSize(40);
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	antwortA.setColor(sf::Color::Blue);
	antwortA.setCharacterSize(40);
	antwortB.setColor(sf::Color::Blue);
	antwortB.setCharacterSize(40);
	antwortC.setColor(sf::Color::Blue);
	antwortC.setCharacterSize(40);
	state.setCharacterSize(40);
	state.setColor(sf::Color::Green);

	
	state.setPosition(MARGIN_LOGO, MARGIN_LOGO);
	frage.setPosition(MARGIN_LEFT_QUESTION, MARGIN_TOP_QUESTION);
	antwortA.setPosition(MARGIN_LEFT_QUESTION, MARGIN_TOP_QUESTION + 20 + 60);
	antwortB.setPosition(MARGIN_LEFT_QUESTION, MARGIN_TOP_QUESTION + 20 + 60 + 60);
	antwortC.setPosition(MARGIN_LEFT_QUESTION, MARGIN_TOP_QUESTION + 20 + 60 + 60 + 60);
		
	

	frage.setString(s);
	antwortA.setString("A  grün");
	antwortB.setString("B  rot");
	antwortC.setString("C  blau");
	state.setString("HS");

	

	while (window.isOpen())
	{
		

		window.clear(sf::Color::White);
		window.draw(logoSprite);
		window.draw(state);
		window.draw(frage);
		window.draw(antwortA);
		window.draw(antwortB);
		window.draw(antwortC);

	
		window.display();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	return 0;
}