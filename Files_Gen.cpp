// Files_Gen.cpp


#include "Files_Gen.h"




bool generate_game_files(const std::vector<std::string>& movie_pool,
            const std::vector<std::string>& animal_pool)
{
    fs::create_directories("parts_files");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> feature_dist(1.0, 10.0);

    for (int part = 1; part <= 10; ++part) {
        int num_movies = 15 + ((part - 1) * 3);
        int num_animals = part;

        if (num_movies > movie_pool.size() || num_animals > animal_pool.size())
        {
            std::cerr << "Error: Not enough data in movies and animals vectors for part "
                        << part << "\n";
            return false;
        }

        std::string movie_filename = "parts_files/part_"
                                     "" + std::to_string(part) + "_movies.txt";
        std::ofstream f_movies(movie_filename);
        f_movies << std::fixed << std::setprecision(1);

        for (int i = 0; i < num_movies; ++i) {
            f_movies << movie_pool[i];
            for (int j = 0; j < 5; ++j) {
                f_movies << " " << feature_dist(gen);
            }
            f_movies << "\n";
        }
        f_movies.close();

        std::string user_filename = "parts_files/part_"
                                    "" + std::to_string(part) + "_users.txt";
        std::ofstream f_users(user_filename);
        f_users << std::fixed << std::setprecision(1);

        for (int i = 0; i < num_movies; ++i) {
            f_users << movie_pool[i] << (i == num_movies - 1 ? "" : " ");
        }
        f_users << "\n";

        for (int a = 0; a < num_animals; ++a) {
            f_users << animal_pool[a];

            std::uniform_int_distribution<> ranked_dist(10,
                                                        num_movies - 5);
            int num_ranked = ranked_dist(gen);

            std::vector<int> indices(num_movies);
            std::iota(indices.begin(), indices.end(), 0);
            std::shuffle(indices.begin(), indices.end(), gen);

            std::vector<bool> is_ranked(num_movies, false);
            for (int i = 0; i < num_ranked; ++i) {
                is_ranked[indices[i]] = true;
            }

            for (int i = 0; i < num_movies; ++i) {
                if (is_ranked[i]) {
                    f_users << " " << feature_dist(gen);
                } else {
                    f_users << " NA";
                }
            }
            f_users << "\n";
        }
        f_users.close();
        // for debugging->
        //std::cout << "Generated " << movie_filename << " and " << user_filename << "\n";
    }
    return true;
}