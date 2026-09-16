// RecommendationSystemLoader.h

#include "RecommendationSystemLoader.h"


void RecommendationSystemLoader::split_by_arrow(const std::string& info,
                                    std::string &name, int &year)
{
    size_t pos = info.find('-');
    if (pos != std::string::npos)
    {
        name = info.substr(0, pos);
        year = std::stoi(info.substr(++pos));
        return;
    }
    throw std::invalid_argument("Error extracting movie info from file");
}


void RecommendationSystemLoader::features_check( double i)
{
    if ( i < MIN_FEATURES_VAL || i > MAX_FEATURES_VAL )
    {
        throw std::invalid_argument("Invalid feature value");
    }
}


std::unique_ptr<RecommendationSystem>
RecommendationSystemLoader::create_rs_from_movies(const std::string &file_path)
{
    std::ifstream file(file_path);
    if (file.is_open())
    {
        auto system = std::make_unique<RecommendationSystem>();

        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream line_stream(line);
            int year;
            std::string name;
            std::vector<double> movie_features;

            std::string buffer;
            line_stream >> buffer;
            split_by_arrow(buffer, name, year);

            double to_add;
            while ( line_stream >> to_add )
            {
                features_check(to_add);
                movie_features.push_back(to_add);
            }
            system->add_movie_to_rs(name, year, movie_features);
        }
        if (file.bad())
        {
            throw std::runtime_error("Error reading file");
        }
        return system;
    }
    throw std::runtime_error("Cannot open file");
}
