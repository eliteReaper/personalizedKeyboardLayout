#include"Key.h"

Key::Key(): 
    index(-1), 
    row(-1), 
    column(-1), 
    primary('~'), 
    secondary('~'){}

Key::Key(int idx, char pr, char sc, int r = -1, int c = -1): 
    index(idx), 
    row(r), 
    column(c), 
    primary(pr), 
    secondary(sc){}

char Key::getPrimary() { return primary; }
char Key::getSecondary() { return secondary; }
int Key::getIndex() { return index; }

void Key::setPrimary(char pr) { primary = pr; }
void Key::setSecondary(char sc) { secondary = sc; }
void Key::setIndex(int idx) { index = idx; }

void Key::showKey() {
    std::cout << primary << secondary << std::endl;
}
