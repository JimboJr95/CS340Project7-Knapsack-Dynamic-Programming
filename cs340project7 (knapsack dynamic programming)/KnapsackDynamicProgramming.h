//
//  Author:     James Callahan
//  Class:      CS 340
//  Assignment: Project 7
//  Date:       28 November 2017
//  File:       KnapsackDynamicProgramming.h
//

#pragma once
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<assert.h>

using namespace std;

static int INFINITY = -999; // to disallow using certain scoring

// Node for items in the array
struct node{
    int value, // Greatest value
        colIn, // If there is a diagonal in, holds column of diagonal in (don't need row)
        currentWeight; // Holds current weight of knapsack
    bool diagIn; // true->there is a diagonal edge in (best)
};

// Nodes for items we can possibly take
struct item{
    int weight;
    int value;
};

class KnapsackDynamicProgramming{
public:
    // Constructor
    KnapsackDynamicProgramming(); // Default constructor
    ~KnapsackDynamicProgramming(); // Destructor
    
    // Functions
    void run(); // Run takes care of everything after being called
    
private:
    // Variables
    int numValues; // Holds number of values in the array
    int file; // 1-> Longest Common Subsequence, 2->Optimal Sequence Alignment
    int W; // Hodls max weight bound
    ifstream infile; // input file
    
    item *items; // Holds all possible items you can take
    
    node **loot; // Got some help creating the 2d array using the following source
    // source: https://stackoverflow.com/questions/936687/how-do-i-declare-a-2d-array-in-c-using-new
    
    string chosenItems; // For outputting chosen items
    int chosenWeight; // For outputting total weight of chosen items
    
    // Functions
    void inputs();
    void getFileData();
    bool validFileSelection(int num);
    void initializeArray();
    void createOptOutput();
    void printArray();
    void openFile(string filename, ifstream &infile);
};
