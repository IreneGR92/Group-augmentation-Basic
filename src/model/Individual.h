
#ifndef GROUP_AUGMENTATION_INDIVIDUAL_H
#define GROUP_AUGMENTATION_INDIVIDUAL_H


#include "FishType.h"
#include "../Parameters.h"

class Individual {

    Individual(double alpha, double alphaAge, double beta, double betaAge, double drift, FishType fishType,
               std::default_random_engine &generator, Parameters &parameters, int &generation);

    Parameters parameters;

    std::default_random_engine generator;

    double survival;
    double help;
    double dispersal;
    double drift;
    double betaAge;
    double beta;
    double alphaAge;
    double alpha;


    FishType fishType;                                                // possible classes: breeder, helper, floater
    int age;
    bool inherit;                                                    //did the new breeder inherit the territory or did it disperse?



    void mutate(int generation);

public:
    Individual(Individual &individual, FishType fishType, Parameters &parameters, std::default_random_engine &generator,
               int &generation);

    Individual(double drift, FishType fishType, Parameters &parameters, std::default_random_engine &generator,
               int &generation);

    void calculateHelp();

    //Functions inside Individual
    double calcDispersal();

    double calcSurvival(int groupSize);

    double getSurvival() const;

    double getHelp() const;

    double getDispersal() const;

    double getDrift() const;

    double getBetaAge() const;

    double getBeta() const;

    double getAlphaAge() const;

    double getAlpha() const;

    FishType getFishType() const;

    int getAge() const;

    bool isInherit() const;

    void setHelp(double help);

    void setInherit(bool inherit);

    void setFishType(FishType fishType);

    void increaseAge(bool alive);

    void increaseAge();
};


#endif //GROUP_AUGMENTATION_INDIVIDUAL_H
