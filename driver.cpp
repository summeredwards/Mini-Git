#include "miniGit.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

int main(){
    string fileName, option, s_commitNumber;
    ifstream read;
    int num = 0, commitNumber = 0;
    bool match = false;
    doublyNode* theHead;

    // ask user if they want to initiate a repository 
    cout << "Do you wish to initialize a new repository?" << endl;
    cout << "Select a numerical option: " << endl << "1. Yes" << endl << "2. No" << endl;
    getline(cin, option);

    num = stoi(option);

    // if no, end program
    if (num == 2){
        cout << "Goodbye" << endl;
        return 0;
    }
    // if invalid input
    else if (num != 1){
        cout << "Invalid input" << endl;
        return 0;
    }

    // otherwise initialize minigit
    MiniGit myGit;

    // print menu 
    cout << "====== MENU ======" << endl;
    cout << "1. Add a File" << endl;
    cout << "2. Remove a File" << endl;
    cout << "3. Commit Changes" << endl;
    cout << "4. Checkout" << endl;
    cout << "5. Quit" << endl;
    cout << "===================" << endl;

    getline(cin, option);
    num = stoi(option);

    while (num != 5){
        switch(num){
            // if choice is 1
            case 1:
                // keep having user enter inputs until its a valid file name 
                while (match == false){
                    cout << "Enter a file name: " << endl;
                    getline(cin, fileName);

                    match = fs::exists(fileName);
                }

                // get the DLL head 
                theHead = myGit.getDHead();

                // add file 
                myGit.add(theHead, fileName);

                match = false;

                break;
            // if choice is 2
            case 2:
                // have user enter file name 
                cout << "Enter a file name: " << endl;
                getline(cin, fileName);

                // get DLL head 
                theHead = myGit.getDHead();

                // remove file 
                myGit.remove(theHead, fileName);
                
                break;
            // if choice is 3
            case 3:
                // get DLL head 
                theHead = myGit.getDHead();

                // commit changes 
                myGit.commit(theHead);

                break;
            // if choice is 4
            case 4:
                // have user enter a commit number 
                cout << "Enter a commit number: " << endl;
                getline(cin, s_commitNumber);

                commitNumber = stoi(s_commitNumber);

                // get DLL head 
                theHead = myGit.getDHead();

                // checkout at specified commit number 
                myGit.checkout(theHead, commitNumber);

                break;
        }

        // print menu 
        cout << "====== MENU ======" << endl;
        cout << "1. Add a File" << endl;
        cout << "2. Remove a File" << endl;
        cout << "3. Commit Changes" << endl;
        cout << "4. Checkout" << endl;
        cout << "5. Quit" << endl;
        cout << "===================" << endl;

        // get option
        getline(cin, option);
        num = stoi(option);
    }

    // if user chooses 5, quit program
    cout << "Quitting... " << endl;
    cout << "Goodbye!" << endl;

    return 0;
}