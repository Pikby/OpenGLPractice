#pragma once
#include <memory>
#include <string>
#include <atomic>
#include <mutex>
#include "../headers/world.h"
#include "../headers/clients.h"
#define MAX_CLIENTS 4
class Server
{
  private:
    static uint ticksPerSecond;
    static std::atomic<bool> serverLogicOn;
    static std::atomic<uint> numberOfClients;
    static std::thread serverCommands,serverLogic;
    static std::mutex clientMutex;
    static std::shared_ptr<Client> clients[MAX_CLIENTS];
    static World* curWorld;
  public:
    static void initServer(World* temp);
    static void initServerCommands();
    static void handleServerLogic();
    static void handleServerCommands();
    static void add(int fd);
    static void remove(int id);
    static void messageAll(std::shared_ptr<Message> msg);
    static void retClients(Client* target);
    static void sendInitAll(Client* target);
    static void parseCommand(std::string command);
    static void say(const std::string &message);
    static void quit(const std::string &message);
    static void teleport(const std::string &message);
};