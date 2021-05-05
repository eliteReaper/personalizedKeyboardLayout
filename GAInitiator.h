#include"KeyBoardLayout.h"
class GAInitiator {
    std::vector<KeyboardLayout*> population;
    std::vector<KeyboardLayout*> matingPool;
    std::vector<KeyboardLayout*> offSprings;  // TODO: Not being used
    std::vector<KeyboardLayout*> nextGeneration;
    // static std::map<char, int> mapper;
    long long int globalSeed;
    bool pollEvents;

    void run();
    double getRandomDouble(int l, int r); // TODO: Remove useless function
    size_t getTimeFromEpoch();
    void assertPerfectOrder(KeyboardLayout*);

    static double calcHandAlteration(KeyboardLayout*);
    static double calcFingerAlternation(KeyboardLayout*);
    static double calcBigStepPenalty(KeyboardLayout*);
    static double calcKeyPositionComfort(KeyboardLayout*);
    double calcFitness(KeyboardLayout*);
    void assignFitness(KeyboardLayout*);

    void rankBasedSelection();
    void elitism();
    void selection();
    std::pair<KeyboardLayout*, KeyboardLayout*> pmx(KeyboardLayout*, KeyboardLayout*);


    void mate(KeyboardLayout*, KeyboardLayout*);
    void mating();

    void replacementPolicy();
public:
    GAInitiator(long long int, bool);
    void initiate();
};