// RecommendationSystemLoader.h

#ifndef RECOMMENDATIONSYSTEMLOADER_H
#define RECOMMENDATIONSYSTEMLOADER_H
#include "RecommendationSystem.h"

#include <fstream>
#include <sstream>


class RecommendationSystemLoader
{
    private:
    /**
    * split a string by two arrow and return by args.
    * @param info
    * @param name
    * @param year
    */
    static void split_by_arrow( const std::string& info,
                                std::string& name, int& year);

    /**
     * check if the feature val given in the range.
     * @param i a feature val
     */
    static void features_check( double i);

    public:

    RecommendationSystemLoader() = delete;
    static std::unique_ptr<RecommendationSystem> create_rs_from_movies(const
                                                    std::string& file_path);
};

#endif // RECOMMENDATIONSYSTEMLOADER_H
