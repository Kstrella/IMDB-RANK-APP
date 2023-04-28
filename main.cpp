#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
using namespace std;

//Sorting Functions
float partition(vector<float>& arr, float low, float high);
void quickSort(vector<float>& arr, float low, float high);
void radixSort(vector<pair<string,float>>& test);
//Parsing Fucntions
vector<vector<string>> GetAllMovieData();
vector<pair<string, float>> PairMoviesAndRatings(vector<vector<string>>& movieData, int votes);
vector<vector<string>> GetMovieDataByGenre(string genre);
vector<pair<string, float>> GetActorMovieRatings(string actor);
vector<pair<string,float>> GetGenreMovieRatings(string genre);
vector<vector<string>> GetMovieDataByMovieIDs(unordered_set<string>& movieIDMap);
unordered_set<string> GetMovieIDsByActor(string actorName);
string GetActorID(string actorName);
vector<pair<string, float>> GetRegionMovieRatings(string region, int votes);
unordered_set<string> GetMovieIDsByRegion(string region, int votes);
vector<pair<string, float>> GetYearMovieRatings(int year, int votes);
unordered_set<string> GetMovieIDByYear(int year, int votes);



float partition(vector<pair<string,float>>& arr, float low, float high) {
    float pivot = arr[low].second;
    float up = low;
    float down = high;
    while(up < down) { //As long as two pointers don't pass one another
        for (float i = up; i < high; i++) { //Moving front pointer up
            if (arr[up].second > pivot)
                break;
            up++;
        }
        for (float j = high; j > low; j--) { //Moving back pointer down
            if (arr[down].second < pivot)
                break;
            down--;
        }
        if (up < down) { //When the two pointers pass, set pivot value and pointers
            string tempOne = arr[up].first;
            float tempTwo = arr[up].second;
            arr[up] = arr[down];
            arr[down].first = tempOne;
            arr[down].second = tempTwo;
        }
    }
    string tempOne = arr[low].first;
    float tempTwo = arr[low].second;
    arr[low] = arr[down];
    arr[down].first = tempOne;
    arr[down].second = tempTwo;
    return down; //Returns appropriate pivot
}
void quickSort(vector<pair<string,float>>& arr, float low, float high) {
    if (low < high) {
        float pivot = partition(arr, low, high); //Find pivot via partitions and sort sub array through pivot
        quickSort(arr, low, pivot - 1.0f); //Sort left of pivot
        quickSort(arr, pivot + 1.0f, high); //Sort right of pivot
    }
}



void radixSort(vector<pair<string,float>>& test) {
    vector<pair<string,int>> curr;
    for (int i = 0; i < test.size(); i++) { //Because input are decimal float values, mulitply by 10 and set to int for sorting
        float temp = test[i].second * 10.0;
        curr.push_back(make_pair(test[i].first, int(temp)));
    }
    queue<pair<string,int>> arr[10]; //Bucket of size 10 due to values in decimal base
    int count = 1;
    while (count <= 10) {
        for (int i = 0; i < curr.size(); i++) { //Push the appropirate digits into queues in the proper bucket
            arr[(curr[i].second / count) % 10].push(make_pair(curr[i].first, curr[i].second));
        }
        int num = 0;
        for (int i = 0; i < 10; i++) {
            while (!arr[i].empty()) { //Push values from queue in order back into the vector
                curr[num] = arr[i].front();
                arr[i].pop();
                num++;
            }
        }
        count = count * 10; //Change which digit to look at next, factor of 10 due to decimal base
    }
    for (int i = 0; i < test.size(); i++) { //divide by ten to revert to decimal float value.
        test[i] = make_pair(curr[i].first, float(curr[i].second) / 10.0f);
    }
}



string GetActorID(string actorName) {
    // Variables
    ifstream inputData("name_basics.tsv");
    string stringToParse;
    // Check if file opened
    if (inputData.is_open()) {
        cout << "File opened. Parsing..." << endl;
        // Dump header
        getline(inputData, stringToParse);
        while (getline(inputData, stringToParse)) {
            istringstream stringToParse2(stringToParse);
            string stringToPush;
            // Parse line
            while (getline(stringToParse2, stringToPush, '\t')) {
                string nameStr;
                getline(stringToParse2, nameStr, '\t');
                if (nameStr == actorName) {
                    return stringToPush;
                }
            }
        }
    }
    else { cout << "Error: File could not be opened." << endl; }
    return "";
}

unordered_set<string> GetMovieIDsByActor(string actorName)
{
    string actorID = GetActorID(actorName);
    // Variables
    ifstream inputData("title_principles.tsv");
    unordered_set<string> outputSet;
    string stringToParse;
    // Check if file opened
    if (inputData.is_open()) {
        cout << "File opened. Parsing..." << endl;
        // Dump header
        getline(inputData, stringToParse);
        while (getline(inputData, stringToParse)) {
            istringstream stringToParse2(stringToParse);
            string movieID;
            // Parse line
            while (getline(stringToParse2, movieID, '\t')) {
                string actorIDFromFile;
                getline(stringToParse2, actorIDFromFile, '\t');
                getline(stringToParse2, actorIDFromFile, '\t');
                if (actorID == actorIDFromFile) {outputSet.emplace(movieID);}
            }
        }
    }
    else { cout << "Error: File could not be opened." << endl; }
    return outputSet;
}

vector<vector<string>> GetMovieDataByMovieIDs(unordered_set<string>& movieIDMap)
{
    vector<vector<string>> movieData = GetAllMovieData();
    vector<vector<string>> outputVector;
    for (vector<string> vec : movieData) {
        auto iter = movieIDMap.find(vec.at(0));
        if (iter != movieIDMap.end()) {
            outputVector.emplace_back(vec);
        }
    }
    return outputVector;
}

vector<vector<string>> GetAllMovieData()
{
/*
Parses data.tsv from IMDB's 'title.basics.tsv' and returns a vector of vector of strings.
Only data from movies is considered.
0: Movie ID
1: Movie Title
2-4: Up to 3 applicable genres (if no genres available, 2 will be "\\N")
*/
    // Variables
    ifstream inputData("title_basics.tsv");
    vector<vector<string>> outputVector;
    string stringToParse;
    // Check if file opened
    if (inputData.is_open()) {
        cout << "File opened. Parsing..." << endl;
        // Dump header
        getline(inputData, stringToParse);
        while (getline(inputData, stringToParse)) {
            // Variables
            int index = -1;
            bool isMovie = false;
            vector<string> placeHolder;
            istringstream stringToParse2(stringToParse);
            string stringToPush;
            // Parse line
            while (getline(stringToParse2, stringToPush, '\t')) {
                index++;
                // Only want identifier, type, title, and genres
                if (index > 2 && index != 8) { continue; }

                // Check if actually a movie
                if (index == 1) {
                    if (stringToPush != "movie") { break; }
                    isMovie = true;
                    continue;
                }
                // Separate genre field
                if (index == 8) {
                    istringstream stringGenre(stringToPush);
                    while (getline(stringGenre, stringToPush, ',')) {
                        placeHolder.emplace_back(stringToPush);
                    }
                    // As genre is final field, exit line parsing loop
                    break;
                }
                placeHolder.emplace_back(stringToPush);
            }
            if (isMovie) { outputVector.emplace_back(placeHolder); }
        }
    }
    else { cout << "Error: File could not be opened." << endl; }
    return outputVector;
}

vector<pair<string, float>> PairMoviesAndRatings(vector<vector<string>>& movieData, int votes)
{
    vector<pair<string, float>> outputVector;
    ifstream inputData("ratings.tsv");
    unordered_map<string, float> ratingMap;
    string stringToParse;
    // Check if file opened
    if (inputData.is_open()) {
        cout << "File opened. Parsing..." << endl;
        // Dump header
        getline(inputData, stringToParse);
        while (getline(inputData, stringToParse)) {
            istringstream stringToParse2(stringToParse);
            string keyString;
            string valueString;
            // Obtain movie ID, rating, and number of votes
            while (getline(stringToParse2, keyString, '\t')) {
                getline(stringToParse2, valueString, '\t');
                string temp;
                getline(stringToParse2, temp, '\t');

                // Check for blank entry
                if (temp != "\\N") {
                    int curvotes = stoi(temp);

                    // Only take in movie if at vote threshold
                    if (votes <= curvotes) {
                        ratingMap[keyString] = stod(valueString);
                    }
                }
                else {
                    break;
                }
            }
        }
    }
    else { cout << "Error: File could not be opened." << endl; }
    // Combine movie title with its rating
    for (vector<string> vec : movieData) {
        auto iter = ratingMap.find(vec.at(0));
        if (iter != ratingMap.end()) {
            outputVector.emplace_back(pair<string, float>(vec.at(1), iter->second));
        }
    }
    return outputVector;

}

vector<vector<string>> GetMovieDataByGenre(string genre)
{
    vector<vector<string>> movieData = GetAllMovieData();
    vector<vector<string>> outputVector;
    for (vector<string> vec : movieData) {
        for (int i = 1; i < vec.size(); i++) {
            if (vec.at(i) == genre) { outputVector.emplace_back(vec); }
        }
    }
    return outputVector;
}

unordered_set<string> GetMovieIDsByRegion(string region)
{
    // Variables
    ifstream inputData("title_akas.tsv");
    unordered_set<string> outputSet;
    string stringToParse;
    // Check if file opened
    if (inputData.is_open()) {
        cout << "File opened. Parsing..." << endl;
        // Dump header
        getline(inputData, stringToParse);
        while (getline(inputData, stringToParse)) {
            istringstream stringToParse2(stringToParse);
            string stringToPush;

            // Parse line
            while (getline(stringToParse2, stringToPush, '\t')) {
                string regionStr;
                getline(stringToParse2, regionStr, '\t');
                getline(stringToParse2, regionStr, '\t');
                getline(stringToParse2, regionStr, '\t');
                if (regionStr == region) {
                    outputSet.emplace(stringToPush);
                }
                else { break; }
            }
        }
    }
    else { cout << "Error: File could not be opened." << endl; }
    return outputSet;
}

unordered_set<string> GetMovieIDByYear(int year) {
    // Variables
    ifstream inputData("title_basics.tsv");
    unordered_set<string> outputSet;
    string stringToParse;
    // Check if file opened
    if (inputData.is_open()) {
        cout << "File opened. Parsing..." << endl;
        // Dump header
        getline(inputData, stringToParse);
        while (getline(inputData, stringToParse)) {
            istringstream stringToParse2(stringToParse);
            string movieID;
            // Parse line
            // Movie year at index 5
            while (getline(stringToParse2, movieID, '\t')) {
                string yearString;
                for (int i = 0; i < 5; i++) {
                    getline(stringToParse2, yearString, '\t');
                }
                //cout << yearString << endl;
                if (yearString != "\\N") {
                    int yearInt = stoi(yearString);
                    if (year == yearInt) {
                        outputSet.emplace(movieID);
                    }
                    break;
                }
                else {
                    break;
                }
            }
        }
    }
    else { cout << "Error: File could not be opened." << endl; }
    return outputSet;
}

vector<pair<string,float>> GetActorMovieRatings(string actor, int votes) { //Gets movies and ratings based off of Actor
    vector<pair<string, float>> outputVector;
    unordered_set<string> actorMap = GetMovieIDsByActor(actor);
    if (actorMap.size() != 0) {
        vector<vector<string>> movieVector = GetMovieDataByMovieIDs(actorMap);
        outputVector = PairMoviesAndRatings(movieVector, votes);
        cout << "All files parsed. Returning data..." << endl;
    }
    else {
        cout << "Error: No movies found for given query." << endl;
    }
    return outputVector;
}

vector<pair<string,float>> GetGenreMovieRatings(string genre, int votes) { //Gets movies and ratings based off of Genre
    vector<pair<string, float>> outputVector;
    vector<vector<string>> genreVector = GetMovieDataByGenre(genre);
    if (genreVector.size() != 0) {
        outputVector = PairMoviesAndRatings(genreVector, votes);
        cout << "All files parsed. Returning data..." << endl;
    }
    else {
        cout << "Error: No movies found for given query." << endl;
    }
    return outputVector;
}

vector<pair<string, float>> GetRegionMovieRatings(string region, int votes) { //Gets movies and ratings based off of Region
    // Regions are 2 letter IDs (i.e. Germany = GR)
    vector<pair<string, float>> outputVector;
    unordered_set<string> regionMap = GetMovieIDsByRegion(region);
    if (regionMap.size() != 0) {
        vector<vector<string>> movieVector = GetMovieDataByMovieIDs(regionMap);
        outputVector = PairMoviesAndRatings(movieVector, votes);
        cout << "All files parsed. Returning data..." << endl;
    }
    else {
        cout << "Error: No movies found for given query." << endl;
    }
    return outputVector;
}

vector<pair<string, float>> GetYearMovieRatings(int year, int votes) { //Gets movies and ratings based off of Year
    // Yep years
    vector<pair<string, float>> outputVector;
    unordered_set<string> regionMap = GetMovieIDByYear(year);
    if (regionMap.size() != 0) {
        vector<vector<string>> movieVector = GetMovieDataByMovieIDs(regionMap);
        outputVector = PairMoviesAndRatings(movieVector, votes);
        cout << "All files parsed. Returning data..." << endl;
    }
    else {
        cout << "Error: No movies found for given query." << endl;
    }
    return outputVector;
}




int main() {
    int choice; //Following couple lines of code is the Menu output
    cout << endl;
    cout << endl;
    cout << "*********************************************************" << endl;
    cout << endl;
    cout << "**************** Welcome to Movie Finder ****************" << endl;
    cout << endl;
    cout << "*********************************************************" << endl;
    cout << endl;


    cout << "Which category you would like to search?" << endl;
    cout << "1. Actor" << endl;
    cout << "2. Genre" << endl;
    cout << "3. Region" << endl;
    cout << "4. Year" << endl;
    cout << endl;
    cout << "Input number:";
    cin >> choice;
    cout << endl;
    int either;
    vector<pair<string, float>> val; //Vector for radix sort
    vector<pair<string, float>> valTwo; //vector for quick sort
    long long elapsedQ; //Execution time for quick sort
    long long elapsedR; //Execution time for radix sort

    switch (choice) {
        case 1: {
            cout << "What actor would you like to search?"; //Specified input from user
            string actor;
            cin >> ws;
            std::getline(std::cin, actor);
            cout << endl;
            cout << "Minimum votes to filter by?"; //Minimum votes from user
            int min;
            cin >> min;
            cout << endl;
            val = GetActorMovieRatings(actor, min); //Gets the movies and ratings based on specifications
            valTwo = val;
            auto beginQ = std::chrono::steady_clock::now(); //Starts execution time for quick
            quickSort(valTwo, 0, val.size() - 1); //Calls quick sort on moves
            auto endQ = std::chrono::steady_clock::now(); //Ends execution time for quick
            auto beginR = std::chrono::steady_clock::now(); //Starts execution time for radix
            radixSort(val); //Calls radix sort on movies
            auto endR = std::chrono::steady_clock::now(); //Ends execution time for radix
            elapsedQ = std::chrono::duration_cast<std::chrono::microseconds>((endQ - beginQ)).count(); //Calculates total execution time
            elapsedR = std::chrono::duration_cast<std::chrono::microseconds>((endR - beginR)).count(); //Calculates total execution time
            cout << endl;
            cout << "1. Output all ratings" << endl;
            cout << "2. Output select number" << endl;
            cout << endl;
            cout << "Input number: "; //Input in the number of ratings to be shown
            cin >> either;
            cout << endl;
            int rankOne;
            if (either == 1) { //Show all movies
                cout << endl;
                cout << "----------------- Your Ratings :  Radix ----------------" << endl;
                cout << endl;
                rankOne = 1;
                for (int i = val.size() - 1; i >= 0; i--) { //Prints out all moves sorted by radix
                    cout << rankOne << ". " << val[i].first << " " << val[i].second << endl;
                    rankOne++;
                }
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Quick ----------------" << endl;
                cout << endl;
                rankOne = 1;
                for (int i = valTwo.size() - 1; i >= 0; i--) { //Prints out all movies sorted by quick
                    cout << rankOne << ". " << valTwo[i].first << " " << valTwo[i].second << endl;
                    rankOne++;
                }
            }
            else { //Show select number of movies
                cout << "How many movies do you want to see? (1 - 100)";
                cin >> choice;
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Radix ----------------" << endl;
                cout << endl;
                rankOne = 1;
                for (int i = val.size() - 1; i > val.size() - choice - 1; i--) { //Prints out all moves sorted by radix
                    cout << rankOne << ". " << val[i].first << " " << val[i].second << endl;
                    rankOne++;
                }
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Quick ----------------" << endl;
                cout << endl;
                rankOne = 1;
                for (int i = valTwo.size() - 1; i > valTwo.size() - choice - 1; i--) { //Prints out all moves sorted by quick
                    cout << rankOne << ". " << valTwo[i].first << " " << valTwo[i].second << endl;
                    rankOne++;
                }
            }
            break;
        }
        case 2: {
            cout << "What Genre would you like to search?";  //Specified input from user
            string names;
            cin >> names;
            cout << endl;
            cout << "Minimum votes to filter by?"; //Minimum votes from user
            int min;
            cin >> min;
            cout << endl;
            val = GetGenreMovieRatings(names, min); //Gets the movies and ratings based on specifications
            valTwo = val;
            auto beginQ = std::chrono::steady_clock::now(); //Starts execution time for quick
            quickSort(valTwo, 0, val.size() - 1); //Calls quick sort on moves
            auto endQ = std::chrono::steady_clock::now(); //Ends execution time for quick
            auto beginR = std::chrono::steady_clock::now(); //Starts execution time for radix
            radixSort(val); //Calls radix sort on movies
            auto endR = std::chrono::steady_clock::now(); //Ends execution time for radix
            elapsedQ = std::chrono::duration_cast<std::chrono::microseconds>((endQ - beginQ)).count(); //Calculates total execution time
            elapsedR = std::chrono::duration_cast<std::chrono::microseconds>((endR - beginR)).count(); //Calculates total execution time
            cout << endl;
            cout << "1. Output all ratings" << endl;
            cout << "2. Output select number" << endl;
            cout << endl;
            cout << "Input number: "; //Input in the number of ratings to be shown
            cin >> either;
            cout << endl;
            int rankTwo;
            if (either == 1) { //Show all movies
                cout << endl;
                cout << "----------------- Your Ratings :  Radix ----------------" << endl;
                cout << endl;
                rankTwo = 1;
                for (int i = val.size() - 1; i >= 0; i--) { //Prints out all moves sorted by radix
                    cout << rankTwo << ". " << val[i].first << " " << val[i].second << endl;
                    rankTwo++;
                }
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Quick ----------------" << endl;
                cout << endl;
                rankTwo = 1;
                for (int i = valTwo.size() - 1; i >= 0; i--) { //Prints out all moves sorted by quick
                    cout << rankTwo << ". " << valTwo[i].first << " " << valTwo[i].second << endl;
                    rankTwo++;
                }
            }
            else { //Show select number of movies
                cout << "How many movies do you want to see? (1 - 100)";
                cin >> choice;
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Radix ----------------" << endl;
                cout << endl;
                rankTwo = 1;
                for (int i = val.size() - 1; i > val.size() - choice - 1; i--) { //Prints out all moves sorted by radix
                    cout << rankTwo << ". " << val[i].first << " " << val[i].second << endl;
                    rankTwo++;
                }
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Quick ----------------" << endl;
                cout << endl;
                rankTwo = 1;
                for (int i = valTwo.size() - 1; i > valTwo.size() - choice - 1; i--) { //Prints out all moves sorted by quick
                    cout << rankTwo << ". " << valTwo[i].first << " " << valTwo[i].second << endl;
                    rankTwo++;
                }
            }
            break;
        }
        case 3: {
            cout << "Which country would you like to search?";  //Specified input from user
            string country;
            cin >> country;
            cout << endl;
            cout << "Minimum votes to filter by?"; //Minimum votes from user
            int min;
            cin >> min;
            cout << endl;
            val = GetRegionMovieRatings(country, min); //Gets the movies and ratings based on specifications
            valTwo = val;
            auto beginQ = std::chrono::steady_clock::now(); //Starts execution time for quick
            quickSort(valTwo, 0, val.size() - 1); //Calls quick sort on moves
            auto endQ = std::chrono::steady_clock::now(); //Ends execution time for quick
            auto beginR = std::chrono::steady_clock::now(); //Starts execution time for radix
            radixSort(val); //Calls radix sort on movies
            auto endR = std::chrono::steady_clock::now(); //Ends execution time for radix
            elapsedQ = std::chrono::duration_cast<std::chrono::microseconds>((endQ - beginQ)).count(); //Calculates total execution time
            elapsedR = std::chrono::duration_cast<std::chrono::microseconds>((endR - beginR)).count(); //Calculates total execution time
            cout << endl;
            cout << "1. Output all ratings" << endl;
            cout << "2. Output select number" << endl;
            cout << endl;
            cout << "Input number: "; //Input in the number of ratings to be shown
            cin >> either;
            cout << endl;
            int rankThree;
            if (either == 1) { //Show all movies
                cout << endl;
                cout << "----------------- Your Ratings :  Radix ----------------" << endl;
                cout << endl;
                rankThree = 1;
                for (int i = val.size() - 1; i >= 0; i--) { //Prints out all moves sorted by radix
                    cout << rankThree << ". " << val[i].first << " " << val[i].second << endl;
                    rankThree++;
                }
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Quick ----------------" << endl;
                cout << endl;
                rankThree = 1;
                for (int i = valTwo.size() - 1; i >= 0; i--) { //Prints out all moves sorted by quick
                    cout << rankThree << ". " << valTwo[i].first << " " << valTwo[i].second << endl;
                    rankThree++;
                }
            }
            else { //Show select number of movies
                cout << "How many movies do you want to see? (1 - 100)";
                cin >> choice;
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Radix ----------------" << endl;
                cout << endl;
                rankThree = 1;
                for (int i = val.size() - 1; i > val.size() - choice - 1; i--) { //Prints out all moves sorted by radix
                    cout << rankThree << ". " << val[i].first << " " << val[i].second << endl;
                    rankThree++;
                }
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Quick ----------------" << endl;
                cout << endl;
                rankThree = 1;
                for (int i = valTwo.size() - 1; i > valTwo.size() - choice - 1; i--) { //Prints out all moves sorted by quick
                    cout << rankThree << ". " << valTwo[i].first << " " << valTwo[i].second << endl;
                    rankThree++;
                }
            }
            break;
        }
        case 4: {
            cout << "Which year would you like to search?";  //Specified input from user
            int year;
            cin >> year;
            cout << endl;
            cout << "Minimum votes to filter by?"; //Minimum votes from user
            int min;
            cin >> min;
            cout << endl;
            val = GetYearMovieRatings(year, min); //Gets the movies and ratings based on specifications
            valTwo = val;
            auto beginQ = std::chrono::steady_clock::now(); //Starts execution time for quick
            quickSort(valTwo, 0, val.size() - 1); //Calls quick sort on moves
            auto endQ = std::chrono::steady_clock::now(); //Ends execution time for quick
            auto beginR = std::chrono::steady_clock::now(); //Starts execution time for radix
            radixSort(val); //Calls radix sort on movies
            auto endR = std::chrono::steady_clock::now(); //Ends execution time for radix
            elapsedQ = std::chrono::duration_cast<std::chrono::microseconds>((endQ - beginQ)).count(); //Calculates total execution time
            elapsedR = std::chrono::duration_cast<std::chrono::microseconds>((endR - beginR)).count(); //Calculates total execution time
            cout << endl;
            cout << "1. Output all ratings" << endl;
            cout << "2. Output select number" << endl;
            cout << endl;
            cout << "Input number: "; //Input in the number of ratings to be shown
            cin >> either;
            cout << endl;
            int rankFour;
            if (either == 1) { //Show all movies
                cout << endl;
                cout << "----------------- Your Ratings :  Radix ----------------" << endl;
                cout << endl;
                rankFour = 1;
                for (int i = val.size() - 1; i >= 0; i--) { //Prints out all moves sorted by radix
                    cout << rankFour << ". " << val[i].first << " " << val[i].second << endl;
                    rankFour++;
                }
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Quick ----------------" << endl;
                cout << endl;
                rankFour = 1;
                for (int i = valTwo.size() - 1; i >= 0; i--) { //Prints out all moves sorted by quick
                    cout << rankFour << ". " << valTwo[i].first << " " << valTwo[i].second << endl;
                    rankFour++;
                }
            }
            else { //Show select number of movies
                cout << "How many movies do you want to see? (1 - 100)";
                cin >> choice;
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Radix ----------------" << endl;
                cout << endl;
                rankFour = 1;
                for (int i = val.size() - 1; i > val.size() - choice - 1; i--) { //Prints out all moves sorted by radix
                    cout << rankFour << ". " << val[i].first << " " << val[i].second << endl;
                    rankFour++;
                }
                cout << endl;
                cout << endl;
                cout << "----------------- Your Ratings :  Quick ----------------" << endl;
                cout << endl;
                rankFour = 1;
                for (int i = valTwo.size() - 1; i > valTwo.size() - choice - 1; i--) { //Prints out all moves sorted by quick
                    cout << rankFour << ". " << valTwo[i].first << " " << valTwo[i].second << endl;
                    rankFour++;
                }
            }
            break;
        }
    }

    cout << endl;
    cout << endl;
    cout << "---------------------- Perfomance ----------------------" << endl;
    cout << endl;
    cout << "Number of data points: " << val.size() << endl;
    cout << "Quick Sort: " << elapsedQ << " microseconds" << endl; //Prints out performance of quick in microseconds
    cout << "Radix Sort: " << elapsedR << " microseconds" << endl; //Prints out perfromance of radix in microseconds
    float ratio = float(elapsedQ) / float(elapsedR);
    if (ratio > 1)
        cout << "Radix sort is faster than Qucik sort by a factor of " << ratio << endl; //Ratio when Radix is faster
    else
        cout << "Quick sort is faster that Radix sort by a factor of " << 1.0 / ratio << endl; //Ratio when Quick is faster


    return 0;
}