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
#include <time.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>


const std::string BKO_LOGO("BKOLogo_80px.png");
const size_t WIDTH = 1400;                  // Fensterbreite
const size_t HEIGHT = 800;					// Fensterhöhe
const size_t PIXEL_LOGO = 80;
const size_t MARGIN_LOGO = 5;				// Abstand - Logo - Bildschirmrand
const size_t MARGIN_TOP_QUESTION = 100;		// Abstand - Bildschirmrand oben - Fragentext
const size_t MARGIN_LEFT_QUESTION = 50;		// Abstadn - Bildschirmrand lings - Fragentext

const std::string HS_FILENAME("hs.txt");
const std::string FOR_FILENAME("for.txt");
const std::string FHR_FILENAME("fhr.txt");
const std::string ABI_FILENAME("abi.txt");

class Frage {
public:
	std::string frage;
	std::string richtige_Anwort;
	std::string falsche_AnwortA;
	std::string falsche_AntwortB;


};

std::vector<Frage> hsFragen;
std::vector<Frage> forFragen;
std::vector<Frage> fhrFragen;
std::vector<Frage> abiFragen;

size_t getRandomNumber() {
	return rand() % 3;  // zufallszahl zwischen 0 und 2 
}

int main()
{
	srand(time(NULL));
	//std::cout << "zufallszahl zwischen 1 und 0:\n";
	//for (size_t i = 0; i != 10; ++i)
	//	std::cout << getRandomNumber() << "\n"; // zufallszahl zwischen 2 und 0 
	
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Slotbahn - Projekt - Berufskolleg Opladen");
	

	sf::Sprite logoSprite;

	sf::Texture logoTexture;
	if (!logoTexture.loadFromFile(BKO_LOGO))
	{
		std::cout << "Konnte Datei " << BKO_LOGO << " nicht laden\n";
	}

	logoSprite.setTexture(logoTexture);
	logoSprite.setPosition(WIDTH - PIXEL_LOGO - MARGIN_LOGO, MARGIN_LOGO);


	sf::Font font;
	if (!font.loadFromFile("sansation.ttf"))
		std::cout << "coud not load font...\n";

	sf::Text text;
	text.setFont(font);
	std::string frage("Welche Farbe hat Blut?");
	std::vector<std::string> antworten{ "rot", "grün", "blau" };
	
	std::string s = frage;
	s += "\n\n\n";
	

	// for schleife ist doof...besser zufalls zahl 0, 1, 2
	// dann zufallzahl 0,1......!!!!!!!!!!!!1
	size_t counter = 0;
	for (auto i : antworten)
	{
		if (counter == 0)
		{
			s += "A  ";
			counter++;
		}
		else if (counter == 1)
		{
			s += "B  ";
			counter++;
		}
		else
		{
			s += "C  ";
		}
			

		s += i + "\n\n";

	}
		
		
	

	text.setString(s);
	text.setPosition(MARGIN_LEFT_QUESTION, MARGIN_TOP_QUESTION);
	text.setColor(sf::Color::Blue);
	text.setCharacterSize(40);
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);


	

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);
		window.draw(logoSprite);
		window.draw(text);
	
		window.display();
	}

	return 0;
}