
#include <iostream>
#include <string>
#include<vector>
#include<fstream>
#include<iomanip>
using namespace std;
string MyFileName = "Clients.txt";
void ShowMainMenue();
enum enChoice{ShowList=1,AddClient=2,DeleteClient=3,UpdateClient=4,FindClient=5,Exit=6};
struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

vector<string> SplitString(string Line, string Seprator = "#//#")
{
	vector<string> vWords;
	short Pos = 0;
	while ((Pos = Line.find(Seprator)) != std::string::npos)
	{
		vWords.push_back(Line.substr(0, Pos));
		Line.erase(0, Pos + Seprator.length());
	}
	if (Line != "")
		vWords.push_back(Line.substr(0, Pos));
	return vWords;
}
stClient ConvertLineToRecord(string Line, string Seprator = "#//#")
{
	stClient Client;
	vector<string> vWords = SplitString(Line, Seprator);
	Client.AccountNumber = vWords[0];
	Client.PinCode = vWords[1];
	Client.Name = vWords[2];
	Client.Phone = vWords[3];
	Client.AccountBalance = stod(vWords[4]);
	return Client;
}
string ConvertRecordToLine(stClient Client, string Sperator = "#//#")
{
	string Line = "";
	Line += Client.AccountNumber + Sperator;
	Line += Client.PinCode + Sperator;
	Line += Client.Name + Sperator;
	Line += Client.Phone + Sperator;
	Line += to_string(Client.AccountBalance);
	return Line;
}

void AddLineToFile(string Line)
{
	fstream MyFile;
	MyFile.open(MyFileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << Line << endl;
		MyFile.close();
	}
}
bool ClientExistsByAccountNumber(string AccountNumber)
{
	vector<stClient> vClients;//I dont know why the teacher use this
	fstream MyFile;
	MyFile.open(MyFileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stClient Client;
		while (getline(MyFile,Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);//I dont know why the teacher use this
		}
		MyFile.close();
	}
	return false;
}
stClient ReadNewClient()
{
	stClient Client;
	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);
	while (ClientExistsByAccountNumber(Client.AccountNumber))
	{
		cout << "Client with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}
		cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter phone? ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;
	return Client;
}
void AddNewClient()
{
	stClient Client = ReadNewClient();
	string Line = ConvertRecordToLine(Client, "#//#");
	AddLineToFile(Line);
}

void AddNewClients()
{
	char AddMoreClients = 'Y';
	while (toupper(AddMoreClients) == 'Y')
	{
		cout << "Adding New Client:\n\n";
		AddNewClient();
		cout << "\n\nClient Added Succesfully, do you want to add more clients? Y/N? ";
		cin >> AddMoreClients;
	}

}


vector<stClient>LoadClientsFromFile()
{
	vector<stClient> vClients;
	fstream MyFile;
	MyFile.open(MyFileName, ios::in);
	if (MyFile.is_open())
	{
		string Line = "";
		while (getline(MyFile, Line))
		{
			stClient TempClient;
			TempClient = ConvertLineToRecord(Line);
			vClients.push_back(TempClient);
		}
		MyFile.close();
	}
	return vClients;
}
void PrintClientRecord(stClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.Phone;
	cout << "| " << left << setw(12) << Client.AccountBalance;;

}
void PrintClientList()
{
	vector<stClient> vClients = LoadClientsFromFile();

	cout << "\t\t\t\t\t\t" << "Client List (" << vClients.size() << ") Clients(s).\n\n";
	cout << "____________________________________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code:";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n\n____________________________________________________________________________________________________________________________\n\n";
	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
	for (stClient Client : vClients)
	{
		PrintClientRecord(Client);
		cout << endl;
	}
	cout << "\n\n____________________________________________________________________________________________________________________________\n\n";
}


string ReadAccountNumber()
{
	string AccountNumber = "";
	cout << "Please enter Account Number? ";
	cin >> AccountNumber;
	return AccountNumber;
}
void PrintClientCard(stClient Client)
{
	cout << "Account Number: " << Client.AccountNumber << "\n";
	cout << "Pin Code      : " << Client.PinCode << "\n";
	cout << "Name          : " << Client.Name << "\n";
	cout << "Phone         : " << Client.Phone << "\n";
	cout << "Account Balance: " << Client.AccountBalance << "\n";
}

bool FindClientByAccountNumber(stClient& Client, vector<stClient>& vClients, string AccountNumber)
{

	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}


void SaveClientsToFile(vector<stClient>& vClients)
{
	fstream MyFile;
	MyFile.open(MyFileName, ios::out);
	if (MyFile.is_open())
	{
		for (stClient C : vClients)
		{
			if (!C.MarkForDelete)
				MyFile << ConvertRecordToLine(C) << endl;
		}
		MyFile.close();
	}
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{

	char DeleteThisClient = 'N';
	stClient Client;
	if (FindClientByAccountNumber(Client, vClients, AccountNumber))
	{
		PrintClientCard(Client);
		cout << "Are you sure you want to delete this client? Y/N ?";
		cin >> DeleteThisClient;
		if (toupper(DeleteThisClient) == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsToFile(vClients);
			vClients = LoadClientsFromFile();
			cout << "\n\nClient Delete Succssfully.\n";
			return true;
		}
	}
	else
	{
		cout << "Client With Account Number (" << AccountNumber << ") Not Found!\n";
		return false;
	}

}
stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;
	Client.AccountNumber = AccountNumber;
	cout << "Enter PinCode? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter phone? ";
	getline(cin, Client.Phone);
	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;
	return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	stClient Client;
	char UpdateThisClient = 'Y';
	if (FindClientByAccountNumber(Client, vClients, AccountNumber))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want to update this client? Y/N ? ";
		cin >> UpdateThisClient;
		if (toupper(UpdateThisClient) == 'Y')
		{

			for (stClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsToFile(vClients);
			cout << "\n\nClient Updated Sussfully.";
			return true;
		}
	}
	else
	{
		cout << "Client With Account Number (" << AccountNumber << ") Not Found!\n";
		return false;
	}
}

enChoice ReadMainMenueChoice()
{
	short Choice = 0;
	cout << "Choose what do you want to do ? [1 to 6]? ";
	cin >> Choice;
	return (enChoice)Choice;
}
void ShowAddNewClientsScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tAdd New Clients Screen\n";
	cout << "------------------------------------\n\n";
	AddNewClient();
}
void ShowDeleteClientScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tDelete Clients Screen\n";
	cout << "------------------------------------\n\n";
	string AccountNumber = ReadAccountNumber();
	vector <stClient> vClients = LoadClientsFromFile();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}
void ShowUpdateClientScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tUpdate Client Info Screen\n";
	cout << "------------------------------------\n\n";
	string AccountNumber = ReadAccountNumber();
	vector<stClient> vClients = LoadClientsFromFile();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}
void ShowFindClientScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tFind Client Screen\n";
	cout << "------------------------------------\n\n";
	string AccountNumber = ReadAccountNumber();
	stClient Client;
	vector<stClient> vClients = LoadClientsFromFile();
	if (FindClientByAccountNumber(Client, vClients,AccountNumber))
	{
		cout << "\nThe following are the client details\n\n";
		PrintClientCard(Client);
	}
	else
		cout << "Client With Account Number (" << AccountNumber << ") Not Found!\n";
}
void ShowProgramEndScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tProgram Ends :-)\n";
	cout << "------------------------------------\n\n";
}
void GoBackToMainMenue()
{
	cout << "\nPress any key to go back to Main Menu...";
	system("pause>0");
	ShowMainMenue();
}
void PerformMainMenueOption(enChoice Choice)
{
	switch (Choice)
	{
		case enChoice::ShowList:
		{
			system("cls");
			PrintClientList();
			GoBackToMainMenue();
			break;
		}
		case enChoice::AddClient:
		{
			system("cls");
			ShowAddNewClientsScreen();
			GoBackToMainMenue();
			break;
		}
		case enChoice::DeleteClient:
		{
			system("cls");
			ShowDeleteClientScreen();
			GoBackToMainMenue();
			break;
		}
		case enChoice::UpdateClient:
		{
			system("cls");
			ShowUpdateClientScreen();
			GoBackToMainMenue();
			break;

		}
		case enChoice::FindClient:
		{
			system("cls");
			ShowFindClientScreen();
			GoBackToMainMenue();
			break;
		}
		case enChoice::Exit:
		{
			system("cls");
			ShowProgramEndScreen();
			break;
		}
	}
	
}
void ShowMainMenue()
{
	system("cls");
	cout << "===============================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===============================================\n";
	cout << "\t" << "[1] Show Client List.\n";
	cout << "\t" << "[2] Add New Client.\n";
	cout << "\t" << "[3] Delete Client.\n";
	cout << "\t" << "[4] Update Client.\n";
	cout << "\t" << "[5] Find Client.\n";
	cout << "\t" << "[6] Exit.\n";
	cout << "===============================================\n";
	PerformMainMenueOption(ReadMainMenueChoice());
}
int main()
{
	ShowMainMenue();
	system("pause>0");
	return 0;
}
