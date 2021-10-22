#include "Population.h"
#include <vector>
#include <iostream>

const std::vector<Group> &Population::getGroups() const {
    return groups;
}

const IndividualVector &Population::getFloaters() const {
    return floaters;
}

int Population::getDeaths() const {
    return deaths;
}


void Population::reset() {
    this->deaths = 0;
    this->inheritance = 0;
    this->newBreederHelper = 0;
    this->newBreederFloater = 0;
}

Population::Population() {
    for (int i = 0; i < Parameters::instance()->getMaxColonies(); i++) {
        Group group(i);
        this->groups.emplace_back(group);
    }
}


void Population::reassignFloaters() {
    std::uniform_int_distribution<int> UniformMaxCol(0, parameters->getMaxColonies() - 1);
    int selectGroup;
    while (!floaters.empty()) {
        Individual floater = floaters.at(floaters.size() - 1);
        floater.setHelp(0);
        selectGroup = UniformMaxCol(*parameters->getGenerator());
        groups[selectGroup].addHelper(floater); //add the floater to the helper vector in a randomly selected group
        floaters.pop_back(); //remove the floater from its vector
    }
}

struct Transaction {
    Transaction(int newGroupId, Individual &allNoRelatedHelpers) : newGroupID(newGroupId),
                                                                   allNoRelatedHelpers(allNoRelatedHelpers) {}

    int newGroupID;
    Individual &allNoRelatedHelpers;
};


void Population::disperse(int generation) {

    int groupID = 0;

    std::vector<Individual> noRelatedHelpers;
    IndividualVector allNoRelatedHelpers;
    std::vector<int> noRelatednessGroupsID;


    for (int i = 0; i < groups.size(); i++) {
        Group &group = groups[i];
        //Dispersal
        this->floaters.merge(group.disperse());

        // In the non relatedness implementation, helpers just born are reassigned to random groups. Groups receive as many helpers as helpers left the group for reassignment.
        if (parameters->isNoRelatedness() && generation > 0) {
            noRelatedHelpers = group.reassignNoRelatedness(i);
            for (int i = 0; i < noRelatedHelpers.size(); i++) {
                noRelatednessGroupsID.push_back(groupID);
            }
            allNoRelatedHelpers.merge(noRelatedHelpers);
            groupID++;
        }
    }

    if (parameters->isNoRelatedness() && !allNoRelatedHelpers.empty()) {

        int selectGroupID;
        int oldGroupID;
        int timeout = 0;
        bool allSameGroup;
        std::vector<int> copyNoRelatednessGroupsID;
        copyNoRelatednessGroupsID = noRelatednessGroupsID;

        std::vector<Transaction> transactionVector;

        if (std::equal(noRelatednessGroupsID.begin() + 1, noRelatednessGroupsID.end(), noRelatednessGroupsID.begin())) {
            allSameGroup = true;
        }

        if (std::adjacent_find(noRelatednessGroupsID.begin(), noRelatednessGroupsID.end(), std::not_equal_to<>()) ==
            noRelatednessGroupsID.end()) {
            allSameGroup = true;
        }

        while (!noRelatednessGroupsID.empty()) {
            int selectGroupIndex = 0;
            if (noRelatednessGroupsID.size() > 1) {
                std::uniform_int_distribution<int> uniformIntDistribution(0, noRelatednessGroupsID.size() - 1);
                selectGroupIndex = uniformIntDistribution(
                        *parameters->getGenerator()); // selects a random index the noRelatednessGroupsID vector
            }
            selectGroupID = noRelatednessGroupsID[selectGroupIndex]; // translates the index to the ID of a group from the noRelatednessGroupsID vector
            auto indexLastIndividual = allNoRelatedHelpers.size() - 1;
            oldGroupID = allNoRelatedHelpers[indexLastIndividual].getGroupIndex();
            if (selectGroupID != oldGroupID || allSameGroup) {
                noRelatednessGroupsID.erase(noRelatednessGroupsID.begin() +
                                            selectGroupIndex); //remove the group ID from the vector to not draw it again

                transactionVector.emplace_back(selectGroupID, allNoRelatedHelpers[indexLastIndividual]);
            } else {
                timeout++;
            }
            if (timeout > 5000) {
                noRelatednessGroupsID = copyNoRelatednessGroupsID;
            }
        }


//        while (!allNoRelatedHelpers.empty()) {
//
//            if (selectGroupID != allNoRelatedHelpers[indexLastIndividual].getGroupIndex() || timeout > 5000) {
//
//                groups[selectGroupID].addHelper(
//                        allNoRelatedHelpers[indexLastIndividual]); //add the no related helper to the helper vector in a randomly selected group
//                allNoRelatedHelpers.pop_back(); //remove the no related helper from its vector
//
//                if (timeout > 5000) { std::cout << "timeout" << std::endl; }
//
//
//            } else {
//                timeout++; //if not other group to put the helper than the original one, do it anyways
//            }
//        }
    }
}


void Population::help() {
    for (Group &group: groups) {
        //Calculate help & cumulative help for group
        group.calculateCumulativeHelp();
    }
}

void Population::survival() {
    for (Group &group: groups) {
        group.survivalGroup();
    }
    this->survivalFloaters();
}


void Population::survivalFloaters() {
    for (Individual &floater: floaters) {
        floater.calculateSurvival(0); // TODO:Change to 1?
    }
}

void Population::mortality() {
    for (Group &group: groups) {
        group.mortalityGroup(deaths);
    }
    this->mortalityFloaters();

}

void Population::mortalityFloaters() {

    std::vector<Individual, std::allocator<Individual>>::iterator floaterIt;
    floaterIt = floaters.begin();
    int size = floaters.size();
    for (int count = 0; !floaters.empty() && size > count; count++) {

        //Mortality floaters
        if (parameters->uniform(*parameters->getGenerator()) > floaterIt->getSurvival()) {
            *floaterIt = floaters[floaters.size() - 1];
            floaters.pop_back();
            deaths++;
        } else {
            floaterIt++;
        }
    }
}

void Population::newBreeder() {
    for (Group &group: groups) {
        if (!group.isBreederAlive()) {
            group.newBreeder(floaters, newBreederFloater, newBreederHelper, inheritance);
        }
    }
}

void Population::increaseAge() {
    for (Group &group: groups) {
        group.increaseAge();
    }
    this->increaseAgeFloaters();
}

void Population::increaseAgeFloaters() {
    for (Individual &floater: floaters) {
        floater.increaseAge();
    }
}

void Population::reproduce(int generation) {
    for (Group &group: groups) {
        group.reproduce(generation);
    }
}

int Population::getNewBreederFloater() const {
    return newBreederFloater;
}

int Population::getNewBreederHelper() const {
    return newBreederHelper;
}

int Population::getInheritance() const {
    return inheritance;
}



