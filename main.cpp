#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

/*int main()
{
    ifstream inputData("title_basics.tsv");
    vector<vector<string>> outputVector;
    string stringToParse;
    while (getline(inputData, stringToParse)) {
        vector<string> placeHolder;
        outputVector.push_back(placeHolder);
        istringstream stringToParse2(stringToParse);
        string stringToPush;
        while (getline(stringToParse2, stringToPush, '\t')) {
            outputVector.back().push_back(stringToPush);
        }
    }
    

    for (int i = 0; i < outputVector.size(); i++) {
        for (int j = 0; j < outputVector[i].size(); j++) {
            cout << outputVector[i][j] << endl;
        }
    }

    return 0;
}*/



int partition(vector<float>& arr, float low, float high) {
    float pivot = arr[low];
    float up = low;
    float down = high;
    while(up < down) {
        for (float i = up; i < high; i++) {
            if (arr[up] > pivot)
                break;
            up++;
        }
        for (float j = high; j > low; j--) {
            if (arr[down] < pivot)
                break;
            down--;
        }
        if (up < down) {
            float temp = arr[up];
            arr[up] = arr[down];
            arr[down] = temp;
        }
    }
    float temp = arr[low];
    arr[low] = arr[down];
    arr[down] = temp;
    return down;
}
void quickSort(vector<float>& arr, float low, float high) {
    if (low < high) {
        float pivot = partition(arr, low, high);
        quickSort(arr, low, pivot - 1.0f);
        quickSort(arr, pivot + 1.0f, high);
    }
}



void radixSort(vector<float>& test) {
    vector<int> curr;
    for (int i = 0; i < test.size(); i++) {
        float temp = test[i] * 10.0;
        curr.push_back(int(temp));
    }
    queue<int> arr[10];
    int count = 1;
    while (count <= 10) {
        for (int i = 0; i < curr.size(); i++) {
            arr[(curr[i] / count) % 10].push(curr[i]);
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
        test[i] = float(curr[i]) / 10.0f;
    }
}



int main() {


    int choice;
    string names;
    cout << "Welcome to Movie Finder" << endl;
    cout << "Where we give you suggestions based off of Ratings" << endl;
    cout << "So you dont have to endlessly scroll" << endl;

    cout << "Choose from below, which category you would like to search for a movie" << endl;
    cout << "1. Actor" << endl;
    cout << "2. Genre" << endl;
    cout << "3. Region" << endl;
    cout << "4. Year" << endl;
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "What actor would you like to search?" << endl;
            cout << "Input legal first and last name" << endl;
            cin >> names;

            break;

        case 2:

            break;

        case 3:
            cout << "Which country would you like to search?" << endl;
            cin >> names;

            ifstream inputData("title_basics.tsv");
            vector<vector<string>> outputVector;
            string stringToParse;
            while (getline(inputData, stringToParse)) {
                vector<string> placeHolder;
                outputVector.push_back(placeHolder);
                istringstream stringToParse2(stringToParse);
                string stringToPush;
                while (getline(stringToParse2, stringToPush, '\t')) {
                    outputVector.back().push_back(stringToPush);
                }
            }


            break;
    }








    vector<float> test{1,2,3};
    quickSort(test, 0, test.size() - 1);
    //radixSort(test);
    int count = 0;
    for (int i = 0; i < test.size(); i++) {
        //cout << test[i] << ", ";
        count++;
    }
    cout << count << endl;
    return 0;
}
