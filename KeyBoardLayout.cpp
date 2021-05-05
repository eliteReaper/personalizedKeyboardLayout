#include"KeyBoardLayout.h"

const std::vector<std::pair<char, char>> defaultArrangement = {
    {'1', '!'}, // 0 => 1!
    {'q', 'Q'}, // 1 => qQ
    {'a', 'A'}, // 2 => aA
    {'z', 'Z'}, // 3 => zZ
    {'2', '@'}, // 4 => 2@
    {'w', 'W'}, // 5 => wW
    {'s', 'S'}, // 6 => sS
    {'x', 'X'}, // 7 => xX
    {'3', '#'}, // 8 => 3#
    {'e', 'E'}, // 9 => eE
    {'d', 'D'}, // 10 => dD
    {'c', 'C'}, // 11 => cC
    {'4', '$'}, // 12 => 4$
    {'r', 'R'}, // 13 => rR
    {'f', 'F'}, // 14 => fF
    {'v', 'V'}, // 15 => vV
    {'5', '%'}, // 16 => 5%
    {'t', 'T'}, // 17 => tT
    {'g', 'G'}, // 18 => gG
    {'b', 'B'}, // 19 => bB
    {'6', '^'}, // 20 => 6^
    {'y', 'Y'}, // 21 => yY
    {'h', 'H'}, // 22 => hH
    {'n', 'N'}, // 23 => nN
    {'7', '&'}, // 24 => 7&
    {'u', 'U'}, // 25 => uU
    {'j', 'J'}, // 26 => jJ
    {'m', 'M'}, // 27 => mM
    {'8', '*'}, // 28 => 8*
    {'i', 'I'}, // 29 => iI
    {'k', 'K'}, // 30 => kK
    {',', '<'}, // 31 => ,<
    {'9', '('}, // 32 => 9(
    {'o', 'O'}, // 33 => oO
    {'l', 'L'}, // 34 => lL
    {'.', '>'}, // 35 => .>
    {'0', ')'}, // 36 => 0)
    {'p', 'P'}, // 37 => pP
    {';', ':'}, // 38 => ;:
    {'/', '?'}, // 39 => /?
    {'-', '_'}, // 40 => -_
    {'[', '{'}, // 41 => [{
    {'\'', '"'}, // 42 => '"
    {'=', '+'}, // 43 => =+
    {']', '}'}, // 44 => ]}
    
};

KeyboardLayout::KeyboardLayout() {
    keys.assign(numberOfKeys, Key());
    fitness = 0.0;
}

KeyboardLayout::KeyboardLayout(long long int seed) : seed(seed) {
    keys.assign(numberOfKeys, Key());
    std::vector<std::pair<char, char>> defaultArrangementCopy = defaultArrangement;
    std::shuffle(
        defaultArrangementCopy.begin(), 
        defaultArrangementCopy.end(), 
        std::default_random_engine(seed)
    );
    
    for(int i = 0; i < numberOfKeys; ++i) {
        keys[i].setIndex(i);
        keys[i].setPrimary(defaultArrangementCopy[i].first);
        keys[i].setSecondary(defaultArrangementCopy[i].second);
    }
    fitness = 0.0;
};

Key KeyboardLayout::getKeyAtIndex(int idx) {
    assert(idx >= 0 && idx < numberOfKeys);
    return keys[idx];
}

void KeyboardLayout::swapKeys(int i, int j) {
    assert(i >= 0 && i <= numberOfKeys);
    assert(j >= 0 && j <= numberOfKeys);
    std::swap(keys[i], keys[j]);
}

void KeyboardLayout::setKeyAtIndex(int idx, Key k) {
    assert(idx >= 0 && idx < numberOfKeys);
    keys[idx] = k;
}

double KeyboardLayout::getFitness() {
    return fitness;
}

void KeyboardLayout::setFitnesss(double ft) {
    fitness = ft;
}

Hand KeyboardLayout::whichHand(int idx) {
    if(idx < handDivideIndex) return Hand::left;
    else return Hand::right;
}

Finger KeyboardLayout::whichFinger(int idx) {
    if(idx >= 0 && idx <= 3) {
        return Finger::LP;
    }
    else if(idx >= 4 && idx <= 7) {
        return Finger::LR;
    }
    else if(idx >= 8 && idx <= 11) {
        return Finger::LM;
    }
    else if(idx >= 12 && idx <= 19) {
        return Finger::LI;
    }
    else if(idx >= 20 && idx <= 27) {
        return Finger::RI;
    }
    else if(idx >= 28 && idx <= 31) {
        return Finger::RM;
    }
    else if(idx >= 32 && idx <= 35) {
        return Finger::RR;
    }
    else {
        return Finger::RP;
    }
}

void KeyboardLayout::printLayout() {
    for(int i = 0; i < numberOfKeys; ++i) {
        std::cout << i << ": ";
        keys[i].showKey();
    }
}

bool KeyboardLayout::isKeyLeftHandPressable(int idx) {
    return idx < handDivideIndex;
}

bool KeyboardLayout::isKeyRightHandPressable(int idx) {
    return idx >= handDivideIndex;
}

bool KeyboardLayout::areKeysLeftHandPressable(int a, int b) {
    return isKeyLeftHandPressable(a) && isKeyLeftHandPressable(b);
}

bool KeyboardLayout::areKeysRightHandPressable(int a, int b) {
    return isKeyRightHandPressable(a) && isKeyRightHandPressable(b);
}

bool KeyboardLayout::areKeysSameHandPressable(int a, int b) {
    return areKeysLeftHandPressable(a, b) || areKeysRightHandPressable(a, b);
}

bool KeyboardLayout::areKeysSameFingerPressable(int a, int b) {
    return whichFinger(a) == whichFinger(b);
}

double KeyboardLayout::calcBigStepPenalty(int i, int j) {
    Finger a = whichFinger(i);
    Finger b = whichFinger(j);
    if(a == RI || a == RM || a == RR || a == RP) a = (Finger)((int)a - 4);
    if(b == RI || b == RM || b == RR || b == RP) b = (Finger)((int)b - 4);

    return bigStepPenalty[(int)a][(int)b];
}

void KeyboardLayout::showKeyBoard() {
    std::cout << "| ";
    for(int i = 0; i <= 40; i+=4) {
        std::string s = " | ";
        if(i == 16) s = " || ";
        std::cout << keys[i].getPrimary() << keys[i].getSecondary() << s;
    }
    std::cout << keys[43].getPrimary() << keys[43].getSecondary() << " | ";

    std::cout << "\n  | ";
    for(int i = 1; i <= 41; i+=4) {
        std::string s = " | ";
        if(i == 17) s = " || ";
        std::cout << keys[i].getPrimary() << keys[i].getSecondary() << s;
    }
    std::cout << keys[44].getPrimary() << keys[44].getSecondary() << " | ";

    std::cout << "\n   | ";
    for(int i = 2; i <= 42; i+=4) {
        std::string s = " | ";
        if(i == 18) s = " || ";
        std::cout << keys[i].getPrimary() << keys[i].getSecondary() << s;
    }

    std::cout << "\n    | ";
    for(int i = 3; i <= 39; i+=4) {
        std::string s = " | ";
        if(i == 19) s = " || ";
        std::cout << keys[i].getPrimary() << keys[i].getSecondary() << s;
    }

}

double KeyboardLayout::euclideanDist(int i, int j) {
    double ri, ci;
    double rj, cj;
    if(i <= 42) {
        ri = (double)(i / 4);
        ci = (double)(i % 4);
    }
    else {
        if(i == 43) {
            ri = 0; ci = 11;
        }
        else {
            ri = 1; ci = 11;
        }
    }
    if(j <= 42) {
        rj = (double)(j / 4);
        cj = (double)(j % 4);
    }
    else {
        if(i == 43) {
            rj = 0; cj = 11;
        }
        else {
            rj = 1; cj = 11;
        } 
    }  
    return sqrt((rj - ri) * 1.0 * (rj - ri) + (cj - ci) * 1.0 * (cj - ci));
}