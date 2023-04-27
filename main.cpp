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
using namespace std::chrono;

int partition(vector<float>& arr, float low, float high);
void quickSort(vector<float>& arr, float low, float high);
void radixSort(vector<pair<string,float>>& test);
vector<vector<string>> GetAllMovieData();
vector<pair<string, float>> PairMoviesAndRatings(vector<vector<string>>& movieData);
vector<vector<string>> GetMovieDataByGenre(string genre);
vector<pair<string, float>> GetActorMovieRatings(string actor);
vector<pair<string,float>> GetGenreMovieRatings(string genre);
vector<vector<string>> GetMovieDataByMovieIDs(unordered_set<string>& movieIDMap);
unordered_set<string> GetMovieIDsByActor(string actorName);
string GetActorID(string actorName);
vector<pair<string, float>> GetRegionMovieRatings(string region);
unordered_set<string> GetMovieIDsByRegion(string region);



int partition(vector<pair<string,float>>& arr, float low, float high) {
    float pivot = arr[low].second;
    float up = low;
    float down = high;
    while(up < down) {
        for (float i = up; i < high; i++) {
            if (arr[up].second > pivot)
                break;
            up++;
        }
        for (float j = high; j > low; j--) {
            if (arr[down].second < pivot)
                break;
            down--;
        }
        if (up < down) {
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
    return down;
}
void quickSort(vector<pair<string,float>>& arr, float low, float high) {
    if (low < high) {
        float pivot = partition(arr, low, high);
        quickSort(arr, low, pivot - 1.0f);
        quickSort(arr, pivot + 1.0f, high);
    }
}



void radixSort(vector<pair<string,float>>& test) {
    vector<pair<string,int>> curr;
    for (int i = 0; i < test.size(); i++) {
        float temp = test[i].second * 10.0;
        curr.push_back(make_pair(test[i].first, int(temp)));
    }
    queue<pair<string,int>> arr[10];
    int count = 1;
    while (count <= 10) {
        for (int i = 0; i < curr.size(); i++) {
            arr[(curr[i].second / count) % 10].push(make_pair(curr[i].first, curr[i].second));
        }
        int num = 0;
        for (int i = 0; i < 10; i++) {
            while (!arr[i].empty()) {
                curr[num] = arr[i].front();
                arr[i].pop();
                num++;
            }
        }
        count = count * 10;
    }
    for (int i = 0; i < test.size(); i++) {
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

vector<pair<string, float>> PairMoviesAndRatings(vector<vector<string>>& movieData)
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

            // Obtain movie ID and rating
            while (getline(stringToParse2, keyString, '\t')) {
                getline(stringToParse2, valueString, '\t');
                ratingMap[keyString] = stod(valueString);
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

vector<pair<string,float>> GetActorMovieRatings(string actor)
{
    vector<pair<string, float>> outputVector;
    unordered_set<string> actorMap = GetMovieIDsByActor(actor);
    if (actorMap.size() != 0) {
        vector<vector<string>> movieVector = GetMovieDataByMovieIDs(actorMap);
        outputVector = PairMoviesAndRatings(movieVector);
        cout << "All files parsed. Returning data..." << endl;
    }
    else {
        cout << "Error: No movies found for given query." << endl;
    }
    return outputVector;
}

vector<pair<string,float>> GetGenreMovieRatings(string genre)
{
    vector<pair<string, float>> outputVector;
    vector<vector<string>> genreVector = GetMovieDataByGenre(genre);
    if (genreVector.size() != 0) {
        outputVector = PairMoviesAndRatings(genreVector);
        cout << "All files parsed. Returning data..." << endl;
    }
    else {
        cout << "Error: No movies found for given query." << endl;
    }
    return outputVector;
}

vector<pair<string, float>> GetRegionMovieRatings(string region)
{
    // Regions are 2 letter IDs (i.e. Germany = GR)
    vector<pair<string, float>> outputVector;
    unordered_set<string> regionMap = GetMovieIDsByRegion(region);
    if (regionMap.size() != 0) {
        vector<vector<string>> movieVector = GetMovieDataByMovieIDs(regionMap);
        outputVector = PairMoviesAndRatings(movieVector);
        cout << "All files parsed. Returning data..." << endl;
    }
    else {
        cout << "Error: No movies found for given query." << endl;
    }
    return outputVector;
}




int main() {

    int choice;
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
    vector<pair<string, float>> val;
    vector<pair<string, float>> valTwo;
    long long elapsedQ;
    long long elapsedR;

    switch (choice) {
        case 1: {
            cout << "What actor would you like to search?";
            string actor;
            cin >> ws;
            std::getline(std::cin, actor);
            val = GetActorMovieRatings(actor);
            valTwo = val;
            auto beginQ = high_resolution_clock::now();
            quickSort(valTwo, 0, val.size() - 1);
            auto endQ = high_resolution_clock::now();
            auto beginR = high_resolution_clock::now();
            radixSort(val);
            auto endR = high_resolution_clock::now();
            elapsedQ = duration_cast<microseconds>((endQ - beginQ)).count();
            elapsedR = duration_cast<microseconds>((endR - beginR)).count();
            cout << endl;
            cout << "1. Output all ratings" << endl;
            cout << "2. Output select number" << endl;
            cout << endl;
            cout << "Input number: ";
            cin >> either;
            cout << endl;
            cout << endl;
            int rankOne;
            if (either == 1) {
                cout << "--------------------- Your  Ratings --------------------" << endl;
                cout << endl;
                rankOne = 1;
                for (int i = val.size() - 1; i >= 0; i--) {
                    cout << rankOne << ". " << val[i].first << " " << val[i].second << endl;
                    rankOne++;
                }
            }
            else {
                cout << "How many movies do you want to see? (1 - 10)";
                cin >> choice;
                cout << endl;
                cout << "--------------------- Your  Ratings --------------------" << endl;
                cout << endl;
                rankOne = 1;
                for (int i = val.size() - 1; i > val.size() - choice - 1; i--) {
                    cout << rankOne << ". " << val[i].first << " " << val[i].second << endl;
                    rankOne++;
                }
            }
            break;
        }
        case 2: {
            cout << "What Genre would you like to search?";
            string names;
            cin >> names;
            val = GetGenreMovieRatings(names);
            valTwo = val;
            auto beginQ = std::chrono::high_resolution_clock::now();
            quickSort(valTwo, 0, val.size() - 1);
            auto endQ = std::chrono::high_resolution_clock::now();
            auto beginR = std::chrono::high_resolution_clock::now();
            radixSort(val);
            auto endR = std::chrono::high_resolution_clock::now();
            elapsedQ = std::chrono::duration_cast<std::chrono::milliseconds>((endQ - beginQ)).count();
            elapsedR = std::chrono::duration_cast<std::chrono::milliseconds>((endR - beginR)).count();
            cout << endl;
            cout << "1. Output all ratings" << endl;
            cout << "2. Output select number" << endl;
            cout << endl;
            cout << "Input number: ";
            cin >> either;
            cout << endl;
            cout << endl;
            int rankTwo;
            if (either == 1) {
                cout << "--------------------- Your  Ratings --------------------" << endl;
                cout << endl;
                rankTwo = 1;
                for (int i = val.size() - 1; i >=0; i--) {
                    cout << rankTwo << ". " << val[i].first << " " << val[i].second << endl;
                    rankTwo++;
                }
            }
            else {
                cout << "How many movies do you want to see? (1 - 100)";
                cin >> choice;
                cout << endl;
                cout << "--------------------- Your  Ratings --------------------" << endl;
                cout << endl;
                rankTwo = 1;
                for (int i = val.size() - 1; i > val.size() - choice - 1; i--) {
                    cout << rankTwo << ". " << val[i].first << " " << val[i].second << endl;
                    rankTwo++;
                }
            }
            break;
        }
        case 3: {
            cout << "Which country would you like to search?";
            string country;
            cin >> country;
            val = GetRegionMovieRatings(country);
            valTwo = val;
            auto beginQ = std::chrono::high_resolution_clock::now();
            quickSort(valTwo, 0, val.size() - 1);
            auto endQ = std::chrono::high_resolution_clock::now();
            auto beginR = std::chrono::high_resolution_clock::now();
            radixSort(val);
            auto endR = std::chrono::high_resolution_clock::now();
            elapsedQ = std::chrono::duration_cast<std::chrono::milliseconds>((endQ - beginQ)).count();
            elapsedR = std::chrono::duration_cast<std::chrono::milliseconds>((endR - beginR)).count();
            cout << endl;
            cout << "1. Output all ratings" << endl;
            cout << "2. Output select number" << endl;
            cout << endl;
            cout << "Input number: ";
            cin >> either;
            cout << endl;
            cout << endl;
            int rankThree;
            if (either == 1) {
                cout << "--------------------- Your  Ratings --------------------" << endl;
                cout << endl;
                rankThree = 1;
                for (int i = val.size() - 1; i >= 0; i--) {
                    cout << rankThree << ". " << val[i].first << " " << val[i].second << endl;
                    rankThree++;
                }
            }
            else {
                cout << "How many movies do you want to see? (1 - 100)";
                cin >> choice;
                cout << endl;
                cout << "--------------------- Your  Ratings --------------------" << endl;
                cout << endl;
                rankThree = 1;
                for (int i = val.size() - 1; i > val.size() - choice - 1; i--) {
                    cout << rankThree << ". " << val[i].first << " " << val[i].second << endl;
                    rankThree++;
                }
            }
            break;
        }
        /*
        case 4: {
            cout << "Which year would you like to search?";
            int year;
            cin >> year;
            vector<pair<string, float>> valFour =
            radixSort(valFour);
            cout << endl;
            cout << "1. Output all ratings" << endl;
            cout << "2. Output select number" << endl;
            cout << endl;
            cout << "Input number: ";
            cin >> either;
            cout << endl;
            cout << endl;
            int rankFour;
            if (either == 1) {
                cout << "--------------------- Your  Ratings --------------------" << endl;
                cout << endl;
                rankFour = 1;
                for (int i = valFour.size() - 1; i >= 0; i--) {
                    cout << rankFour << ". " << valFour[i].first << " " << valFour[i].second << endl;
                    rankFour++;
                }
            }
            else {
                cout << "How many movies do you want to see? (1 - 100)";
                cin >> choice;
                cout << endl;
                cout << "--------------------- Your  Ratings --------------------" << endl;
                cout << endl;
                rankFour = 1;
                for (int i = valFour.size() - 1; i > valFour.size() - choice - 1; i--) {
                    cout << rankFour << ". " << valFour[i].first << " " << valFour[i].second << endl;
                    rankFour++;
                }
            }
            break;
        }*/
    }

    cout << endl;
    cout << endl;
    cout << "---------------------- Perfomance ----------------------" << endl;
    cout << endl;
    cout << "Number of data points: " << val.size() << endl;
    cout << "Quick Sort: " << elapsedQ << " ms" << endl;
    cout << "Radix Sort: " << elapsedR << " ms" << endl;


    return 0;
}