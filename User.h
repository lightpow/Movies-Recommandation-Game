// User.h

#ifndef USER_H
#define USER_H
#include <unordered_map>
#include <vector>
#include <string>

#include "Movie.h"
#include "RecommendationSystem.h"



class RecommendationSystem;

typedef std::unordered_map<sp_movie, double, hash_func,equal_func> rank_map;


class User
{
friend class RecommendationSystem;

private:
	const std::string _name;
	rank_map _ranks;
	std::shared_ptr<RecommendationSystem> _data_base;

	/**
	 *
	 * @return user ranks vector with every elem being normalized by the avg.
	 */
	rank_map normalized_ranks() const;

	/**
	 * check if the feature val given in the range.
	 * @param i a feature val
	 */
	static void features_check( double i);

public:
		/**
		 * construct the user.
		 * @param name
		 * @param ranks
		 * @param data_base
		 */
		User( std::string name, rank_map ranks,
				 const std::shared_ptr<RecommendationSystem>& data_base);

	/**
	 * @return the user name.
	 */
	std::string get_name() const;

	/**
	 * @return the user rank map.
	 */
	const rank_map& get_rank() const;


	/**
	 * function for adding a movie to the DB
	 * @param name name of movie
     * @param year year it was made
	 * @param features a vector of the movie's features
	 * @param rate the user rate for this movie
	 */
	void add_movie_to_user(const std::string &name, int year,
                         const std::vector<double> &features,
                         double rate);

	/**
	 * returns a recommendation according to the movie's content
	 * @return recommendation
	 */
	sp_movie get_rs_recommendation_by_content() const;

	/**
	 * returns a recommendation according to the similarity recommendation method
	 * @param k the number of the most similar movies to calculate by
	 * @return recommendation
	 */
	sp_movie get_rs_recommendation_by_cf(int k) const;

	/**
	 * predicts the score for a given movie
	 * @param name the name of the movie
	 * @param year the year the movie was created
	 * @param k the parameter which represents the number of the most similar movies to predict the score by
	 * @return predicted score for the given movie
	 */
	double get_rs_prediction_score_for_movie(const std::string& name,
												int year, int k) const;

	/**
     * prints the user name, movies, given and expected ranks to a given stream in a specific format.
	 * @param os
	 * @param user
	 * @return
	 */
	friend std::ostream& operator<<(std::ostream& os, const User& user);

};



#endif //USER_H
