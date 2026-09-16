// Files_Gen.h

#ifndef P_FILES_GEN_H
#define P_FILES_GEN_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <numeric>

namespace fs = std::filesystem;

inline std::vector<std::string> movies = {
    "ToyStory-1995", "TheLionKing-1994", "FindingNemo-2003",
    "Shrek-2001", "Frozen-2013", "Moana-2016", "Cars-2006",
    "Aladdin-1992", "Zootopia-2016", "DespicableMe-2010",
    "Up-2009", "WallE-2008", "Coco-2017", "Mulan-1998",
    "Tangled-2010", "Brave-2012", "Ratatouille-2007",
    "InsideOut-2015", "MonstersInc-2001", "Incredibles-2004",
    "KungFuPanda-2008", "HowToTrainYourDragon-2010", "Madagascar-2005",
    "IceAge-2002", "BigHero6-2014","Tarzan-1999", "Hercules-1997",
    "BeautyAndTheBeast-1991", "LittleMermaid-1989", "PeterPan-1953",
    "Bambi-1942", "Dumbo-1941", "SnowWhite-1937", "Cinderella-1950",
    "SleepingBeauty-1959", "Pocahontas-1995", "LiloAndStitch-2002",
    "EmperorsNewGroove-2000", "Atlantis-2001", "TreasurePlanet-2002",
    "BrotherBear-2003", "RobinHood-1973"
};



inline std::vector<std::string> animals = {
    "Lion", "Bear", "Cheetah", "Dolphin", "Eagle",
    "Fox", "Giraffe", "Panda", "Tiger", "Wolf",
    "Zebra", "Rhino", "Hippo", "Koala", "Lemur"
};

/**
 * creates the files directory and randomly initiates the users and movies files for each part.
 * @param movie_pool
 * @param animal_pool
 * @return true if successful, false otherwise.
 */
bool generate_game_files(const std::vector<std::string>& movie_pool,
                            const std::vector<std::string>& animal_pool);

#endif //P_FILES_GEN_H
