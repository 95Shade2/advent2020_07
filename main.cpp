#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <cstdlib>

using namespace std;

class Bag {
    public:
        string name;
        vector<string> bag_names;   //the names of the bags that this bag can hold
        vector<int> bag_count;  //the count of each bag type in parallel to the bag_names
        int shiny;

        Bag() {
            name = "";
            shiny = -1;
        }

        Bag(string bag_name, vector<string> bag_bags, vector<int> bag_bags_count) {
            name = bag_name;
            bag_names = bag_bags;
            bag_count = bag_bags_count;
            shiny = -1;
        }

        operator=(const Bag *other_bag) {
            this->name = other_bag->name;
            this->bag_names = other_bag->bag_names;
            this->bag_count = other_bag->bag_count;
        }
};

int Int(string num) {
    int num_i = 0;

    while (num.size() > 0) {
        num_i *= 10;
        num_i += num[0] - '0';
        num = num.substr(1);
    }

    return num_i;
}

Bag Get_Bag(string bag_data) {
    string bag_name;
    vector<string> bags_contained;
    string bag_contained;
    vector<int> bags_count;
    int index_plus;
    int bag_count;
    Bag bag;
    string eob; //end of bag - comma or a period to determine the end of the current inside bag

    bag_name = bag_data.substr(0, bag_data.find(" bags"));

    bag_data = bag_data.substr(bag_data.find("contain")+8); //make bag_data only have the list of the bags that this bag holds

    while (bag_data.size() > 0) {
        if (bag_data != "no other bags.") {
            bag_count = Int(bag_data.substr(0, bag_data.find(" ")));
            bag_data = bag_data.substr(bag_data.find(" ")+1);

            if (bag_data.find(",") < bag_data.size()) {
                eob = ",";
                index_plus = 2; //what to add to the index from the comma to be at the start of the next bag name
            }
            else {
                eob = ".";
                index_plus = 1; //make big_data an empty string
            }

            bag_contained = bag_data.substr(0, bag_data.find("bag")-1);
            bag_data = bag_data.substr(bag_data.find(eob)+index_plus);

            bags_contained.push_back(bag_contained);
            bags_count.push_back(bag_count);
        }
        //if this bag contains no other bags
        else {
            bag_data = "";
        }
    }

    return Bag(bag_name, bags_contained, bags_count);
}

bool Contains_Shiny(map<string, Bag> *bags, Bag bag, int depth = 0) {
    vector<string> names = bag.bag_names;

    if (bag.shiny == 1) {
        return true;
    }
    else if (bag.shiny == 0) {
        return false;
    }

    if (names.size() == 0) {
        bag.shiny = 0;
        return false;
    }
    else {
        for (int n = 0; n < names.size(); n++) {
            string name = names[n];
            Bag sub_bag = (*bags)[name];

            if (name == "shiny gold" || sub_bag.shiny == 1 || Contains_Shiny(bags, sub_bag, ++depth)) {
                sub_bag.shiny = 1;
                return true;
            }
        }
    }
    bag.shiny = 0;
    return false;
}

int Get_Shiny_Count(map<string, Bag> *bags) {
    ifstream file;
    string line;
    vector<string> bag_keys;
    Bag bag;
    int shiny_count = 0;

    file.open("bags.txt");

    getline(file, line);
    while (file) {
        bag = Get_Bag(line);
        bag_keys.push_back(bag.name);
        (*bags)[bag.name] = bag;

        getline(file, line);
    }

    for (int b = 0; b < bag_keys.size(); b++) {
        bag = (*bags)[bag_keys[b]];

        if (Contains_Shiny(bags, bag)) {
            shiny_count++;
        }
    }

    file.close();

    return shiny_count;
}

int Get_Sub_Bags(map<string, Bag> *bags, string bag_name = "shiny gold") {
    int shiny_count = 0;
    Bag shiny_bag = (*bags)[bag_name];
    int sub_count = 0;
    string name;

    if (shiny_bag.bag_names.size() == 0) {
        return 0;
    }

    for (int b = 0; b < shiny_bag.bag_names.size(); b++) {
        name = shiny_bag.bag_names[b];

        sub_count = Get_Sub_Bags(bags, name);

        shiny_count += shiny_bag.bag_count[b] * sub_count;
        shiny_count += shiny_bag.bag_count[b];
    }

    return shiny_count;
}

int main()
{
    int shiny;
    int shiny_bags;
    map<string, Bag> bags;

    shiny = Get_Shiny_Count(&bags);
    shiny_bags = Get_Sub_Bags(&bags);

    cout << "Total shiny bags count: " << shiny << endl;

    cout << "Total sub bags under shiny: " << shiny_bags << endl;

    return 0;
}
