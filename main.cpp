//
//  main.cpp
//  familytree
//
//  Created by 王潘怡 on 2023/2/25.
//

#include <iostream>
#include "header.h"
int main(int argc, const char * argv[]) {
    FMember *father;
    char family[MAXSIZE];
    char findPerson[MAXSIZE] = {"吴德龙"};
    ReadFile(family);
    CreateFTree(father, family);
    OutputFTree(father);
    cout<<"共有"<<GetHeightFTree(father)<<"代人"<<endl;
    FindLevelNum(father, 4, 1);
    OutputPerson(father,findPerson);
    return 0;
}
