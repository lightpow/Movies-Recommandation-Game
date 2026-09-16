// UsersLoader.h

#ifndef USERFACTORY_H
#define USERFACTORY_H

#include "User.h"

#include <fstream>
#include <sstream>


class UsersLoader
{
    private:

    /**
     * extract movies info from a string line, and returns ptr from rs to them.
     * @param movies_line
     * @param rs
     * @return vector of ptr to movies.
     */
    static std::vector<sp_movie> movies_extract(const std::string&
        movies_line, const std::shared_ptr<RecommendationSystem>& rs);

    /**
     * checks if the string is a valid rank val, throws exception otherwise.
     * @param str
     * @return the string as a number.
     */
    static bool string_to_number( const std::string& str);

    /**
     * check if the feature val given in the range.
     * @param i a feature val
     */
    static void features_check( double i);

    /**
    * split a string by two arrow and return by args.
    * @param info
    * @param name
    * @param year
    */
    static void split_by_arrow( const std::string& info,
                                std::string& name, int& year);


    public:
    UsersLoader() = delete;

    /**
     * create the users with their ratings from a file.
     * @param user_file_path
     * @param rs
     * @return a vector of the created users.
     */
    static std::vector<User> create_users( const std::string& user_file_path,
                                    std::unique_ptr<RecommendationSystem> rs );
};

#endif //USERFACTORY_H
