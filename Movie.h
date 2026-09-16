//Movie.h

#ifndef P_MOVIE_H
#define P_MOVIE_H


#include <iostream>
#include <vector>
#include <utility>
#include <memory>

#define HASH_START 17
#define RES_MULT 31


class Movie;

typedef std::shared_ptr<Movie> sp_movie;

typedef std::size_t (*hash_func)(const sp_movie& movie);
typedef bool (*equal_func)(const sp_movie& m1,const sp_movie& m2);


/**
 * hash function used for a unordered_map.
 * @param movie shared pointer to movie.
 * @return an integer for the hash map.
 */
std::size_t sp_movie_hash(const sp_movie& movie);


/**
 * equal function used for an unordered_map.
 * @param m1
 * @param m2
 * @return true if the year and name are equal else false.
 */
bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2);

class Movie
{
    const std::string movie_name;
    const int movie_year;

    public:

    /**
     * function used to initialize a movie object.
     * @param movie_name
     * @param movie_year
     */
    Movie( std::string movie_name, int movie_year);

    /**
     * prints the movie name and year to a given stream in a specific format.
     * @param os
     * @param movie
     * @return reference to the ostream got as a parameter.
     */
    friend std::ostream& operator<<(std::ostream& os, const Movie& movie);

    /**
     * return the year diffs, unless equal, returns the names diffs.
     * @param movie
     * @return the diffs value between the two movies
     */
    bool operator<(const Movie& movie) const;

    /**
    *
    * @return a copy of the movie name.
    */
    std::string get_name() const;

    /**
     *
     * @return a copy of the movie year.
     */
    int get_year() const;

};


#endif //P_MOVIE_H
