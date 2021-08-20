#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
int main()
{
    const int windowWidth = 800;
    const int windowHeight = 600;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML window");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

           
        }
        window.clear();
        window.display();
    }
    return EXIT_SUCCESS;
}