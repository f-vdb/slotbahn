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
const size_t HEIGHT = 750;					// Fensterhoehe
const size_t PIXEL_LOGO = 80;
const size_t MARGIN_LOGO = 5;				// Abstand - Logo - Bildschirmrand
const size_t MARGIN_TOP_QUESTION = 100;		// Abstand - Bildschirmrand oben - Fragentext
const size_t MARGIN_LEFT_QUESTION = 50;		// Abstadn - Bildschirmrand lings - Fragentext
const size_t CHARACTER_SIZE = 40;           // Zeichengroesse 

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

enum State {START, HS, FOR, FHR, AHR, STOP};
enum RichtigOderFalsch {NIX ,RICHTIG, FALSCH};

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
	State state = START;
	RichtigOderFalsch richtigOderFalsch = NIX;

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
	
	sf::Text sfTextStarttext;
	sf::Text sfTextState;
	sf::Text sfTextFrage;
	sf::Text sfTextAntwortA;
	sf::Text sfTextAntwortB;
	sf::Text sfTextAntwortC;
	sf::Text sfTextAuswertung;
	
	sfTextStarttext.setFont(font);
	sfTextState.setFont(font);
	sfTextFrage.setFont(font);
	sfTextAntwortA.setFont(font);
	sfTextAntwortB.setFont(font);
	sfTextAntwortC.setFont(font);
	sfTextAuswertung.setFont(font);
	

	std::string str("Welche Farbe hat Blut?");
	std::vector<std::string> antworten{ "rot", "grün", "blau" };
	
	std::string s = str;
	s += "\n\n\n";
	
	sfTextStarttext.setCharacterSize(CHARACTER_SIZE);
	sfTextStarttext.setColor(sf::Color::Black);
	sfTextState.setCharacterSize(CHARACTER_SIZE);
	sfTextState.setColor(sf::Color::Black);
	sfTextFrage.setColor(sf::Color::Blue);
	sfTextFrage.setCharacterSize(CHARACTER_SIZE);
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	sfTextAntwortA.setColor(sf::Color::Blue);
	sfTextAntwortA.setCharacterSize(CHARACTER_SIZE);
	sfTextAntwortB.setColor(sf::Color::Blue);
	sfTextAntwortB.setCharacterSize(CHARACTER_SIZE);
	sfTextAntwortC.setColor(sf::Color::Blue);
	sfTextAntwortC.setCharacterSize(CHARACTER_SIZE);
	sfTextAuswertung.setColor(sf::Color::Magenta);
	sfTextAuswertung.setCharacterSize(CHARACTER_SIZE);
	

	
	sfTextStarttext.setPosition(MARGIN_LEFT_QUESTION, MARGIN_TOP_QUESTION);
	sfTextState.setPosition(MARGIN_LOGO, MARGIN_LOGO);
	sfTextFrage.setPosition(MARGIN_LEFT_QUESTION, MARGIN_TOP_QUESTION);
	sfTextAntwortA.setPosition(MARGIN_LEFT_QUESTION, MARGIN_TOP_QUESTION + 20 + 60);
	sfTextAntwortB.setPosition(MARGIN_LEFT_QUESTION, MARGIN_TOP_QUESTION + 20 + 60 + 60);
	sfTextAntwortC.setPosition(MARGIN_LEFT_QUESTION, MARGIN_TOP_QUESTION + 20 + 60 + 60 + 60);
	sfTextAuswertung.setPosition(MARGIN_LEFT_QUESTION, MARGIN_TOP_QUESTION + 20 + 60 + 60 + 60 + 60);
		
	sfTextStarttext.setString("Drücke Leertaste zum Starten oder zum Neustart");
	sfTextState.setString("HS");
	sfTextFrage.setString(s);
	sfTextAntwortA.setString("A  grün");
	sfTextAntwortB.setString("B  rot");
	sfTextAntwortC.setString("C  blau");
	sfTextAuswertung.setString("RICHTIG  - Lass es krachen...");
	

	

	while (window.isOpen())
	{
		window.clear(sf::Color::White);

		if (state == START)
		{
			window.draw(logoSprite);
			window.draw(sfTextStarttext);

		}
		else if (state != START)
		{
			window.draw(logoSprite);
			window.draw(sfTextState);
			window.draw(sfTextFrage);
			window.draw(sfTextAntwortA);
			window.draw(sfTextAntwortB);
			window.draw(sfTextAntwortC);
			if (richtigOderFalsch == RICHTIG)
			{
				sfTextAuswertung.setColor(sf::Color::Green);
				sfTextAuswertung.setString("RICHTIG  - Lass es krachen...");
				window.draw(sfTextAuswertung);
			}
			else if (richtigOderFalsch == FALSCH)
			{
				sfTextAuswertung.setColor(sf::Color::Red);
				sfTextAuswertung.setString("FALSCH  - Du musst leider warten...");
				window.draw(sfTextAuswertung);
			}
		}
		

		
		
		
		
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
					{
						window.close();
					}
					else if (event.key.code == sf::Keyboard::Space)
					{
						if (state == START)
						{
							state = HS;
						}
						else if (state != START)
						{
							state = START;
						}
					}
					else if (event.key.code == sf::Keyboard::A)
					{
						richtigOderFalsch = FALSCH;
					}
					else if (event.key.code == sf::Keyboard::B)
					{
						richtigOderFalsch = RICHTIG;
					}
					else if (event.key.code == sf::Keyboard::C)
					{
						richtigOderFalsch = FALSCH;
					}
					break;


				default:
					break;

			}
			window.display();

		}
	}

	return 0;
}