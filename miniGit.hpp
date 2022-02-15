#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

struct singlyNode{
    string fileName;
    string fileVersion;
    int k = 0; // this is the file version number 
    singlyNode* next;
};

struct doublyNode{
    int commitNumber;
    doublyNode* previous;
    doublyNode* next;

    singlyNode* sHead;
};

class MiniGit{
    private:
        doublyNode* dHead; // make head the most recent commit
        doublyNode* tail; // make tail the first commit 

    public:
        MiniGit(); // createDirectory() could be in constructor // create single doubly node when initialized by user
        ~MiniGit();

        void createDirectory();

        void add(doublyNode* &D, string fileName); // create new SLL node in driver w naming, and then put it into the function call 
        void remove(doublyNode* &D, string fileName);
        void commit(doublyNode* &D);
        void checkout(doublyNode* &D, int commitNumber);

        doublyNode* checkoutSearch(int commitNumber);
        singlyNode* singlySearch(doublyNode* D, string fileName);

        bool compareFiles(string fileDir, string fileRepo);
        void overwriteFiles(string fileDir, string fileRepo);

        void traverseDoubly();

        doublyNode* getDHead();
};