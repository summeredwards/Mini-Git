#include "miniGit.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;


MiniGit::MiniGit(){
    fs::remove_all("minigit");

    // create DLL node
    doublyNode* start = new doublyNode;

    start->commitNumber = 0;
    start->sHead = NULL;
    start->previous = NULL;
    start->next = NULL;

    dHead = start;
    tail = start;

    // create minigit directory 
    fs::create_directory("minigit");

    // check if directory exists 
    if (fs::exists("minigit") == true){
        cout << endl;
        cout << "minigit directory was created." << endl;
        cout << endl;
    }
}

MiniGit::~MiniGit(){
    // remove minigit directory 
    fs::remove_all("minigit");

    // pointers to DLL and SLL heads/tails
    doublyNode* doublyTemp = tail;
    singlyNode* singlyTemp;

    // traverse DLL and delete all DLL nodes 
    while (doublyTemp != NULL){
        singlyTemp = tail->sHead;
        doublyTemp = tail;

        // traverse SLL and delete all SLL nodes 
        while (singlyTemp != NULL){
            tail->sHead = tail->sHead->next;
            delete singlyTemp;
            singlyTemp = NULL;
        }

        tail = tail->next;
        delete doublyTemp;
        doublyTemp = NULL;
    }
}

void MiniGit::add(doublyNode* &D, string fileName){
    int index_txt;
    string newName;

    // traverse the SLL to see if file has already been added
    singlyNode* check = singlySearch(D, fileName);

    // create a new node for the new file 
    singlyNode* newFile = new singlyNode;
    newFile->fileName = fileName;
    newFile->next = NULL;

    // if NOT in SLL and sHead is NULL 
    if (check == NULL && D->sHead == NULL){
        D->sHead = newFile;
        D->sHead->fileVersion = D->sHead->fileName;

        // find index of '.'
        for (int i=0; i < D->sHead->fileName.length(); i++){
            if (D->sHead->fileName[i] == '.'){
                index_txt = i;
                break;
            }
        }

        // copy file namme in directory up to '.'
        for (int j=0; j < index_txt; j++){
            newName = newName + D->sHead->fileName[j];
        } 
        // add file version number and '.txt'
        newName = newName + "_0.txt";  

        cout << newName << endl;

        // update file version name in SLL
        D->sHead->fileVersion = newName;

        return; 
    }

    // create a temp pointer to point to the SLL head
    singlyNode* temp = D->sHead;

    // if file is NOT in SLL 
    if (check == NULL){
        // insert SLL node at the end of the SLL
        while (temp->next != NULL){
            temp = temp->next;
        }

        temp->next = newFile;

        // find index of '.'
        for (int i=0; i < newFile->fileName.length(); i++){
            if (newFile->fileName[i] == '.'){
                index_txt = i;
                break;
            }
        }

        // copy file namme in directory up to '.'
        for (int j=0; j < index_txt; j++){
            newName = newName + newFile->fileName[j];
        } 
        // add file version number and '.txt'
        newName = newName + "_0.txt";  

        cout << newName << endl;

        // update file version name in SLL
        newFile->fileVersion = newName;

        newFile->k = 0;
    }
    // if file is already in SLL 
    else{
        cout << "File cannot be added twice" << endl;
    }
} 

void MiniGit::remove(doublyNode* &D, string fileName){
    // check if file is in current SLL
    singlyNode* temp = singlySearch(D, fileName);

    // if file is not in SLL
    if (temp == NULL){
        cout << "File not found." << endl;
        return;
    }
    // if file is in SLL 
    else{
        singlyNode* curr = D->sHead;
        singlyNode* prev = 0;

        // traverse SLL
        while (curr != 0){
            // if you find the correct node, delete it 
            if (curr == temp){
                // if deleting head node 
                if (prev == 0){
                    D->sHead = D->sHead->next;
                    // free memory
                    delete curr;
                    curr = NULL;
                    return;
                }
                // otherwise
                else{
                    prev->next = curr->next;
                    // free memory
                    delete curr;
                    curr = NULL;
                    return;
                }
            }
            else{
                prev = curr;
                curr = curr->next;
            }
        }
    }
}

void MiniGit::commit(doublyNode* &D){
    bool match, changed = false;
    singlyNode* curr = D->sHead;
    string fileVersion, temp, line, lineRepo, lineDir, newName;
    ofstream newFile, newVersion;
    ifstream readWrite, compareDir;

    // if files have not been added, return 
    if (curr == NULL){
        cout << "Add files before committing." << endl;
        return;
    }

    // traverse SLL and see if there have been any file changes 
    while (curr != 0){
        fileVersion = curr->fileVersion;
        temp = "minigit/" + fileVersion;

        // check if fileVersion is in .minigit
        match = fs::exists(temp);

        // if it exists
        if (match == true){
            // compare files 
            compareFiles(curr->fileName, temp);

            // if file is changed 
            if (!compareFiles(curr->fileName, temp)){

                int index_txt = 0;

                /*--------------------------------------------------*/ // finding new file version name 
                newName = "";

                // update file version number 
                curr->k = curr->k + 1;

                // find index of '.'
                for (int i=0; i < curr->fileName.length(); i++){
                    if (curr->fileName[i] == '.'){
                        index_txt = i;
                        break;
                    }
                }

                // copy file name in directory up to '.'
                for (int j=0; j < index_txt; j++){
                    newName = newName + curr->fileName[j];
                } 
                // add file version number and '.txt'
                newName = newName + "_" + to_string(curr->k) + ".txt";   

                curr->k = curr->k - 1;
                /*--------------------------------------------------*/   

                // open files 
                compareDir.open(curr->fileName);
                newVersion.open("minigit/" + newName);

                // copy to repository
                while (getline(compareDir, line)){
                    newVersion << line << endl;
                }

                // close files 
                compareDir.close();
                newVersion.close();
            }
        }
        // if it does NOT exist 
        else{
            // open files 
            newFile.open(temp); // in minigit
            readWrite.open(curr->fileName); // in current directory 

            // copy file to repository file
            while (getline(readWrite, line)){
                newFile << line << endl;
            }

            // close files 
            newFile.close();
            readWrite.close();
        }
        curr = curr->next;
    }

    int newIndex = 0;

    // pointer to DLL head 
    doublyNode* tempD = dHead;

    // create new DLL node 
    doublyNode* newDoubly = new doublyNode;
    newDoubly->commitNumber = dHead->commitNumber + 1;
    newDoubly->next = NULL;
    newDoubly->sHead = NULL;

    // set next and previous pointers 
    tempD->next = newDoubly;
    newDoubly->previous = tempD;

    // print the commit number 
    cout << "Commit Number: " << newDoubly->commitNumber << endl;

    // copy over SLL to new DLL node 
    // create new SLL head node 
    singlyNode* newSHead = new singlyNode;
    newSHead->fileName = dHead->sHead->fileName;
    newSHead->fileVersion = dHead->sHead->fileVersion;
    newSHead->k = dHead->sHead->k;
    newSHead->next = NULL;

    newDoubly->sHead = newSHead;

    /*--------------------------------------------------------------*/ // updating file version name if there have been file changes 
    if (!compareFiles(dHead->sHead->fileName, "minigit/" + dHead->sHead->fileVersion)){

    newName = "";
    // add one to file version number 
    newDoubly->sHead->k = newDoubly->sHead->k + 1;

    // find index of '.'
    for (int i=0; i < newDoubly->sHead->fileName.length(); i++){
        if (newDoubly->sHead->fileName[i] == '.'){
            newIndex = i;
            break;
        }
    }

    // copy file name in directory up to '.'
    for (int j=0; j < newIndex; j++){
        newName = newName + newDoubly->sHead->fileName[j];
    } 
    // add file version number and '.txt'
    newName = newName + "_" + to_string(newDoubly->sHead->k) + ".txt"; 

    // update file version name 
    newDoubly->sHead->fileVersion = newName;
    }
    /*--------------------------------------------------------------*/ 

    singlyNode* prevSingly = newDoubly->previous->sHead;
    singlyNode* currSingly = newDoubly->sHead;

    // update head pointer 
    dHead = newDoubly;

    // if only one node in SLL, return
    if (prevSingly->next == NULL){
        cout << "Changes committed successfully." << endl;
        return;
    }

    // if there is more than one node in the SLL do the following:
    prevSingly = prevSingly->next;

    // traverse through previous SLL to copy over to new SLL 
    while (prevSingly != NULL){
        // create new SLL node 
        singlyNode* newSNode = new singlyNode;
        newSNode->fileName = prevSingly->fileName;
        newSNode->fileVersion = prevSingly->fileVersion;
        newSNode->k = prevSingly->k;
        newSNode->next = NULL;

        currSingly->next = newSNode;

        /*--------------------------------------------------------------*/ // updating file version name if there have been file changes 
        if (!compareFiles(prevSingly->fileName, "minigit/" + prevSingly->fileVersion)){

        newName = "";
        // add one to file version number
        currSingly->next->k = currSingly->next->k + 1;

        // find index of '.'
        for (int i=0; i < currSingly->next->fileName.length(); i++){
            if (currSingly->next->fileName[i] == '.'){
                newIndex = i;
                break;
            }
        }

        // copy file name in directory up to '.'
        for (int j=0; j < newIndex; j++){
            newName = newName + currSingly->next->fileName[j];
        } 
        // add file version number and '.txt'
        newName = newName + "_" + to_string(currSingly->next->k) + ".txt"; 

        // update file version name 
        currSingly->next->fileVersion = newName;
        }
        /*--------------------------------------------------------------*/

        currSingly = currSingly->next;
        prevSingly = prevSingly->next;
    }

    cout << "Changes committed successfully." << endl;
}

void MiniGit::checkout(doublyNode* &D, int commitNumber){
    ifstream repoFile;
    ofstream dirFile;
    singlyNode* temp = D->sHead;

    // traverse SLL and see if there have been any file changes 
    while (temp != 0){
        // if there have been changes, disallow checkout 
        if (!compareFiles(temp->fileName, "minigit/" + temp->fileVersion)){
            cout << "1 or more files in your current directory have uncommitted changes. Commit before checking out." << endl;
            return;
        }
        temp = temp->next;
    }

    // find DLL node with correct commit number 
    doublyNode* theVersion = checkoutSearch(commitNumber);

    // if commit number cannot be found 
    if (theVersion == NULL){
        cout << "Commit version could not be found. Enter valid commit number." << endl;
        return;
    }

    // pointer to SLL head 
    temp = theVersion->sHead;

    // traverse through SLL and overwrite the current directory files with SLL files 
    while (temp != 0){
        overwriteFiles(temp->fileName, "minigit/" + temp->fileVersion);
        temp = temp->next;
    }
    return;
}

doublyNode* MiniGit::checkoutSearch(int commitNumber){
    // pointer to the tail
    doublyNode* temp = tail;

    // traverse through DLL 
    while (temp != 0){
        // if node with correct number is found, return DLL node
        if (temp->commitNumber == commitNumber){
            return temp;
        }
        temp = temp->next;
    }
    // if node is not found return NULL
    return NULL;
}

singlyNode* MiniGit::singlySearch(doublyNode* D, string fileName){
    // pointer to SLL head
    singlyNode* temp = D->sHead;

    // traverse SLL
    while (temp != NULL){
        // if node with correct file name is found, return node 
        if (temp->fileName == fileName){
            break;
        }
        temp = temp->next;
    }

    return temp;
}

void MiniGit::overwriteFiles(string fileDir, string fileRepo){
    ifstream readRepo;
    ofstream writeDir;
    string line;

    // open files 
    readRepo.open(fileRepo);
    writeDir.open(fileDir);

    // if failed to open, return
    if (writeDir.fail()){
        cout << "Failed to open current directory file." << endl;
        return;
    }
    if (readRepo.fail()){
        cout << "Failed to open repository file." << endl;
        return;
    }

    // read repository file 
    while (getline(readRepo, line)){
        // write to directory file
        writeDir << line << endl;
    }

    // close files 
    readRepo.close();
    writeDir.close();
}

bool MiniGit::compareFiles(string fileDir, string fileRepo){
    ifstream compareDir, compareRepo;
    char charDir, charRepo;
    bool match = false;

    //open files 
    compareDir.open(fileDir);
    compareRepo.open(fileRepo);

    // if failed to open files, return false 
    if (compareDir.fail() || compareRepo.fail()){
        cout << "Failed to open files." << endl;
        return false;
    }

    // read files 
    while (match == false){
        // compare characters 
        charDir = compareDir.get();
        charRepo = compareRepo.get();

        // if characters do not match, return false 
        if (charDir != charRepo){
            compareDir.close();
            compareRepo.close();

            return false;
        }

        /* if current directory file ends before repository file, return false 
        if (compareDir.eof() && !compareRepo.eof()){
            compareDir.close();
            compareRepo.close();

            return false;
        }
        // if repository file ends before current directory file, return false 
        if (!compareDir.eof() && compareRepo.eof()){
            compareDir.close();
            compareRepo.close();

            return false;
        }*/
        // if both files end at the same time, return true 
        if (compareDir.eof() && compareRepo.eof()){
            match = true;
        }
    }
    compareDir.close();
    compareRepo.close();

    return true;
}

void MiniGit::traverseDoubly(){
    doublyNode* temp = tail;
    singlyNode* curr;

    // traverse DLL 
    while (temp != NULL){
        curr = temp->sHead;

        // print commit number 
        cout << endl;
        cout << "==================" << endl;
        cout << "Commit Number: " << temp->commitNumber << endl;

        // for each DLL node, traverse its SLL nodes 
        while (curr != NULL){
            // print file names 
            cout << "File name: " << curr->fileName << endl;
            cout << "File version: " << curr->fileVersion << endl;
            cout << endl;

            curr = curr->next;
        }
        cout << "==================" << endl;

        temp = temp->next;
    }
}

doublyNode* MiniGit::getDHead(){
    // returns DLL head 
    return dHead;
}