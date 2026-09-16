// UsersLoader.cpp

#include "UsersLoader.h"


void UsersLoader::split_by_arrow(const std::string& info,
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


void UsersLoader::features_check( double i)
{
    if ( i < MIN_FEATURES_VAL || i > MAX_FEATURES_VAL )
    {
        throw std::invalid_argument("Invalid feature value");
    }
}


std::vector<sp_movie> UsersLoader::movies_extract(const std::string&
                movies_line, const std::shared_ptr<RecommendationSystem>& rs)
{
    std::vector<sp_movie> movies;
    std::stringstream ss(movies_line);
    std::string item;

    while (ss >> item)
    {
        std::string name;
        int year;
        split_by_arrow(item, name, year);
        movies.push_back(rs->get_movie(name, year));
    }
    return movies;
}


bool UsersLoader::string_to_number(const std::string &str)
{
    if ( str != "NA")
    {
        for ( char c : str)
        {
            if ( !std::isdigit(c) && c != '.')
            {
                throw std::invalid_argument("Invalid user rank");
            }
        }
        return true;
    }
    return false;
}


std::vector<User> UsersLoader::create_users(const std::string &user_file_path,
                                    std::unique_ptr<RecommendationSystem> rs)
{
    std::ifstream file(user_file_path);
    if (file.is_open())
    {
        std::shared_ptr<RecommendationSystem> new_rs = std::move(rs);
        std::vector<User> users;

        std::string line;
        std::getline(file, line);
        auto movies = movies_extract(line, new_rs);

        while (std::getline(file, line))
        {
            std::string name;
            rank_map user_map(0, sp_movie_hash, sp_movie_equal);
            std::stringstream ss(line);

            ss >> name;
            User user(name, user_map, new_rs);

            int i = 0;
            std::string tmp;
            while (ss >> tmp)
            {
                if (string_to_number(tmp))
                {
                    if (i >= static_cast<int>(movies.size()))
                    {
                        throw std::invalid_argument("Invalid file content");
                    }

                    double to_add = std::stod(tmp);
                    features_check(to_add);

                    const sp_movie& movie = movies[i];
                    if (movie == nullptr)
                    {
                        throw std::invalid_argument("Invalid file content");
                    }

                    user.add_movie_to_user(movie->get_name(),
                        movie->get_year(),new_rs->get_features(movie), to_add);
                }
                i++;
            }
            if ( i < static_cast<int>(movies.size()))
            {
                throw std::invalid_argument("Invalid file content");
            }
            users.push_back(user);
        }
        if ( file.bad())
        {
            throw std::runtime_error("Error reading file");
        }
        return users;
    }
    throw std::runtime_error("Cannot open file");
}
