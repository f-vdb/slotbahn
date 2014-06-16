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
const float MARGIN_LOGO = 5;				// Abstand - Logo - Bildschirmrand
const float MARGIN_TOP_QUESTION = 100;		// Abstand - Bildschirmrand oben - Fragentext
const float MARGIN_LEFT_QUESTION = 50;		// Abstadn - Bildschirmrand lings - Fragentext
const size_t CHARACTER_SIZE = 40;           // Zeichengroesse 

const std::string HS_FILENAME("hs.txt");
const std::string FOR_FILENAME("for.txt");
const std::string FHR_FILENAME("fhr.txt");
const std::string AHR_FILENAME("ahr.txt");

class Answer {
private:
	bool isRight_;
	std::string answer_;
public:
	Answer(bool i, std::string a) :isRight_(i), answer_(a) {}
	bool getIsAnswerRight() const { return isRight_; }
	std::string getAnswer() const { return answer_; }
};

class Frage {
public:
	std::string frage_;
	Answer answerA_;
	Answer answerB_;
	Answer answerC_;

	Frage(std::string f, std::string fA, std::string fB, std::string fC) :
		frage_(f), answerA_(true, fA), answerB_(false, fB), answerC_(false, fC) { }

	std::string getFrage() const { return frage_; }
	std::string getRichtigeAntwortA() const { return answerA_.getAnswer(); }
	std::string getFalscheAntwortB() const { return answerB_.getAnswer(); }
	std::string getFalscheAntwortC() const { return answerC_.getAnswer(); }
};

enum class State {START, HS, FOR, FHR, AHR, ENDE};      // Status fuer: Wo befinden wir uns
enum class RichtigeAntwortIst {START, A, B, C};         // Status fuer: Welche Antwort ist richtig
enum class RichtigOderFalsch {START, RICHTIG, FALSCH};  // Status fuer: Richtig oder Falsch
enum class NeueFrage {JA, NEIN};                        // Status fuer: Neue Frage Ja oder Nein-> warte auf KeyPressed

std::vector<Frage> hsAlleFragen;
std::vector<Frage> forAlleFragen;
std::vector<Frage> fhrAlleFragen;
std::vector<Frage> ahrAlleFragen;

size_t getRandomNumberZeroToThree()
{
	return rand() % 3;  // zufallszahl zwischen 0 und 2 
}

size_t getRandomNumberZeroToA(int a) 
{
	return rand() % a; // zufallszahl zwischen 0 und A-1
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
	srand(static_cast<unsigned int>(time(0)));
	State state = State::START;
	RichtigOderFalsch richtigOderFalsch = RichtigOderFalsch::START;
	RichtigeAntwortIst richtigeAntwortIst = RichtigeAntwortIst::START;
	NeueFrage neueFrage = NeueFrage::JA;
		
	if (!leseDateiInVector(HS_FILENAME, hsAlleFragen))
		std::cout << "Fehler: konnte Datei " << HS_FILENAME << " nicht lesen.\n";
	if (!leseDateiInVector(FOR_FILENAME, forAlleFragen))
		std::cout << "Fehler: konnte Datei " << FOR_FILENAME << " nicht lesen.\n";
	if (!leseDateiInVector(FHR_FILENAME, fhrAlleFragen))
		std::cout << "Fehler: konnte Datei " << FHR_FILENAME << " nicht lesen.\n";
	if (!leseDateiInVector(AHR_FILENAME, ahrAlleFragen))
		std::cout << "Fehler: konnte Datei " << AHR_FILENAME << " nicht lesen.\n";
	
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Slotbahn - Projekt - Berufskolleg Opladen");
	window.setKeyRepeatEnabled(false);  // damit wenn der Pneumatik-Stempel zu lange auf die Tasten haut nur ein Zeichen genommen wird
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
		
	sfTextStarttext.setString("Dr�cke Leertaste zum Starten oder zum Neustart");
		
	while (window.isOpen())
	{
		if (state == State::START)
		{
			window.clear(sf::Color::White);
			window.draw(logoSprite);
			window.draw(sfTextStarttext);
			window.display();

		}
		else if (state == State::HS && neueFrage == NeueFrage::JA)
		{
			size_t rand = getRandomNumberZeroToA(hsAlleFragen.size());
			std::string frage = hsAlleFragen[rand].getFrage();
			sfTextFrage.setString(frage);
			std::vector<Answer> vecAnswer;
			vecAnswer.push_back(hsAlleFragen[rand].answerA_);
			vecAnswer.push_back(hsAlleFragen[rand].answerB_);
			vecAnswer.push_back(hsAlleFragen[rand].answerC_);
			size_t r = getRandomNumberZeroToA(vecAnswer.size());
			std::string aA = "A  " + vecAnswer[r].getAnswer();
			sfTextAntwortA.setString(aA);
			if (vecAnswer[r].getIsAnswerRight())
			{
				richtigeAntwortIst = RichtigeAntwortIst::A;
			}
			vecAnswer.erase(vecAnswer.begin() + r);
			
			r = getRandomNumberZeroToA(vecAnswer.size());
			std::string aB = "B  " + vecAnswer[r].getAnswer();
			sfTextAntwortB.setString(aB);
			if (vecAnswer[r].getIsAnswerRight())
			{
				richtigeAntwortIst = RichtigeAntwortIst::B;
			}
			vecAnswer.erase(vecAnswer.begin() + r);
			
			std::string aC = "C  " + vecAnswer[0].getAnswer();
			sfTextAntwortC.setString(aC);
			if (vecAnswer[0].getIsAnswerRight())
			{
				richtigeAntwortIst = RichtigeAntwortIst::C;
			}
			vecAnswer.erase(vecAnswer.begin());

			window.clear(sf::Color::White);
			window.draw(logoSprite);
			sfTextState.setString("HS");
			window.draw(sfTextState);
			window.draw(sfTextFrage);
			window.draw(sfTextAntwortA);
			window.draw(sfTextAntwortB);
			window.draw(sfTextAntwortC);
			window.display();
			neueFrage = NeueFrage::NEIN;
		}
		else if (state == State::HS && neueFrage == NeueFrage::NEIN)
		{
			if (richtigOderFalsch == RichtigOderFalsch::RICHTIG)
			{
				sfTextAuswertung.setColor(sf::Color::Green);
				sfTextAuswertung.setString("RICHTIG  - Lass es krachen...");
				window.draw(sfTextAuswertung);
				richtigOderFalsch = RichtigOderFalsch::START;
				window.display();
				// continue; ???????????
			}
			else if (richtigOderFalsch == RichtigOderFalsch::FALSCH)
			{
				sfTextAuswertung.setColor(sf::Color::Red);
				sfTextAuswertung.setString("FALSCH  - Du musst leider warten...");
				window.draw(sfTextAuswertung);
				richtigOderFalsch = RichtigOderFalsch::START;
				window.display();
			}
		}
		else if (state == State::FOR)
		{
			window.clear(sf::Color::White);
			window.draw(logoSprite);
			sfTextState.setString("FOR");
			window.draw(sfTextState);
			window.display();
		}
		else if (state == State::FHR)
		{
			window.clear(sf::Color::White);
			window.draw(logoSprite);
			sfTextState.setString("FHR");
			window.draw(sfTextState);
			window.display();
		}
		else if (state == State::AHR)
		{
			window.clear(sf::Color::White);
			window.draw(logoSprite);
			sfTextState.setString("AHR");
			window.draw(sfTextState);
			window.display();
		}
		else if (state == State::ENDE)
		{			
			window.clear(sf::Color::White);
			window.draw(logoSprite);
			sfTextState.setString("Ende - geschafft - Neustart mit Space");
			window.draw(sfTextState);
			window.display();
		}
		// Event-Loop
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
						if (state == State::START)
						{
							state = State::HS;
							neueFrage = NeueFrage::JA;
						}
						else if (state != State::START)
						{
							state = State::START;
						}
					}
					else if (event.key.code == sf::Keyboard::F)
					{
						if (state == State::HS)
						{
							state = State::FOR;
						}
						else if (state == State::FOR)
						{
							state = State::FHR;
						}
						else if (state == State::FHR)
						{
							state = State::AHR;
						}		
						else if (state == State::AHR)
						{
							std::cout << "in ahr -> state:end\n";
							state = State::ENDE;
						}
						
					}
					else if (richtigeAntwortIst == RichtigeAntwortIst::A)
					{
						if (event.key.code == sf::Keyboard::A)
						{
							richtigOderFalsch = RichtigOderFalsch::RICHTIG;
						}
						else if (event.key.code == sf::Keyboard::B)
						{
							richtigOderFalsch = RichtigOderFalsch::FALSCH;
						}
						else if (event.key.code == sf::Keyboard::C)
						{
							richtigOderFalsch = RichtigOderFalsch::FALSCH;
						}
					}
					else if (richtigeAntwortIst == RichtigeAntwortIst::B) 
					{
						if (event.key.code == sf::Keyboard::A)
						{
							richtigOderFalsch = RichtigOderFalsch::FALSCH;
						}
						else if (event.key.code == sf::Keyboard::B)
						{
							richtigOderFalsch = RichtigOderFalsch::RICHTIG;
						}
						else if (event.key.code == sf::Keyboard::C)
						{
							richtigOderFalsch = RichtigOderFalsch::FALSCH;
						}
					}
					else if (richtigeAntwortIst == RichtigeAntwortIst::C)
					{
						if (event.key.code == sf::Keyboard::A)
						{
							richtigOderFalsch = RichtigOderFalsch::FALSCH;
						}
						else if (event.key.code == sf::Keyboard::B)
						{
							richtigOderFalsch = RichtigOderFalsch::FALSCH;
						}
						else if (event.key.code == sf::Keyboard::C)
						{
							richtigOderFalsch = RichtigOderFalsch::RICHTIG;
						}
					}
					// ......
					break;


				default:
					break;
			}
		}
	}
	return 0;
}