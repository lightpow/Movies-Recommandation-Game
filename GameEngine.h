// GameEngine.h

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "User.h"
#include "RecommendationSystem.h"
#include "RecommendationSystemLoader.h"
#include "UsersLoader.h"


#include <iostream>
#include <algorithm>
#include <thread>
#include <random>
#include <vector>
#include <string>
#include <memory>

#define END (-1)
#define MIN_SCORE 11.0
#define GENERALIZER 3
#define CONTINUE_LIMIT (0.5)

/**
 * A struct representing a single guessing challenge in the game.
 * Holds the target movie, the algorithmic prediction, and the expected outcome.
 */
struct GameTarget {
    sp_movie movie;
    std::string challenge_type;
    double predicted_score;
    int expected_level;

    /**
     *
     * @param other
     * @return true if this equal other, false otherwise.
     */
    bool operator==(const GameTarget& other) const;
};

class GameEngine {
private:
    int _current_part;
    RecommendationSystem* _rs;
    std::vector<User> _current_animals;

    const std::vector<std::string> _feature_names = {
        "Horror", "Comedy", "Drama", "Adventure", "Fiction"
    };

    /**
     * Converts a continuous movie rating (1.0 to 10.0) into a discrete 5-level scale.
     * Maps the intervals in steps of 2.0 to categorize the score.
     * @param score the predicted or actual double score of the movie.
     * @return an integer from 1 to 5 representing the rating level.
     */
    static int score_to_level(double score);

    /**
     * Maps the 1-5 integer level to a visual emoji representation.
     * @param level the discrete rating level (1-5).
     * @return a formatted string containing the emoji and its description.
     */
    static std::string get_level_emoji(int level);

    /**
     * Retrieves a random sample of up to 5 movies that the animal has already rated.
     * Utilizes the RecommendationSystem's filter to get seen movies, then shuffles them.
     * @param animal the user representing the animal whose hints are being generated.
     * @return a vector containing up to 5 shared pointers to rated movies.
     */
    std::vector<sp_movie> get_animal_hints(const User& animal) const;

    /**
     * Retrieves all movies in the system that the specified animal has NOT yet rated.
     * Utilizes the RecommendationSystem's filter to extract unseen movies.
     * @param animal the user representing the animal.
     * @return a vector containing shared pointers to all unrated movies.
     */
    std::vector<sp_movie> get_unrated_pool(const User& animal) const;
    
    /**
     * Generates an "Anti-Recommendation" challenge by finding the movie with the lowest predicted score.
     * Iterates through all unrated movies and predicts the score using Collaborative Filtering.
     * @param animal the user representing the animal.
     * @param k the number of similar movies to use for the prediction algorithm.
     * @return a GameTarget struct containing the worst matching movie and its expected level.
     */
    GameTarget get_worst_match_target(const User& animal, int k) const;

    /**
     * Generates a "Wildcard" challenge by selecting a completely random unrated movie.
     * Tests the player's ability to deduce feature vectors without algorithmic bias.
     * @param animal the user representing the animal.
     * @param k the number of similar movies to use for the prediction algorithm.
     * @return a GameTarget struct containing a random movie and its expected level.
     */
    GameTarget get_wildcard_target(const User& animal, int k) const;

public:
    /**
     * Constructs a new GameEngine object and initializes the starting part to 1.
     */
    GameEngine();

    /**
     * Loads the movies and users for a specific part from the dataset files.
     * @param part_num the integer representing which part (1-10) to load.
     * @return true if data was loaded successfully, false otherwise.
     */
    bool load_part_data(int part_num);

    /**
     * Executes the main gameplay loop for the current part.
     * Generates challenges, prompts the player, and evaluates their guesses.
     * @return 1 if the player achieves an 75% or higher accuracy, 0 if didn't, -1 to quit.
     */
    int play_part();

    /**
     * The primary entry point for the game.
     * Manages progression through all 10 parts and handles level retries upon failure.
     */
    void run();
};

#endif // GAMEENGINE_H