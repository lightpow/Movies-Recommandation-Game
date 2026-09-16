// RecommendationSystem.cpp

#include "RecommendationSystem.h"



bool Sp_movie_compare::operator()(const sp_movie& lhs, const sp_movie& rhs) const
{
    return *lhs < *rhs;
}



void RecommendationSystem::features_check( double i)
{
    if ( i < MIN_FEATURES_VAL || i > MAX_FEATURES_VAL )
    {
        throw std::invalid_argument("Invalid feature value");
    }
}


bool Custom_Sort::operator()(const std::pair<sp_movie, double> &lhs,
                    const std::pair<sp_movie, double> &rhs) const
{
    return lhs.second < rhs.second;
}

sp_movie RecommendationSystem::get_movie(const std::string &name, int year)
{
    sp_movie movie = std::make_shared<Movie>(name, year);
    auto it = _base.find(movie);
    if (it != _base.end())
    {
        return it->first;
    }
    return nullptr;
}

std::ostream &operator<<(std::ostream &os, const RecommendationSystem &base)
{
    for ( const std::pair< const sp_movie
        , std::vector<double>>& it : base._base)
    {
        os << *(it.first);
    }
    return os;
}

double RecommendationSystem::norm(const std::vector<double>& v)
{
    double sum = 0;
    for (const double& it : v)
    {
        sum += it * it;
    }
    return std::sqrt(sum);
}

double RecommendationSystem::inner_mult(const std::vector<double> &v1,
                                        const std::vector<double> &v2)
{
    if(v1.size() != v2.size())
    {
        throw std::invalid_argument("features sizes are incompatible");
    }
    double sum = 0;
    size_t length = v1.size();
    for (size_t i = 0; i < length; i++)
    {
        sum += v1[i] * v2[i];
    }
    return sum;

}


double RecommendationSystem::angle_cos(const std::vector<double>& v1,
                                       const std::vector<double>& v2)
{
    if(v1.size() != v2.size())
    {
        throw std::invalid_argument("features sizes are incompatible");
    }
    if ( norm(v1) == 0 || norm(v2) == 0)
    {
        throw std::invalid_argument("Dividing by zero");
    }
    return inner_mult( v1, v2) / ( norm(v1) * norm(v2) );
}


sp_movie RecommendationSystem::add_movie_to_rs(const std::string &name,
                                int year, const std::vector<double> &features)
{
    for ( const auto& it : features)
    {
        features_check(it);
    }
    sp_movie movie = std::make_shared<Movie>(name, year);
    _base[movie] = features;
    return movie;
}



Movies_Features RecommendationSystem::movies_filter(bool filter,
                                    const User& user_rankings) const
{
    Movies_Features features;
    for ( const std::pair< const sp_movie, std::vector<double>>& it : _base)
    {
        if ( !filter && user_rankings._ranks.find(
                        it.first) != user_rankings._ranks.end() )
        {
            features[it.first] = it.second;
        }
        else
        {
            if ( filter && user_rankings._ranks.find(
                        it.first) == user_rankings._ranks.end() )
            {
                features[it.first] = it.second;
            }
        }
    }
    return features;
}


sp_movie RecommendationSystem::recommend_by_content(const
                                                    User &user_rankings) const
{
    auto user_ranks = user_rankings.normalized_ranks();

    Movies_Features seen = movies_filter(false, user_rankings);
    Movies_Features unseen = movies_filter(true, user_rankings);

    size_t features_size = seen.begin()->second.size();
    std::vector<double> fav_features( features_size, 0.0);

    for ( auto& it : seen)
    {
        for ( size_t j = 0; j < it.second.size(); j++)
        {
            it.second[j] *=  user_ranks[it.first];
            fav_features[j] += it.second[j];
        }
    }

    double mx = -std::numeric_limits<double>::infinity();
    sp_movie to_recommend;
    for ( auto& it : unseen)
    {
        double tmp = angle_cos(it.second, fav_features);
        if ( tmp > mx)
        {
            mx = tmp;
            to_recommend = it.first;
        }
    }
    return to_recommend;
}


Similarity_Set RecommendationSystem::similar_movies( const Movies_Features
                                &movies, const sp_movie &movie, int k) const
{
    if (_base.find(movie) == _base.end())
    {
        throw std::invalid_argument("movie not found");
    }

    size_t new_k = k;
    Similarity_Set redesigned((Custom_Sort()));
    Movies_Features most_similar;
    const auto& movie_features = _base.find(movie)->second;

    for ( const std::pair< const sp_movie, std::vector<double>>& it : movies)
    {
        redesigned.insert({it.first,
                                angle_cos(it.second, movie_features )
                                });
        if ( redesigned.size() > new_k )
        {
            redesigned.erase(redesigned.begin());
        }
    }
    return redesigned;
}


double RecommendationSystem::predict_movie_score(const User &user_rankings,
                                        const sp_movie &movie, int k) const
{
    if ( !movie)
    {
        throw std::invalid_argument("Requesting score prediction for "
                                    "a nullptr");
    }
    auto user_ranks = user_rankings._ranks;
    auto seen = movies_filter(false, user_rankings);
    auto alike = similar_movies(seen, movie, k);

    double sum1 = 0.0;
    double sum2 = 0.0;
    for ( const std::pair< sp_movie, double>& it : alike)
    {
        sum1 += it.second;
        sum2 += it.second *  user_ranks[it.first];
    }
    return sum2 / sum1;
}


sp_movie RecommendationSystem::recommend_by_cf(const User &user_rankings,
                                                                int k) const
{
    auto unseen = movies_filter(true, user_rankings);

    double mx = -std::numeric_limits<double>::infinity();
    sp_movie to_recommend;
    for (auto& it : unseen)
    {
        double tmp = predict_movie_score(user_rankings, it.first, k);
        if ( tmp > mx)
        {
            mx = tmp;
            to_recommend = it.first;
        }
    }
    return to_recommend;
}


std::vector<double> RecommendationSystem::get_features( const sp_movie& movie)
{
    return _base[movie];
}


const Movies_Features& RecommendationSystem::get_all_movies() const
{
    return _base;
}
