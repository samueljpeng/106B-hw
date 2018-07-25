#include "DoctorsWithoutOrders.h"
#include "map.h"
#include <climits>
#include <iostream>
using namespace std;

/* * * * Doctors Without Orders * * * */

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */

bool findSolution(Vector<Doctor> &doctors,
                  Vector<Patient> &patients,
                  Map<string, Set<string>>& schedule) {
    if (patients.isEmpty()) return true;
    Patient current = patients.back();
    for (int i = 0; i < doctors.size(); i++) {
        if(doctors[i].hoursFree >= current.hoursNeeded) {
            doctors[i].hoursFree -= current.hoursNeeded;
            schedule[doctors[i].name].add(current.name);
            patients.pop_back();
            if (findSolution(doctors, patients, schedule)) return true;
            patients.add(current);
            schedule[doctors[i].name].remove(current.name);
            doctors[i].hoursFree += current.hoursNeeded;
        }
    }
    return false;
}
bool canAllPatientsBeSeen(Vector<Doctor> &doctors,
                          Vector<Patient> &patients,
                          Map<string, Set<string>>& schedule) {
    Vector<Doctor> myDoctors = doctors;
    Vector<Patient> myPatients = patients;
    return findSolution(myDoctors, myPatients, schedule);
}
