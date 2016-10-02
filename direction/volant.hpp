#include "Leap.h"
#include "const.hpp"

class Volant{
	public:
	Volant(); //constructeur
	float getDirection(); //retourne une valeur de direction, négatif -> gauche, positif -> droite
	float updateDirection(uint64_t timestamp); //met a jour la direction en fonction du temps. utile seulement pour ralentir, si aucun doigt n'est tendu
	
	/** ALGO ***
	Le système accélère dans une direction à chaque tour, d'une valeur def dans const.h
	Si un doigt est tendu mais qu'aucun tour n'est fait, la valeur de direction actuelle est maintenue
	Si l'utilisateur replie son doigt, le système réduit la direction vers 0, à une vitesse (dep du temps) constante (ou pas ???) def dans const.h




	***/
	void handleFrame(Leap::Controller Controller, Leap::Frame frame); //gère chaque frame, met à jour l'état des tours, ou de l'absence de doigt


	private:

	float ACC_RATE;
	float DEC_RATE;
	float MAX_DIR;
	float MIN_DIR;
	float m_direction;
	uint64_t m_lastTimestamp;
	int m_nbTours;

	bool m_isFingerUp;

	float m_lastProgress;

};