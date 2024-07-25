#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include "game.hpp"
#include "manual.hpp"

using namespace std;

void imply_window_scale_to_coordinates(vector<float> &inputData)
{
    for (int index = 0; index < inputData.size(); ++index)
        inputData[index] *= WINDOW_ASPECT_SCALE;
}

vector<float> seperate_words_by_comma(const string str)
{
    vector<float> result;
    int start_pos = 0;
    while (start_pos < str.length())
    {
        int comma_pos = str.find(",", start_pos);
        if (comma_pos == string::npos)
        {
            result.push_back(stoi(str.substr(start_pos)));
            break;
        }
        result.push_back(stoi(str.substr(start_pos, comma_pos - start_pos)));
        start_pos = comma_pos + 1;
    }
    return result;
}

void store_floor(string floorLine, vector<FloorData> &floorData)
{
    vector<float> seperatedFloorData = seperate_words_by_comma(floorLine);
    imply_window_scale_to_coordinates(seperatedFloorData);
    FloorData temp;
    for (int index = 0; index < seperatedFloorData.size(); index++)
    {
        if (index % 3 == 0)
        {
            temp.coordinate.x = seperatedFloorData[index];
        }
        if (index % 3 == 1)
        {
            temp.coordinate.y = seperatedFloorData[index];
        }
        if (index % 3 == 2)
        {
            temp.length = seperatedFloorData[index];
            floorData.push_back(temp);
        }
    }
}

void store_multiple_instance_classes(string line, vector<Coordinate> &objCoordinate)
{
    vector<float> sepratedFloorData = seperate_words_by_comma(line);
    imply_window_scale_to_coordinates(sepratedFloorData);
    Coordinate temp;
    for (int index = 0; index < sepratedFloorData.size(); index++)
    {
        if (index % 2 == 0)
        {
            temp.x = sepratedFloorData[index];
        }
        if (index % 2 == 1)
        {
            temp.y = sepratedFloorData[index];
            objCoordinate.push_back(temp);
        }
    }
}

void store_one_instance_classes(string line, Coordinate &objCor)
{
    vector<float> sepratedFloorData = seperate_words_by_comma(line);
    imply_window_scale_to_coordinates(sepratedFloorData);
    objCor.x = sepratedFloorData[0];
    objCor.y = sepratedFloorData[1];
}

void store_data(vector<FloorData> &floorData, Coordinate &portalCor, vector<Coordinate> &babyTurtlesCor,
                vector<Coordinate> &starsCor, vector<Coordinate> &diamondCor,
                vector<Coordinate> &enemyOneCor, vector<Coordinate> &enemyTwoCor, char const *argv, int round)
{
    string dirName = (string)argv;
    string line;
    ifstream csvInput;
    csvInput.open((dirName + MAPS_ADD[round]).c_str());
    getline(csvInput, line);
    store_floor(line, floorData);
    getline(csvInput, line);
    store_one_instance_classes(line, portalCor);
    getline(csvInput, line);
    store_multiple_instance_classes(line, babyTurtlesCor);
    getline(csvInput, line);
    store_multiple_instance_classes(line, starsCor);
    getline(csvInput, line);
    store_multiple_instance_classes(line, diamondCor);
    getline(csvInput, line);
    store_multiple_instance_classes(line, enemyOneCor);
    getline(csvInput, line);
    store_multiple_instance_classes(line, enemyTwoCor);
}

int main(int argc, char const *argv[])
{
    int totalScore = 0;
    bool isGameOver = false;
    for (int indexRound = 0; indexRound < NUM_OF_ROUNDS; indexRound++)
    {
        if (!isGameOver)
        {
            vector<FloorData> floorData;
            Coordinate portalCor;
            vector<Coordinate> babyTurtlesCor, starsCor, diamondCor, enemyOneCor, enemyTwoCor;
            store_data(floorData, portalCor, babyTurtlesCor, starsCor, diamondCor, enemyOneCor, enemyTwoCor, argv[1], indexRound);
            Game game(floorData, portalCor, babyTurtlesCor, starsCor, diamondCor, enemyOneCor, enemyTwoCor, indexRound);
            game.start(indexRound, isGameOver);
            totalScore += game.get_score();
        }
    }
    cout << "You Scored: " << totalScore << "!" << endl;
}