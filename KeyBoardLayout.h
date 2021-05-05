#include"Key.h"
class KeyboardLayout {
    std::vector<Key> keys;
    long long int seed;
    double fitness;
public:
    KeyboardLayout();
    KeyboardLayout(long long int);

    double euclideanDist(int i, int j);
    double calcBigStepPenalty(int , int);
    double getFitness();
    Key getKeyAtIndex(int);

    void setFitnesss(double);
    void setKeyAtIndex(int, Key);

    bool isKeyLeftHandPressable(int); 
    bool isKeyRightHandPressable(int);
    bool areKeysLeftHandPressable(int, int);
    bool areKeysRightHandPressable(int, int);
    bool areKeysSameHandPressable(int, int);
    bool areKeysSameFingerPressable(int, int);
    Hand whichHand(int);
    Finger whichFinger(int);
    void printLayout();
    void swapKeys(int , int);

    void showKeyBoard();
};