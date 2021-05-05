#include<bits/stdc++.h>
#include"Constants.h"
class Key {
    int index, row, column;
    char primary, secondary;
public:
    //Constructors
    Key();
    Key(int idx, char pr, char sc, int r, int c);

    // Getters
    char getPrimary();
    char getSecondary();
    int getIndex();

    // Setters
    void setPrimary(char pr);
    void setSecondary(char sc);
    void setIndex(int idx);


    // Display
    void showKey();
};