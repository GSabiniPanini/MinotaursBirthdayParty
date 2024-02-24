// MinotaursBirthdayParty.cpp : This file contains the 'main' function. Program execution begins and ends there.
// The Minotaur invited N guests to his birthday party. When the guests arrived, he made the following announcement
// The guests may enter his labyrinth, one at a time and only when he invites them to do so. At the end of the labyrinth
// the miniotaur placed a birthday cupcake on a plate. When a guest finds a way out of the labyrinth, they may eat the cupcake
// or leave it. If the cupcake is eaten by the previous guest, the next guest will find the cupcake empty and may request another
// cupcake by asking the servants. When the servants bring a new cupcake the guest may decide to eat it or leave it. 
//
// The minotaur's only request is for each guest to not talk to other guests about their visit to the labyrinth after they leave.
// The guests are allowed to talk about a strategy prior to the game. The minotaur may pick the same guests multiple times and
// ask them to enter the labyrinth again. Before the party is over, the minotaur wants to know if all of his guests have had the 
// chance to enter his labyrinth. To do so, the guests must announce that they have all visited the labyrinth at least once
//
// The guests must come up with a strategy to let the minotaur know that every guest entered the Minotaur's labyrinth. It is known 
// that there is a cupcake at the end of the labyrinth at the beginning of the party.
//
// Create a program to simulate the winning strategy where each guest is represented by one running thread. In your program, the
// minotaur will be represented by the main thread. The main thread will create N guest threads and then wait for them to finish.
// The main thread will then print out a message that all guests have visited the labyrinth at least once. The guest threads will
// simulate the guests entering the labyrinth and eating or leaving the cupcake. The guest threads will also print out a message
#include <iostream>
#include <thread>
#include <mutex>
#include <array>
#include <atomic>

#define N 5

using namespace std;
mutex mtx;

// Minotaur's Labyrinth functions
atomic<bool> cupcakeAvailable(true);
atomic<bool> showroomAvailable(true);
// an array signifying how many times a cupcake has been eaten in real time application,
// one of the guests will watch how many times the servant has gone to replace a cupcake.
// if the servant has gone to replace a cupcake N-1 times, then all guests have visited the labyrinth
array<bool, 5> guestsVisited = { false, false, false, false, false };
// an array for overview of which guests have visited the showroom
array<bool, 5> showroomVisited = { false, false, false, false, false };

void eatCupcake(int guestID)
{
	cout << "Guest " << guestID << " has eaten the cupcake" << endl;
	cupcakeAvailable = false;
}

void requestCupcake(int guestID)
{
	cout << "Guest " << guestID << " has requested a new cupcake" << endl;
	cupcakeAvailable = true;
}

void leaveCupcake(int guestID)
{
	cout << "Guest " << guestID << " has left the cupcake" << endl;
}

bool checkGuestsVisited()
{
	for (int i = 0; i < N; i++)
	{
		if (!guestsVisited[i])
			return false;
	}
	return true;
}

void Labyrinth(int guestID)
{
	// no locks needed here, as the minotaur is the only one who can invite the guests
	cout << "Guest " << guestID << " has entered the labyrinth" << endl;

	// simulate exploring the labyrinth
	this_thread::sleep_for(chrono::seconds(1 + rand() % 3));

	// Only eat a cupcake once
	if (!guestsVisited[guestID])
	{
		// if the cupcake is available, eat it, else request a new one
		if (cupcakeAvailable)
		{
			mtx.lock();
			eatCupcake(guestID);
			mtx.unlock();
		}
		else
		{
			mtx.lock();
			// request THEN eat it :)
			requestCupcake(guestID);
			eatCupcake(guestID);
			mtx.unlock();
		}
	}
	else
	{
		mtx.lock();
		if (cupcakeAvailable)
			leaveCupcake(guestID);
		else
		{
			requestCupcake(guestID);
			leaveCupcake(guestID);
		}
		mtx.unlock();
	}

	// set the corresponding index to true
	guestsVisited[guestID] = true;
}

void printVisited(bool flag)
{
	// should always be true
	if (flag)
		cout << "All guests have visited the labyrinth at least once" << endl;
	else
		cout << "What did you do, not every guest has visited the labyrinth?" << endl;
}

// Crystal Vase functions

void printShowroom()
{
	for (int i = 0; i < N; i++)
	{
		if (showroomVisited[i])
			cout << "Guest " << i << " has visited the showroom" << endl;
	}
}

void Showroom(int guestID)
{
	// a time randomizer to somewhat emulate making the decision of seeing the showroom
	this_thread::sleep_for(chrono::seconds(1 + rand() % 5));

	mtx.lock();
	if (showroomAvailable)
	{
		cout << "Guest " << guestID << " has entered the showroom" << endl;
		showroomAvailable = false;
		mtx.unlock();

		// simulate looking at the vase
		this_thread::sleep_for(chrono::seconds(1 + rand() % 3));

		mtx.lock();
		showroomAvailable = true;
		cout << "Guest " << guestID << " has left the showroom" << endl;
	}
	else
	{
		cout << "Guest " << guestID << " has found the room busy..." << endl;
	}
	mtx.unlock();
}

int main()
{
	thread labyrinthGuests[N];
	bool allVisited = true;
	int guestID, cupcakesReplaced = 0;
	
	// Minotaur's Birthday Party
	do
	{
		// randomly pick a guest to enter the labyrinth and waits for them to finish
		guestID = rand() % N;
		labyrinthGuests[guestID] = thread(Labyrinth, guestID);
		labyrinthGuests[guestID].join();
		
		// check if all guests have visited the labyrinth
		allVisited = checkGuestsVisited();
	} while (!allVisited);
	
	// exits after all guests have visited the labyrinth
	printVisited(allVisited);

	// Minotaur's Crystal Vase
	thread showroomGuests[N];
	
	for (int i = 0; i < N; i++)
	{
		showroomGuests[i] = thread(Showroom, i);
	}

	for (int i = 0; i < N; i++)
	{
		showroomGuests[i].join();
	}

	printShowroom();
	
	return 0;
}

