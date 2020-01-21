
#ifndef GROUP_AUGMENTATION_GROUP_H
#define GROUP_AUGMENTATION_GROUP_H


#include "Individual.h"
#include "../Parameters.h"

class Group {


    std::default_random_engine generator;
    Parameters parameters;
    double cumHelp;
    bool breederAlive;                                     // for the breeder: 1 alive, 0 dead
    bool helpersPresent;                                   //helpers present in group before reassign?
    int groupSize;
    double fecundity;
    int realFecundity;

    int generation;





public:
    Individual breeder;
    std::vector<Individual> helpers; // create a vector of helpers inside each group

    Group(Parameters &parameters, std::default_random_engine &generator, int &generation);

    void calcGroupSize();

    void disperse(std::vector<Individual> &floaters);

    void calcCumHelp();

    void mortality(int &deaths);

    void newBreeder(std::vector<Individual> &floaters, int &newBreederFloater, int &newBreederHelper, int &inheritance);

    void increaseAge();

    void reproduce();

    const std::vector<Individual> &getHelpers() const;

    const Individual &getBreeder() const;

    int getGroupSize() const;

    bool isBreederAlive() const;

    double getCumHelp() const;

    bool isHelpersPresent() const;


};


#endif //GROUP_AUGMENTATION_GROUP_H
