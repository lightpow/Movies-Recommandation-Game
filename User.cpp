// User.cpp

#include "User.h"


User::User( std::string name, rank_map ranks,
                    const std::shared_ptr<RecommendationSystem>& data_base) :
    _name(std::move(name)), _ranks(std::move(ranks)), _data_base(data_base) {};

std::string User::get_name() const
{
    return this->_name;
}

const rank_map& User::get_rank() const
{
    return this->_ranks;
}


void User::features_check( double i)
{
    if ( i < MIN_FEATURES_VAL || i > MAX_FEATURES_VAL )
    {
        throw std::invalid_argument("Invalid feature value");
    }
}


void User::add_movie_to_user(const std::string &name, int year,
                            const std::vector<double> &features, double rate)
{
    features_check(rate);
    sp_movie movie = _data_base->add_movie_to_rs( name, year, features);
    _ranks[movie] = rate;
}

rank_map User::normalized_ranks() const
{
    auto user_ranks = _ranks;
    double avg_ranking = 0;
    for ( auto& it : user_ranks)
    {
        avg_ranking += it.second;
    }
    avg_ranking /= static_cast<double>(user_ranks.size());

    for ( auto& it : user_ranks)
    {
        it.second-= avg_ranking;
    }
    return user_ranks;
}


sp_movie User::get_rs_recommendation_by_content() const
{
    return _data_base->recommend_by_content(*this);
}

double User::get_rs_prediction_score_for_movie(const std::string &name,
                                                    int year, int k) const
{
    sp_movie movie = _data_base->get_movie(name, year);
    if ( movie == nullptr)
    {
        throw std::invalid_argument("Movie not found");
    }
    return _data_base->predict_movie_score(*this, movie, k);
}

sp_movie User::get_rs_recommendation_by_cf(int k) const
{
    return _data_base->recommend_by_cf(*this, k);
}

std::ostream& operator<<(std::ostream& os, const User& user)
{
    os << "name: " << user.get_name() << "\n";
    os << *(user._data_base);
    os << std::endl;
    return os;
}
