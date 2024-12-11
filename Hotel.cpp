// Nikita Hakala
// C++-programming basics, trying to aim for 5p

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
using namespace std;

// Setting up random number generators
random_device seed;
mt19937 gen(seed());
uniform_int_distribution<>dist_discount(1, 3); // For discount
uniform_int_distribution<>dist_number(10000, 99999); // For reservation number
uniform_int_distribution<>dist_rooms(40, 300); // For number of rooms

// Creating a struct for booker information
struct Hotel
{
	string name;
	int roomNumber;
	int roomSize;
	int price;
	int reservation;
	bool isOccupied;
};

vector <Hotel> hotelRooms; // Vector to save all hotel information

// Function prototypes
void mainMenu(int rooms);
bool newBooking(int rooms);
void infoCheck(int rooms);
void searchBooking();
bool yesOrNo(string prompt);


// Contains first steps of setups for hotel
int main()
{
	bool valid;
	int oldOrNew;
	char really;
	do {
		cout << "Continue with previous hotel (1) or start a new one (2)? ";
		cin >> oldOrNew;

		// Checking for invalid inputs
		if (cin.fail() || oldOrNew < 1 || oldOrNew > 2)
		{
			cout << "Invalid input. Try again.\n";
			valid = false;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		// Confirm input
		cout << "Are you sure (y/n)? ";
		cin >> really;

		if (really == 'y' || really == 'Y') {
			valid = true;
		}
		else if (really == 'n' || really == 'N') {
			cout << "You entered no. Try again.\n";
			valid = false;
		}
		else {
			cout << "Invalid input. Try again.\n";
			valid = false;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	} while (!valid);

	// Set up new hotel
	int rooms;
	if (oldOrNew == 2) {
		do {
			rooms = dist_rooms(gen);
		} while (rooms % 2 == 1); // Check that number of rooms is dividend by two
		fstream HotelInfo;
		HotelInfo.open("HotelInfo.txt", ios::out);
		HotelInfo << rooms << endl; // Save number of rooms to first line


		// Set up rooms
		for (int i = 1; i <= rooms; ++i) {
			Hotel room;
			room.roomNumber = i;
			room.name = ""; // No name yet
			room.roomSize = (i <= rooms / 2) ? 1 : 2; // Conditional operator, first half 1 person rooms, second half 2 person rooms
			room.price = 0; // No price yet
			room.reservation = 0; // No booking number yet
			room.isOccupied = false;

			hotelRooms.push_back(room); // Add room to vector

			// Add info to file
			HotelInfo << room.roomNumber << "\n" << room.name << "\n" << room.roomSize << "\n"
				<< room.price << "\n" << room.reservation << "\n" << room.isOccupied << "\n";
		}
		HotelInfo.close();
		cout << "\nNew hotel has been set up with " << rooms << " rooms total, with half (" << rooms / 2 << ") of them being 1 person rooms and the other half 2 person rooms.\n\n";
		mainMenu(rooms);
	}

	// Read an old file
	else if (oldOrNew == 1) {
		fstream HotelInfo;
		HotelInfo.open("HotelInfo.txt", ios::in);
		HotelInfo >> rooms; // Save number of rooms from file
		hotelRooms.clear(); // Clear vector just in case so no funny business
		for (int i = 0; i < rooms; ++i)
		{
			Hotel room;
			HotelInfo >> room.roomNumber;
			HotelInfo.ignore();
			getline(HotelInfo, room.name);
			HotelInfo >> room.roomSize;
			HotelInfo >> room.price;
			HotelInfo >> room.reservation;
			HotelInfo >> room.isOccupied;
			HotelInfo.ignore();

			hotelRooms.push_back(room);
		}
		HotelInfo.close();

		mainMenu(rooms);
	}
	return 0;
}


// Function to check if answer is yes or no
bool yesOrNo(string prompt) {
	char response;
	while (true) {
		cout << prompt;
		cin >> response;
		if (response == 'y' || response == 'Y') {
			return true;
		}
		else if (response == 'n' || response == 'N') {
			cout << "You answered no. Try again.\n";
			return false;
		}
		else {
			cout << "Invalid input. Try again.\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}


// Function for main menu
void mainMenu(int rooms)
{
	int choice;
	bool valid = false;

	// Main menu appearance
	while (true) {
		cout << "\nMAIN MENU\n\n"
			<< "[ 1 ] New booking\n"
			<< "[ 2 ] Check-out/Search booking\n"
			<< "[ 3 ] See info on all rooms\n"
			<< "[ 4 ] Close program\n\n";

		do {
			cout << "Choose an action: ";
			cin >> choice;

			// Validating choice.
			if (cin.fail() || choice < 1 || choice>4) {
				cout << "Invalid input. Try again.\n";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			else {
				valid = true;
			}
		} while (!valid);

		//Process valid choice
		switch (choice) {

		case 1:
			if (!newBooking(rooms)) {
				continue;
			}
			break;

		case 2:
			searchBooking();
			break;

		case 3:
			infoCheck(rooms);
			break;

		case 4:
			cout << "Closing program. Good bye!\n\n";
			return;
		}
	}
}


// Program for a new booking
bool newBooking(int rooms)
{
	// Ask booker name
	Hotel booker;
	char isNameCorrect;
	do {
		cout << "Name of booker: ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, booker.name);

		// Confirm input
	} while (!yesOrNo("Confirm name (y/n)? "));

	// 1 or 2 person room
	bool valid;
	do {
		cout << "1 or 2 person room? ";
		cin >> booker.roomSize;

		// Checking for invalid inputs
		if (cin.fail() || booker.roomSize < 1 || booker.roomSize > 2) {
			cout << "Invalid input. Try again.\n";
			valid = false;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		// Confirm input
		valid = yesOrNo("Confirm room type (y/n)? ");
	} while (!valid);

	// Check if room type is available
	bool roomAvailable = false;
	cout << "Checking availability...\n";

	// Loop through the rooms and check availability
	for (int i = 0; i < hotelRooms.size(); ++i) {
		if (hotelRooms[i].roomSize == booker.roomSize && !hotelRooms[i].isOccupied) {
			roomAvailable = true;
			booker.roomNumber = hotelRooms[i].roomNumber; // Store available room number
			cout << "Room number " << hotelRooms[i].roomNumber << " for this room type is available.\n";
			break;
		}
	}

	if (!roomAvailable) {
		cout << "No available rooms by that type available. Going back to main menu.\n\n";
		return false;
		mainMenu;
	}

	// How many nights
	int nights;
	do {
		cout << "How many nights (max. 14)? ";
		cin >> nights;

		// Checking for invalid inputs
		if (cin.fail() || nights < 1 || nights > 14) {
			cout << "Invalid input. Try again.\n";
			valid = false;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		// Confirm input
		valid = yesOrNo("Confirm how many nights (y/n)? ");
	} while (!valid);

	// Sets room price based on room size
	int price;
	if (booker.roomSize == 1) {
		price = 100;
	}
	else {
		price = 150;
	}

	// Randomizing discount
	int discount = 0;
	int random = dist_discount(gen); // Gives random number

	// Gives out a discount based on the random number
	if (random == 1) {
		discount = 10;
	}
	else if (random == 2) {
		discount = 20;
	}

	// Calculate price of stay
	booker.price = (nights * price) * ((100 - discount) / 100.0);

	// Print out booking information
	cout << "\nName of booker: " << booker.name << "\n"
		<< "1 or 2 person room: " << booker.roomSize << "\n"
		<< "How many nights stay: " << nights << "\n"
		<< "Discount: " << discount << " %\n"
		<< "Total price of stay: " << booker.price << " euros.\n\n";

	// Ask for confirmation
	if (yesOrNo("Confirm booking (y/n)? ")) {
		// Randomize booking number
		booker.reservation = dist_number(gen);
		cout << "\nBooking has been saved.\nBooking number: " << booker.reservation << "\nRoom number: " << booker.roomNumber << "\n\n";

		// Save booking info to file
		for (auto& room : hotelRooms) {
			if (room.roomNumber == booker.roomNumber) {
				room.name = booker.name;
				room.roomNumber = booker.roomNumber;
				room.roomSize = booker.roomSize;
				room.price = booker.price;
				room.reservation = booker.reservation;
				room.isOccupied = true;
				break;
			}
		}

		fstream HotelInfo;
		HotelInfo.open("HotelInfo.txt", ios::out | ios::trunc); // Clear file to overwrite old data
		HotelInfo << rooms << endl; // First line contains total number of rooms
		for (int i = 0; i < hotelRooms.size(); ++i) {
			HotelInfo << hotelRooms[i].roomNumber << "\n"
				<< hotelRooms[i].name << "\n"
				<< hotelRooms[i].roomSize << "\n"
				<< hotelRooms[i].price << "\n"
				<< hotelRooms[i].reservation << "\n"
				<< hotelRooms[i].isOccupied << "\n";
		}

		HotelInfo.close();
		return true;  // Booking is confirmed, return true
	}
	else {
		cout << "Booking not confirmed. Returning no main menu.\n";
		return false;
	}
}


// Function for showing all hotel info
void infoCheck(int rooms) {
	int counterSingle = 0; // Counters for both room types
	int counterDouble = 0;
	for (int i = 0; i < rooms; ++i) {
		cout << "\nRoom number: " << hotelRooms[i].roomNumber << "\n"
			<< "Room type: " << hotelRooms[i].roomSize << " person\n";
		if (hotelRooms[i].isOccupied == 0) {
			if (hotelRooms[i].roomSize == 1) {
				cout << "Price per night is 100 euros.\n";
				counterSingle += 1;
			}
			if (hotelRooms[i].roomSize == 2) {
				cout << "Price per night is 150 euros.\n";
				counterDouble += 1;
			}
			cout << "Room available.\n";
		}
		else {
			cout << "Booker name: " << hotelRooms[i].name << "\n";
			cout << "Booking number: " << hotelRooms[i].reservation << "\n";
			cout << "Room not available.\n";
		}
	}
	cout << "\nTotal number of rooms: " << rooms << "\n";
	cout << "Number of available 1 person rooms: " << counterSingle << "\n";
	cout << "Number of available 2 person rooms: " << counterDouble << "\n\n";
}

// Function for searching a booking and/or checkout
void searchBooking()
{
	int choice;
	bool valid = false;
	bool found = false;

	// Menu
	cout << "\n\nSearch booking by:\n"
		<< "[ 1 ] Name\n"
		<< "[ 2 ] Booking number\n\n"
		<< "[ 3 ] Return to main menu\n";

	while (!valid) {
		cout << "Choose an action: ";
		cin >> choice;

		// Invalid input check
		if (cin.fail() || choice < 1 || choice > 3) {
			cout << "Invalid input. Enter a valid number (1, 2, or 3).\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		valid = true;
		string searchName;
		int searchNumber;

		// Search by name
		if (choice == 1) {
			cout << "Enter name to search: ";
			cin.ignore(); // Just in case again
			getline(cin, searchName);

			for (int i = 0; i < hotelRooms.size(); ++i) {
				// Name is valid
				if (hotelRooms[i].name == searchName) {
					found = true;
					cout << "\nBooking found. Booking information:\n"
						<< "Room number: " << hotelRooms[i].roomNumber << "\n"
						<< "Name: " << hotelRooms[i].name << "\n"
						<< "Room size: " << hotelRooms[i].roomSize << " person\n"
						<< "Booking price: " << hotelRooms[i].price << " euros\n"
						<< "Booking number: " << hotelRooms[i].reservation << "\n\n";

					bool confirmvalid = false;
					char confirm;
					// Confirm input
					do {
						cout << "Do you want to checkout (y/n)? ";
						cin >> confirm;

						if (confirm == 'y' || confirm == 'Y') {
							confirmvalid = true;
							hotelRooms[i].name = "";
							hotelRooms[i].price = 0;
							hotelRooms[i].reservation = 0;
							hotelRooms[i].isOccupied = false;

							fstream HotelInfo;
							HotelInfo.open("HotelInfo.txt", ios::out | ios::trunc);
							HotelInfo << hotelRooms.size() << endl;
							for (size_t i = 0; i < hotelRooms.size(); ++i) {
								HotelInfo << hotelRooms[i].name << "\n"
									<< hotelRooms[i].roomSize << "\n"
									<< hotelRooms[i].price << "\n"
									<< hotelRooms[i].reservation << "\n"
									<< hotelRooms[i].isOccupied << "\n";
							}
							HotelInfo.close();
							cout << "Checkout complete. Returning to main menu.\n\n";
						}
						else if (confirm == 'n' || confirm == 'N') {
							cout << "Returning to main menu.\n\n";
							confirmvalid = true;
							return;
						}
						else {
							cout << "Invalid input. Try again.\n";
							confirmvalid = false;
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
						}
					} while (!confirmvalid);
				}
			}
			// Name not found
			if (!found) {
				cout << "\nNo booking found with the name " << searchName << ". Going back to main menu.\n";
			}
		}

		// Search with number
		else if (choice == 2) {
			cout << "Enter booking number to search: ";

			// Invalid input check
			while (true) {
				cin >> searchNumber;

				if (cin.fail()) {
					cout << "Invalid input. Try again: ";
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				else {
					break;
				}
			}

			for (int i = 0; i < hotelRooms.size(); ++i) {
				// Number is valid
				if (hotelRooms[i].reservation == searchNumber) {
					found = true;
					cout << "\nBooking found. Booking information:\n"
						<< "Room number: " << hotelRooms[i].roomNumber << "\n"
						<< "Name: " << hotelRooms[i].name << "\n"
						<< "Room size: " << hotelRooms[i].roomSize << " person\n"
						<< "Booking price: " << hotelRooms[i].price << " euros\n"
						<< "Booking number: " << hotelRooms[i].reservation << "\n\n";

					bool confirmvalid = false;
					char confirm;
					// Confirm input
					do {
						cout << "Do you want to checkout (y/n)? ";
						cin >> confirm;

						if (confirm == 'y' || confirm == 'Y') {
							confirmvalid = true;
							hotelRooms[i].name = "";
							hotelRooms[i].price = 0;
							hotelRooms[i].reservation = 0;
							hotelRooms[i].isOccupied = false;

							fstream HotelInfo;
							HotelInfo.open("HotelInfo.txt", ios::out | ios::trunc);
							HotelInfo << hotelRooms.size() << endl;
							for (size_t i = 0; i < hotelRooms.size(); ++i) {
								HotelInfo << hotelRooms[i].name << "\n"
									<< hotelRooms[i].roomSize << "\n"
									<< hotelRooms[i].price << "\n"
									<< hotelRooms[i].reservation << "\n"
									<< hotelRooms[i].isOccupied << "\n";
							}
							HotelInfo.close();
							cout << "Checkout complete. Returning to main menu.\n\n";
						}
						else if (confirm == 'n' || confirm == 'N') {
							cout << "Returning to main menu.\n\n";
							confirmvalid = true;
							return;
						}
						else {
							cout << "Invalid input. Try again.\n";
							confirmvalid = false;
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
						}
					} while (!confirmvalid);
				}
			}
			// Number not found
			if (!found) {
				cout << "\nNo booking found with the number " << searchNumber << ". Going back to main menu.\n";
			}
		}
		else if (choice == 3) {
			cout << "Returning to main menu...\n";
			return;
		}
	}
}