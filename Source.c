#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>

//Constants
#define MAX_FIELD_SPACE 12
#define TOTAL_CHARACTERS 20

//Prototypes
void getInput(char response[]);
bool checkValidInput(char response[MAX_FIELD_SPACE], char options[6][MAX_FIELD_SPACE]);
bool hasRecords(struct forceSensitive arr[]);
void printMenu();
void displayRecords(struct forceSensitive arr[]);
void addRecord(struct forceSensitive arr[]);
void deleteRecord(struct forceSensitive arr[]);
void modifyRecord(struct forceSensitive arr[]);
bool hasDuplicates(struct forceSensitive arr[], char *check);


// used to only allow certain inputs for blade color and affiliation members
const char colors[7][MAX_FIELD_SPACE] = {"red", "blue", "green", "yellow", "purple", "white"};
const char affiliations[3][MAX_FIELD_SPACE] = { "jedi", "sith", "other" };

struct forceSensitive {
	//ID is auto generated starting at 1
	unsigned int id;
	char name[MAX_FIELD_SPACE];
	char affiliation[MAX_FIELD_SPACE];
	char rank[MAX_FIELD_SPACE];
	char bladeColor[MAX_FIELD_SPACE];
	bool deleted;
};

// keep this instance of struct global so I dont need to keep passing it as an argument
struct forceSensitive defaultValues = { 0, "Empty", "None", "None", "None", true};

void main() {
	struct forceSensitive forceSensitiveCharacters[TOTAL_CHARACTERS];
	// Initializes the array with all default values
	for (int i = 0; i < TOTAL_CHARACTERS; i++) forceSensitiveCharacters[i] = defaultValues;

	char ans[MAX_FIELD_SPACE];
	printMenu();

	do {
		printf("\n > ");
		getInput(ans);
		// will be commented out while testing to demonstrate input
		//system("cls");

		// begin main logic
		if (!strcmp(ans, "display"))
			displayRecords(forceSensitiveCharacters);
		else if (!strcmp(ans, "add"))
			addRecord(forceSensitiveCharacters);
		else if (!strcmp(ans, "delete"))
			deleteRecord(forceSensitiveCharacters);
		else if (!strcmp(ans, "modify"))
			modifyRecord(forceSensitiveCharacters);
		else if (!strcmp(ans, "quit"))
			break;
		else
			puts("\aThere was an issue with your response, please try again");
		// end main logic

		printMenu();
	} while (true);
}

// mostly used to stop members from overflowing 
void getInput(char response[]) {
	gets(response);
	// converts the input to all lowercase to easily compare strings
	for (int i = 0; i < strlen(response); i++) response[i] = tolower(response[i]);
	// Only takes the first 8 characters of input
	if (strlen(response) >= MAX_FIELD_SPACE) 
		strncpy_s(response, MAX_FIELD_SPACE, response, 2 * sizeof(response));
}

// compares user input to a list of acceptable responses
bool checkValidInput(char response[MAX_FIELD_SPACE], char options[6][MAX_FIELD_SPACE]) {
	for (int i = 0; i < 6; i++) {
		if (!strcmp(response, options[i]))
			return true;
	}
	return false;
}

void printMenu() {
	puts("\nPlease select one of the following options by name:");
	puts(" -Display\n -Add\n -Delete\n -Modify\n -Quit");
}

// checks that at least one record is not deleted
bool hasRecords(struct forceSensitive arr[]) {

	for (int i = 0; i < TOTAL_CHARACTERS; i++) {
		if (!arr[i].deleted) {
			return true;
		}
	}

	return false;
}

void displayRecords(struct forceSensitive arr[]) {
	// Used to make sure there is anything to display before forming the table
	// does not use hasRecords() here so that it can keep count of how many records there are rather than just if any exist
	int count = 0;
	for (int i = 0; i < TOTAL_CHARACTERS; i++) {
		if (!arr[i].deleted) count++;
	}
	if (count == 0) {
		puts("You have nothing to display");
		return;
	}
	

	puts("\n\n  ID |    Name    | Affiliation |    Rank    | Blade Color");
	puts(" ----------------------------------------------------------");

	for (int i = 0; i < TOTAL_CHARACTERS; i++) {
		if (!arr[i].deleted) {
			printf(" %03u | %-10s | %-11s | %-10s | %-11s\n", arr[i].id, arr[i].name, arr[i].affiliation, arr[i].rank, arr[i].bladeColor);
		}
	}

	printf("\n Spaces left: %d\n", TOTAL_CHARACTERS - count);
}

void addRecord(struct forceSensitive arr[]) {
	
	static unsigned int id = 1;
	
	for (int i = 0; i < TOTAL_CHARACTERS; i++) {
		//finds the first avaliable space 
		if (arr[i].deleted) {

			printf("Enter the name: ");
			getInput(arr[i].name);
			if (hasDuplicates(arr, arr[i].name)) {
				puts("They have already been added");
				return;
			}

			printf("\nNext enter their affiliation: ");
			getInput(arr[i].affiliation);
			if (!checkValidInput(arr[i].affiliation, affiliations)) {
				puts("There was an error with your response, the field has been temporarily set to \"None\"");
				strcpy_s(arr[i].affiliation, MAX_FIELD_SPACE, "NONE");
			}

			printf("\nNext enter their rank: ");
			getInput(arr[i].rank);

			printf("\nFinally, enter their blade color: ");
			getInput(arr[i].bladeColor);
			if (!checkValidInput(arr[i].bladeColor, colors)) {
				puts("There was an error with your response, the field has been temporarily set to \"White\"");
				strcpy_s(arr[i].bladeColor, MAX_FIELD_SPACE, "WHITE");
			}

			arr[i].deleted = false;
			arr[i].id = id++;

			return;
		}
	}
	// if the function hasnt returned yet then it did not find a free space
	puts("There are no open spaces left in your list");
}

void deleteRecord(struct forceSensitive arr[]) {
	int selectedRecord;
	char trash[MAX_FIELD_SPACE];

	if (hasRecords(arr)) {
		printf("Which record would you like to delete (Please enter the ID): ");
		scanf_s("%3d", &selectedRecord);
		// needed after each scan....not sure why
		gets(trash);

		for (int i = 0; i < TOTAL_CHARACTERS; i++) {
			if (arr[i].id == selectedRecord && !arr[i].deleted) {
				printf("%s (id: %d) was deleted sucessfully\n", arr[i].name, arr[i].id);
				arr[i].deleted = true;
				return;
			}
		}
		printf("There is no record with that ID");
	}
	else
		puts("You have no records to delete");
}

void modifyRecord(struct forceSensitive arr[]) {
	char ans[MAX_FIELD_SPACE];
	unsigned int idCheck;
	
	// prevents data from being overwritten during the process
	struct forceSensitive temporaryValues;

	if (hasRecords(arr)) {

		puts("Please enter the character's ID");
		printf("\n > ");
		scanf_s("%3d", &idCheck);

		//not sure why this needs to be here but it does
		//needed after every scan
		//uses ans as trash variable to avoid needing to put aside more memory for a new variable
		gets(ans);

		//deffined outside of the loop to keep the value once the loop ends
		int i;
		for (i = 0; i < TOTAL_CHARACTERS; i++) {
			if (arr[i].id == idCheck) {
				break;
			}
		}


		if (!arr[i].deleted) {
			temporaryValues = arr[i];
			puts("Which of the following fields would you like to change:\n -Name\n -Affiliation\n -Rank\n -Blade Color");
			printf("\n> ");
			getInput(ans);

			if (!strcmp(ans, "name")) {
				printf("Please enter the new name: ");
				getInput(arr[i].name);
				if (hasDuplicates(arr, arr[i].name)) {
					puts("That name was already taken");
					strcpy_s(arr[i].name, MAX_FIELD_SPACE, temporaryValues.name);
				}

			}
			else if (!strcmp(ans, "affiliation")) {
				printf("Please enter their new affiliation: ");
				getInput(arr[i].affiliation);
				if (!checkValidInput(arr[i].affiliation, affiliations)) {
					puts("There was an error with your response, the field has been kept the same");
					strcpy_s(arr[i].affiliation, MAX_FIELD_SPACE, temporaryValues.affiliation);
				}
			}
			else if (!strcmp(ans, "rank")) {
				printf("Please enter the new rank: ");
				getInput(arr[i].rank);
			}
			else if (!strcmp(ans, "blade color")) {
				printf("Please enter their new blade color: ");
				getInput(arr[i].bladeColor);
				if (!checkValidInput(arr[i].bladeColor, colors)) {
					puts("There was an error with your response, the field has been kept the same");
					strcpy_s(arr[i].bladeColor, MAX_FIELD_SPACE, temporaryValues.bladeColor);
				}
			}
			else if (!strcmp(ans, "quit"))
				return;
			else
				puts("\aThere was an issue with your response, please try again");
		
		}
		else
			puts("There was an issue finding someone with that ID. Returning to the main menue");
	}
	else
		puts("You have no records to modify");
}

bool hasDuplicates(struct forceSensitive arr[], char *check) {
	int flags = 0;
	for (int i = 0; i < TOTAL_CHARACTERS; i++) {
		if (!strcmp(check, arr[i].name)) 
			flags++;
	}
	// one flag will always be triggered sice that is the current value of the name at the index being created/modified
	// if there are two flags then there is already someone with that name
	return (flags > 1);
}