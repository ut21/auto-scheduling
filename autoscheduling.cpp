/*events: 3 preferences for venues, 3 preferences for time slots. name, participation
time-slots: 24 * 2 * 3 (30 minute time slots, so 24*2 slots per day for 3 days)
venue_list = enum of venues
venues: one value from venue_list, total capacity, one availability array of time-slots (24*2*3 elements)
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <unordered_map>
// #include "fast-cpp-csv-parser/csv.h"
using namespace std;

#define NUM_VENUES 3 //changing this requires venueToString() to be modified
#define NUM_DAYS 3
#define SLOT_DURATION 30
#define NUM_TIMESLOTS 24*(60/SLOT_DURATION)*NUM_DAYS
#define NUM_VENUE_PREF 3
#define NUM_TIME_PREF 3 // changing this also requires modifying the Event constructor

enum venue_list { venue1, venue2, venue3, unassigned };

class Event {
    public:
    string name;
    int participation;
    venue_list preferences[NUM_VENUES];
    venue_list assigned_venue;
    array<array<int, 2>, NUM_TIME_PREF> idx_preferences;
    Event(string name, int participation, venue_list v1, venue_list v2, venue_list v3, int day1, int day2, int day3, int time1_start, int time2_start, int time3_start, int time1_end, int time2_end, int time3_end) {
    this->name = name;
    this->participation = participation;
    this->preferences[0] = v1;
    this->preferences[1] = v2;
    this->preferences[2] = v3;
    array<int, 2> temp1 = {day1*48 + (time1_start%100==30)+2*(int)(time1_start/100), day1*48 + (time1_end%100==30)+2*(int)(time1_end/100)};
    this->idx_preferences[0] = temp1;
    array<int, 2> temp2 = {day2*48 + (time2_start%100==30)+2*(int)(time2_start/100), day2*48 + (time2_end%100==30)+2*(int)(time2_end/100)};
    this->idx_preferences[1] = temp2;
    array<int, 2> temp3 = {day3*48 + (time3_start%100==30)+2*(int)(time3_start/100), day3*48 + (time3_end%100==30)+2*(int)(time3_end/100)};
    this->idx_preferences[2] = temp3;
    this->assigned_venue = unassigned;
}
};

class Venue {
    public:
    venue_list name;
    int capacity;
    array<int, NUM_TIMESLOTS> availability;
    Venue(venue_list name, int capacity) {
        this->name = name;
        this->capacity = capacity;
        for (int i = 0; i < NUM_TIMESLOTS; i++) {
            this->availability[i] = 0;
        }
    }
};



string venueToString(venue_list venue) {
    if (venue==0){
        return "venue1";
    }
    else if (venue==1){
        return "venue2";
    }
    else if (venue==2){
        return "venue3";
    }
    else {
        return "unassigned";
    }
}



bool assign_venue(Event* e, array<Venue*, NUM_VENUES> v_array){
    std::ofstream outfile;
    outfile.open("output.txt", std::ios_base::app); // append instead of overwrite, modify accordingly

    for (int i=0; i<NUM_VENUE_PREF; i++){
        if(e->assigned_venue != unassigned){
            break;
        }
        for (int j=0; j<NUM_VENUES; j++){
            if(e->preferences[i] == v_array[j]->name){
                for (int time_pref=0; time_pref<NUM_TIME_PREF; time_pref++){
                    bool slot_available = true;
                    for (int time_slot=e->idx_preferences[time_pref][0]; time_slot<e->idx_preferences[time_pref][1]; time_slot++){
                        if(v_array[j]->availability[time_slot]==1){
                            slot_available = false;
                            break;
                        }
                    }
                    if (!slot_available) {
                        continue;
                    }
                    for (int time_slot=e->idx_preferences[time_pref][0]; time_slot<e->idx_preferences[time_pref][1]; time_slot++){
                        v_array[j]->availability[time_slot] = 1;
                    }
                    outfile << "Event " << e->name << " assigned to venue " << venueToString(v_array[j]->name) << " at time preference " << time_pref + 1 << "\n";
                    e->assigned_venue = v_array[j]->name;
                    return true;
                }
            }
        }
    }
    if(e->assigned_venue == unassigned){
        outfile << "Event " << e->name << " could not be assigned to any venue" << "\n";
        return false;
    }
    outfile.close();
    return true;
}

vector<Event> read_csv(string filename){
    FILE* f = fopen(filename.c_str(), "r");
    char line[1024];
    char* token;
    vector<Event> event_vector;
    while(fgets(line, 1024, f)){
        Event e = Event("", 0, unassigned, unassigned, unassigned, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        token = strtok(line, ",");
        token = strtok(NULL, ",");
        e.name = token;
        token = strtok(NULL, ",");
        e.participation = atoi(token);
        token = strtok(NULL, ",");
        e.preferences[0] = (venue_list)atoi(token);
        token = strtok(NULL, ",");
        e.preferences[1] = (venue_list)atoi(token);
        token = strtok(NULL, ",");
        e.preferences[2] = (venue_list)atoi(token);
        token = strtok(NULL, ",");
        e.idx_preferences[0][0] = atoi(token);
        token = strtok(NULL, ",");
        e.idx_preferences[0][1] = atoi(token);
        token = strtok(NULL, ",");
        e.idx_preferences[1][0] = atoi(token);
        token = strtok(NULL, ",");
        e.idx_preferences[1][1] = atoi(token);
        token = strtok(NULL, ",");
        e.idx_preferences[2][0] = atoi(token);
        token = strtok(NULL, ",");
        e.idx_preferences[2][1] = atoi(token);
        event_vector.push_back(e);
    }
    return event_vector;
}

//int main(){

    //TEST MAIN FUNCTION BELOW

    // Venue v1 = Venue(venue1, 100);
    // Venue v2 = Venue(venue2, 200);
    // Venue v3 = Venue(venue3, 300);
    // array<Venue*, 3> venue_array = {&v1, &v2, &v3};
    // Event event1 = Event("event1", 50, venue1, venue2, venue3, 1, 1, 2, 1000, 2000, 1000, 1030, 1030, 1230);
    // Event event2 = Event("event2", 150, venue2, venue2, venue3, 1, 1, 2, 1000, 2000, 1000, 1030, 1030, 1230);
    // Event event3 = Event("event3", 250, venue1, venue2, venue3, 1, 3, 2, 0, 330, 1000, 1030, 1030, 1430);
    // Event event4 = Event("event4", 350, venue1, venue2, venue3, 3, 2, 3, 1530, 1600, 1000, 1030, 1030, 1530);
    // Event event5 = Event("event5", 450, venue1, venue1, venue1, 0, 0, 0, 1000, 2000, 1000, 2000, 1000, 2000);
    // Event event6 = Event("event6", 550, venue1, venue2, venue3, 0, 0, 0, 1030, 1100, 1000, 1030, 1030, 1730);
    // Event event7 = Event("event7", 650, venue1, venue2, venue3, 1, 1, 0, 2000, 2200, 1000, 1030, 1030, 1830);
    // vector<Event> event_vector;
    // event_vector.push_back(event1);
    // event_vector.push_back(event2);
    // event_vector.push_back(event3);
    // event_vector.push_back(event4);
    // event_vector.push_back(event5);
    // event_vector.push_back(event6);
    // event_vector.push_back(event7);

    // vector<Event> e2 = read_csv("data.csv");

    // for(int i=0; i<7; i++){
    //     assign_venue(&event_vector[i], venue_array);
    // }

    //^^ TEST MAIN FUNCTION ^^
//}

int main(int argc, char *argv[]){
    if(argc < 2) {
        cout << "No filename provided. Please run the program as follows: ./program filename" << endl;
        return 1;
    }
    string filename = argv[1];
    vector<Event> event_vector = read_csv(filename);

    Venue v1 = Venue(venue1, 100);
    Venue v2 = Venue(venue2, 200);
    Venue v3 = Venue(venue3, 300);
    array<Venue*, 3> venue_array = {&v1, &v2, &v3};

    for(int i=0; i<event_vector.size(); i++){
        assign_venue(&event_vector[i], venue_array);
    }
}