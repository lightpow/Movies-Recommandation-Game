#include "GameEngine.h"
#include "Files_Gen.h"

using namespace std::chrono;

int main()
{
    std::cout << "\nThis game includes 10 parts of animal's movies rate guessing\n";
    std::this_thread::sleep_for(25ms);
    std::cout << "You have to get 50% of the challenges right each part to continue\n";
    std::cout << "Don't rush :) New animals will be unlocked each part\n";
    std::cout << "You can exit by pressing <q>\n\n";
    std::this_thread::sleep_for(40ms);
    std::cout << "Press enter to start...\n";
    std::cin.get();
    if ( ! generate_game_files(movies, animals))
    {
        return EXIT_FAILURE;
    }
    GameEngine engine;
    engine.run();
    std::cout << "Have a nice day!" << std::endl;
    return EXIT_SUCCESS;
}