#include"GAInitiator.h"
#include <chrono>
#include <thread>
#include<thread>
#include <future>

#define chk std::cerr<<"-----------------------HERE---------------------" << std::endl;

GAInitiator::GAInitiator(long long int seed = defaultSeed, bool poll = false) {
    globalSeed = seed;
    srand(time(0));
    for(int i = 0; i < populationSize; ++i) {
        population.push_back(new KeyboardLayout(rand()));
        assignFitness(population.back());
    }
    pollEvents = poll;

    for(int i = 0; i < (int)charset.size(); ++i) {
        mapper[charset[i]] = i;
    }
}

size_t GAInitiator::getTimeFromEpoch() {
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto result = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    return (size_t)result;
}

void GAInitiator::initiate() {
    matingPool.clear();
    offSprings.clear();
    nextGeneration.clear();
    run();
}

double GAInitiator::calcKeyPositionComfort(KeyboardLayout* kl, std::map<char, int> mapper) {
    double res = 0.0;
    // std::map<char, int> mapper;
    // for(int i = 0; i < (int)charset.size(); ++i) {
    //     mapper[charset[i]] = i;
    // }
    for(int i = 0 ;i < numberOfKeys; ++i) {
        double f1 = charFreq[mapper[kl->getKeyAtIndex(i).getPrimary()]];
        double f2 = charFreq[mapper[kl->getKeyAtIndex(i).getSecondary()]];
        res += (f1 + f2) * keyEaseWeight[i]; 
    }
    return res;
}

double GAInitiator::calcBigStepPenalty(KeyboardLayout *kl, std::map<char, int> mapper) {
    double res = 0.0;
    int rowSize = (int)charset.size();
    // std::map<char, int> mapper;
    // for(int i = 0; i < (int)charset.size(); ++i) {
    //     mapper[charset[i]] = i;
    // }
    for(int i = 0; i < numberOfKeys; ++i) {
        for(int j = 0; j < numberOfKeys; ++j) {
            int r, c;
            double steps = kl->calcBigStepPenalty(i, j);
            if(i != j && kl->areKeysSameFingerPressable(i, j)) {
                r = mapper[kl->getKeyAtIndex(i).getPrimary()];
                c = mapper[kl->getKeyAtIndex(j).getPrimary()];

                res += steps * charPairFreq[r * rowSize + c] * 1.0 / 100;

                r = mapper[kl->getKeyAtIndex(i).getPrimary()];
                c = mapper[kl->getKeyAtIndex(j).getSecondary()];

                res += steps * charPairFreq[r * rowSize + c] * 1.0 / 100;

                r = mapper[kl->getKeyAtIndex(i).getSecondary()];
                c = mapper[kl->getKeyAtIndex(j).getPrimary()];

                res += steps * charPairFreq[r * rowSize + c] * 1.0 / 100;

                r = mapper[kl->getKeyAtIndex(i).getSecondary()];
                c = mapper[kl->getKeyAtIndex(j).getSecondary()];

                res += steps * charPairFreq[r * rowSize + c] * 1.0 / 100;
            }
        }
    }

    return res;
}

double GAInitiator::calcFingerAlternation(KeyboardLayout *kl, std::map<char, int> mapper) {
    double res = 0.0;
    int rowSize = (int)charset.size();
    // std::map<char, int> mapper;
    // for(int i = 0; i < (int)charset.size(); ++i) {
    //     mapper[charset[i]] = i;
    // }
    for(int i = 0; i < numberOfKeys; ++i) {
        for(int j = 0; j < numberOfKeys; ++j) {
            int r, c;
            double dist = kl->euclideanDist(i, j);
            if(i != j && kl->areKeysSameFingerPressable(i, j)) {
                r = mapper[kl->getKeyAtIndex(i).getPrimary()];
                c = mapper[kl->getKeyAtIndex(j).getPrimary()];

                res += dist * charPairFreq[r * rowSize + c] * 1.0 / 100;

                r = mapper[kl->getKeyAtIndex(i).getPrimary()];
                c = mapper[kl->getKeyAtIndex(j).getSecondary()];

                res += dist * charPairFreq[r * rowSize + c] * 1.0 / 100;

                r = mapper[kl->getKeyAtIndex(i).getSecondary()];
                c = mapper[kl->getKeyAtIndex(j).getPrimary()];

                res += dist * charPairFreq[r * rowSize + c] * 1.0 / 100;

                r = mapper[kl->getKeyAtIndex(i).getSecondary()];
                c = mapper[kl->getKeyAtIndex(j).getSecondary()];

                res += dist * charPairFreq[r * rowSize + c] * 1.0 / 100;
            }
        }
    }
    return res;
}

double GAInitiator::calcHandAlteration(KeyboardLayout *kl, std::map<char, int> mapper) {
    double res = 0.0;
    int rowSize = (int)charset.size();
    // std::map<char, int> mapper;
    // for(int i = 0; i < (int)charset.size(); ++i) {
    //     mapper[charset[i]] = i;
    // }
    for(int i = 0; i < numberOfKeys; ++i) {
        for(int j = 0; j < numberOfKeys; ++j) {
            int r, c;
            if(i != j && kl->areKeysSameHandPressable(i, j)) {
                r = mapper[kl->getKeyAtIndex(i).getPrimary()];
                c = mapper[kl->getKeyAtIndex(j).getPrimary()];

                res += charPairFreq[r * rowSize + c] * 1.0 / 100;

                r = mapper[kl->getKeyAtIndex(i).getPrimary()];
                c = mapper[kl->getKeyAtIndex(j).getSecondary()];

                res += charPairFreq[r * rowSize + c] * 1.0 / 100;

                r = mapper[kl->getKeyAtIndex(i).getSecondary()];
                c = mapper[kl->getKeyAtIndex(j).getPrimary()];

                res += charPairFreq[r * rowSize + c] * 1.0 / 100;

                r = mapper[kl->getKeyAtIndex(i).getSecondary()];
                c = mapper[kl->getKeyAtIndex(j).getSecondary()];

                res += charPairFreq[r * rowSize + c] * 1.0 / 100;
            }
        }
    }
    return res;
}

double GAInitiator::calcFitness(KeyboardLayout* kl) {
    double param1 = 0.0, param2 = 0.0, param3 = 0.0, param4 = 0.0, param5 = 0.0;
    std::future<double> ret1 = std::async(&calcKeyPositionComfort, kl, mapper);
    std::future<double> ret2 = std::async(&calcHandAlteration, kl, mapper);
    std::future<double> ret3 = std::async(&calcFingerAlternation, kl, mapper);
    std::future<double> ret4 = std::async(&calcBigStepPenalty, kl, mapper);
    param1 = ret1.get();
    param2 = ret2.get();
    param3 = ret3.get();
    param4 = ret4.get();
    // t1.join(); t2.join(); t3.join(); t4.join();

    return 0.45 * param1 - param2 - 0.80 * param3 - 0.70 * param4 + param5;
}

void GAInitiator::assignFitness(KeyboardLayout* kl) {
    kl->setFitnesss(calcFitness(kl));
}

double GAInitiator::getRandomDouble(int l, int r) {
    std::uniform_real_distribution<double> unif(l, r);
    std::default_random_engine re(getTimeFromEpoch());
    double num = unif(re);
    return num;
}

// TODO: Remove this function
void GAInitiator::assertPerfectOrder(KeyboardLayout* kl) {
    std::set<char> dist;
    for(int i = 0; i < numberOfKeys; ++i) {
        dist.insert(kl->getKeyAtIndex(i).getPrimary());
    }
    assert((int)dist.size() == numberOfKeys);
}

std::pair<KeyboardLayout*, KeyboardLayout*> GAInitiator::pmx(KeyboardLayout* p1, KeyboardLayout* p2) {
    std::uniform_real_distribution<double> unif(0, numberOfKeys);
    std::default_random_engine re(getTimeFromEpoch());
    int r1 = unif(re);
    int r2 = unif(re);
    int l = std::min(r1, r2), r = std::max(r1, r2);

    // TODO: Remove this after stress test
    assert(l >= 0 && r < numberOfKeys);
    assert(l >= 0 && r < numberOfKeys);
    

    KeyboardLayout *c1 = new KeyboardLayout(), *c2 = new KeyboardLayout();
    std::map<char, Key> mpc1, mpc2;

    int done1 = 0, done2 = 0;
    for(int i = l; i <= r; ++i) {
        Key p1KeyAtIndex = p1->getKeyAtIndex(i);
        Key p2KeyAtIndex = p2->getKeyAtIndex(i);
        c1->setKeyAtIndex(i, p1KeyAtIndex);
        c2->setKeyAtIndex(i, p2KeyAtIndex);
        mpc1.insert({p1KeyAtIndex.getPrimary(), p2KeyAtIndex});
        mpc2.insert({p2KeyAtIndex.getPrimary(), p1KeyAtIndex});
        ++done1; ++done2;
    }

    // Iterate over p2 and match with mpc1
    for(int i = 0; i < numberOfKeys; ++i) {
        if(i >= l && i <= r) continue;
        Key k = p2->getKeyAtIndex(i);
        auto it = mpc1.find(k.getPrimary());

        // TODO: Remove the counter
        int counter = 0;
        while(it != mpc1.end() && counter < 100) {
            k = it->second;
            it = mpc1.find(k.getPrimary());
            ++counter;
        }
        assert(counter != 100);

        c1->setKeyAtIndex(i, k);
        ++done1;
    }

    for(int i = 0; i < numberOfKeys; ++i) {
        if(i >= l && i <= r) continue;
        Key k = p1->getKeyAtIndex(i);
        auto it = mpc2.find(k.getPrimary());

        // TODO: Remove the counter
        int counter = 0;
        while(it != mpc2.end() && counter < 100) {
            k = it->second;
            it = mpc2.find(k.getPrimary());
        }
        assert(counter != 100);

        c2->setKeyAtIndex(i, k);
        ++done2;
    }

    // TODO: Remove this assert as well
    assert(done1 == done2 && done1 == numberOfKeys);
    return {c1, c2};
}

void GAInitiator::mate(KeyboardLayout *p1, KeyboardLayout *p2) {
    std::pair<KeyboardLayout*, KeyboardLayout*> children = pmx(p1, p2);
    KeyboardLayout *c1 = children.first;
    KeyboardLayout *c2 = children.second;

    // Mutation Operation
    std::uniform_real_distribution<double> unif(0, 1);
    std::uniform_real_distribution<double> unif2(0, numberOfKeys);
    std::default_random_engine re(getTimeFromEpoch());
    for(int i = 0; i < numberOfKeys; ++i) {
        double r = unif(re);
        if(r <= mutationChance) {
            int j = i;
            do{
                j = unif2(re);
            }while(j == i);
            c1->swapKeys(i, j);
        }
    }
    for(int i = 0; i < numberOfKeys; ++i) {
        double r = unif(re);
        if(r <= mutationChance) {
            int j = i;
            do{
                j = unif2(re);
            }while(j == i);
            c2->swapKeys(i, j);
        }
    }
    nextGeneration.push_back(c1);
    nextGeneration.push_back(c2);
}

void GAInitiator::mating() {
    std::shuffle(
        matingPool.begin(), 
        matingPool.end(), 
        std::default_random_engine(globalSeed)
    );
    // TODO: Remove this assert
    assert((int)matingPool.size() >= 2);
    
    std::uniform_real_distribution<double> unif(0, 1);
    std::default_random_engine re(getTimeFromEpoch());
    int cnt = 0;
    for(int i = 1; i < matingPoolPopulationSize; i+=2) {
        double r = unif(re);
        if(r <= matingChance) {
            cnt+=2;
            mate(matingPool[i], matingPool[i - 1]);
        }
        else {
            nextGeneration.push_back(matingPool[i]);
            nextGeneration.push_back(matingPool[ i - 1]);
        }
    }

    // std::cerr << "Mating Happened: " << cnt << " times, which is " << (cnt * 1.0/ matingPoolPopulationSize) * 100 << " % mating pool size.\n";
}

void GAInitiator::elitism() {
    // Directly choosing the elites into the next generation
    for(int i = 0; i < elitePopulationSize; ++i) {
        nextGeneration.push_back(population[i]);
    }
}

void GAInitiator::rankBasedSelection() {
    double currRank = populationSize * 1.0;
    std::vector<std::pair<int, double>> rank;
    rank.push_back({-1, 0.0});
    rank.push_back({0, currRank});

    double totalRankSum = currRank;
    for(int i = 1; i < populationSize; ++i) {
        if(population[i]->getFitness() == population[i - 1]->getFitness()) {
            totalRankSum += currRank;
            rank.push_back({i, currRank});
        }
        else {
            --currRank;
            totalRankSum+=currRank;
            rank.push_back({i, currRank});
        }
    }

    for(int i = 1; i <= populationSize; ++i) {
        rank[i].second /= totalRankSum;
        rank[i].second += rank[i - 1].second;
    }
    // TODO: Remove this later
    // std::cerr<< "Should be 1.0: " << rank[populationSize].second << std::endl;

    int left = matingPoolPopulationSize;
    std::uniform_real_distribution<double> unif(0, 1);
    std::default_random_engine re(getTimeFromEpoch());
    while(left) {
        double r = unif(re);
        // TODO: Remove this after stress test
        assert(r >= 0.0 && r <= 1.0);

        int L = 0, R = populationSize + 1;
        while(R > L + 1) {
            int M = L + (R - L) / 2;
            if(r > rank[M].second) R = M;
            else L = M;
        }
        int idx = rank[R + 1].first;
        assert(idx >= 0);
        matingPool.push_back(population[idx]);
        // TODO: Add binary search here later
        // for(int i = 1; i <= populationSize; ++i) {
        //     if(r <= rank[i].second && r > rank[i - 1].second) {
        //         int idx = rank[i].first;
        //         // TODO: Remove this after stress test
        //         assert(idx >= 0);
        //         matingPool.push_back(population[idx]);
        //     }
        // }
        --left;
    }
    // TODO: Remove this after stress test
    assert((int)matingPool.size() == matingPoolPopulationSize);
}

void GAInitiator::replacementPolicy() {
    int current = (int)nextGeneration.size();
    // Only taking the best of the remaining after the elites
    for(int i = elitePopulationSize; current < populationSize && i < populationSize; ++i) {
        nextGeneration.push_back(population[i]);
        ++current;
    }
    assert((int)nextGeneration.size() == populationSize);
}

void GAInitiator::selection() {
    sort(population.begin(), population.end(), [](KeyboardLayout *a, KeyboardLayout *b) {
        return a->getFitness() > b->getFitness();
    });

    elitism();
    rankBasedSelection();
}

void GAInitiator::run() {
    int epoch = 0;
    std::cout << "Processing Epoch ( " << maxEpoch  << " ): " << std::endl;
    while(epoch < maxEpoch) {
        std::cout << epoch << ", ";
        fflush(stdout);
        selection();
        mating();
        replacementPolicy();

        for(auto e: nextGeneration) {
            assignFitness(e);
            // assertPerfectOrder(e);
        }

        population = nextGeneration;
        nextGeneration.clear();
        matingPool.clear();
        offSprings.clear();
        epoch++;
    }
    std::cout << std::endl;
    sort(population.begin(), population.end(), [](KeyboardLayout *a, KeyboardLayout *b) {
        return a->getFitness() > b->getFitness();
    });
    
    std::cout << population[0]->getFitness() << std::endl;
    population[0]->showKeyBoard();
}