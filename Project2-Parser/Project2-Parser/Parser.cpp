//
//  Parser.cpp
//  Project2-Parser
//
//  Created by Truc Le on 9/22/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include "Parser.h"


//===----------------------------------------------------------------------===//
// Parser constructor
// Initialize input and scanner in initilization list because they are const
// thus cannot be initialized (aka changed) inside Parser constructor
//===----------------------------------------------------------------------===//
Parser::Parser( ifstream &i ) : input( i ), scanner( i )
{
   
    do
    {
        token = scanner.next();
        cout << token << endl;

    } while ( token.type != EOF_T );
    cout << endl;
}