//
//  Parser.h
//  Project2-Parser
//
//  Created by Truc Le on 9/22/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#ifndef __Project2_Parser__Parser__
#define __Project2_Parser__Parser__

#include <stdio.h>
#include <fstream>
#include "Scanner.h"


class Parser
{
    
private:
    ifstream& input;
    
public:
    Parser( ifstream& i );
    Scanner scanner;
    Token token;
    
    
    bool isValidStatement( Token token );
};


#endif /* defined(__Project2_Parser__Parser__) */
