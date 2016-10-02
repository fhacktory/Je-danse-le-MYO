
#include <iostream>
#include <cstring>
#include "Leap.h"
#include <unistd.h>
#include "const.hpp"
#include "volant.hpp"
#include <cstdio> //printf
#include <cstring>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
using namespace Leap;

Volant volant;
        int sock;
class LeapListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);

  private:
};

int create_socket()
{
    struct sockaddr_in server;
    
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("192.168.101.103");
    server.sin_family = AF_INET;
    server.sin_port = htons( 5001 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");

    return 0;
}


void LeapListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void LeapListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
}

void LeapListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void LeapListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}



void LeapListener::onFrame(const Controller& controller) {
  static int64_t lastTimestamp = 0;
  char message[1000];

  const Frame frame = controller.frame();
  volant.handleFrame(controller, frame);
  
  sprintf(message, "%f\n", volant.getDirection());
  std::cout << message;
  if(frame.timestamp() - lastTimestamp > 1000)
  {
    
    if(send(sock , message , strlen(message) , 0)<0)
      std::cout << "erreur envoi\n"; //on envoie
  //  else std::cout << "envoi ok\n";
    lastTimestamp = frame.timestamp();
  } 
}



void LeapListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void LeapListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void LeapListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void LeapListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void LeapListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char** argv) {
  // Create a sample listener and controller
  create_socket();

  LeapListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

  // Keep this process running until Enter is pressed
  while(1){
  //  std::cout <<"loop\n";
  }


  /*std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();*/

  //close(sock);
  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}
