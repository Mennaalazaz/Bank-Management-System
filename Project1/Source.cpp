#include <iostream>
#include <fstream>
#include <vector>
#include<iomanip>
#include<string>
#include <algorithm>
using namespace std;

enum enMainMenuOptions{ eShowClientList=1,eAddClient=2 ,eDeleteClient=3 , eUpdataClient=4 , eFindClient=5 ,eTransacrion=6, eExit=7 };
enum eTransacrionsOptions {eDeposit = 1, eWithdraw = 2, eTotalBalances=3, eMainMenu=4 };
struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkDelete = false;
};
const string FileName = "BankClients.txt";

void RunProgram(vector <sClient>  & vClients);

string ConvertRecordToLine(sClient c, string seperator="#//#") {
    string Line = "";

        Line += c.AccountNumber + seperator;
        Line += c.PinCode + seperator;
        Line += c.Name + seperator;
        Line += c.Phone + seperator;
        Line += to_string(c.AccountBalance) ;
    return Line;
}

void LoadDataToFile(string FileName,vector <sClient> vClients) {
    fstream MyFile;
    string line;
    MyFile.open(FileName, ios::out );

    if (MyFile.is_open()) {
        for (const sClient& c : vClients) {
            MyFile << ConvertRecordToLine(c) << endl;
        }
        MyFile.close();
    }
}

vector<string> SplitString(string S1, string Delim) {
    vector<string> vString;
    short pos = 0;
    string sWord;
    while ((pos = S1.find(Delim)) != std::string::npos) {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.  
    }
    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#") { // change vector to structure

    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double

    return Client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName) { //store client data to vector of structure
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);  //read Mode
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line)) {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

enMainMenuOptions ShowMenuScreen() {
    cout << "================================================\n";
    cout << "             Main Menu Screen                   \n";
    cout << "================================================\n";
    cout << "            [1] Show Client List                \n";
    cout << "            [2] Add New Client                   \n";
    cout << "            [3] Delete  Client                   \n";
    cout << "            [4] Update  Client                   \n";
    cout << "            [5] Find Client                      \n";
    cout << "            [6] Transactions                      \n";
    cout << "            [7] Exit                             \n";
    cout << "=================================================\n";

    cout << "What do you want to do ? [1 to 7]?";
    int Answer;
    cin >> Answer;
    return (enMainMenuOptions)Answer;
}

vector <sClient> AddClient(vector <sClient>& vClients) {
    sClient Client;
    bool accountExists;

    do {
        accountExists = false;
        cout << "Enter Account Number (10 digits): ";
        getline(cin >> ws, Client.AccountNumber);

        // Validate if the account number is exactly 10 digits
        while (Client.AccountNumber.length() != 10 || !all_of(Client.AccountNumber.begin(), Client.AccountNumber.end(), ::isdigit)) {
            cout << "Invalid Account Number! It should be exactly 10 digits. Enter again: ";
            getline(cin, Client.AccountNumber);
        }

        for (const sClient& c : vClients) {
            if (c.AccountNumber == Client.AccountNumber) {
                accountExists = true;
                cout << "\nClient with [" << c.AccountNumber << "] already exists. Enter another account number: ";
                break;
            }
        }
    } while (accountExists);

    // Validate if the Pin Code is exactly 4 digits
    cout << "Enter Pin Code (4 digits): ";
    getline(cin, Client.PinCode);
    while (Client.PinCode.length() != 4 || !all_of(Client.PinCode.begin(), Client.PinCode.end(), ::isdigit)) {
        cout << "Invalid Pin Code! It should be exactly 4 digits. Enter again: ";
        getline(cin, Client.PinCode);
    }

    // Validate if the Phone Number is exactly 11 digits
    cout << "Enter Phone Number (11 digits): ";
    getline(cin, Client.Phone);
    while (Client.Phone.length() != 11 || !all_of(Client.Phone.begin(), Client.Phone.end(), ::isdigit)) {
        cout << "Invalid Phone Number! It should be exactly 11 digits. Enter again: ";
        getline(cin, Client.Phone);
    }

    // Validate Name
    cout << "Enter Name: ";
    getline(cin, Client.Name);
    while (!all_of(Client.Name.begin(), Client.Name.end(), [](char c) { return isalpha(c) || isspace(c); })) {
        cout << "Invalid Name! It should only contain letters and spaces. Enter again: ";
        getline(cin, Client.Name);
    }

    // Validate if the Account Number is positive
    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;
    while (cin.fail() || Client.AccountBalance < 0) {
        cout << "Invalid Balance! Enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> Client.AccountBalance;
    }

    vClients.push_back(Client);

    cout << "Do you want to add more Clients ? Y/N" << endl;
    char choice;
    cin >> choice;
    if (tolower(choice) == 'y') { AddClient(vClients); }


    return vClients;
}

void ShowAllClients(vector <sClient> vClients) {

    for (sClient c : vClients) {
        cout << "| " << setw(15) << left << c.AccountNumber << "| " << setw(12) << left << c.PinCode << "| " << setw(32) << left << c.Name << "| " << setw(16) << left << c.Phone << "| " << setw(15) << left << c.AccountBalance << " \n";

    }
    cout << endl;
    cout << "-------------------------------------------------------------------------------------------\n";
}

void ShowClientsList(vector <sClient> vClients) {
    cout << "                  Client List (" << vClients.size() << ") Client(s).                          \n";
    cout << "-----------------------------------------------------------------------------------------------\n";
    cout << "| " << setw(15) << left << " Account Number" << "| " << setw(12) << left << " Pin Code" << "| " << setw(32) << left << " Client Name " << "| " << setw(16) << left << " Phone Number " << "| " << setw(15) << left << " Balance \n";
    cout << "--------------------------------------------------------------------------------------------\n";
    ShowAllClients(vClients);
}

vector <sClient> saveClients(vector <sClient> &vClients) {
    vector <sClient> vNewClients;
    for (sClient &c : vClients) {
        if (c.MarkDelete == false) {
            vNewClients.push_back(c);
        }
    }
    return vNewClients;
}

void PrintClientCard(sClient Client) {
    cout << "The Following are Client details:" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Account NUmber: " << Client.AccountNumber << endl;
    cout << "Pin Code: " << Client.PinCode << endl;
    cout << "Name: " << Client.Name << endl;
    cout << "phone: " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
    cout << "-------------------------------------" << endl;
}

vector <sClient> DeleteClient(vector <sClient>& vClients) {
    cout << "------------------------------\n";
    cout << "     Delete client Screen     \n";
    cout << "------------------------------\n";

    cout << "\n\n Please enter account Number?\n";
    string AccountNumber;
    cin >> AccountNumber;
    vector <sClient> vNewClients;
    for (sClient & c : vClients) {
        if (AccountNumber == c.AccountNumber) {
            PrintClientCard(c);
            cout << "Are you sure you want to delete this client? [y][n]?";
            char choice;
            cin >> choice;
            if (tolower(choice) == tolower('Y')) {
                c.MarkDelete = true;
                vNewClients = saveClients(vClients);
                cout << "Client Deleted Successfully." << endl;
            }
            return vNewClients;
        }     
    }
    cout << "\nClient with [" << AccountNumber << "] does not exist \n";
    return vClients;
}

vector <sClient> UpdateClientInfo( vector <sClient>& vClients) {
    cout << "------------------------------\n";
    cout << "     Update client Screen     \n";
    cout << "------------------------------\n";
    cout << "\n\n Please enter account Number?\n";
    string AccountNumber;
    cin >> AccountNumber;
    for (sClient& c : vClients) {
        if (AccountNumber == c.AccountNumber) {
            PrintClientCard(c);
            cout << "Are you sure you want to update this client? [y][n]?";
            char choice;
            cin >> choice;
            if (tolower(choice) == tolower('Y'))
            {
                c.AccountNumber = AccountNumber;
                cout << "Enter PinCode? ";
                getline(cin >> ws, c.PinCode);

                cout << "Enter Name? ";
                getline(cin, c.Name);

                cout << "Enter Phone? ";
                getline(cin, c.Phone);

                cout << "Enter AccountBalance? ";
                cin >> c.AccountBalance;

                cout << "Client updated successfully.\n";
            }
            return vClients;
        }
    }
    cout << "\nClient with [" << AccountNumber << "] does not exist \n";
    return vClients;
}

void FindClient(vector <sClient>& vClients) {
    cout << "------------------------------\n";
    cout << "     Find client Screen     \n";
    cout << "------------------------------\n";

    cout << "\n\n Please enter account Number?\n";
    string AccountNumber;
    cin >> AccountNumber;
    for (sClient& c : vClients) {
        if (c.AccountNumber == AccountNumber) {
            PrintClientCard(c);
            return;
        }     
    }
    cout << "\nClient with [" << AccountNumber << "] does not exist \n";
}

void Exit() {
    system("cls");
    cout << "------------------------------\n";
    cout << "         Program Ends         \n";
    cout << "------------------------------\n";
}

void clear() {
    cout << "\n\npress any key to go back to main menu...";
    system("pause");
    system("cls");

}

eTransacrionsOptions ShowTransactionMenuScreen() {
    system("cls");
    cout << "================================================\n";
    cout << "             Transaction Menu Screen            \n";
    cout << "================================================\n";
    cout << "            [1] Deposit                          \n";
    cout << "            [2] Withdraw                         \n";
    cout << "            [3] Total Balances                   \n";
    cout << "            [4]  Main Menu                        \n";
    cout << "=================================================\n";

    cout << "What do you want to do ? [1 to 4]?";
    int Answer;
    cin >> Answer;
    return (eTransacrionsOptions)Answer;
}

vector <sClient> Deposit(vector <sClient>& vClients) {
    cout << "------------------------------\n";
    cout << "       Deposit Screen         \n";
    cout << "------------------------------\n";
    cout << "\n\n Please enter account Number?\n";
    string AccountNumber;
    cin >> AccountNumber;

    for (sClient& c : vClients) {
        if (AccountNumber == c.AccountNumber) {
            PrintClientCard(c);
            cout << "\nPlease enter deposit amount ?";
            double amount;
            cin >> amount;

            cout << "Are you sure you want to Preform this transaction? [y][n]?";
            char choice;
            cin >> choice;
            if (tolower(choice) == tolower('Y')) {
                c.AccountBalance += amount;
                LoadDataToFile(FileName,vClients);
            }

            return vClients;
        }
    }
    cout << "\nClient with [" << AccountNumber << "] does not exist \n";
    return vClients;
}

void TotalBalances(vector <sClient> vClients) {
    cout << "                  Client List (" << vClients.size() << ") Client(s).                          \n";
    cout << "-----------------------------------------------------------------------------------------------\n";
    cout << "| " << setw(15) << left << " Account Number" << "| " << setw(32) << left << " Client Name " << "| "  << setw(15) << left << " Balance \n";
    cout << "--------------------------------------------------------------------------------------------\n";

    double TotalBalances=0;
    for (sClient c : vClients) {
        cout << "| " << setw(15) << left << c.AccountNumber << "| "  << setw(32) << left << c.Name << "| "  << setw(15) << left << c.AccountBalance << " \n";
        TotalBalances += c.AccountBalance;
    }
    cout << endl;
    cout << "-------------------------------------------------------------------------------------------\n";
    cout << "                                      total balances = " << TotalBalances;


}

vector <sClient> Withdraw(vector <sClient>& vClients) {
    cout << "------------------------------\n";
    cout << "       Withdraw Screen        \n";
    cout << "------------------------------\n";
    cout << "\n\n Please enter account Number?\n";
    string AccountNumber;
    cin >> AccountNumber;

    for (sClient& c : vClients) {
        if (AccountNumber == c.AccountNumber) {
            PrintClientCard(c);

            cout << "\nPlease enter withdraw amount ?";
            double amount;
            cin >> amount;
            while (amount > c.AccountBalance) {
                cout << "Amount Exceeds the balance,you can withdraw up to " << c.AccountBalance << "\n";
                cout << "\nPlease enter another amount ?";
                cin >> amount;
            }

            cout << "Are you sure you want to Preform this transaction? [y][n]?";
            char choice;
            cin >> choice;
            if (tolower(choice) == tolower('Y')) {
                c.AccountBalance -= amount;
                cout << "\nTransaction completed successfully. New balance: " << c.AccountBalance << endl;
                LoadDataToFile(FileName,vClients);
            }
            else {
                cout << "\nTransaction canceled.\n";
            }
            return vClients;
        }
    }
    cout << "\nClient with [" << AccountNumber << "] does not exist \n";
    return vClients;
}

void RunTransaction(vector <sClient> &vClients) {
    eTransacrionsOptions options;

    do {
        options = ShowTransactionMenuScreen();
        system("cls");
        switch (options) {
        case eTransacrionsOptions::eDeposit:
        {
            Deposit(vClients);
            clear();
            break;
        }
        case eTransacrionsOptions::eWithdraw:
        {
            Withdraw(vClients);
            clear();
            break;
        }
        case eTransacrionsOptions::eTotalBalances:
        {
            TotalBalances(vClients);
            clear();
            break;
        }
        case eTransacrionsOptions::eMainMenu:
        {
            RunProgram(vClients);
            clear();
            break;
        }
        default: {
            cout << "Invalid Option. Please choose again.\n";
            break;
        }

        }
    } while (options != eTransacrionsOptions::eMainMenu);
}

void RunProgram(vector <sClient> & vClients){

    enMainMenuOptions  option ;
    LoadCleintsDataFromFile(FileName);

    do {
        option = ShowMenuScreen();
        switch (option) {
        case enMainMenuOptions::eShowClientList:
        {
            ShowClientsList(vClients);
            clear();
            break;
        }
        case enMainMenuOptions::eAddClient:
        {
            AddClient(vClients);
            LoadDataToFile(FileName,vClients);
            clear();
            break;
        }
        case enMainMenuOptions::eDeleteClient:
        {
            vClients = DeleteClient(vClients);
            LoadDataToFile(FileName,vClients);
            clear();
            break;
        }
        case enMainMenuOptions::eUpdataClient:
        {
            UpdateClientInfo(vClients);
            LoadDataToFile(FileName,vClients);
            clear();
            break;
        }
        case enMainMenuOptions::eFindClient:
        {
            FindClient(vClients);
            clear();
            break;
        }
        case enMainMenuOptions::eTransacrion:
        {
            RunTransaction(vClients);
            clear();
            break;
        }

        case enMainMenuOptions::eExit:
        {
            Exit();
            break;
        }
        default:{
            cout << "Invalid Option. Please choose again.\n";
            break;
        }

        }
    } while (option != enMainMenuOptions::eExit);
}

int main() {
    vector <sClient> vClients=LoadCleintsDataFromFile(FileName);
    RunProgram(vClients);


    system("pause>0");
    return 0;
}