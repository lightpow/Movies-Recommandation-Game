//Movie.cpp

#include "Movie.h"


std::size_t sp_movie_hash(const sp_movie& movie){
    std::size_t res = HASH_START;
    res = res * RES_MULT + std::hash<std::string>()(movie->get_name());
    res = res * RES_MULT + std::hash<int>()(movie->get_year());
    return res;
}


bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2){
    return !(*m1 < *m2) && !(*m2 < *m1);
}


Movie::Movie( std::string movie_name, int movie_year) :
    movie_name(std::move(movie_name)), movie_year(movie_year) {};


std::ostream& operator<<(std::ostream& os, const Movie& movie)
{
    os << movie.get_name() << " (" << movie.get_year() << ")" << std::endl;
    return os;
}



bool Movie::operator<(const Movie& movie) const
{
    if ( movie_year != movie.movie_year)
    {
        return movie_year < movie.movie_year;
    }
    return movie_name < movie.movie_name;
}


std::string Movie::get_name() const
{
    return movie_name;
}

int Movie::get_year() const
{
    return movie_year;
}
