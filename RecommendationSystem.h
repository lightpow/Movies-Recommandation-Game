 // RecommendationSystem.h

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H

#include <map>
#include <set>
#include <limits>
#include <cmath>

#include "User.h"

#define MAX_FEATURES_VAL 10.0
#define MIN_FEATURES_VAL 1.0

class User;


/**
 * a compare functor for the method similar_movies.
 */
struct Custom_Sort
{
    /**
     * compare two pairs based on the second elem.
     * @param lhs
     * @param rhs
     * @return true of rhs second bigger than lhs second, false otherwise.
     */
    bool operator()(const std::pair< sp_movie, double>& lhs,
                    const std::pair< sp_movie, double>& rhs ) const;
};

 /**
  * a compare functor for the sp_movie type.
  */
 struct Sp_movie_compare {
    bool operator()(const sp_movie& lhs, const sp_movie& rhs) const;
};


typedef std::map< sp_movie, std::vector<double>,
                                            Sp_movie_compare> Movies_Features;
typedef std::multiset<std::pair< sp_movie, double>,
                                                Custom_Sort > Similarity_Set;

class RecommendationSystem
{
    friend class User;

    private:

        Movies_Features _base;


    /**
     * check if the feature val given in the range.
     * @param i a feature val
     */
    static void features_check( double i);


    /**
     * sum the inner mult of two vectors.
     * @param v1
     * @param v2
     * @return the sum of the inner mult.
     */
    static double inner_mult(const std::vector<double>& v1,
                                const std::vector<double>& v2);

    /**
     * calculate the Euclidean norm.
     * @param v
     * @return the Euclidean norm.
     */
    static double norm(const std::vector<double>& v);

    /**
     * calculate the angle cos.
     * @param v1
     * @param v2
     * @return the length between the two vectors.
     */
    static double angle_cos(const std::vector<double>& v1,
                                const std::vector<double>& v2);


    /**
     *
     * @param movies
     * @param movie to search similar to.
     * @param k the num of the similar movies.
     * @return the k similar movies.
     */
    Similarity_Set similar_movies( const Movies_Features
                                &movies, const sp_movie &movie, int k) const;





    public:
    /**
     * initialize a new RecommendationSystem object.
     */
    RecommendationSystem() = default;


    /**
     * search for the movie in the data base.
     * @param name
     * @param year
     * @return a smart ptr to the movie in the base, otherwise nullptr if not find.
     */
    sp_movie get_movie(const std::string& name, int year);

    /**
     * search for the given sp_movie in the _base and returns its features.
     * @param movie
     * @return a copy of the features of the given sp_movie in _base.
     */
    std::vector<double> get_features( const sp_movie& movie);

    /**
     *
     * @return a const ref to the data base unordered map
     */
    const Movies_Features& get_all_movies() const;


    /**
     * print all the movies to the stream in an ordered way.
     * @param os
     * @param base
     * @return ref to the given stream.
     */
    friend std::ostream& operator<<(std::ostream& os,
                                    const RecommendationSystem& base);

    /**
     * initialize a movie and inserts it to the base.
     * @param name
     * @param year
     * @param features
     * @return a pointer to the movie.
     */
    sp_movie add_movie_to_rs(const std::string& name, int year,
                             const std::vector<double>& features);

    /**
     * filters the movies in the base based on what the user saw.
     * @param filter if true returns the unseen movies, returns seen movies otherwise.
     * @param user_rankings
     * @return filtered movies_features map.
     */
    Movies_Features movies_filter( bool filter,
                        const User& user_rankings) const;

    /**
     * recommend a movie based on the implemented algorithms.
     * @param user_rankings
     * @return ptr to the chosen movie.
     */
    sp_movie recommend_by_content( const User& user_rankings) const;

    /**
     * predict the score the given user may give to the given movie.
     * @param user_rankings
     * @param movie
     * @param k a parameter for generalizing the algorithm.
     * @return a possible user movie rank.
     */
    double predict_movie_score( const User& user_rankings,
                                const sp_movie& movie, int k) const;

    /**
     * recommend a movie based on the implemented algorithms.
     * @param user_rankings
     * @param k a parameter for generalizing the algorithm.
     * @return ptr to chosen movie.
     */
    sp_movie recommend_by_cf(const User& user_rankings, int k) const;

};

#endif // RECOMMENDATIONSYSTEM_H
