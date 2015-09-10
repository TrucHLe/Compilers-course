//
//  main.cpp
//  Project1-Scanner
//
//  Created by Truc Le on 9/6/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Constants.h"
#include "Scanner.h"

using namespace std;
using std::ifstream;

int main(int argc, const char * argv[])
{
    
    // Open text file and pass it to Scanner object
    string filename;
    ifstream input;
    
    cout << endl;
    cout << "---------- SCANNER ----------" << endl;
    cout << "File path > ";
    cin >> filename;
    input.open( filename );
    
    while ( input.fail() )
    {
        input.clear();
        cout << "(!) Cannot open " << filename << ", enter again." << endl;
        cout << "File path > ";
        cin >> filename;
        input.open( filename );
    }
    
    cout << endl;
    Scanner scanner( input );
    
    
    
    // Loop through all characters of
    // text file and output each token
    Token token;
    
    do
    {
        token = scanner.next();
        cout << token << endl;
        
    } while ( token.type != EOF_T );
    cout << endl;
    
    
    return 0;
}
