// GameEngine.cpp

#include "GameEngine.h"


#include "RecommendationSystemLoader.h"
#include "UsersLoader.h"



using namespace std::chrono_literals;

bool GameTarget::operator==(const GameTarget &other) const
{
    int res = 1;
    res = res && Sp_movie_compare()(this->movie, other.movie);
    res = res && ( this->expected_level == other.expected_level);
    res = res && ( this->predicted_score == other.predicted_score);
    res = res && ( this->challenge_type == other.challenge_type);
    return res;
}

GameEngine::GameEngine() : _current_part(1), _rs(nullptr) {}


int GameEngine::score_to_level(double score)
{
    if (score < 2.5) return 1;
    if (score < 4.5) return 2;
    if (score < 6.5) return 3;
    if (score < 8.5) return 4;
    return 5;
}


std::string GameEngine::get_level_emoji(int level)
{
    switch (level) {
        case 1: return "🤢 (Very Bad)";
        case 2: return "👎 (Bad)";
        case 3: return "😐 (Average)";
        case 4: return "👍 (Good)";
        case 5: return "🤩 (Excellent)";
        default: return "❓";
    }
}


bool GameEngine::GameTarget_Comp::operator()(const GameTarget& lhs,
                                                const GameTarget& rhs) const
{
    return Sp_movie_compare()(lhs.movie, rhs.movie);
}

std::vector<sp_movie> GameEngine::get_animal_hints(const User& animal) const
{
    Movies_Features rated_map = _rs->movies_filter(false, animal);
    std::vector<sp_movie> rated_list;
    
    for (const auto& pair : rated_map) {
        rated_list.push_back(pair.first);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(rated_list.begin(), rated_list.end(), gen);

    if (rated_list.size() > 5) {
        rated_list.resize(5);
    }
    return rated_list;
}


std::vector<sp_movie> GameEngine::get_unrated_pool(const User& animal) const
{
    Movies_Features unrated_map = _rs->movies_filter(true, animal);
    std::vector<sp_movie> unrated_list;
    for (const auto& pair : unrated_map) {
        unrated_list.push_back(pair.first);
    }
    return unrated_list;
}


GameTarget GameEngine::get_worst_match_target(const User& animal, int k) const
{
    auto unrated = get_unrated_pool(animal);
    sp_movie worst_movie = nullptr;
    double min_score = MIN_SCORE;

    for (const auto& movie : unrated) {
        double score = animal.get_rs_prediction_score_for_movie(
                                    movie->get_name(), movie->get_year(), k);
        if (score < min_score) {
            min_score = score;
            worst_movie = movie;
        }
    }
    return {worst_movie,
        "Worst Match",
                min_score, score_to_level(min_score)};
}


GameTarget GameEngine::get_wildcard_target(const User& animal, int k) const
{
    auto unrated = get_unrated_pool(animal);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(
                                                        unrated.size() - 1));
    
    sp_movie random_movie = unrated[dis(gen)];
    // Calling user class prediction method
    double score = animal.get_rs_prediction_score_for_movie(
                        random_movie->get_name(), random_movie->get_year(), k);
    return {random_movie,
        "Random Wildcard",
                score, score_to_level(score)};
}


bool GameEngine::load_part_data(int part_num) {
    std::string movie_file = "parts_files/part_"
                            + std::to_string(part_num) + "_movies.txt";
    std::string user_file = "parts_files/part_"
                            + std::to_string(part_num) + "_users.txt";

    auto rs_ptr = RecommendationSystemLoader::create_rs_from_movies(
                                                                movie_file);
    if (!rs_ptr) {
        std::cerr << "Error: Could not load movies from " << movie_file<< "\n";
        return false;
    }

    _rs = rs_ptr.get();

    _current_animals = UsersLoader::create_users(user_file, std::move(rs_ptr));

    if (_current_animals.empty()) {
        std::cerr << "Error: Could not load users from " << user_file << "\n";
        return false;
    }

    return true;
}


int GameEngine::play_part() {
    int k = _current_part * GENERALIZER;
    int total_challenges = 0;
    int correct_guesses = 0;

    std::cout << "\n=== STARTING PART " << _current_part << " ===\n";
    std::this_thread::sleep_for(1s);

    for (const auto& animal : _current_animals) {
        std::cout << "\n--- Animal: " << animal.get_name() << " ---\n";
        
        // 1. Show Hints
        std::cout << "Here are 5 movies this animal previously ranked:\n";
        auto hints = get_animal_hints(animal);
        auto current_ranks = animal.get_rank();
        
        for (const auto& hint : hints) {
            double actual_rating = current_ranks.at(hint);
            std::this_thread::sleep_for(50ms);
            std::cout << "- " << hint->get_name() << " (" <<
                hint->get_year() << ") - Rating: " << actual_rating << "\n";
        }

        // 2. Generate Challenges
        std::set<GameTarget, GameTarget_Comp> targets;

        sp_movie cf_movie = animal.get_rs_recommendation_by_cf(k);
        double cf_score = animal.get_rs_prediction_score_for_movie(
                                cf_movie->get_name(), cf_movie->get_year(), k);
        targets.insert({cf_movie, "CF Top Pick",
                                    cf_score, score_to_level(cf_score)}
                                    );

        sp_movie content_movie = animal.get_rs_recommendation_by_content();
        double content_score = animal.get_rs_prediction_score_for_movie(
                    content_movie->get_name(), content_movie->get_year(), k);
        targets.insert({content_movie, "Content Top Pick",
                                content_score, score_to_level(content_score)}
                                );

        targets.insert(get_wildcard_target(animal, k));
        targets.insert(get_worst_match_target(animal, k));

        // 3. Execute Challenges
        int idx = 1;
        for (const auto& target : targets)
        {
            std::this_thread::sleep_for(100ms);
            total_challenges++;
            std::cout << "\nChallenge " << idx++ <<" : " << *(target.movie);
            
            // Print feature vectors
            std::this_thread::sleep_for(50ms);
            std::vector<double> features = _rs->get_features(target.movie);
            std::this_thread::sleep_for(50ms);
            std::cout << "Global Rates: ";
            std::this_thread::sleep_for(50ms);
            size_t i = 0;
            for (;i < features.size() && i < _feature_names.size(); ++i)
            {
                std::cout << _feature_names[i]
                << "(" << get_level_emoji(score_to_level(features[i])) << ") ";
            }
            std::cout << "\n";
            
            std::string guess;
            std::this_thread::sleep_for(50ms);
            std::cout << "Guess Level (1:🤢, 2:👎, 3:😐, 4:👍, 5:🤩): ";
            std::cin >> guess;

            if ( guess == "q")
            {
                return END;
            }
            int i_guess = stoi(guess);
            if (i_guess == target.expected_level) {
                std::this_thread::sleep_for(50ms);
                std::cout << "Correct! 🎉\n";
                correct_guesses++;
            } else {
                std::this_thread::sleep_for(50ms);
                std::cout << "Wrong! Correct level was: "
                            << get_level_emoji(target.expected_level) << "\n";
            }
        }
    }

    double accuracy = static_cast<double>(correct_guesses) / total_challenges;
    std::cout << "\nPart " << _current_part
                << " Accuracy: " << (accuracy * 100) << "%\n";

    return (accuracy >= CONTINUE_LIMIT);
}



void GameEngine::run() {
    while (_current_part <= 10) {
        if (!load_part_data(_current_part)) {
            std::cerr << "Failed to load data for Part "
                        << _current_part << ". Exiting.\n";
            break;
        }
        int val = play_part();
        if (val == 1)
        {
            std::cout << "Moving to the next part...\n";
            std::this_thread::sleep_for(2s);
            _current_part++;
        }
        else
        {
            if (val == END)
            {
                return;
            }
            std::cout << "You failed to reach 50%. Retrying Part "
                        << _current_part << "...\n";
            std::this_thread::sleep_for(2s);
        }
    }
    
    if (_current_part > 10) {
        std::cout << "Congratulations! You completed all 10 parts!\n";
    }
}