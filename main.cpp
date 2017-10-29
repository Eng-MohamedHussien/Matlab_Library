#include <iostream>
#include "CMatrix.h"
using namespace std;
int main()
{
    CMatrix A ("[3 4 5; 9 7 8; 7 1 6];");
    CMatrix B ("[1 4 5; 3 7 4; 0 2 6];");
    cout<<"\n"<<A.getString();
    cout<<"\n"<<B.getString();
    CMatrix C=A+B;
    cout<<"\n"<<C.getString();
    C=A-B;
    cout<<"\n"<<C.getString();
    C=A*B;
    cout<<"\n"<<C.getString();
    C=A/B;
    cout<<"\n"<<C.getString();
    return 0;
}
