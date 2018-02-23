//
//  Author:     James Callahan
//  Class:      CS 340
//  Assignment: Project 7
//  Date:       28 November 2017
//  File:       KnapsackDynamicProgramming.cpp
//

#include<stdlib.h>
#include<iostream>
#include "KnapsackDynamicProgramming.h"

using namespace std;

// Default (and only) constructor
KnapsackDynamicProgramming::KnapsackDynamicProgramming(){
    
}



// Custom destructor
KnapsackDynamicProgramming::~KnapsackDynamicProgramming(){
    
    // Got some help deleting the 2d array using the following source
    // source: https://stackoverflow.com/questions/936687/how-do-i-declare-a-2d-array-in-c-using-new
    for (int i = 0; i < W; i++) {
        delete [] loot[i];
    }
    
    delete []loot;

    delete []items;
}



// Essentially runs the whole sequence alignment
void KnapsackDynamicProgramming::run(){
    
    inputs(); // Gets all inputs needed from user
    
    // Get number of elements and the upper weight (W) bound
    getFileData();
    
    // Initialize the loot array
    //
    // Got some help initializing the 2d array using the following source
    // source: https://stackoverflow.com/questions/936687/how-do-i-declare-a-2d-array-in-c-using-new
    loot = new node *[numValues + 1];
    for (int i = 0; i < numValues + 1; i++) {
        loot[i] = new node[W + 1];
        loot[i]->currentWeight = 0;
        loot[i]->value = 0;
    }
    
    // Populate the 2D array with the sequence alignment data
    initializeArray();
    
    // Prints our the sequence alignment array
    printArray();
    
    // Output string data
    createOptOutput();
    
    infile.close(); // Close input file
}



// Gets input values from user such as what type of sequence alignment to use and the words to compare
void KnapsackDynamicProgramming::inputs(){
    bool validSelection;
    
    // Find which file they want to use for the knapsack problem
    do{
        // Get sequence alignment selection
        cout << "Which file would you like to solve knapsack on?\n";
        cout << "1: knapsack1\n";
        cout << "2: knapsack2\n";
        cout << "3: knapsack3\n";
        cout << "Selection: ";
        cin >> file;
        
        // Get whether the selected file is a valid one
        validSelection = validFileSelection(file);
        
        // Output if an incorrect number was entered
        if (!validSelection) {
            cout << "Enter 1, 2 or 3.\n";
        }
        
    }while(!validSelection);
    
    // Open file
    switch (file) {
        case 1:
            openFile("knapsack1.txt", infile);
            break;
        
        case 2:
            openFile("knapsack2.txt", infile);
            break;
            
        case 3:
            openFile("knapsack3.txt", infile);
            break;
    }
    
}



// Gets data about available items from the input file
void KnapsackDynamicProgramming::getFileData(){
    // Get number of values and upper weight bound
    infile >> numValues;
    infile >> W;
    
    // Initialize size of items array
    items = new item[numValues];
    
    // Get the value and weight of each item
    for (int i = numValues - 1; i >= 0; i--) {
        infile >> items[i].weight;
        infile >> items[i].value;
    }
    
    // Output item data (it would look wierd if I displayed it in the order I initialized it
    for (int i = 0; i < numValues; i++) {
        cout << "\nItem " << (numValues - i) << ":\tweight=" << items[i].weight << "\tvalue=" << items[i].value;
    }
}



// Validates that the selected sequence alignment is a valid one
bool KnapsackDynamicProgramming::validFileSelection(int num){
    bool val;
    
    if ( (num <= 0) || (num >= 4) ) {
        val = false;
    }
    else{
        val = true;
    }
    
    return val;
}



// Sets up the values of the 2D array
void KnapsackDynamicProgramming::initializeArray(){
    
    // Goes through each row
    for (int row = numValues; row >= 0; row--) {
        // Goes through each column for each row
        for (int col = 0; col <= W; col++) {
            node tmp;
            int largest = INFINITY;
            
            // bottom left corner, first column or last row
            if( (row == (numValues) && (col == 0) ) || (row == (numValues)) || (col == 0) ){
                tmp.diagIn = false;
                tmp.value = 0;
                tmp.currentWeight = 0;
                tmp.colIn = INFINITY;
                loot[row][col] = tmp;
            }
            // Everything in between
            else{
                
                // If there is a possible diagonal in
                if(col >= items[row].weight){
                    
                    // If the diagonal doesn't exceed the weight limit
                    if( (loot[row][col].currentWeight + items[row].weight) <= W){
                        
                        // Set the largest value, set columnIn and set diagIn to true
                        tmp.diagIn = true;
                        tmp.currentWeight = loot[row + 1][col - items[row].weight].currentWeight + items[row].weight;
                        tmp.value = loot[row + 1][col - items[row].weight].value + items[row].value;
                        tmp.colIn = col - items[row].weight;
                        
                        largest = tmp.value;
                    }
                }
                
                // Check to see if the value immidiately below is a better option
                if(loot[row + 1][col].value > largest){
                    // If it is, set diag in to false
                    tmp.diagIn = false;
                    tmp.colIn = INFINITY;
                    tmp.currentWeight = loot[row + 1][col].currentWeight;
                    tmp.value = loot[row + 1][col].value;
                }
                
                // Set array at index to created node
                loot[row][col] = tmp;
            }
        }
    }
}



// This outputs the sequence alignment selected from the 2D array
void KnapsackDynamicProgramming::createOptOutput(){
    int largest = 0, row = 0, col = W; // largest value to pick next, current row and column
    string temp; // used for construction sequence alignment strings
    chosenItems = ""; // make empty string for output
    chosenWeight = loot[row][col].currentWeight; // set total chosen weight top right corner
    
    int chosen = numValues; // for adding to chosenItems
    
    largest = loot[row][col].value; // grabs the top right corner first for largest

    // This is the construction of the output that gives a visualization of the options selected
    //  for the knapsack problem
    while( (row <= numValues) && (col >= 0) ){
        
        // If there is a diagonal in
        if(loot[row][col].diagIn){
            temp = to_string(chosen);
            chosenItems.insert(0, temp);
            chosenItems.insert(1, " ");
            
            col = loot[row][col].colIn;
            row++;
            chosen--;
        }
        // Else it is the item below
        else{
            chosen--;
            row++;
        }
    }
    
    // Outputs the selected items from above
    cout << "Chosen items: " << chosenItems << endl;
    cout << "Total weight used: " << chosenWeight << endl;
    cout << endl << endl;
}



// Prints out the 2D array of nodes using whatever value I set it to
void KnapsackDynamicProgramming::printArray(){
    cout << "\n\nY-axis: items\tX-axis: represents W (weight capacity)\n\n";
    
    // For each row
    for (int i = 0; i < numValues + 1; i++) {
        
        // If not the last row (output letter)
        if(i != numValues){
            cout << (numValues - i) << "\t";
        }
        else{
            cout << "-\t";
        }
        
        // For each item in the row (goes through column items) output value
        for (int j = 0; j < W + 1; j++) {
            cout << loot[i][j].value << "\t";
        }
        cout << endl;
    }
    
    // For each letter in the bottom row
    for (int i = 0; i < W + 1; i++) {
        if(i == 0){
            cout << " \t-\t";
        }
        else{
            cout << i << "\t";
        }
    }
    
    cout << endl;
}



// Ensures that the file opens, or assert
void KnapsackDynamicProgramming::openFile(string filename, ifstream &in){
    in.open(filename);
    
    if(in.fail()){
        cout << "Error opening file " << filename << "\n";
        assert(!in.fail());
    }
}


