#include "volant.hpp"

using namespace Leap;
using namespace std;

const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

Volant::Volant()
{
	m_direction = 0;
	m_lastProgress = 0;
	ACC_RATE = 0.01;
	DEC_RATE = 0.8;
	MIN_DIR = 0.35;
	MAX_DIR = 1;
}

float Volant::getDirection()
{
 if(m_direction >1)return 1;
 if(m_direction < -1) return -1;
 return m_direction;

}


float Volant::updateDirection(uint64_t timestamp)
{
	if(abs(m_direction)  < MIN_DIR)
	{
		m_direction = 0;
		m_lastTimestamp = timestamp;		
		return 0;
	} 
	double delta_t;

	if(m_lastTimestamp == 0)
		delta_t = 0;
	else
		delta_t = (timestamp - m_lastTimestamp)/(double)1000000.0; //calcul de l'intervalle de temps écoulé, passage en secondes

	double increment = -signe(m_direction) * DEC_RATE * delta_t;

	//cout << "Etat ralentissement: dt: " << delta_t << "dx: " << delta_x << "increment: " << increment << "timestamp: " << timestamp << "last timestamp: " << m_lastTimestamp << endl;
	m_lastTimestamp = timestamp;
	if(signe(increment) == signe(m_direction))
		return 0;
	m_direction += increment;
	}

void Volant::handleFrame(Controller controller, Frame frame)
{
	if(frame.fingers().extended().count() == 0)
	{
		updateDirection(frame.timestamp());
	}
	else{

		m_isFingerUp = true;
		//détection des gestes:
		GestureList gestures = frame.gestures();
		if(!gestures.isEmpty())
		{
			const GestureList gestures = frame.gestures();
			for (int i = 0; i < gestures.count(); ++i) {
			    Gesture gesture = gestures[i];

			    if(gesture.type() == Gesture::TYPE_CIRCLE) //GESTION
			    {
			        CircleGesture circle = gesture;
			        float progress = circle.progress();

			        

				        if(progress - m_lastProgress >= 0.1)
				        {
				    //    	cout << "diff: " << progress - m_lastProgress << endl;
				        	m_lastProgress = progress;

				        	if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
				        	  
					        	  m_direction += ACC_RATE;
					        	  if(m_direction > MAX_DIR)
					        	  	m_direction = MAX_DIR;
					        	  if(m_direction > 0 && m_direction < MIN_DIR)
					        	  	m_direction = MIN_DIR;
					          
				        	} else {
				        	  	m_direction -= ACC_RATE;
				        	  	if(m_direction < -MAX_DIR)
				        	  		m_direction = -MAX_DIR;
				        	  	if(m_direction < 0 && m_direction > -MIN_DIR)
				        	  		m_direction = -MIN_DIR;
				        	}
			       		}
			        	break;
			    }
			}
		}else
			m_lastProgress = 0;
	}
	m_lastTimestamp = frame.timestamp();

}
