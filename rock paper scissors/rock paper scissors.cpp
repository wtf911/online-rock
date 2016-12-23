#include "stdafx.h"
#include <algorithm>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <limits>
#include <SFML/Network.hpp>
#include <wininet.h>

#pragma comment(lib, "wininet")

using namespace std;

double player, computer, wins, losses, ties, serverorclient, port;
string name, opponent;

sf::TcpSocket client;

int external()
{
	external:
	HINTERNET hInternet, hFile;
	DWORD rSize;
	char buffer[47];

	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	hFile = InternetOpenUrlA(hInternet, "http://myexternalip.com/raw", NULL, 0, INTERNET_FLAG_RELOAD, 0);
	InternetReadFile(hFile, &buffer, sizeof(buffer), &rSize);
	buffer[rSize] = '\0';

	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);

	if (buffer == "") { 
		goto external;
	}

	cout << "Share this IP: " << buffer << "\n";
	return 0;
}

int onlineplay()
{
start:
	system("CLS");
	cout << "Your choices are: Rock, Paper, Scissors.     *press 4 for the main menu*" << endl;
	cout << "Here are the rules, " << endl << "Rock crushes scissors. " << endl << "Paper covers rock. " << endl << "Scissors cut paper. ";
	cout << endl << endl;
	cout << "Your current game score - " << "Wins:" << wins << " Losses:" << losses << " Ties:" << ties;
	cout << endl << endl;
	cout << "Type 1 for Rock, 2 for Paper, 3 for Scissors: ";
	//player choice

	player = 0;
	while (true) {
		cin >> player;
		if (player == 1 || player == 2 || player == 3) {
			break;
		}
		if (player == 4) return 1;
		else {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			system("CLS");
			cout << "You must enter 1, 2, or 3!";
			Sleep(3000);
			goto start;
		}
	}

	if (serverorclient == 1) {
		//send
		sf::Packet packet;
		packet << player;

		// TCP socket:
		if (client.send(packet) != sf::Socket::Done)
		{
			// error...
		}
		//send end

		//receive
		sf::Packet packetreceive;
		if (client.receive(packetreceive) != sf::Socket::Done)
		{
			// error...
		}
		//receive end

		packetreceive >> computer;
	}
	if (serverorclient == 2) {
		//receive
		sf::Packet packetreceive;
		if (client.receive(packetreceive) != sf::Socket::Done)
		{
			// error...
		}
		//receive end

		packetreceive >> computer;

		//send
		sf::Packet packet;
		packet << player;

		// TCP socket:
		if (client.send(packet) != sf::Socket::Done)
		{
			// error...
		}
		//send end
	}

	//defining player choice
	string playeritem;
	if (player == 1) playeritem = "rock";
	if (player == 2) playeritem = "paper";
	if (player == 3) playeritem = "scissors";
	//defining computer choice
	string computeritem;
	if (computer == 1) computeritem = "rock";
	if (computer == 2) computeritem = "paper";
	if (computer == 3) computeritem = "scissors";

	system("CLS");

	cout << "You chose: " << playeritem << endl << "Your opponent " << opponent << " chose: " << computeritem << endl << endl;

	if (player == computer) {
		ties += 1;
		cout << "You Tied!" << endl << "Try again!";
	}
	if (player == 1 && computer == 2) {
		losses += 1;
		cout << "You lose!" << endl << "Paper covers rock.";
	}
	if (player == 1 && computer == 3) {
		wins += 1;
		cout << "You win!" << endl << "Rock crushes scissors.";
	}
	if (player == 2 && computer == 1) {
		wins += 1;
		cout << "You win!" << endl << "Paper covers rock.";
	}
	if (player == 2 && computer == 3) {
		losses += 1;
		cout << "You lose!" << endl << "Scissors cuts paper.";
	}
	if (player == 3 && computer == 1) {
		losses += 1;
		cout << "You lose!" << endl << "Rock crushes scissors.";
	}
	if (player == 3 && computer == 2) {
		wins += 1;
		cout << "You win!" << endl << "Scissors cuts paper.";
	}
	Sleep(3000);
	system("CLS");
	return 0;
}

int onlinegame()
{
	system("CLS");

menu:
	cout << "You have chosen to play against a friend." << endl << endl << "Press 1 to act as the server.";
	cout << endl << "Press 2 to connect as a client.";
	cout << endl << endl << "Enter your choice: ";

	int menu = 0;
	while (true) {
		cin >> menu;
		if (menu == 1 || menu == 2) {
			break;
		}
		else {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			system("CLS");
			cout << "You must enter 1 or 2!";
			Sleep(3000);
			system("CLS");
			goto menu;
		}
	}
	if (menu == 1) {
		serverorclient = 1;
		system("CLS");

		portchoice:
		cout << "Choose a port: ";

		while (true) {
			cin >> port;
			if (port >= 1 && port <= 65535) {
				system("CLS");
				cout << "Share this port: " << port << endl;
				break;
			}
			else {
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				system("CLS");
				cout << "You must enter a number between 1 and 65535!";
				Sleep(3000);
				system("CLS");
				goto portchoice;
			}
		}

		sf::TcpListener listener;

		// bind the listener to a port
		if (listener.listen(port) != sf::Socket::Done)
		{
			// error...
			system("CLS");
			cout << "There was a problem binding to port: " << port;
			Sleep(3000);
			goto portchoice;
		}

		external();

		// accept a new connection
		cout << "Waiting for a connection..." << endl;
		if (listener.accept(client) != sf::Socket::Done)
		{
			// error...
			cout << "There was a problem connecting to the client.";
			Sleep(3000);
			goto menu;
		}

		//send
		sf::Packet packet;
		packet << name;

		// TCP socket:
		if (client.send(packet) != sf::Socket::Done)
		{
			// error...
		}
		//send end

		//receive
		sf::Packet packetreceive;
		if (client.receive(packetreceive) != sf::Socket::Done)
		{
			// error...
		}
		//receive end

		packetreceive >> opponent;
		cout << endl << "You are now connected to " << opponent;
		Sleep(3000);
		// use "client" to communicate with the connected client,
		// and continue to accept new connections with the listener
		return 2;
	}
	if (menu == 2) {
	client:
		serverorclient = 2;
		system("CLS");
		cout << "Please enter the IP address you would like to connect to: ";
		string ipaddress;
		cin >> ipaddress;

	portclient:
		cout << "Choose a port: ";

		while (true) {
			cin >> port;
			if (port >= 1 && port <= 65535) {
				break;
			}
			else {
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				system("CLS");
				cout << "You must enter a number between 1 and 65535!";
				Sleep(3000);
				system("CLS");
				goto portclient;
			}
		}

		cout << "Trying to connect..." << endl;
		sf::Socket::Status status = client.connect(ipaddress, port);
		if (status != sf::Socket::Done)
		{
			// error...
		clienterror:
			system("CLS");
			cout << "The connection was unsuccessful." << endl << endl;
			cout << "Press 1 to return to the main menu." << endl;
			cout << "Press 2 to try again.";
			cout << endl << endl << "Enter your choice: ";

			int menu = 0;
			while (true) {
				cin >> menu;
				if (menu == 1 || menu == 2) {
					break;
				}
				else {
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					system("CLS");
					cout << "You must enter 1 or 2!";
					Sleep(3000);
					goto clienterror;
				}
			}
			if (menu == 1) {
				return 1;
			}
			if (menu == 2) {
				goto client;
			}
		}

		//receive
		sf::Packet packetreceive;
		if (client.receive(packetreceive) != sf::Socket::Done)
		{
			// error...
		}
		//receive end

		//send
		sf::Packet packet;
		packet << name;

		// TCP socket:
		if (client.send(packet) != sf::Socket::Done)
		{
			// error...
		}
		//send end

		packetreceive >> opponent;
		cout << endl << "You are now connected to " << opponent;
		Sleep(3000);
		return 2;
	}
	return 0;
}

int printgame()
{
start:
	system("CLS");
	cout << "Your choices are: Rock, Paper, Scissors.     *press 4 for the main menu*" << endl;
	cout << "Here are the rules, " << endl << "Rock crushes scissors. " << endl << "Paper covers rock. " << endl << "Scissors cut paper. ";
	cout << endl << endl;
	cout << "Your current game score - " << "Wins:" << wins << " Losses:" << losses << " Ties:" << ties;
	cout << endl << endl;
	cout << "Type 1 for Rock, 2 for Paper, 3 for Scissors: ";
	//player choice

	player = 0;
	while (true) {
		cin >> player;
		if (player == 1 || player == 2 || player == 3) {
			break;
		}
		if (player == 4) return 1;
		else {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			system("CLS");
			cout << "You must enter 1, 2, or 3!";
			Sleep(3000);
			goto start;
		}
	}

	//computer choice
	computer = rand() % 3 + 1;
	//defining player choice
	string playeritem;
	if (player == 1) playeritem = "rock";
	if (player == 2) playeritem = "paper";
	if (player == 3) playeritem = "scissors";
	//defining computer choice
	string computeritem;
	if (computer == 1) computeritem = "rock";
	if (computer == 2) computeritem = "paper";
	if (computer == 3) computeritem = "scissors";

	system("CLS");

	cout << "You chose: " << playeritem << endl << "The computer chose: " << computeritem << endl << endl;

	if (player == computer) {
		ties += 1;
		cout << "You Tied!" << endl << "Try again!";
	}
	if (player == 1 && computer == 2) {
		losses += 1;
		cout << "You lose!" << endl << "Paper covers rock.";
	}
	if (player == 1 && computer == 3) {
		wins += 1;
		cout << "You win!" << endl << "Rock crushes scissors.";
	}
	if (player == 2 && computer == 1) {
		wins += 1;
		cout << "You win!" << endl << "Paper covers rock.";
	}
	if (player == 2 && computer == 3) {
		losses += 1;
		cout << "You lose!" << endl << "Scissors cuts paper.";
	}
	if (player == 3 && computer == 1) {
		losses += 1;
		cout << "You lose!" << endl << "Rock crushes scissors.";
	}
	if (player == 3 && computer == 2) {
		wins += 1;
		cout << "You win!" << endl << "Scissors cuts paper.";
	}
	Sleep(3000);
	system("CLS");
	return 0;
}

int main()
{
	cout << "Welcome to Rock Paper Scissors!" << endl << "Please enter your name: ";
	cin >> name;
	system("CLS");
	cout << "Hello " << name << ". " << endl << "Welcome and good luck!";
	Sleep(3000);
menu:
	system("CLS");
	cout << "*Main Menu*" << endl << endl << "Press 1 to play against the computer.";
	cout << endl << "Press 2 to play against a friend.";
	cout << endl << endl << "Enter your choice: ";

	int menu = 0;
	while (true) {
		cin >> menu;
		if (menu == 1 || menu == 2) {
			break;
		}
		else {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			system("CLS");
			cout << "You must enter 1 or 2!";
			Sleep(3000);
			system("CLS");
			goto menu;
		}
	}

	wins = 0;
	losses = 0;
	ties = 0;

	if (menu == 1) {
		while (0 == printgame());
		goto menu;
	}
	if (menu == 2) {
		int result = onlinegame();
		if (result == 1) goto menu;
		if (result == 2) {
			while (0 == onlineplay());
			goto menu;
		}
	}
	return 0;
}